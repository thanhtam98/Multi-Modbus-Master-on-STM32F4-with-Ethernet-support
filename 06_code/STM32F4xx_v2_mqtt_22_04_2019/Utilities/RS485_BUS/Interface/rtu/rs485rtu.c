/* 
 * RS485Bus Libary: A portable RS485Bus implementation for RS485Bus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: rs485rtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"
#include "stm32f4xx_conf.h"
/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- RS485bus includes ----------------------------------*/
#include "rs485.h"
#include "rs485rtu.h"

#include "rs485frame.h"

#include "rs485crc.h"
#include "rs485port.h"
#ifdef SLAVE

/* ----------------------- Defines ------------------------------------------*/
#define RS485_SER_PDU_SIZE_MIN 8   /*!< Minimum size of a RS485bus RTU frame. */
#define RS485_SER_PDU_SIZE_MAX 256 /*!< Maximum size of a RS485bus RTU frame. */
#define RS485_SER_PDU_SIZE_CRC 2   /*!< Size of CRC field in PDU. */
// #define RS485_SER_PDU_START_OFF 	0       /*!< Offset of start in Ser-PDU. */
// #define RS485_SER_PDU_LENGTH_OFF   	1       /*!< Offset of Length in Ser-PDU. */
#define RS485_SER_PDU_ADDR_OFF 0 /*!< Offset of Address in Ser-PDU. */
#define RS485_SER_PDU_PDU_OFF 1  /*!< Offset of RS485bus-PDU in Ser-PDU. */
// #define RS485_SER_PDU_START_BYTE    0xAA
// #define RS485_SER_PDU_START_BYTE    ':'
/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT, /*!< Receiver is in initial state. */
    STATE_RX_IDLE, /*!< Receiver is in idle state. */
    STATE_RX_RCV,  /*!< Frame is beeing received. */
    STATE_RX_ERROR /*!< If the frame is invalid. */
} eRS485RcvState;

typedef enum
{
    STATE_TX_IDLE, /*!< Transmitter is in idle state. */
    STATE_TX_XMIT  /*!< Transmitter is in transfer state. */
} eRS485SndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eRS485SndState eSndState[RS485_PORT_NUMBER];
static volatile eRS485RcvState eRcvState[RS485_PORT_NUMBER];

volatile UCHAR ucRTUBuf[RS485_PORT_NUMBER][RS485_SER_PDU_SIZE_MAX];
volatile UCHAR usSendPDULength[RS485_PORT_NUMBER];

static volatile UCHAR *pucSndBufferCur[RS485_PORT_NUMBER];
static volatile USHORT usSndBufferCount[RS485_PORT_NUMBER];

static volatile USHORT usRcvBufferPos[RS485_PORT_NUMBER];

