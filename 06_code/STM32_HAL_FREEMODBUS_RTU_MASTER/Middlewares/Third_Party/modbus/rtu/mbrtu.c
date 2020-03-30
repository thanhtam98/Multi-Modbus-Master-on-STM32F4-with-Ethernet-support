/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
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
 * File: $Id: mbrtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbrtu.h"
#include "mbframe.h"

#include "mbcrc.h"
#include "mbport.h"

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN 4   /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_MAX 256 /*!< Maximum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC 2   /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF 0   /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF 1    /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_INIT, /*!< Receiver is in initial state. */
    STATE_RX_IDLE, /*!< Receiver is in idle state. */
    STATE_RX_RCV,  /*!< Frame is beeing received. */
    STATE_RX_ERROR /*!< If the frame is invalid. */
} eMBRcvState;

typedef enum
{
    STATE_TX_IDLE, /*!< Transmitter is in idle state. */
    STATE_TX_XMIT  /*!< Transmitter is in transfer state. */
} eMBSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eMBSndState eSndState[MB_RS485_MAX_PORT];
static volatile eMBRcvState eRcvState[MB_RS485_MAX_PORT];

volatile UCHAR  ucMasterRTUSndBuf[MB_RS485_MAX_PORT][MB_SER_PDU_SIZE_MAX];
volatile UCHAR  ucMasterRTURcvBuf[MB_RS485_MAX_PORT][MB_SER_PDU_SIZE_MAX];
volatile USHORT usMasterSendPDULength[MB_RS485_MAX_PORT];

static volatile UCHAR *pucMasterSndBufferCur[MB_RS485_MAX_PORT];
static volatile USHORT usMasterSndBufferCount[MB_RS485_MAX_PORT];

static volatile USHORT usMasterRcvBufferPos[MB_RS485_MAX_PORT];
static volatile BOOL   xFrameIsBroadcast = FALSE;

// static volatile eMBMasterTimerMode eMasterCurTimerMode;
/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit(UCHAR ucSlaveAddress, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity)
{
    eMBErrorCode eStatus = MB_ENOERR;
    ULONG usTimerT35_50us;

    (void)ucSlaveAddress;
    ENTER_CRITICAL_SECTION();

    /* Modbus RTU uses 8 Databits. */
    /*HAL has already configuarated*/
    if (xMBPortSerialInit(ucPort, ulBaudRate, 8, eParity) != TRUE)
    {
        eStatus = MB_EPORTERR;
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
        if (xMBPortTimersInit((USHORT)usTimerT35_50us) != TRUE)
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION();

    return eStatus;
}

void eMBRTUStart(UCHAR ucPort)
{
    ENTER_CRITICAL_SECTION();
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    eRcvState[ucPort] = STATE_RX_INIT;
    vMBPortSerialEnable(ucPort, TRUE, FALSE);
    vMBPortTimersEnable(ucPort);

    EXIT_CRITICAL_SECTION();
}

void eMBRTUStop(UCHAR ucPort)
{
    ENTER_CRITICAL_SECTION();
    vMBPortSerialEnable(ucPort, FALSE, FALSE);
    vMBPortTimersDisable(ucPort);
    EXIT_CRITICAL_SECTION();
}

eMBErrorCode
eMBRTUReceive(UCHAR ucPort, UCHAR *pucRcvAddress, UCHAR **pucFrame, USHORT *pusLength)
{
    BOOL xFrameReceived = FALSE;
    eMBErrorCode eStatus = MB_ENOERR;

    (void)(xFrameReceived);

    ENTER_CRITICAL_SECTION();
    assert(usMasterRcvBufferPos[ucPort] < MB_SER_PDU_SIZE_MAX);

    /* Length and CRC check */
    if ((usMasterRcvBufferPos[ucPort] >= MB_SER_PDU_SIZE_MIN) && (usMBCRC16((UCHAR *)ucMasterRTURcvBuf[ucPort], usMasterRcvBufferPos[ucPort]) == 0))
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = ucMasterRTURcvBuf[ucPort][MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = (USHORT)(usMasterRcvBufferPos[ucPort] - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC);

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = (UCHAR *)&ucMasterRTURcvBuf[ucPort][MB_SER_PDU_PDU_OFF];
        xFrameReceived = TRUE;
    }
    else
    {
        eStatus = MB_EIO;
    }

    EXIT_CRITICAL_SECTION();
    return eStatus;
}

eMBErrorCode
eMBRTUSend(UCHAR ucPort, UCHAR ucSlaveAddress, const UCHAR *pucFrame, USHORT usLength)
{
    eMBErrorCode eStatus = MB_ENOERR;
    USHORT usCRC16;

    ENTER_CRITICAL_SECTION();

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if (eRcvState[ucPort] == STATE_RX_IDLE)
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucMasterSndBufferCur[ucPort] = (UCHAR *)pucFrame - 1;
        usMasterSndBufferCount[ucPort] = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        pucMasterSndBufferCur[ucPort][MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        usMasterSndBufferCount[ucPort] += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16((UCHAR *)pucMasterSndBufferCur[ucPort], usMasterSndBufferCount[ucPort]);
        ucMasterRTUSndBuf[ucPort][usMasterSndBufferCount[ucPort]++] = (UCHAR)(usCRC16 & 0xFF);
        ucMasterRTUSndBuf[ucPort][usMasterSndBufferCount[ucPort]++] = (UCHAR)(usCRC16 >> 8);

        /* Activate the transmitter. */
        eSndState[ucPort] = STATE_TX_XMIT;
        vMBPortSerialEnable(ucPort,FALSE, TRUE);
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION();
    return eStatus;
}

BOOL xMBRTUReceiveFSM(UCHAR ucPort)
{
    BOOL xTaskNeedSwitch = FALSE;
    UCHAR ucByte;

    assert(eSndState[ucPort] == STATE_TX_IDLE);

    /* Always read the character. */
    (void)xMBPortSerialGetByte(ucPort,(CHAR *)&ucByte);

    switch (eRcvState[ucPort])
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vMBPortTimersEnable(ucPort);
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vMBPortTimersEnable(ucPort);
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
        usMasterRcvBufferPos[ucPort] = 0;
        ucMasterRTURcvBuf[ucPort][usMasterRcvBufferPos[ucPort]++] = ucByte;
        eRcvState[ucPort] = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        vMBPortTimersEnable(ucPort);
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if (usMasterRcvBufferPos[ucPort] < MB_SER_PDU_SIZE_MAX)
        {
            ucMasterRTURcvBuf[ucPort][usMasterRcvBufferPos[ucPort]++] = ucByte;
        }
        else
        {
            eRcvState[ucPort] = STATE_RX_ERROR;
        }
        vMBPortTimersEnable(ucPort);
        break;
    }
    return xTaskNeedSwitch;
}

BOOL xMBRTUTransmitFSM(UCHAR ucPort)
{
    BOOL xNeedPoll = FALSE;

    assert(eRcvState[ucPort] == STATE_RX_IDLE);

    switch (eSndState[ucPort])
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMBPortSerialEnable(ucPort,TRUE, FALSE);
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if (usMasterSndBufferCount[ucPort] != 0)
        {
            xMBPortSerialPutByte(ucPort,(CHAR)*pucMasterSndBufferCur[[ucPort]]);
            pucMasterSndBufferCur[ucPort]++; /* next byte in sendbuffer. */
            usMasterSndBufferCount[ucPort]--;
        }
        else
        {
            xNeedPoll = xMBPortEventPost(ucPort,EV_FRAME_SENT);
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMBPortSerialEnable(ucPort,TRUE, FALSE);
            eSndState[ucPort] = STATE_TX_IDLE;
        }
        break;
    }

    return xNeedPoll;
}

