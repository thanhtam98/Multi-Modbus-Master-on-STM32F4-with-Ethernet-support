/* 
 * RS485bus Libary: A portable Modbus implementation for RS485bus ASCII/RTU.
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: rs485rtu_m.c,v 1.60 2013/08/20 11:18:10 Armink Add Master Functions $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "user_debug.h"

/* ----------------------- RS485bus includes ----------------------------------*/

#include "rs485.h"
#include "rs485_m.h"
#include "rs485config.h"
#include "rs485frame.h"
#include "rs485proto.h"
#include "rs485func.h"

#include "rs485port.h"
#if RS485_MASTER_RTU_ENABLED == 1
#include "rs485rtu.h"
#endif
#if RS485_MASTER_ASCII_ENABLED == 1
#include "rs485ascii.h"
#endif

#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0

#ifndef RS485_PORT_HAS_CLOSE
#define RS485_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/
static UCHAR    ucRS485MasterDestAddress;
static BOOL     xRS485RunInMasterMode[RS485_PORT_NUMBER] ;
static eMasterErrorEventType eMasterCurErrorType[RS485_PORT_NUMBER];
static UCHAR ucMBMasterDestAddress[RS485_PORT_NUMBER];
static enum
{
    STATE_ENABLED,
    STATE_DISABLED,
    STATE_NOT_INITIALIZED,
    STATE_ESTABLISHED,
} eRS485State = STATE_NOT_INITIALIZED;

/* Functions pointer which are initialized in eRS485Init( ). Depending on the
 * mode (RTU or ASCII) the are set to the correct implementations.
 * Using for RS485bus Master,Add by Armink 20130813
 */
static peRS485FrameSend peRS485MasterFrameSendCur;
static pvRS485FrameStart pvRS485MasterFrameStartCur;
static pvRS485FrameStop pvRS485MasterFrameStopCur;
static peRS485FrameReceive peRS485MasterFrameReceiveCur;
static pvRS485FrameClose pvRS485MasterFrameCloseCur;

/* Callback functions required by the porting layer. They are called when
 * an external event has happend which includes a timeout or the reception
 * or transmission of a character.
 * Using for RS485bus Master,Add by Armink 20130813
 */
BOOL( *pxMasterFrameCBByteReceived ) ( UCHAR ucPort );
BOOL( *pxMasterFrameCBTransmitterEmpty ) ( UCHAR ucPort );
BOOL( *pxMasterPortCBTimerExpired ) ( UCHAR ucPort );

BOOL( *pxRS485MasterFrameCBReceiveFSMCur ) ( UCHAR ucPort );
BOOL( *pxRS485MasterFrameCBTransmitFSMCur ) ( UCHAR ucPort );

/* An array of RS485bus functions handlers which associates RS485bus function
 * codes with implementing functions.
 */
static xRS485MasterFunctionHandler xMasterFuncHandlers[RS485_FUNC_HANDLERS_MAX]
	= {
#if RS485_FUNC_CHECK> 0
{FUNC_CHECK,FRAME_RESPONSE, eRS485MasterFuncCheck},
#endif
#if RS485_FUNC_PERIODIC_PING> 0
    {FUNC_PERIODIC_PING, eRS485MasterFuncPeriodicPing},
#endif
#if RS485_FUNC_TAMPER_DETECT> 0
    {FUNC_TAMPER_DETECT, eRS485MasterFuncTemperDetec},
#endif
#if RS485_FUNC_NFC_DETECT > 0
    {FUNC_NFC_DETECT, eRS485MasterFuncNFCDetect},
#endif
#if RS485_FUNC_UNKNOWN_ERROR  > 0
    {FUNC_UNKNOWN_ERROR, eRS485MasterFuncErrorDetect},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
 {MB_FUNC_WRITE_REGISTER ,eRS485MasterFuncWriteHoldingRegister}
};
#endif

