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
 * File: $Id: rs485port.h,v 1.17 2006/12/07 22:10:34 wolti Exp $
 *            rs485port.h,v 1.60 2013/08/17 11:42:56 Armink Add Master Functions  $
 */

#ifndef _RS485_PORT_H
#define _RS485_PORT_H


#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
#include "port.h"
/* ----------------------- Defines ------------------------------------------*/

/* ----------------------- Type definitions ---------------------------------*/

typedef enum
{
    EV_READY            = 1<<0,         /*!< Startup finished. */
    EV_FRAME_RECEIVED   = 1<<1,         /*!< Frame received. */
    EV_EXECUTE          = 1<<2,         /*!< Execute function. */
    EV_FRAME_SENT       = 1<<3          /*!< Frame sent. */
} eEventType;

typedef struct
{
	char Port;
	eEventType Event;
}PortEventType;

//typedef enum
//{
//    EV_MASTER_READY                    = 1<<0,  /*!< Startup finished. */
//    EV_MASTER_FRAME_RECEIVED           = 1<<1,  /*!< Frame received. */
//    EV_MASTER_EXECUTE                  = 1<<2,  /*!< Execute function. */
//    EV_MASTER_FRAME_SENT               = 1<<3,  /*!< Frame sent. */
//    EV_MASTER_ERROR_PROCESS            = 1<<4,  /*!< Frame error process. */
//    EV_MASTER_PROCESS_SUCESS           = 1<<5,  /*!< Request process success. */
//    EV_MASTER_ERROR_RESPOND_TIMEOUT    = 1<<6,  /*!< Request respond timeout. */
//    EV_MASTER_ERROR_RECEIVE_DATA       = 1<<7,  /*!< Request receive data error. */
//    EV_MASTER_ERROR_EXECUTE_FUNCTION   = 1<<8,  /*!< Request execute function error. */
//} eMasterEventType;
typedef enum
{
    EV_MASTER_READY                   =0 ,  /*!< Startup finished. */
    EV_MASTER_FRAME_RECEIVED           ,  /*!< Frame received. */
    EV_MASTER_EXECUTE                  ,  /*!< Execute function. */
    EV_MASTER_FRAME_SENT               ,  /*!< Frame sent. */
    EV_MASTER_ERROR_PROCESS            ,  /*!< Frame error process. */
    EV_MASTER_PROCESS_SUCESS           ,  /*!< Request process success. */
    EV_MASTER_ERROR_RESPOND_TIMEOUT    ,  /*!< Request respond timeout. */
    EV_MASTER_ERROR_RECEIVE_DATA       ,  /*!< Request receive data error. */
    EV_MASTER_ERROR_EXECUTE_FUNCTION   ,  /*!< Request execute function error. */
} eMasterEventType;

typedef struct
{
	char Port;
	eMasterEventType Event;
}PortMasterEventType;

typedef enum
{
    EV_ERROR_RESPOND_TIMEOUT,         /*!< Slave respond timeout. */
    EV_ERROR_RECEIVE_DATA,            /*!< Receive frame data erroe. */
    EV_ERROR_EXECUTE_FUNCTION,        /*!< Execute function error. */
} eMasterErrorEventType;

typedef struct
{
	char Port;
	eMasterErrorEventType ErrorEvent;
}PortMasterErrorEventType;

/*! \ingroup modbus
 * \brief Parity used for characters in serial mode.
 *
 * The parity which should be applied to the characters sent over the serial
 * link. Please note that this values are actually passed to the porting
 * layer and therefore not all parity modes might be available.
 */
typedef enum
{
    PORT_PAR_NONE,                /*!< No parity. */
    PORT_PAR_ODD,                 /*!< Odd parity. */
    PORT_PAR_EVEN                 /*!< Even parity. */
} eParity;

/* ----------------------- Supporting functions -----------------------------*/
BOOL xPortEventInit();

BOOL xPortEventPost(UCHAR Port, eEventType event );

BOOL xPortEventGet( PortEventType * eEvent );

BOOL xMasterPortEventInit();

BOOL xMasterPortEventPost(UCHAR ucPort, eMasterEventType eEvent );

BOOL xMasterPortEventGet(PortMasterEventType * eEvent );

BOOL vMasterOsResInit( UCHAR ucPort );

BOOL xMasterRunResTake(UCHAR ucPort, int32_t time );

void vMasterRunResRelease( UCHAR ucPort );

/* ----------------------- Serial port functions ----------------------------*/

BOOL xPortSerialInit( UCHAR ucPort, ULONG ulBaudRate,
                                   UCHAR ucDataBits, eParity eParity );