BOOL xMBRTUTimerT35Expired(UCHAR ucPort)
{
    BOOL xNeedPoll = FALSE;

    switch (eRcvState[ucPort])
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        xNeedPoll = xMBPortEventPost(ucPort,EV_READY);
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:
        xNeedPoll = xMBPortEventPost(ucPort,EV_FRAME_RECEIVED);
        break;

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. */
    default:
        assert((eRcvState[ucPort] == STATE_RX_INIT) ||
               (eRcvState[ucPort] == STATE_RX_RCV) || (eRcvState[ucPort] == STATE_RX_ERROR));
    }

    vMBPortTimersDisable( ucPort);
    eRcvState[ucPort] = STATE_RX_IDLE;

    return xNeedPoll;
}

/*Modbus Function handle buffer */
/* Get Modbus Master send RTU's buffer address pointer.*/
void vMBMasterGetRTUSndBuf( UCHAR ucPort, UCHAR ** pucFrame )
{
	*pucFrame = ( UCHAR * ) ucMasterRTUSndBuf[ucPort];
}

/* Get Modbus Master send PDU's buffer address pointer.*/
void vMBMasterGetPDUSndBuf(  UCHAR ucPort, UCHAR ** pucFrame )
{
	*pucFrame = ( UCHAR * ) &ucMasterRTUSndBuf[ucPort][MB_SER_PDU_PDU_OFF];
}

/* Set Modbus Master send PDU's buffer length.*/
void vMBMasterSetPDUSndLength( UCHAR ucPort,  USHORT SendPDULength )
{
	usMasterSendPDULength[ucPort] = SendPDULength;
}

/* Get Modbus Master send PDU's buffer length.*/
USHORT usMBMasterGetPDUSndLength(  UCHAR ucPort  )
{
	return usMasterSendPDULength[ucPort];
}

// /* Set Modbus Master current timer mode.*/
// void vMBMasterSetCurTimerMode( eMBMasterTimerMode eMBTimerMode )
// {
// 	eMasterCurTimerMode = eMBTimerMode;
// }

// /* The master request is broadcast? */
// BOOL xMBMasterRequestIsBroadcast( void ){
// 	return xFrameIsBroadcast;
// }