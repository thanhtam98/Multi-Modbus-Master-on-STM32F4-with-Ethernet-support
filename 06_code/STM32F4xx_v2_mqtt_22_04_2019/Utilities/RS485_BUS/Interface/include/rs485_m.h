/* 
 * RS485bus Libary: A portable RS485bus implementation for RS485bus ASCII/RTU.
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
 * File: $Id: rs485_m.h,v 1.60 2013/09/03 10:20:05 Armink Add Master Functions $
 */

#ifndef _RS485_M_H
#define _RS485_M_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/*! \defgroup rs485bus RS485bus
 * \code #include "rs485_m.h" \endcode
 *
 * This module defines the interface for the application. It contains
 * the basic functions and types required to use the Modbus Master protocol stack.
 * A typical application will want to call eRS485MasterInit() first. If the device
 * is ready to answer network requests it must then call eRS485Enable() to activate
 * the protocol stack. In the main loop the function eRS485MasterPoll() must be called
 * periodically. The time interval between pooling depends on the configured
 * Modbus timeout. If an RTOS is available a separate task should be created
 * and the task should always call the function eRS485MasterPoll().
 *
 * \code
 * // Initialize protocol stack in RTU mode for a Master
 * eRS485MasterInit( RS485_RTU, 38400, RS485_PAR_EVEN );
 * // Enable the Modbus Protocol Stack.
 * eRS485MasterEnable(  );
 * for( ;; )
 * {
 *     // Call the main polling loop of the Modbus Master protocol stack.
 *     eRS485MasterPoll(  );
 *     ...
 * }
 * \endcode
 */

/* ----------------------- Defines ------------------------------------------*/


/* ----------------------- Type definitions ---------------------------------*/
/*! \ingroup rs485bus
 * \brief Errorcodes used by all function in the Master request.
 */
typedef enum
{
    RS485_MRE_NO_ERR,                  /*!< no error. */
    RS485_MRE_NO_REG,                  /*!< illegal register address. */
    RS485_MRE_ILL_ARG,                 /*!< illegal argument. */
    RS485_MRE_REV_DATA,                /*!< receive data error. */
    RS485_MRE_TIMEDOUT,                /*!< timeout error occurred. */
    RS485_MRE_MASTER_BUSY,             /*!< master is busy now. */
    RS485_MRE_EXE_FUN                  /*!< execute function error. */
} eRS485MasterReqErrCode;
/*! \ingroup rs485bus
 *  \brief TimerMode is Master 3 kind of Timer modes.
 */
typedef enum
{
	RS485_TMODE_T35,                   /*!< Master receive frame T3.5 timeout. */
	RS485_TMODE_RESPOND_TIMEOUT,       /*!< Master wait respond for slave. */
	RS485_TMODE_CONVERT_DELAY          /*!< Master sent broadcast ,then delay sometime.*/
}eRS485MasterTimerMode;

/* ----------------------- Function prototypes ------------------------------*/
/*! \ingroup rs485bus
 * \brief Initialize the RS485bus Master protocol stack.
 *
 * This functions initializes the ASCII or RTU module and calls the
 * init functions of the porting layer to prepare the hardware. Please
 * note that the receiver is still disabled and no Modbus frames are
 * processed until eRS485MasterEnable( ) has been called.
 *
 * \param eMode If ASCII or RTU mode should be used.
 * \param ucPort The port to use. E.g. 1 for COM1 on windows. This value
 *   is platform dependent and some ports simply choose to ignore it.
 * \param ulBaudRate The baudrate. E.g. 19200. Supported baudrates depend
 *   on the porting layer.
 * \param eParity Parity used for serial transmission.
 *
 * \return If no error occurs the function returns eRS485ErrorCode::RS485_ENOERR.
 *   The protocol is then in the disabled state and ready for activation
 *   by calling eRS485MasterEnable( ). Otherwise one of the following error codes
 *   is returned:
 *    - eRS485ErrorCode::RS485_EPORTERR IF the porting layer returned an error.
 */
eRS485ErrorCode    eRS485MasterInit( eRS485Mode eMode,
		                 ULONG ulBaudRate, eParity eParity );
										 
/*! \ingroup rs485bus
 * \brief Release resources used by the protocol stack.
 *
 * This function disables the Modbus Master protocol stack and release all
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
eRS485ErrorCode    eRS485MasterClose( UCHAR ucPort );

/*! \ingroup rs485bus
 * \brief Enable the RS485bus Master protocol stack.
 *
 * This function enables processing of RS485bus Master frames. Enabling the protocol
 * stack is only possible if it is in the disabled state.
 *
 * \return If the protocol stack is now in the state enabled it returns 
 *   eRS485ErrorCode::RS485_ENOERR. If it was not in the disabled state it 
 *   return eRS485ErrorCode::RS485_EILLSTATE.
 */