/* ----------------------- Start implementation -----------------------------*/
eRS485ErrorCode
eRS485MasterInit( eRS485Mode eMode, ULONG ulBaudRate, eParity eParity )
{
	UCHAR port;
  eRS485ErrorCode    eStatus = RS485_ENOERR;

	switch (eMode)
	{
#if RS485_MASTER_RTU_ENABLED > 0
	case RS485_RTU:
		pvRS485MasterFrameStartCur = eRS485MasterRTUStart;
		pvRS485MasterFrameStopCur = eRS485MasterRTUStop;
		peRS485MasterFrameSendCur = eRS485MasterRTUSend;
		peRS485MasterFrameReceiveCur = eRS485MasterRTUReceive;
		pvRS485MasterFrameCloseCur = RS485_PORT_HAS_CLOSE ? vMasterPortClose : NULL;
		pxMasterFrameCBByteReceived = xRS485MasterRTUReceiveFSM;
		pxMasterFrameCBTransmitterEmpty = xRS485MasterRTUTransmitFSM;
		pxMasterPortCBTimerExpired = xRS485MasterRTUTimerExpired;
		for(port=0;port<RS485_PORT_NUMBER;port++)
		{
			eStatus = eRS485MasterRTUInit(port, ulBaudRate, eParity);
		//	
      /* initialize the OS resource for rs485bus master. */
      if (eStatus == RS485_ENOERR)
      {
        vMasterOsResInit(port);
			
      }
			
		}
		break;
#endif
#if RS485_MASTER_ASCII_ENABLED > 0
		case RS485_ASCII:
		pvRS485MasterFrameStartCur = eRS485MasterASCIIStart;
		pvRS485MasterFrameStopCur = eRS485MasterASCIIStop;
		peRS485MasterFrameSendCur = eRS485MasterASCIISend;
		peRS485MasterFrameReceiveCur = eRS485MasterASCIIReceive;
		pvRS485MasterFrameCloseCur = RS485_PORT_HAS_CLOSE ? vMasterPortClose : NULL;
		pxMasterFrameCBByteReceived = xRS485MasterASCIIReceiveFSM;
		pxMasterFrameCBTransmitterEmpty = xRS485MasterASCIITransmitFSM;
		pxMasterPortCBTimerExpired = xRS485MasterASCIITimerT1SExpired;
		for(port=0;port<RS485_PORT_NUMBER;port++)
		{
			eStatus = eRS485MasterASCIIInit(ucPort, ulBaudRate, eParity );
		}
		break;
#endif
	default:
		eStatus = RS485_EINVAL;
		break;
	}

	if (eStatus == RS485_ENOERR)
	{
		if (!xMasterPortEventInit())
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
eRS485MasterClose( UCHAR ucPort )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;

    if( eRS485State == STATE_DISABLED )
    {
        if( pvRS485MasterFrameCloseCur != NULL )
        {
            pvRS485MasterFrameCloseCur( ucPort );
        }
    }
    else
    {
        eStatus = RS485_EILLSTATE;
    }
    return eStatus;
}

eRS485ErrorCode
eRS485MasterEnable( UCHAR ucPort )
{
    eRS485ErrorCode    eStatus = RS485_ENOERR;
		pvRS485MasterFrameStartCur(ucPort);	
    if( eRS485State == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        
        eRS485State = STATE_ENABLED;
    }
    else
    {
        eStatus = RS485_EILLSTATE;
    }
    return eStatus;
}

eRS485ErrorCode
eRS485MasterDisable( UCHAR ucPort )
{
    eRS485ErrorCode    eStatus;

    if(( eRS485State == STATE_ENABLED ) || ( eRS485State == STATE_ESTABLISHED))
    {
        pvRS485MasterFrameStopCur( ucPort );
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

BOOL
eRS485MasterIsEstablished( void )
{
    if(eRS485State == STATE_ESTABLISHED)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}


eRS485ErrorCode
eRS485MasterPoll( void )
{
    static UCHAR   *ucRS485Frame;
    static UCHAR    ucRcvAddress;
    static UCHAR    ucFunctionCode;
    static USHORT   usLength;
    static eRS485Exception eException;

    int             i , j;
    eRS485ErrorCode    eStatus = RS485_ENOERR;
    PortMasterEventType    eEvent;
    eMasterErrorEventType errorType;

    /* Check if the protocol stack is ready. */
    if(( eRS485State != STATE_ENABLED ) && ( eRS485State != STATE_ESTABLISHED))
    {
        return RS485_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMasterPortEventGet( &eEvent ) == TRUE )
    {
        switch ( eEvent.Event )
        {
        case EV_MASTER_READY:
            eRS485State = STATE_ESTABLISHED;
            break;

        case EV_MASTER_FRAME_RECEIVED:
            eStatus = peRS485MasterFrameReceiveCur( eEvent.Port, &ucRcvAddress ,&ucRS485Frame, &usLength );
            /* Check if the frame is for us. If not ,send an error process event. */
            if ( eStatus == RS485_ENOERR )
            {
                ( void ) xMasterPortEventPost(eEvent.Port, EV_MASTER_EXECUTE );
							 break;
            }
            else
            {
                vMasterSetErrorType(eEvent.Port,EV_ERROR_RECEIVE_DATA);
                ( void ) xMasterPortEventPost( eEvent.Port, EV_MASTER_ERROR_PROCESS );
								eEvent.Event = EV_MASTER_ERROR_PROCESS;
								break;
            }
           

        case EV_MASTER_EXECUTE:
				{
            ucFunctionCode = ucRS485Frame[RS485_PDU_FUNC_OFF];
            eException = RS485_EX_ILLEGAL_FUNCTION;
            /* If receive frame has exception .The receive function code highest bit is 1.*/
            if(ucFunctionCode > 7) {
            	eException = (eRS485Exception)ucRS485Frame[RS485_PDU_DATA_OFF];
            }
            else
            {
              for (i = 0; i < RS485_FUNC_HANDLERS_MAX; i++)
              {
                /* No more function handlers registered. Abort. */
                if (xMasterFuncHandlers[i].ucFunctionCode == 0)	{
                  break;
                }
                else if (xMasterFuncHandlers[i].ucFunctionCode == ucFunctionCode) {
                  vRS485MasterSetCBRunInMasterMode(eEvent.Port,TRUE);
                  /* The master need execute function for all slave.
                   */

                  eException = xMasterFuncHandlers[i].pxHandler(eEvent.Port,ucRcvAddress,ucRS485Frame, &usLength);
                  vRS485MasterSetCBRunInMasterMode(eEvent.Port,FALSE);
                  break;
                }
              }
            }
            /* If master has exception ,Master will send error process.Otherwise the Master is idle.*/
            if (eException != RS485_EX_NONE) {
            	vMasterSetErrorType(eEvent.Port,EV_ERROR_EXECUTE_FUNCTION);
            	( void ) xMasterPortEventPost(eEvent.Port, EV_MASTER_ERROR_PROCESS );
            }
            else {
            	vMasterCBRequestScuuess(eEvent.Port );
            	vMasterRunResRelease(eEvent.Port);
            }
            break;
					}
        case EV_MASTER_FRAME_SENT:
            /* Master is busy now. */
            vRS485MasterGetPDUSndBuf( eEvent.Port,&ucRS485Frame );
            eStatus = eRS485MasterRTUSend( eEvent.Port,ucRS485MasterGetDestAddress(eEvent.Port) ,ucRS485Frame, usRS485MasterGetPDUSndLength( eEvent.Port) );
            break;

        case EV_MASTER_ERROR_PROCESS:
            /* Execute specified error process callback function. */
            errorType = eMasterGetErrorType(eEvent.Port);
            vRS485MasterGetPDUSndBuf(eEvent.Port, &ucRS485Frame );
						DBG("\n\r RS485Bus_m.c Tao bug roi ne ");
            switch (errorType) {
              case EV_ERROR_RESPOND_TIMEOUT:
                vMasterErrorCBRespondTimeout(eEvent.Port,
                    ucRS485Frame, usRS485MasterGetPDUSndLength(eEvent.Port));
                break;
              case EV_ERROR_RECEIVE_DATA:
                vMasterErrorCBReceiveData(eEvent.Port,
                    ucRS485Frame, usRS485MasterGetPDUSndLength(eEvent.Port));
                break;
              case EV_ERROR_EXECUTE_FUNCTION:
                vMasterErrorCBExecuteFunction(eEvent.Port,
                    ucRS485Frame, usRS485MasterGetPDUSndLength(eEvent.Port));
                break;
            }
            vMasterRunResRelease(eEvent.Port);
            break;
		default:
			break;
        }
    }
    return RS485_ENOERR;
}

/* Get whether the RS485bus Master is run in master mode.*/
BOOL xRS485MasterGetCBRunInMasterMode( UCHAR ucPort )
{
	return xRS485RunInMasterMode[ucPort];
}
/* Set whether the RS485bus Master is run in master mode.*/
void vRS485MasterSetCBRunInMasterMode(UCHAR ucPort, BOOL IsMasterMode )
{
	xRS485RunInMasterMode[ucPort] = IsMasterMode;
}
/* Get Modbus Master send destination address. */
UCHAR ucRS485MasterGetDestAddress( UCHAR ucPort )
{
	return ucMBMasterDestAddress[ucPort];
}
/* Set Modbus Master send destination address. */
void vRS485MasterSetDestAddress(UCHAR ucPort, UCHAR Address )
{
	ucMBMasterDestAddress[ucPort] = Address;
}
/* Get RS485bus Master current error event type. */
eMasterErrorEventType eMasterGetErrorType( UCHAR ucPort )
{
	return eMasterCurErrorType[ucPort];
}
/* Set RS485bus Master current error event type. */
void vMasterSetErrorType(UCHAR ucPort, eMasterErrorEventType errorType )
{
	eMasterCurErrorType[ucPort] = errorType;
}

#endif //RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
