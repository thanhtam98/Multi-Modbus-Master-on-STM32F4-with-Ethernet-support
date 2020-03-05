/* 
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
 * File: $Id: rs485proto.h,v 1.14 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _RS485_PROTO_H
#define _RS485_PROTO_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
#define RS485_FUNC_HANDLERS_MAX          ( 5 )
/* ----------------------- Defines command function ------------------------------------------*/
#define FUNC_NONE             0x00
#define	FUNC_CHECK  					0x01
#define	FUNC_PERIODIC_PING  	0x02
#define	FUNC_TAMPER_DETECT   	0x03
#define	FUNC_NFC_DETECT 			0x04
#define FUNC_KEY_ENTER        0x05
#define	FUNC_ERROR  	        0x80
/* ----------------------- Type definitions ---------------------------------*/
/*! \ingroup RS485Bus
 * \brief FRAME_TYPE used by all function in the protocol stack.
 */
typedef enum
{
	FRAME_INVALID = 0x00,
	FRAME_REQUEST = 0x01,
	FRAME_RESPONSE = 0x02,
}FRAME_TYPE;

typedef enum
{
  CHECK_STATUS = 0x01,
  CHECK_UID_AUTHENTICATION = 0x02,
  CHECK_KEY_AUTHENTICATION = 0x03,
}PROTOCOL_CHECK;

typedef enum
{
	RS485_EX_ACK = 0x00,
	RS485_EX_NONE = 0x00,
	RS485_EX_UNRECOGNIZED_FRAME_TYPE = 0x01,
	RS485_EX_ILLEGAL_FUNCTION = 0x02,
	RS485_EX_LENGTH_NOT_MATCHING = 0x03,
	RS485_EX_BAD_CHECKSUM = 0x04,
	RS485_EX_ILLEGAL_DATA_VALUE = 0x05,
	RS485_EX_NAK = 0x06,
	RS485_EX_TIMEOUT = 0x07,
} eRS485Exception;

typedef eRS485Exception( *pxRS485FunctionHandler ) ( UCHAR ucPort, UCHAR * pucFrame, USHORT * pusLength );

typedef struct
{
    UCHAR ucFunctionCode;
    pxRS485FunctionHandler pxHandler;	
} xRS485FunctionHandler;

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
