/* 
 * RS485bus Libary: A portable RS485bus implementation for RS485bus ASCII/RTU.
 * Copyright (c) 2013 China Beijing Armink <armink.ztl@gmail.com>
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
 * File: $Id: mbrtu_m.c,v 1.60 2013/08/17 11:42:56 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"
#include "stm32f4xx_conf.h"
/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- RS485bus includes ----------------------------------*/
#include "rs485.h"
#include "rs485_m.h"
#include "rs485rtu.h"
#include "rs485frame.h"

#include "rs485crc.h"
#include "rs485port.h"

#if RS485_MASTER_RTU_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
#define RS485_SER_PDU_SIZE_MIN     	8       /*!< Minimum size of a RS485bus RTU frame. */
#define RS485_SER_PDU_SIZE_MAX     	256     /*!< Maximum size of a RS485bus RTU frame. */
#define RS485_SER_PDU_SIZE_CRC     	2       /*!< Size of CRC field in PDU. */
// #define RS485_SER_PDU_START_OFF		  0		/*!< Offset of start in Ser-PDU. */
#define RS485_SER_PDU_ADDR_OFF 0       /*!< Offset of Length in Ser-PDU. */
#define RS485_SER_PDU_PDU_OFF 1       /*!< Offset of RS485bus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_M_RX_INIT,              /*!< Receiver is in initial state. */
    STATE_M_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_M_RX_RCV,               /*!< Frame is beeing received. */
    STATE_M_RX_ERROR,              /*!< If the frame is invalid. */
} eRS485MasterRcvState;

typedef enum
{
    STATE_M_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_M_TX_XMIT,              /*!< Transmitter is in transfer state. */
    STATE_M_TX_XFWR,              /*!< Transmitter is in transfer finish and wait receive state. */
} eRS485MasterSndState;

/* ----------------------- Static variables ---------------------------------*/
static volatile eRS485MasterSndState eSndState[RS485_PORT_NUMBER];
static volatile eRS485MasterRcvState eRcvState[RS485_PORT_NUMBER];

static volatile UCHAR  ucMasterRTUSndBuf[RS485_PORT_NUMBER][RS485_PDU_SIZE_MAX];
static volatile UCHAR  ucMasterRTURcvBuf[RS485_PORT_NUMBER][RS485_SER_PDU_SIZE_MAX];
static volatile USHORT usMasterSendPDULength[RS485_PORT_NUMBER];

static volatile UCHAR *pucMasterSndBufferCur[RS485_PORT_NUMBER];
static volatile USHORT usMasterSndBufferCount[RS485_PORT_NUMBER];

static volatile USHORT usMasterRcvBufferPos[RS485_PORT_NUMBER];
static volatile BOOL   xFrameIsBroadcast[RS485_PORT_NUMBER];

static volatile eRS485MasterTimerMode eMasterCurTimerMode[RS485_PORT_NUMBER];

