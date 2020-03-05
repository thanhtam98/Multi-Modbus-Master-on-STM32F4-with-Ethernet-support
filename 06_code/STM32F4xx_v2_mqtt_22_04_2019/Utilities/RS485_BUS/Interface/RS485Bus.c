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
 * File: $Id: mb.c,v 1.27 2007/02/18 23:45:41 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "user_debug.h"
/* ----------------------- Modbus includes ----------------------------------*/

#include "rs485.h"
#include "rs485config.h"
#include "rs485frame.h"
#include "rs485proto.h"
#include "rs485func.h"

#include "rs485port.h"
#if RS485_SLAVE_RTU_ENABLED == 1
#include "rs485rtu.h"
#endif
#if RS485_SLAVE_ASCII_ENABLED == 1
#include "rs485ascii.h"
#endif
#include "rs485ascii.h"
#ifndef RS485_PORT_HAS_CLOSE
#define RS485_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/


static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED
} eRS485State = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eRS485Init( ).
 */
static peRS485FrameSend peRS485FrameSendCur;
static pvRS485FrameStart pvRS485FrameStartCur;
static pvRS485FrameStop pvRS485FrameStopCur;
static peRS485FrameReceive peRS485FrameReceiveCur;
static pvRS485FrameClose pvRS485FrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 * Using for RS485 Slave
 */
BOOL( *pxFrameCBByteReceived ) ( UCHAR ucPORT );
BOOL( *pxFrameCBTransmitterEmpty ) ( UCHAR ucPORT );
BOOL( *pxPortCBTimerExpired ) ( UCHAR ucPORT );

BOOL( *pxRS485FrameCBReceiveFSMCur ) ( UCHAR ucPORT );
BOOL( *pxRS485FrameCBTransmitFSMCur ) ( UCHAR ucPORT );

/* An array of RS485bus functions handlers which associates RS485bus function
 * codes with implementing functions.
 */
static xRS485FunctionHandler xFuncHandlers[RS485_FUNC_HANDLERS_MAX] = {
#if RS485_FUNC_CHECK > 0
    {FUNC_CHECK, eRS485FuncCheck},
#endif
#if RS485_FUNC_PERIODIC_PING > 0
    {FUNC_PERIODIC_PING, eRS485FuncPeriodicPing},
#endif
#if RS485_FUNC_TAMPER_DETECT > 0
    {FUNC_TAMPER_DETECT, eRS485FuncTemperDetect},
#endif
#if RS485_FUNC_NFC_DETECT > 0
    {FUNC_NFC_DETECT, eRS485FuncNFCDetect},
#endif
#if RS485_FUNC_KEY_ENTER >0
    {FUNC_KEY_ENTER, eRS485FuncKeyEnter},
#endif
};

/* ----------------------- Start implementation -----------------------------*/
eRS485ErrorCode
eRS485Init( eRS485Mode eMode, ULONG ulBaudRate, eParity eParity )
{
		UCHAR port;
    eRS485ErrorCode    eStatus = RS485_ENOERR;

		switch ( eMode )
		{
#if RS485_SLAVE_RTU_ENABLED > 0
			case RS485_RTU:
				pvRS485FrameStartCur = eRS485RTUStart;
				pvRS485FrameStopCur = eRS485RTUStop;
				peRS485FrameSendCur = eRS485RTUSend;
				peRS485FrameReceiveCur = eRS485RTUReceive;
				pvRS485FrameCloseCur = RS485_PORT_HAS_CLOSE ? vPortClose : NULL;
				pxFrameCBByteReceived = xRS485RTUReceiveFSM;
				pxFrameCBTransmitterEmpty = xRS485RTUTransmitFSM;
				pxPortCBTimerExpired = xRS485RTUTimerT35Expired;
				
				for(port=0;port<RS485_PORT_NUMBER;port++)
				{
					eStatus = eRS485RTUInit( port, ulBaudRate, eParity );
				}
				break;
#endif

#if RS485_SLAVE_ASCII_ENABLED > 0
			case RS485_ASCII:
				pvRS485FrameStartCur = eRS485ASCIIStart;
				pvRS485FrameStopCur = eRS485ASCIIStop;
				peRS485FrameSendCur = eRS485ASCIISend;
				peRS485FrameReceiveCur = eRS485ASCIIReceive;
				pvRS485FrameCloseCur = RS485_PORT_HAS_CLOSE ? vPortClose : NULL;
				pxFrameCBByteReceived = xRS485ASCIIReceiveFSM;
				pxFrameCBTransmitterEmpty = xRS485ASCIITransmitFSM;
				pxPortCBTimerExpired = xRS485ASCIITimerT1SExpired;
				
				for(port=0;port<RS485_PORT_NUMBER;port++)
				{
					eStatus = eRS485ASCIIInit( port, ulBaudRate, eParity );
				}
				break;
#endif			
			default:
        eStatus = RS485_EINVAL;
        break;
		}
		if( eStatus == RS485_ENOERR )
		{
				if( !xPortEventInit( ) )
				{
						/* port dependent event module initalization failed. */
						eStatus = RS485_EPORTERR;
				}
				else
				{
						eRS485State = STATE_DISABLED;
				}
		}
    return eStatus;
}