eRS485ErrorCode    eRS485MasterEnable( UCHAR ucPort );

/*! \ingroup rs485bus
 * \brief Disable the RS485bus Master protocol stack.
 *
 * This function disables processing of Modbus frames.
 *
 * \return If the protocol stack has been disabled it returns 
 *  eRS485ErrorCode::RS485_ENOERR. If it was not in the enabled state it returns
 *  eRS485ErrorCode::RS485_EILLSTATE.
 */
eRS485ErrorCode    eRS485MasterDisable( UCHAR ucPort );

/*! \ingroup rs485bus
 * \brief Check the RS485bus Master protocol stack has established or not.
 *
 * This function must be called and check the return value before calling
 * any other functions.
 *
 * \return If the protocol stack has been established or not
 *  TRUE.  the protocol stack has established
 *  FALSE. the protocol stack hasn't established
 */
BOOL            eRS485MasterIsEstablished( void );

/*! \ingroup rs485bus
 * \brief The main pooling loop of the RS485bus Master protocol stack.
 *
 * This function must be called periodically. The timer interval required
 * is given by the application dependent RS485bus slave timeout. Internally the
 * function calls xRS485MasterPortEventGet() and waits for an event from the receiver or
 * transmitter state machines. 
 *
 * \return If the protocol stack is not in the enabled state the function
 *   returns eRS485ErrorCode::MB_EILLSTATE. Otherwise it returns 
 *   eRS485ErrorCode::RS485_ENOERR.
 */
eRS485ErrorCode    eRS485MasterPoll( void );

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
eRS485ErrorCode eMasterResponseChecksCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus Periodic Ping
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
eRS485ErrorCode eMasterFuncPeriodicPingCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus NFC Detect
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
eRS485ErrorCode eMasterFuncNFCDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);

/*! \ingroup rs485bus Error Detect
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
eRS485ErrorCode eMasterFuncErrorDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen);


/*! \ingroup rs485bus
 *\brief These RS485bus functions are called for user when RS485bus run in Master Mode.
 */
eRS485MasterReqErrCode
eRS485MasterReqFuncCheck( UCHAR ucPort, LONG lTimeOut );
eRS485MasterReqErrCode
eRS485MasterReqPeriodicPing( UCHAR ucPort, LONG lTimeOut );
eRS485MasterReqErrCode
eRS485MasterReqTemperDetec( UCHAR ucPort, UCHAR * pucFrame, USHORT * usLen, LONG lTimeOut );
eRS485MasterReqErrCode
eRS485MasterReqNFCDetect( UCHAR ucPort, UCHAR * pucFrame, USHORT usLen, LONG lTimeOut );

eRS485Exception
eRS485MasterFuncCheck( UCHAR ucPort, UCHAR * pucFrame, USHORT * usLen );
eRS485Exception 
eRS485MasterFuncPeriodicPing( UCHAR ucPort, UCHAR * pucFrame, USHORT * usLen );
eRS485Exception 
eRS485MasterFuncTemperDetec( UCHAR ucPort, UCHAR * pucFrame, USHORT * usLen );
eRS485Exception 
eRS485MasterFuncNFCDetect( UCHAR ucPort, UCHAR * pucFrame, USHORT * usLen );
eRS485Exception 
eRS485MasterFuncErrorDetect( UCHAR ucPort, UCHAR * pucFrame, USHORT * usLen );
/*£¡ \ingroup RS485bus
 *\brief These functions are interface for RS485bus Master
 */
void vRS485MasterGetPDUSndBuf( UCHAR ucPort,UCHAR ** pucFrame );
BOOL xRS485MasterGetCBRunInMasterMode( UCHAR ucPort );
void vRS485MasterSetCBRunInMasterMode( UCHAR ucPort, BOOL IsMasterMode );
USHORT usRS485MasterGetPDUSndLength( UCHAR ucPort );
void vRS485MasterSetPDUSndLength( UCHAR ucPort,USHORT SendPDULength );
void vRS485MasterSetCurTimerMode( UCHAR ucPort,eRS485MasterTimerMode eRS485TimerMode );
eMasterErrorEventType eMasterGetErrorType( UCHAR ucPort );
void vMasterSetErrorType( UCHAR ucPort,eMasterErrorEventType errorType );
eRS485MasterReqErrCode eMasterWaitRequestFinish( UCHAR ucPort );

/* ----------------------- Callback -----------------------------------------*/

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
