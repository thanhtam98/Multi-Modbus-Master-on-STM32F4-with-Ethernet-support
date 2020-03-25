/* 
 * RS485bus Libary: A portable RS485bus implementation for RS485bus ASCII/RTU.
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
 * File: $Id: rs485ascii.c,v 1.15 2007/02/18 23:46:48 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "rs485.h"
#include "rs485config.h"
#include "rs485ascii.h"
#include "rs485frame.h"

#include "rs485crc.h"
#include "rs485port.h"

#if RS485_SLAVE_ASCII_ENABLED > 0

/* ----------------------- Defines ------------------------------------------*/
#define RS485_ASCII_DEFAULT_CR     '\r'    /*!< Default CR character for Modbus ASCII. */
#define RS485_ASCII_DEFAULT_LF     '\n'    /*!< Default LF character for Modbus ASCII. */
#define RS485_SER_PDU_SIZE_MIN     3       /*!< Minimum size of a Modbus ASCII frame. */
#define RS485_SER_PDU_SIZE_MAX     256     /*!< Maximum size of a  ASCII frame. */
#define RS485_SER_PDU_SIZE_LRC     1       /*!< Size of LRC field in PDU. */
#define RS485_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define RS485_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STATE_RX_IDLE,              /*!< Receiver is in idle state. */
    STATE_RX_RCV,               /*!< Frame is beeing received. */
    STATE_RX_WAIT_EOF           /*!< Wait for End of Frame. */
} eRS485RcvState;

typedef enum
{
    STATE_TX_IDLE,              /*!< Transmitter is in idle state. */
    STATE_TX_START,             /*!< Starting transmission (':' sent). */
    STATE_TX_DATA,              /*!< Sending of data (Address, Data, LRC). */
    STATE_TX_END,               /*!< End of transmission. */
    STATE_TX_NOTIFY             /*!< Notify sender that the frame has been sent. */
} eRS485SndState;

typedef enum
{
    BYTE_HIGH_NIBBLE,           /*!< Character for high nibble of byte. */
    BYTE_LOW_NIBBLE             /*!< Character for low nibble of byte. */
} eRS485BytePos;

/* ----------------------- Static functions ---------------------------------*/
static UCHAR    prvucRS485CHAR2BIN( UCHAR ucCharacter );

static UCHAR    prvucRS485BIN2CHAR( UCHAR ucByte );

static UCHAR    prvucRS485LRC( UCHAR * pucFrame, USHORT usLen );

/* ----------------------- Static variables ---------------------------------*/
static volatile eRS485SndState eSndState;
static volatile eRS485RcvState eRcvState;

/* We reuse the Modbus RTU buffer because only one buffer is needed and the
 * RTU buffer is bigger. */
extern volatile UCHAR ucRTUBuf[];
static volatile UCHAR *ucASCIIBuf = ucRTUBuf;

static volatile USHORT usRcvBufferPos;
static volatile eRS485BytePos eBytePos;

static volatile UCHAR *pucSndBufferCur;
static volatile USHORT usSndBufferCount;

static volatile UCHAR ucLRC;
static volatile UCHAR ucRS485LFCharacter;

/* ----------------------- Start implementation -----------------------------*/
eRS485ErrorCode
eRS485ASCIIInit( UCHAR ucPort, ULONG ulBaudRate, eParity eParity )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    
    ENTER_CRITICAL_SECTION(  );
    ucRS485LFCharacter = RS485_ASCII_DEFAULT_LF;

    if( xPortSerialInit( ucPort, ulBaudRate, 7, eParity ) != TRUE )
    {
        eStatus = RS485_EPORTERR;
    }
    else if( xPortTimersInit( ucPort, RS485_ASCII_TIMEOUT_SEC * 20000UL ) != TRUE )
    {
        eStatus = RS485_EPORTERR;
    }

    EXIT_CRITICAL_SECTION(  );

    return eStatus;
}

void
eRS485ASCIIStart( UCHAR ucPort )
{
    ENTER_CRITICAL_SECTION(  );
    vPortSerialEnable( ucPort, TRUE, FALSE );
    eRcvState = STATE_RX_IDLE;
    EXIT_CRITICAL_SECTION(  );

    /* No special startup required for ASCII. */
    ( void )xPortEventPost( ucPort, EV_READY );
}

void
eRS485ASCIIStop( UCHAR ucPort )
{
    ENTER_CRITICAL_SECTION(  );
    vPortSerialEnable(ucPort, FALSE, FALSE );
    vPortTimersDisable( ucPort );
    EXIT_CRITICAL_SECTION(  );
}

