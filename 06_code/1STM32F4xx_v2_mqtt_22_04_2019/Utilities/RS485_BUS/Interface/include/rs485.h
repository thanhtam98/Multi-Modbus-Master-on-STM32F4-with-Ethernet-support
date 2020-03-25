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
 * File: $Id: rs485.h,v 1.17 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _RS485_H
#define _RS485_H


#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
#include "rs485port.h"
#include "rs485proto.h"

/*! \defgroup RS485 Bus
 * \code #include "rs485.h" \endcode
 *
 * This module defines the interface for the application. It contains
 * the basic functions and types required to use the Rs485 protocol stack.
 * A typical application will want to call eRS485Init() first. If the device
 * is ready to answer network requests it must then call eRS485Enable() to activate
 * the protocol stack. In the main loop the function eRS485Poll() must be called
 * periodically. The time interval between pooling depends on the configured
 * RS485bus timeout. If an RTOS is available a separate task should be created
 * and the task should always call the function eRS485Poll().
 *
 * \code
 * // Initialize protocol stack in RS485 for the slave
 * eRS485Init( 19200, RS485_PAR_EVEN );
 * // Enable the RS485 Protocol Stack.
 * eRS485Enable(  );
 * for( ;; )
 * {
 *     // Call the main polling loop of the Modbus protocol stack.
 *     eRS485Poll(  );
 *     ...
 * }
 * \endcode
 */

/* ----------------------- Defines ------------------------------------------*/
 

/* ----------------------- Type definitions ---------------------------------*/
 /*! \ingroup RS485Bus
 * \brief RS485Bus serial transmission modes (RTU/ASCII).
 *
 * RS485Bus serial supports two transmission modes. Either ASCII or RTU. RTU
 * is faster but has more hardware requirements and requires a network with
 * a low jitter. ASCII is slower and more reliable on slower links (E.g. modems)
 */
typedef enum
{
    RS485_RTU,                     /*!< RTU transmission mode. */
    RS485_ASCII,                   /*!< ASCII transmission mode. */
} eRS485Mode;

/*! \ingroup RS485Bus
 * \brief Errorcodes used by all function in the protocol stack.
 */
typedef enum
{
    RS485_ENOERR,                  /*!< no error. */
    RS485_EINVAL,                  /*!< illegal argument. */
    RS485_EPORTERR,                /*!< porting layer error. */
    RS485_ENORES,                  /*!< insufficient resources. */
    RS485_EIO,                     /*!< I/O error. */
    RS485_EILLSTATE,               /*!< protocol stack in illegal state. */
    RS485_ETIMEDOUT                /*!< timeout error occurred. */
} eRS485ErrorCode;


/* ----------------------- Function prototypes ------------------------------*/
/*! \ingroup Rs485
 * \brief Initialize the Rs485 protocol stack.
 *
 * This functions initializes and calls the
 * init functions of the porting layer to prepare the hardware. Please
 * note that the receiver is still disabled and no Rs485 frames are
 * processed until eRS485Enable( ) has been called.
 *
 * \param ucPort The port to use. E.g. 1 for COM1 on windows. This value
 *   is platform dependent and some ports simply choose to ignore it.
 * \param ulBaudRate The baudrate. E.g. 19200. Supported baudrates depend
 *   on the porting layer.
 * \param eParity Parity used for serial transmission.
 *
 * \return If no error occurs the function returns eRS485ErrorCode::RS485_ENOERR.
 *   The protocol is then in the disabled state and ready for activation
 *   by calling eRS485Enable( ). Otherwise one of the following error codes 
 *   is returned:
 *    - eRS485ErrorCode::RS485_EINVAL If the slave address was not valid. Valid
 *        slave addresses are in the range 1 - 247.
 *    - eRS485ErrorCode::RS485_EPORTERR IF the porting layer returned an error.
 */
eRS485ErrorCode    eRS485Init( eRS485Mode eMode, ULONG ulBaudRate, eParity eParity );

/*! \ingroup RS485bus
 * \brief Release resources used by the protocol stack.
 *
 * This function disables the Modbus protocol stack and release all
 * hardware resources. It must only be called when the protocol stack 
 * is disabled. 
 *
 * \note Note all ports implement this function. A port which wants to 
 *   get an callback must define the macro RS485_PORT_HAS_CLOSE to 1.
 *
 * \return If the resources where released it return eRS485ErrorCode::RS485_ENOERR.
 *   If the protocol stack is not in the disabled state it returns
 *   eRS485ErrorCode::RS485_EILLSTATE.
 */
