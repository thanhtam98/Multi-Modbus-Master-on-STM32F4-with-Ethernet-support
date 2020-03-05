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
 * File: $Id: rs485ascii.h,v 1.8 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _RS485_ASCII_H
#define _RS485_ASCII_H
#include "rs485config.h"
#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

#if RS485_SLAVE_ASCII_ENABLED > 0
eRS485ErrorCode    	eRS485ASCIIInit( UCHAR ucPort,
                              ULONG ulBaudRate, eParity eParity );
void            		eRS485ASCIIStart( UCHAR ucPort );
void            		eRS485ASCIIStop( UCHAR ucPort );

eRS485ErrorCode    	eRS485ASCIIReceive( UCHAR ucPort, UCHAR ** pucFrame,
                                 USHORT * pusLength );
eRS485ErrorCode    	eRS485ASCIISend( UCHAR ucPort, const UCHAR * pucFrame,
                              USHORT usLength );
BOOL            		xRS485ASCIIReceiveFSM( UCHAR ucPort );
BOOL            		xRS485ASCIITransmitFSM( UCHAR ucPort );
BOOL            		xRS485ASCIITimerT1SExpired( UCHAR ucPort );
#endif

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