/* ----------------------- Start implementation -----------------------------*/
eRS485ErrorCode
eRS485MasterRTUInit(UCHAR ucPort, ULONG ulBaudRate, eParity eParity )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    ULONG           usTimerT35_50us;

    ENTER_CRITICAL_SECTION(  );
	xFrameIsBroadcast[ucPort] = FALSE;
    /* RS485bus RTU uses 8 Databits. */
    if( xMasterPortSerialInit( ucPort, ulBaudRate, 8, eParity ) != TRUE )
    {
        eStatus = RS485_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
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
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        if( xMasterPortTimersInit( ucPort,( USHORT ) usTimerT35_50us ) != TRUE )
        {
            eStatus = RS485_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eRS485MasterRTUStart( UCHAR ucPort )
{
    ENTER_CRITICAL_SECTION(  );
    /* Initially the receiver is in the state STATE_M_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_M_RX_IDLE. This makes sure that we delay startup of the
     * rs485bus protocol stack until the bus is free.
     */
    eRcvState[ucPort] = STATE_M_RX_INIT;
    vMasterPortSerialEnable(ucPort, TRUE, FALSE );
    vMasterPortTimersT35Enable(ucPort);

    EXIT_CRITICAL_SECTION(  );
}

void
eRS485MasterRTUStop( UCHAR ucPort )
{
    ENTER_CRITICAL_SECTION(  );
    vMasterPortSerialEnable(ucPort, FALSE, FALSE );
    vMasterPortTimersDisable( ucPort );
    EXIT_CRITICAL_SECTION(  );
}

eRS485ErrorCode
eRS485MasterRTUReceive( UCHAR ucPort,UCHAR *pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    assert_param( usMasterRcvBufferPos[ucPort] < RS485_SER_PDU_SIZE_MAX );

    /* Length and CRC check */
    // if ( usMasterRcvBufferPos[ucPort] == ((((USHORT)(ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_LENGTH_OFF]<<8))|ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_LENGTH_OFF])- 3))
    // {
    //   if ( usRS485CRC16( ( UCHAR * ) ucMasterRTURcvBuf[ucPort], usMasterRcvBufferPos[ucPort] ) == 0 )
    //   {
    //     /* Total length of RS485bus-PDU is RS485bus-Serial-Line-PDU minus
    //      * size of address field and CRC checksum.
    //      */
    //     *pusLength = (((USHORT)(ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_LENGTH_OFF]<<8))|ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_LENGTH_OFF])-2;
    //     /* Return the start of the RS485bus PDU to the caller. */
    //   }
    //   else
	// 		{
	// 			eStatus = RS485_EIO;
	// 		}
    // }
     if (usMasterRcvBufferPos[ucPort] >= RS485_SER_PDU_SIZE_MIN)
    {
        if (usRS485CRC16((UCHAR *)ucMasterRTURcvBuf[ucPort], usMasterRcvBufferPos[ucPort]) == 0)
        {
            /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
            *pucRcvAddress = ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_ADDR_OFF];

            /* Total length of RS485-PDU is RS485-Serial-Line-PDU minus
					 * size of Frame begin field, Lenght and CRC checksum.
					 */
            *pusLength = (USHORT)(usMasterRcvBufferPos[ucPort] - RS485_SER_PDU_PDU_OFF - RS485_SER_PDU_SIZE_CRC);
            *pucFrame = (UCHAR *)&ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_PDU_OFF];
        }
    else
    {
        eStatus = RS485_EIO;
    }
    /* Return the start of the RS485Bus PDU to the caller. */
		// *pucFrame = ( UCHAR * ) & ucMasterRTURcvBuf[ucPort][RS485_SER_PDU_PDU_OFF];
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eRS485ErrorCode
eRS485MasterRTUSend( UCHAR ucPort,  UCHAR ucSlaveAddress,const UCHAR * pucFrame, USHORT usLength )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    USHORT          usCRC16;

    ENTER_CRITICAL_SECTION(  );

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState[ucPort] == STATE_M_RX_IDLE )
    {
        // /* Add two byte of CRC16*/
        // usLength += 2;
        //  /* First byte before the RS485-PDU is the Start. */
        // pucMasterSndBufferCur[ucPort] = ( UCHAR * ) pucFrame - 3;
        // usMasterSndBufferCount[ucPort] = 3;

        // /* Now copy the RS485bus-PDU into the RS485bus-Serial-Line-PDU. */
        // pucMasterSndBufferCur[ucPort][RS485_SER_PDU_START_OFF] = 0xAA;
        // pucMasterSndBufferCur[ucPort][RS485_SER_PDU_LENGTH_OFF] = ( UCHAR ) (usLength & 0xFF);
		// 		pucMasterSndBufferCur[ucPort][RS485_SER_PDU_LENGTH_OFF+1] = ( UCHAR ) (usLength >> 8) & 0xFF;
        // usMasterSndBufferCount[ucPort] += usLength;

        // /* Calculate CRC16 checksum for RS485bus-Serial-Line-PDU. */
        // usCRC16 = usRS485CRC16( ( UCHAR * ) pucMasterSndBufferCur[ucPort], usMasterSndBufferCount[ucPort] );
        // ucMasterRTUSndBuf[ucPort][usMasterSndBufferCount[ucPort]++] = ( UCHAR )( usCRC16 & 0xFF );
        // ucMasterRTUSndBuf[ucPort][usMasterSndBufferCount[ucPort]++] = ( UCHAR )( usCRC16 >> 8 );
  /* Mapping pointer of source data tp Buffer and offset lenght for STX and Slave ADR */
        pucMasterSndBufferCur[ucPort] = (UCHAR *)pucFrame - 1;
        pucMasterSndBufferCur[ucPort] = 1;
        /* First byte before the RS485-PDU is the Start. */

        /* Now copy the RS485Bus-PDU into the RS485Bus-Serial-Line-PDU. */
        // pucSndBufferCur[ucPort][RS485_SER_PDU_START_OFF] = RS485_SER_PDU_START_BYTE;
        pucMasterSndBufferCur[ucPort][RS485_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        // pucSndBufferCur[ucPort][RS485_SER_PDU_LENGTH_OFF] = ( UCHAR ) (usLength & 0xFF);
        // 		pucSndBufferCur[ucPort][RS485_SER_PDU_LENGTH_OFF+1] = ( UCHAR ) (usLength >> 8) & 0xFF;
        usMasterSndBufferCount[ucPort] += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usRS485CRC16((UCHAR *)pucMasterSndBufferCur[ucPort], usMasterSndBufferCount[ucPort]);
        ucMasterRTUSndBuf[ucPort][usMasterSndBufferCount[ucPort]++] = (UCHAR)(usCRC16 & 0xFF);
        ucMasterRTUSndBuf[ucPort][usMasterSndBufferCount[ucPort]++] = (UCHAR)(usCRC16 >> 8);

        /* Activate the transmitter. */
        eSndState[ucPort] = STATE_M_TX_XMIT;
        vMasterPortSerialEnable(ucPort, FALSE, TRUE );
    }
    else
    {
        eStatus = RS485_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

BOOL
xRS485MasterRTUReceiveFSM( UCHAR ucPort )
{
    BOOL            xTaskNeedSwitch = FALSE;
    UCHAR           ucByte;

    assert_param(( eSndState[ucPort] == STATE_M_TX_IDLE ) || ( eSndState[ucPort] == STATE_M_TX_XFWR ));

    /* Always read the character. */
    ( void )xMasterPortSerialGetByte(ucPort, ( CHAR * ) & ucByte );

    switch ( eRcvState[ucPort] )
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_M_RX_INIT:
        vMasterPortTimersT35Enable( ucPort );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_M_RX_ERROR:
        vMasterPortTimersT35Enable( ucPort );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE and disable early
         * the timer of respond timeout .
         */
    case STATE_M_RX_IDLE:
    	/* In time of respond timeout,the receiver receive a frame.
    	 * Disable timer of respond timeout and change the transmiter state to idle.
    	 */
    	// vMasterPortTimersDisable( ucPort ); // check this line 
    	// eSndState[ucPort] = STATE_M_TX_IDLE;

        usMasterRcvBufferPos[ucPort] = 0;
        ucMasterRTURcvBuf[ucPort][usMasterRcvBufferPos[ucPort]++] = ucByte;
        eRcvState[ucPort] = STATE_M_RX_RCV;

        /* Enable t3.5 timers. */
        vMasterPortTimersT35Enable( ucPort );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a rs485bus frame is received the frame is
         * ignored.
         */
    case STATE_M_RX_RCV:
        if( usMasterRcvBufferPos[ucPort] < RS485_SER_PDU_SIZE_MAX )
        {
            ucMasterRTURcvBuf[ucPort][usMasterRcvBufferPos[ucPort]++] = ucByte;
        }
        else
        {
            eRcvState[ucPort] = STATE_M_RX_ERROR;
        }
        vMasterPortTimersT35Enable(ucPort);
        break;
		default:
				break;
    }
    return xTaskNeedSwitch;
}

BOOL
xRS485MasterRTUTransmitFSM( UCHAR ucPort )
{
    BOOL            xNeedPoll = FALSE;

    assert_param( eRcvState[ucPort] == STATE_M_RX_IDLE );

    switch ( eSndState[ucPort] )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_M_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMasterPortSerialEnable( ucPort,TRUE, FALSE );
        break;

    case STATE_M_TX_XMIT:
        /* check if we are finished. */
        if( usMasterSndBufferCount[ucPort] != 0 )
        {
            xMasterPortSerialPutByte( ucPort,( CHAR )*pucMasterSndBufferCur[ucPort] );
            pucMasterSndBufferCur[ucPort]++;  /* next byte in sendbuffer. */
            usMasterSndBufferCount[ucPort]--;
        }
        else
        {
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMasterPortSerialEnable( ucPort, TRUE, FALSE );
            eSndState[ucPort] = STATE_M_TX_XFWR;
            /* Master is enable timer of respond timeout. */
            vMasterPortTimersRespondTimeoutEnable( ucPort);
        }
        break;
		default:
			break;
    }

    return xNeedPoll;
}

BOOL
xRS485MasterRTUTimerExpired(UCHAR ucPort)
{
	BOOL xNeedPoll = FALSE;

	switch (eRcvState[ucPort])
	{
		/* Timer t35 expired. Startup phase is finished. */
	case STATE_M_RX_INIT:
		xNeedPoll = xMasterPortEventPost(ucPort, EV_MASTER_READY);
		break;

		/* A frame was received and t35 expired. Notify the listener that
		 * a new frame was received. */
	case STATE_M_RX_RCV:
		xNeedPoll = xMasterPortEventPost(ucPort, EV_MASTER_FRAME_RECEIVED);
		break;

		/* An error occured while receiving the frame. */
	case STATE_M_RX_ERROR:
		vMasterSetErrorType(ucPort,EV_ERROR_RECEIVE_DATA);
		xNeedPoll = xMasterPortEventPost( ucPort, EV_MASTER_ERROR_PROCESS );
		break;

		/* Function called in an illegal state. */
	default:
		assert_param(
				( eRcvState[ucPort] == STATE_M_RX_INIT ) || ( eRcvState[ucPort] == STATE_M_RX_RCV ) ||
				( eRcvState[ucPort] == STATE_M_RX_ERROR ) || ( eRcvState[ucPort] == STATE_M_RX_IDLE ));
		break;
	}
	eRcvState[ucPort] = STATE_M_RX_IDLE;

	switch (eSndState[ucPort])
	{
		/* A frame was send finish and convert delay or respond timeout expired.
		 * If the frame is broadcast,The master will idle,and if the frame is not
		 * broadcast.Notify the listener process error.*/
	case STATE_M_TX_XFWR:
		if ( xFrameIsBroadcast[ucPort] == FALSE ) {
			vMasterSetErrorType( ucPort,EV_ERROR_RESPOND_TIMEOUT);
			xNeedPoll = xMasterPortEventPost( ucPort, EV_MASTER_ERROR_PROCESS);
		}
		break;
		/* Function called in an illegal state. */
	default:
		assert_param(
				( eSndState[ucPort] == STATE_M_TX_XFWR ) || ( eSndState[ucPort] == STATE_M_TX_IDLE ));
		break;
	}
	eSndState[ucPort] = STATE_M_TX_IDLE;

	vMasterPortTimersDisable( ucPort);
	/* If timer mode is convert delay, the master event then turns EV_MASTER_EXECUTE status. */
	if (eMasterCurTimerMode[ucPort] == RS485_TMODE_CONVERT_DELAY) {
		xNeedPoll = xMasterPortEventPost( ucPort, EV_MASTER_EXECUTE );
	}

	return xNeedPoll;
}

/* Get RS485bus Master send RTU's buffer address pointer.*/
void vRS485MasterGetRTUSndBuf(UCHAR ucPort, UCHAR ** pucFrame )
{
	*pucFrame = ( UCHAR * ) ucMasterRTUSndBuf[ucPort];
}

/* Get RS485bus Master send PDU's buffer address pointer.*/
void vRS485MasterGetPDUSndBuf( UCHAR ucPort, UCHAR ** pucFrame )
{
	*pucFrame = ( UCHAR * ) &ucMasterRTUSndBuf[ucPort][RS485_SER_PDU_PDU_OFF];
}

/* Set RS485bus Master send PDU's buffer length.*/
void vRS485MasterSetPDUSndLength(UCHAR ucPort, USHORT SendPDULength )
{
	usMasterSendPDULength[ucPort] = SendPDULength;
}

/* Get RS485bus Master send PDU's buffer length.*/
USHORT usRS485MasterGetPDUSndLength( UCHAR ucPort )
{
	return usMasterSendPDULength[ucPort];
}

/* Set RS485bus Master current timer mode.*/
void vMasterSetCurTimerMode(UCHAR ucPort, eRS485MasterTimerMode eRS485TimerMode )
{
	eMasterCurTimerMode[ucPort] = eRS485TimerMode;
}

#endif