eRS485ErrorCode
eRS485Close( UCHAR Port )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;

    if( eRS485State == STATE_DISABLED )
    {
        if( pvRS485FrameCloseCur != NULL )
        {
            pvRS485FrameCloseCur(Port);
        }
    }
    else
    {
        eStatus = RS485_EILLSTATE;
    }
    return eStatus;
}


eRS485ErrorCode
eRS485Enable( void )
{
  UCHAR ucPort;
  eRS485ErrorCode  eStatus = RS485_ENOERR;

  if( eRS485State == STATE_DISABLED )
  {
      /* Activate the protocol stack. */
      for(ucPort=0;ucPort<RS485_PORT_NUMBER;ucPort++)
      {
        pvRS485FrameStartCur( ucPort );
      }
      eRS485State = STATE_ENABLED;
  }
  else
  {
      eStatus = RS485_EILLSTATE;
  }
  return eStatus;
}

eRS485ErrorCode
eRS485Disable( UCHAR ucPort )
{
    eRS485ErrorCode    eStatus;

    if( eRS485State == STATE_ENABLED )
    {
        pvRS485FrameStopCur( ucPort );
        eRS485State = STATE_DISABLED;
        eStatus = RS485_ENOERR;
    }
    else if( eRS485State == STATE_DISABLED )
    {
        eStatus = RS485_ENOERR;
    }
    else
    {
        eStatus = RS485_EILLSTATE;
    }
    return eStatus;
}

eRS485ErrorCode eRS485Poll( void )
{
    static UCHAR   *ucRS485Frame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eRS485Exception eException;

    int i;
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    PortEventType    eEvent;

    /* Check if the protocol stack is ready. */
    if( eRS485State != STATE_ENABLED )
    {
        return RS485_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xPortEventGet( &eEvent ) == TRUE )
    {
        switch ( eEvent.Event )
        {
        case EV_READY:
            break;

        case EV_FRAME_RECEIVED:
            eStatus = peRS485FrameReceiveCur( eEvent.Port,&ucRcvAddress, &ucRS485Frame, &usLength );

            if( eStatus == RS485_ENOERR )
            {
                ( void )xPortEventPost( eEvent.Port,EV_EXECUTE );
            }
            break;

        case EV_EXECUTE:
            ucFunctionCode = ucRS485Frame[RS485_PDU_FUNC_OFF];
            DBG("\r\n Frame recived: ");
//						for(uint8_t i = 0; i < usLength ; i++)
//							{
//								DBG(ucRS485Frame[i]);
//							}
//									
            eException = RS485_EX_ILLEGAL_FUNCTION;
            for( i = 0; i < RS485_FUNC_HANDLERS_MAX; i++ )
            {
                /* No more function handlers registered. Abort. */
                if( xFuncHandlers[i].ucFunctionCode == 0 )
                {
                    break;
                }
                else if( xFuncHandlers[i].ucFunctionCode == ucFunctionCode)
                {
                    eException = xFuncHandlers[i].pxHandler(eEvent.Port, ucRS485Frame, &usLength );
                    break;
                }
            }

            /* we return a reply.*/
						if( eException != RS485_EX_NONE )
						{
								/* An exception occured. Build an error frame. */
								usLength = 0;
								ucRS485Frame[usLength++] = ( UCHAR )( 0x00 );
								ucRS485Frame[usLength++] = ( UCHAR )( ucFunctionCode |FUNC_ERROR);
                                ucRS485Frame[usLength++] = ( UCHAR )( FRAME_RESPONSE );
								ucRS485Frame[usLength++] = eException;
						}
						eStatus = peRS485FrameSendCur( eEvent.Port,0x00,ucRS485Frame, usLength );
            break;

        case EV_FRAME_SENT:
            break;
        }
    }
    return RS485_ENOERR;
}

void xRS485Send(UCHAR Port, UCHAR ucSlaveAddress ,const UCHAR * pucFrame,  USHORT usLength)
	{
		   eRS485ErrorCode    eStatus = RS485_ENOERR;
				eStatus = peRS485FrameSendCur( Port,ucSlaveAddress,pucFrame, usLength );
	}