eRS485ErrorCode
eRS485ASCIIReceive( UCHAR ucPort, UCHAR ** pucFrame, USHORT * pusLength )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;

    ENTER_CRITICAL_SECTION(  );
    assert( usRcvBufferPos < RS485_SER_PDU_SIZE_MAX );

    /* Length and CRC check */
    if( ( usRcvBufferPos >= RS485_SER_PDU_SIZE_MIN )
        && ( prvucRS485LRC( ( UCHAR * ) ucASCIIBuf, usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pusLength = ucASCIIBuf[RS485_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( USHORT )( usRcvBufferPos - RS485_SER_PDU_PDU_OFF - RS485_SER_PDU_SIZE_LRC );

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( UCHAR * ) & ucASCIIBuf[RS485_SER_PDU_PDU_OFF];
    }
    else
    {
        eStatus = RS485_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

eRS485ErrorCode
eRS485ASCIISend( UCHAR ucPort, const UCHAR * pucFrame, USHORT usLength )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    UCHAR           usLRC;

    ENTER_CRITICAL_SECTION(  );
    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( eRcvState == STATE_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        pucSndBufferCur = ( UCHAR * ) pucFrame - 1;
        usSndBufferCount = 1;

        /* Now copy the RS485bus-PDU into the RS485bus-Serial-Line-PDU. */
        pucSndBufferCur[RS485_SER_PDU_ADDR_OFF] = ucPort;
        usSndBufferCount += usLength;

        /* Calculate LRC checksum for Modbus-Serial-Line-PDU. */
        usLRC = prvucRS485LRC( ( UCHAR * ) pucSndBufferCur, usSndBufferCount );
        ucASCIIBuf[usSndBufferCount++] = usLRC;

        /* Activate the transmitter. */
        eSndState = STATE_TX_START;
        vPortSerialEnable(ucPort, FALSE, TRUE );
    }
    else
    {
        eStatus = RS485_EIO;
    }
    EXIT_CRITICAL_SECTION(  );
    return eStatus;
}

BOOL
xRS485ASCIIReceiveFSM( UCHAR ucPort )
{
    BOOL            xNeedPoll = FALSE;
    UCHAR           ucByte;
    UCHAR           ucResult;

    assert( eSndState == STATE_TX_IDLE );

     ( void )xPortSerialGetByte(ucPort, ( CHAR * ) & ucByte );;
    switch ( eRcvState )
    {
        /* A new character is received. If the character is a ':' the input
         * buffer is cleared. A CR-character signals the end of the data
         * block. Other characters are part of the data block and their
         * ASCII value is converted back to a binary representation.
         */
    case STATE_RX_RCV:
        /* Enable timer for character timeout. */
        vPortTimersEnable( ucPort );
        if( ucByte == ':' )
        {
            /* Empty receive buffer. */
            eBytePos = BYTE_HIGH_NIBBLE;
            usRcvBufferPos = 0;
        }
        else if( ucByte == RS485_ASCII_DEFAULT_CR )
        {
            eRcvState = STATE_RX_WAIT_EOF;
        }
        else
        {
            ucResult = prvucRS485CHAR2BIN( ucByte );
            switch ( eBytePos )
            {
                /* High nibble of the byte comes first. We check for
                 * a buffer overflow here. */
            case BYTE_HIGH_NIBBLE:
                if( usRcvBufferPos < RS485_SER_PDU_SIZE_MAX )
                {
                    ucASCIIBuf[usRcvBufferPos] = ( UCHAR )( ucResult << 4 );
                    eBytePos = BYTE_LOW_NIBBLE;
                    break;
                }
                else
                {
                    /* not handled in Modbus specification but seems
                     * a resonable implementation. */
                    eRcvState = STATE_RX_IDLE;
                    /* Disable previously activated timer because of error state. */
                    vPortTimersDisable( ucPort );
                }
                break;

            case BYTE_LOW_NIBBLE:
                ucASCIIBuf[usRcvBufferPos++] |= ucResult;
                eBytePos = BYTE_HIGH_NIBBLE;
                break;
            }
        }
        break;

    case STATE_RX_WAIT_EOF:
        if( ucByte == ucRS485LFCharacter )
        {
            /* Disable character timeout timer because all characters are
             * received. */
            vPortTimersDisable( ucPort );
            /* Receiver is again in idle state. */
            eRcvState = STATE_RX_IDLE;

            /* Notify the caller of eMBASCIIReceive that a new frame
             * was received. */
            xNeedPoll = xPortEventPost( ucPort, EV_FRAME_RECEIVED );
        }
        else if( ucByte == ':' )
        {
            /* Empty receive buffer and back to receive state. */
            eBytePos = BYTE_HIGH_NIBBLE;
            usRcvBufferPos = 0;
            eRcvState = STATE_RX_RCV;

            /* Enable timer for character timeout. */
            vPortTimersEnable( ucPort );
        }
        else
        {
            /* Frame is not okay. Delete entire frame. */
            eRcvState = STATE_RX_IDLE;
        }
        break;

    case STATE_RX_IDLE:
        if( ucByte == ':' )
        {
            /* Enable timer for character timeout. */
            vPortTimersEnable( ucPort );
            /* Reset the input buffers to store the frame. */
            usRcvBufferPos = 0;;
            eBytePos = BYTE_HIGH_NIBBLE;
            eRcvState = STATE_RX_RCV;
        }
        break;
    }

    return xNeedPoll;
}

BOOL
xRS485ASCIITransmitFSM( UCHAR ucPort )
{
    BOOL            xNeedPoll = FALSE;
    UCHAR           ucByte;

    assert( eRcvState == STATE_RX_IDLE );
    switch ( eSndState )
    {
        /* Start of transmission. The start of a frame is defined by sending
         * the character ':'. */
    case STATE_TX_START:
        ucByte = ':';
        xPortSerialPutByte( ucPort, ( CHAR )ucByte );
        eSndState = STATE_TX_DATA;
        eBytePos = BYTE_HIGH_NIBBLE;
        break;

        /* Send the data block. Each data byte is encoded as a character hex
         * stream with the high nibble sent first and the low nibble sent
         * last. If all data bytes are exhausted we send a '\r' character
         * to end the transmission. */
    case STATE_TX_DATA:
        if( usSndBufferCount > 0 )
        {
            switch ( eBytePos )
            {
            case BYTE_HIGH_NIBBLE:
                ucByte = prvucRS485BIN2CHAR( ( UCHAR )( *pucSndBufferCur >> 4 ) );
                xPortSerialPutByte( ucPort,( CHAR ) ucByte );
                eBytePos = BYTE_LOW_NIBBLE;
                break;

            case BYTE_LOW_NIBBLE:
                ucByte = prvucRS485BIN2CHAR( ( UCHAR )( *pucSndBufferCur & 0x0F ) );
                xPortSerialPutByte( ucPort,( CHAR )ucByte );
                pucSndBufferCur++;
                eBytePos = BYTE_HIGH_NIBBLE;
                usSndBufferCount--;
                break;
            }
        }
        else
        {
            xPortSerialPutByte( ucPort, RS485_ASCII_DEFAULT_CR );
            eSndState = STATE_TX_END;
        }
        break;

        /* Finish the frame by sending a LF character. */
    case STATE_TX_END:
        xPortSerialPutByte( ucPort,( CHAR )ucRS485LFCharacter );
        /* We need another state to make sure that the CR character has
         * been sent. */
        eSndState = STATE_TX_NOTIFY;
        break;

        /* Notify the task which called eMBASCIISend that the frame has
         * been sent. */
    case STATE_TX_NOTIFY:
        eSndState = STATE_TX_IDLE;
        xNeedPoll = xPortEventPost( ucPort, EV_FRAME_SENT );

        /* Disable transmitter. This prevents another transmit buffer
         * empty interrupt. */
        vPortSerialEnable( ucPort, TRUE, FALSE );
        eSndState = STATE_TX_IDLE;
        break;

        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vPortSerialEnable( ucPort, TRUE, FALSE );
        break;
    }

    return xNeedPoll;
}

BOOL
xRS485ASCIITimerT1SExpired( UCHAR ucPort )
{
    switch ( eRcvState )
    {
        /* If we have a timeout we go back to the idle state and wait for
         * the next frame.
         */
    case STATE_RX_RCV:
    case STATE_RX_WAIT_EOF:
        eRcvState = STATE_RX_IDLE;
        break;

    default:
        assert( ( eRcvState == STATE_RX_RCV ) || ( eRcvState == STATE_RX_WAIT_EOF ) );
        break;
    }
    vPortTimersDisable( ucPort );

    /* no context switch required. */
    return FALSE;
}


static          UCHAR
prvucRS485CHAR2BIN( UCHAR ucCharacter )
{
    if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) )
    {
        return ( UCHAR )( ucCharacter - '0' );
    }
    else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) )
    {
        return ( UCHAR )( ucCharacter - 'A' + 0x0A );
    }
    else
    {
        return 0xFF;
    }
}

static          UCHAR
prvucRS485BIN2CHAR( UCHAR ucByte )
{
    if( ucByte <= 0x09 )
    {
        return ( UCHAR )( '0' + ucByte );
    }
    else if( ( ucByte >= 0x0A ) && ( ucByte <= 0x0F ) )
    {
        return ( UCHAR )( ucByte - 0x0A + 'A' );
    }
    else
    {
        /* Programming error. */
        assert( 0 );
    }
    return '0';
}


static          UCHAR
prvucRS485LRC( UCHAR * pucFrame, USHORT usLen )
{
    UCHAR           ucLRC = 0;  /* LRC char initialized */

    while( usLen-- )
    {
        ucLRC += *pucFrame++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( UCHAR ) ( -( ( CHAR ) ucLRC ) );
    return ucLRC;
}

#endif
