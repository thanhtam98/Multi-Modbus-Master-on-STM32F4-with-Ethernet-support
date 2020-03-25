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
 * File: $Id: mbfunccoils.c,v 1.8 2007/02/18 23:47:16 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "user_debug.h"
/* -----------------------RS485bus includes ----------------------------------*/
#include "rs485.h"
#include "rs485_m.h"
#include "rs485frame.h"
#include "rs485proto.h"
#include "rs485config.h"
#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
#define HANDSHAKEBYTE	0x55
/* ----------------------- Static functions ---------------------------------*/
eRS485Exception    prveRS485Error2Exception( eRS485ErrorCode eErrorCode );

/* ----------------------- Start implementation -----------------------------*/

#if RS485_FUNC_PERIODIC_PING > 0

eRS485MasterReqErrCode
eRS485MasterReqPeriodicPing( UCHAR ucPort, LONG lTimeOut )
{
  UCHAR  *ucRS485Frame;
  eRS485MasterReqErrCode eErrStatus = RS485_MRE_NO_ERR;
  
  if(ucPort > RS485_PORT_NUMBER)  eErrStatus = RS485_MRE_ILL_ARG;
  else if(xMasterRunResTake( ucPort,lTimeOut ) == FALSE ) eErrStatus = RS485_MRE_MASTER_BUSY;
  else
  {
		vRS485MasterGetPDUSndBuf(ucPort,&ucRS485Frame);
		ucRS485Frame[RS485_PDU_FUNC_OFF] = FUNC_PERIODIC_PING;
		ucRS485Frame[RS485_PDU_DATA_OFF] = FRAME_PERIODIC_PING;
		 vRS485MasterSetPDUSndLength(ucPort,2);
		( void ) xMasterPortEventPost(ucPort, EV_MASTER_FRAME_SENT );
			eErrStatus = eMasterWaitRequestFinish( ucPort);
	}
  return eErrStatus;
}

eRS485Exception
eRS485MasterFuncPeriodicPing(UCHAR ucPort, UCHAR ucRcvAddress, UCHAR * pucFrame, USHORT * usLen )
{
    UCHAR           ucNBytes;
    UCHAR          *pucFrameCur;

    eRS485Exception    eStatus = RS485_EX_NONE;
    eRS485ErrorCode    eCheckStatus;
		if(pucFrame[RS485_PDU_FUNC_OFF] == FRAME_PERIODIC_PING)
		{
			DBG("Recived the repond");
			
//				/* Set the current PDU data pointer to the beginning. */
//				pucFrameCur = &pucFrame[RS485_PDU_FUNC_OFF];
//				*usLen = RS485_PDU_FUNC_OFF;
//				
//				*pucFrameCur++ = FUNC_CHECK;
//				*usLen +=1;
//				*pucFrameCur++ = FRAME_RESPONSE;
//				*usLen +=1;
//				
//				
//				eCheckStatus = eMasterFuncPeriodicPingCB( ucPort,pucFrameCur,&ucNBytes);

//				/* If an error occured convert it into a RS485 exception. */
//				if( eCheckStatus != RS485_ENOERR )
//				{
//						eStatus = prveRS485Error2Exception( eCheckStatus );
//				}
//				else
//				{
//						/* The response contains the function code, the starting address
//						 * and the quantity of registers. We reuse the old values in the 
//						 * buffer because they are still valid. */
//						*usLen += ucNBytes;;
//				}
		}
    else
    {
        /* Unrecognized Frame Type  */
        eStatus = RS485_EX_UNRECOGNIZED_FRAME_TYPE;
    }
    return eStatus;
}
#endif
#endif