eRS485ErrorCode    eRS485Close(UCHAR Port);

/*! \ingroup RS485bus
 * \brief Enable the RS485bus protocol stack.
 *
 * This function enables processing of RS485bus frames. Enabling the protocol
 * stack is only possible if it is in the disabled state.
 *
 * \return If the protocol stack is now in the state enabled it returns 
 *   eRS485ErrorCode::RS485_ENOERR. If it was not in the disabled state it 
 *   return eRS485ErrorCode::RS485_EILLSTATE.
 */
eRS485ErrorCode    eRS485Enable( void );

/*! \ingroup RS485bus
 * \brief Disable the RS485bus protocol stack.
 *
 * This function disables processing of RS485bus frames.
 *
 * \return If the protocol stack has been disabled it returns 
 *  eRS485ErrorCode::RS485_ENOERR. If it was not in the enabled state it returns
 *  eRS485ErrorCode::RS485_EILLSTATE.
 */
eRS485ErrorCode    eRS485Disable( UCHAR ucPort );

/*! \ingroup RS485bus
 * \brief The main pooling loop of the RS485bus protocol stack.
 *
 * This function must be called periodically. The timer interval required
 * is given by the application dependent Modbus slave timeout. Internally the
 * function calls xRS485PortEventGet() and waits for an event from the receiver or
 * transmitter state machines. 
 *
 * \return If the protocol stack is not in the enabled state the function
 *   returns eRS485ErrorCode::RS485_EILLSTATE. Otherwise it returns 
 *   eRS485ErrorCode::RS485_ENOERR.
 */
eRS485ErrorCode    eRS485Poll( void );

/*! \ingroup rs485bus check
 * \brief Callback function used if a <em>Frame check</em> is received
 *  by the protocol stack.
 *
 * \param ucport The port to check
 * \param pucBuffer 
 * \param ucLen Number of pucBuffer.
 * \return The function must return one of the following error codes:
 *   - eRS485ErrorCode::RS485_ENOERR If no error occurred. In this case a normal
 *       RS485Bus response is sent.
 */
eRS485ErrorCode eResponseChecksCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus Periodic Ping
 * \brief Callback function used if a <em>Frame Periodic Ping</em> is received
 *  by the protocol stack.
 *
 * \param ucport The port to check
 * \param pucBuffer 
 * \param ucLen Number of pucBuffer.
 * \return The function must return one of the following error codes:
 *   - eRS485ErrorCode::RS485_ENOERR If no error occurred. In this case a normal
 *       RS485Bus response is sent.
 */
eRS485ErrorCode ePeriodicPingCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus Temper Detect
 * \brief Callback function used if a <em>Frame Temper Detect</em> is received
 *  by the protocol stack.
 *
 * \param ucport The port to check
 * \param pucBuffer 
 * \param ucLen Number of pucBuffer.
 * \return The function must return one of the following error codes:
 *   - eRS485ErrorCode::RS485_ENOERR If no error occurred. In this case a normal
 *       RS485Bus response is sent.
 */
eRS485ErrorCode eFuncTemperDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus NFC Detect
 * \brief Callback function used if a <em>Frame Temper Detect</em> is received
 *  by the protocol stack.
 *
 * \param ucport The port to check
 * \param pucBuffer 
 * \param ucLen Number of pucBuffer.
 * \return The function must return one of the following error codes:
 *   - eRS485ErrorCode::RS485_ENOERR If no error occurred. In this case a normal
 *       RS485Bus response is sent.
 */
eRS485ErrorCode eFuncNFCDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus Funtion Error Detect
 * \brief Callback function used if a <em>Frame by error </em> is received
 *  by the protocol stack.
 *
 * \param ucport The port to check
 * \param pucBuffer 
 * \param ucLen Number of pucBuffer.
 * \return The function must return one of the following error codes:
 *   - eRS485ErrorCode::RS485_ENOERR If no error occurred. In this case a normal
 *       RS485Bus response is sent.
 */
eRS485ErrorCode eFuncKeyEnterCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);
void vRS485GetPDUSndBuf( UCHAR ucPort, UCHAR ** pucFrame );
void vRS485SetPDUSndLength(UCHAR ucPort, USHORT SendPDULength );
USHORT usRS485GetPDUSndLength( UCHAR ucPort );
void xRS485Send(UCHAR Port, UCHAR ucSlaveAddress ,const UCHAR * pucFrame,  USHORT usLength);
#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