/* ----------------------- Start implementation -----------------------------*/
eRS485ErrorCode
eRS485RTUInit(UCHAR ucPort, ULONG ulBaudRate, eParity eParity)
{
    eRS485ErrorCode eStatus = RS485_ENOERR;
    ULONG usTimerT35_50us;

    ENTER_CRITICAL_SECTION();

    /* RS485 RTU uses 8 Databits. */
    if (xPortSerialInit(ucPort, ulBaudRate, 8, eParity) != TRUE)
    {
        eStatus = RS485_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if (ulBaudRate > 19200)
        {
            usTimerT35_50us = 35; /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = (7UL * 220000UL) / (2UL * ulBaudRate);
        }
        if (xPortTimersInit(ucPort, (USHORT)usTimerT35_50us) != TRUE)
        {
            eStatus = RS485_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void eRS485RTUStart(UCHAR ucPort)
{
    ENTER_CRITICAL_SECTION();
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    eRcvState[ucPort] = STATE_RX_INIT;
    vPortSerialEnable(ucPort, TRUE, FALSE);
    vPortTimersEnable(ucPort);

    EXIT_CRITICAL_SECTION();
}

void eRS485RTUStop(UCHAR ucPort)
{
    ENTER_CRITICAL_SECTION();
    vPortSerialEnable(ucPort, FALSE, FALSE);
    vPortTimersDisable(ucPort);
    EXIT_CRITICAL_SECTION();
}
USHORT crc;
eRS485ErrorCode
eRS485RTUReceive(UCHAR ucPort, UCHAR *pucRcvAddress, UCHAR **pucFrame, USHORT *pusLength)
{
    eRS485ErrorCode eStatus = RS485_ENOERR;

    ENTER_CRITICAL_SECTION();
    assert_param(usRcvBufferPos[ucPort] < RS485_SER_PDU_SIZE_MAX);

    /* Length and CRC check */
    // if ( usRcvBufferPos[ucPort] == ((((USHORT)(ucRTUBuf[ucPort][RS485_SER_PDU_LENGTH_OFF+1]<<8))|ucRTUBuf[ucPort][RS485_SER_PDU_LENGTH_OFF])+ 5))
    // 	{
    // 		if ( usRS485CRC16( ( UCHAR * ) ucRTUBuf[ucPort], usRcvBufferPos[ucPort] ) == 0 )
    // 		{
    // 				/* Total length of RS485-PDU is RS485-Serial-Line-PDU minus
    // 				 * size of Frame begin field, Lenght and CRC checksum.
    // 				 */
    // 				*pusLength = ((USHORT)(ucRTUBuf[ucPort][RS485_SER_PDU_LENGTH_OFF+1]<<8))|ucRTUBuf[ucPort][RS485_SER_PDU_LENGTH_OFF];
    // 		}
    // 		else
    // 		{
    // 			eStatus = RS485_EIO;
    // 		}
    // 	}
    if (usRcvBufferPos[ucPort] >= RS485_SER_PDU_SIZE_MIN)
    {
        if (usRS485CRC16((UCHAR *)ucRTUBuf[ucPort], usRcvBufferPos[ucPort]) == 0)
        {
            /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
            *pucRcvAddress = ucRTUBuf[RS485_PORT_NUMBER][RS485_SER_PDU_ADDR_OFF];

            /* Total length of RS485-PDU is RS485-Serial-Line-PDU minus
					 * size of Frame begin field, Lenght and CRC checksum.
					 */
            *pusLength = (USHORT)(usRcvBufferPos[ucPort] - RS485_SER_PDU_PDU_OFF - RS485_SER_PDU_SIZE_CRC);
            *pucFrame = (UCHAR *)&ucRTUBuf[ucPort][RS485_SER_PDU_PDU_OFF];
        }
        else
        {
            eStatus = RS485_EIO;
        }
    }
    else
    {
        eStatus = RS485_EIO;
    }
    /* Return the start of the RS485Bus PDU to the caller. */
    *pucFrame = (UCHAR *)&ucRTUBuf[ucPort][RS485_SER_PDU_PDU_OFF];

    EXIT_CRITICAL_SECTION();
    return eStatus;
}
/*
    @param:
        ucPort: which port used to transfer data 
        pucFrame: Protocol data unit of modbus
        usLength: Length of PDU for CRC checksum
*/
eRS485ErrorCode
eRS485RTUSend(UCHAR ucPort, UCHAR ucSlaveAddress, const UCHAR *pucFrame, USHORT usLength)
{
    eRS485ErrorCode eStatus = RS485_ENOERR;
    USHORT usCRC16;

    ENTER_CRITICAL_SECTION();

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if (eRcvState[ucPort] == STATE_RX_IDLE)
    {
        /* Mapping pointer of source data tp Buffer and offset lenght for STX and Slave ADR */
        pucSndBufferCur[ucPort] = (UCHAR *)pucFrame - 1;
        usSndBufferCount[ucPort] = 1;
        /* First byte before the RS485-PDU is the Start. */

        /* Now copy the RS485Bus-PDU into the RS485Bus-Serial-Line-PDU. */
        // pucSndBufferCur[ucPort][RS485_SER_PDU_START_OFF] = RS485_SER_PDU_START_BYTE;
        pucSndBufferCur[ucPort][RS485_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        // pucSndBufferCur[ucPort][RS485_SER_PDU_LENGTH_OFF] = ( UCHAR ) (usLength & 0xFF);
        // 		pucSndBufferCur[ucPort][RS485_SER_PDU_LENGTH_OFF+1] = ( UCHAR ) (usLength >> 8) & 0xFF;
        usSndBufferCount[ucPort] += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usRS485CRC16((UCHAR *)pucSndBufferCur[ucPort], usSndBufferCount[ucPort]);
        ucRTUBuf[ucPort][usSndBufferCount[ucPort]++] = (UCHAR)(usCRC16 & 0xFF);
        ucRTUBuf[ucPort][usSndBufferCount[ucPort]++] = (UCHAR)(usCRC16 >> 8);

        /* Activate the transmitter. */
        eSndState[ucPort] = STATE_TX_XMIT;
        vPortSerialEnable(ucPort, FALSE, TRUE);
    }
    else
    {
        eStatus = RS485_EIO;
    }
    EXIT_CRITICAL_SECTION();
    return eStatus;
}

BOOL xRS485RTUReceiveFSM(UCHAR ucPort)
{
    BOOL xTaskNeedSwitch = FALSE;
    UCHAR ucByte;

    assert_param(eSndState[ucPort] == STATE_TX_IDLE);

    /* Always read the character. */
    (void)xPortSerialGetByte(ucPort, (CHAR *)&ucByte);

    switch (eRcvState[ucPort])
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vPortTimersEnable(ucPort);
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vPortTimersEnable(ucPort);
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
    {
        usRcvBufferPos[ucPort] = 0;
        ucRTUBuf[ucPort][usRcvBufferPos[ucPort]++] = ucByte;
        eRcvState[ucPort] = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        vPortTimersEnable(ucPort);
    }
    break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if (usRcvBufferPos[ucPort] < RS485_SER_PDU_SIZE_MAX)
        {
            ucRTUBuf[ucPort][usRcvBufferPos[ucPort]++] = ucByte;
        }
        else
        {
            eRcvState[ucPort] = STATE_RX_ERROR;
        }
        vPortTimersEnable(ucPort);
        break;
    }
    return xTaskNeedSwitch;
}

BOOL xRS485RTUTransmitFSM(UCHAR ucPort)
{
    BOOL xNeedPoll = FALSE;

    assert_param(eRcvState[ucPort] == STATE_RX_IDLE);

    switch (eSndState[ucPort])
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vPortSerialEnable(ucPort, TRUE, FALSE);
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if (usSndBufferCount[ucPort] != 0)
        {
            xPortSerialPutByte(ucPort, (CHAR)*pucSndBufferCur[ucPort]);
            pucSndBufferCur[ucPort]++; /* next byte in sendbuffer. */
            usSndBufferCount[ucPort]--;
        }
        else
        {
            xNeedPoll = xPortEventPost(ucPort, EV_FRAME_SENT);
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            /*function below call a task to process software tx serial @!!!!!!!*/
            vPortSerialEnable(ucPort, TRUE, FALSE);
            eSndState[ucPort] = STATE_TX_IDLE;
        }
        break;
    }

    return xNeedPoll;
}

BOOL xRS485RTUTimerT35Expired(UCHAR ucPort)
{
    BOOL xNeedPoll = FALSE;

    switch (eRcvState[ucPort])
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        xNeedPoll = xPortEventPost(ucPort, EV_READY);
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:
        xNeedPoll = xPortEventPost(ucPort, EV_FRAME_RECEIVED);
        break;	

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. */
    default:
        assert_param((eRcvState[ucPort] == STATE_RX_INIT) ||
                     (eRcvState[ucPort] == STATE_RX_RCV) || (eRcvState[ucPort] == STATE_RX_ERROR));
        break;
    }

    vPortTimersDisable(ucPort);
    eRcvState[ucPort] = STATE_RX_IDLE;

    return xNeedPoll;
}


/* Get RS485bus Master send PDU's buffer address pointer.*/
void vRS485GetPDUSndBuf( UCHAR ucPort, UCHAR ** pucFrame )
{
	*pucFrame = ( UCHAR * ) &ucRTUBuf[ucPort][RS485_SER_PDU_PDU_OFF];
}
/* Set RS485bus Master send PDU's buffer length.*/
/* Set RS485bus Master send PDU's buffer length.*/
void vRS485SetPDUSndLength(UCHAR ucPort, USHORT SendPDULength )
{
	usSendPDULength[ucPort] = SendPDULength;
}
/* Get RS485bus Master send PDU's buffer length.*/
USHORT usRS485GetPDUSndLength( UCHAR ucPort )
{
	return usSendPDULength[ucPort];
}


#endif