void vPortClose( UCHAR port );

void xPortSerialClose( UCHAR ucPORT );

void vPortSerialEnable( UCHAR port, BOOL xRxEnable, BOOL xTxEnable );

BOOL xPortSerialGetByte( UCHAR ucPORT,CHAR * pucByte );

BOOL xPortSerialPutByte( UCHAR ucPORT,CHAR ucByte );

BOOL xMasterPortSerialInit( UCHAR ucPort, ULONG ulBaudRate,
                                   UCHAR ucDataBits, eParity eParity );


void vMasterPortClose( UCHAR ucPort );

void xMasterPortSerialClose( UCHAR ucPort );

void vMasterPortSerialEnable( UCHAR ucPort, BOOL xRxEnable, BOOL xTxEnable );

BOOL xMasterPortSerialGetByte( UCHAR ucPort, UCHAR * pucByte );

BOOL xMasterPortSerialPutByte( UCHAR ucPort, UCHAR ucByte );

/* ----------------------- Timers functions ---------------------------------*/
BOOL xPortTimersInit( UCHAR port,USHORT usTimeOut50us );

void xPortTimersClose( UCHAR ucPort );

void vPortTimersEnable( UCHAR port );

void vPortTimersDisable( UCHAR port );


BOOL xMasterPortTimersInit( UCHAR ucPort, USHORT usTimeOut50us );

void xMasterPortTimersClose( UCHAR ucPort );

void vMasterPortTimersT35Enable( UCHAR ucPort );

void vMasterPortTimersConvertDelayEnable( UCHAR ucPort );

void vMasterPortTimersRespondTimeoutEnable( UCHAR ucPort );

void vMasterPortTimersDisable( UCHAR ucPort );

/* ----------------- Callback for the master error process ------------------*/
void vMasterErrorCBRespondTimeout( UCHAR ucDestAddress, const UCHAR* pucPDUData,
                                                USHORT ucPDULength );
    
void vMasterErrorCBReceiveData( UCHAR ucDestAddress, const UCHAR* pucPDUData,
                                             USHORT ucPDULength );

void vMasterErrorCBExecuteFunction( UCHAR ucDestAddress, const UCHAR* pucPDUData,
                                                 USHORT ucPDULength );

void vMasterCBRequestScuuess( UCHAR ucPort );

/* ----------------------- Callback for the protocol stack ------------------*/

/*!
 * \brief Callback function for the porting layer when a new byte is
 *   available.
 *
 * Depending upon the mode this callback function is used by the RTU or
 * ASCII transmission layers. In any case a call to xMBPortSerialGetByte()
 * must immediately return a new character.
 *
 * \return <code>TRUE</code> if a event was posted to the queue because
 *   a new byte was received. The port implementation should wake up the
 *   tasks which are currently blocked on the eventqueue.
 */
extern BOOL( *pxFrameCBByteReceived ) ( UCHAR ucPORT );

extern BOOL( *pxFrameCBTransmitterEmpty ) ( UCHAR ucPORT );

extern BOOL( *pxPortCBTimerExpired ) ( UCHAR ucPORT );

extern BOOL( *pxMasterFrameCBByteReceived ) ( UCHAR ucPORT );

extern BOOL( *pxMasterFrameCBTransmitterEmpty ) ( UCHAR ucPORT );

extern BOOL( *pxMasterPortCBTimerExpired ) ( UCHAR ucPORT );


/* ----------------------- Callback for the protocol stack ------------------*/

/*!
 * \brief Callback function for the porting layer when a new byte is
 *   available.
 *
 * Depending upon the mode this callback function is used by the RTU or
 * ASCII transmission layers. In any case a call to xMBPortSerialGetByte()
 * must immediately return a new character.
 *
 * \return <code>TRUE</code> if a event was posted to the queue because
 *   a new byte was received. The port implementation should wake up the
 *   tasks which are currently blocked on the eventqueue.
 */
extern          BOOL( *pxMBFrameCBByteReceived ) ( void );

extern          BOOL( *pxMBFrameCBTransmitterEmpty ) ( void );

extern          BOOL( *pxMBPortCBTimerExpired ) ( void );

/* ----------------------- TCP port functions -------------------------------*/
BOOL            xMBTCPPortInit( USHORT usTCPPort );

void            vMBTCPPortClose( void );

void            vMBTCPPortDisable( void );

BOOL            xMBTCPPortGetRequest( UCHAR **ppucMBTCPFrame, USHORT * usTCPLength );

BOOL            xMBTCPPortSendResponse( const UCHAR *pucMBTCPFrame, USHORT usTCPLength );
#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
