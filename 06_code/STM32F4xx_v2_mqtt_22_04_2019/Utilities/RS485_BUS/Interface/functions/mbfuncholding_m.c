/* 
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006-2018 Christian Walter <cwalter@embedded-solutions.at>
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
 * File: $Id: mbfuncholding.c,v 1.12 2007/02/18 23:48:22 wolti Exp $
 */

/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "rs485_m.h"
#include "rs485frame.h"
#include "rs485proto.h"
#include "rs485config.h"
#include "rs485port.h"
#include "user_debug.h"
/* ----------------------- Defines ------------------------------------------*/
#define MB_PDU_REQ_READ_ADDR_OFF                ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READ_REGCNT_OFF              ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READ_SIZE                    ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX             ( 0x007D )
#define MB_PDU_FUNC_READ_BYTECNT_OFF            ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READ_VALUES_OFF             ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READ_SIZE_MIN               ( 1 )

#define MB_PDU_REQ_WRITE_ADDR_OFF               ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_REQ_WRITE_VALUE_OFF              ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_WRITE_SIZE                   ( 4 )
#define MB_PDU_FUNC_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define MB_PDU_FUNC_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_SIZE                  ( 4 )

#define MB_PDU_REQ_WRITE_MUL_ADDR_OFF           ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_WRITE_MUL_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF        ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_REQ_WRITE_MUL_VALUES_OFF         ( MB_PDU_DATA_OFF + 5 )
#define MB_PDU_REQ_WRITE_MUL_SIZE_MIN           ( 5 )
#define MB_PDU_REQ_WRITE_MUL_REGCNT_MAX         ( 0x0078 )
#define MB_PDU_FUNC_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_WRITE_MUL_SIZE              ( 4 )

#define MB_PDU_REQ_READWRITE_READ_ADDR_OFF      ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_REQ_READWRITE_READ_REGCNT_OFF    ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF     ( MB_PDU_DATA_OFF + 4 )
#define MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF   ( MB_PDU_DATA_OFF + 6 )
#define MB_PDU_REQ_READWRITE_WRITE_BYTECNT_OFF  ( MB_PDU_DATA_OFF + 8 )
#define MB_PDU_REQ_READWRITE_WRITE_VALUES_OFF   ( MB_PDU_DATA_OFF + 9 )
#define MB_PDU_REQ_READWRITE_SIZE_MIN           ( 9 )
#define MB_PDU_FUNC_READWRITE_READ_BYTECNT_OFF  ( MB_PDU_DATA_OFF + 0 )
#define MB_PDU_FUNC_READWRITE_READ_VALUES_OFF   ( MB_PDU_DATA_OFF + 1 )
#define MB_PDU_FUNC_READWRITE_SIZE_MIN          ( 1 )

/* ----------------------- Static functions ---------------------------------*/
eRS485Exception    prveMBError2Exception( eRS485ErrorCode eErrorCode );

/* ----------------------- Start implementation -----------------------------*/
#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0

/**
 * This function will request write holding register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usRegData register data to be written
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eRS485MasterReqErrCode
eMBMasterReqWriteHoldingRegister( UCHAR ucPort, UCHAR ucSndAddr, USHORT usRegAddr, USHORT usRegData, LONG lTimeOut )
{
    UCHAR                 *ucMBFrame;
    eRS485MasterReqErrCode    eErrStatus = RS485_MRE_NO_ERR;
     DBG("\r\n eMBMasterReqWriteHoldingRegister is called");
    if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = RS485_MRE_ILL_ARG;
    else if ( xMasterRunResTake(ucPort, lTimeOut ) == FALSE ) eErrStatus = RS485_MRE_MASTER_BUSY;
    else
    {

        // chưa có cái source nào viết khó hiểu như cái source này =_= 
		vRS485MasterGetPDUSndBuf(ucPort,&ucMBFrame);
		vRS485MasterSetDestAddress(ucPort,ucSndAddr);
		ucMBFrame[RS485_PDU_FUNC_OFF]                = MB_FUNC_WRITE_REGISTER;
		ucMBFrame[MB_PDU_REQ_WRITE_ADDR_OFF]      = usRegAddr >> 8;
		ucMBFrame[MB_PDU_REQ_WRITE_ADDR_OFF + 1]  = usRegAddr;
		ucMBFrame[MB_PDU_REQ_WRITE_VALUE_OFF]     = usRegData >> 8;
		ucMBFrame[MB_PDU_REQ_WRITE_VALUE_OFF + 1] = usRegData ;
		vRS485MasterSetPDUSndLength(ucPort, MB_PDU_SIZE_MIN  + MB_PDU_REQ_WRITE_SIZE );
		( void ) xMasterPortEventPost(ucPort, EV_MASTER_FRAME_SENT );
		eErrStatus = eMasterWaitRequestFinish(ucPort );
    }
    return eErrStatus;
}

/**
 * This function will be called when protocol stack received a frame reponse the request write holding register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usRegData register data to be written
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
eRS485Exception
eRS485MasterFuncWriteHoldingRegister( UCHAR ucPort, UCHAR ucRcvAddress, UCHAR * pucFrame, USHORT * usLen )
// eMBMasterFuncWriteHoldingRegister( UCHAR * pucFrame, UCHAR ucRcvAddress, USHORT * usLen )
{
    USHORT          usRegAddress;
    eRS485Exception    eStatus = RS485_EX_NONE;
    eRS485ErrorCode    eRegStatus;
    DBG("\r\n eRS485MasterFuncWriteHoldingRegister is called");
    if( *usLen == ( MB_PDU_SIZE_MIN + MB_PDU_FUNC_WRITE_SIZE ) )
    {
        usRegAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_WRITE_ADDR_OFF + 1] );
        usRegAddress++;

        /* Make callback to update the value. */
        // check the 1 value in param
        eRegStatus = eRS485MasterRegHoldingCB(ucPort ,usRegAddress,&pucFrame[MB_PDU_FUNC_WRITE_VALUE_OFF], 1, RS485_REG_WRITE );

        /* If an error occured convert it into a Modbus exception. */
        if( eRegStatus != RS485_MRE_NO_ERR )
        {
            eStatus = prveMBError2Exception( eRegStatus );
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = RS485_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}
#endif

#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
/**
 * This function will request write multiple holding register.
 *
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usNRegs register total number
 * @param pusDataBuffer data to be written
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */

/*
eMBMasterReqErrCode
eMBMasterReqWriteMultipleHoldingRegister( UCHAR ucSndAddr,
		USHORT usRegAddr, USHORT usNRegs, USHORT * pusDataBuffer, LONG lTimeOut )
{
    UCHAR                 *ucMBFrame;
    USHORT                 usRegIndex = 0;
    eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;

    if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( xMBMasterRunResTake( lTimeOut ) == FALSE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vMBMasterGetPDUSndBuf(&ucMBFrame);
		vMBMasterSetDestAddress(ucSndAddr);
		ucMBFrame[MB_PDU_FUNC_OFF]                     = MB_FUNC_WRITE_MULTIPLE_REGISTERS;
		ucMBFrame[MB_PDU_REQ_WRITE_MUL_ADDR_OFF]       = usRegAddr >> 8;
		ucMBFrame[MB_PDU_REQ_WRITE_MUL_ADDR_OFF + 1]   = usRegAddr;
		ucMBFrame[MB_PDU_REQ_WRITE_MUL_REGCNT_OFF]     = usNRegs >> 8;
		ucMBFrame[MB_PDU_REQ_WRITE_MUL_REGCNT_OFF + 1] = usNRegs ;
		ucMBFrame[MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF]    = usNRegs * 2;
		ucMBFrame += MB_PDU_REQ_WRITE_MUL_VALUES_OFF;
		while( usNRegs > usRegIndex)
		{
			*ucMBFrame++ = pusDataBuffer[usRegIndex] >> 8;
			*ucMBFrame++ = pusDataBuffer[usRegIndex++] ;
		}
		vMBMasterSetPDUSndLength( MB_PDU_SIZE_MIN + MB_PDU_REQ_WRITE_MUL_SIZE_MIN + 2*usNRegs );
		( void ) xMBMasterPortEventPost( EV_MASTER_FRAME_SENT );
		eErrStatus = eMBMasterWaitRequestFinish( );
    }
    return eErrStatus;
}
*/

// eMBException
// eMBMasterFuncWriteMultipleHoldingRegister( UCHAR * pucFrame, USHORT * usLen )
// {
//     UCHAR          *ucMBFrame;
//     USHORT          usRegAddress;
//     USHORT          usRegCount;
//     UCHAR           ucRegByteCount;

//     eMBException    eStatus = MB_EX_NONE;
//     eMBErrorCode    eRegStatus;

//     /* If this request is broadcast, the *usLen is not need check. */
//     if( ( *usLen == MB_PDU_SIZE_MIN + MB_PDU_FUNC_WRITE_MUL_SIZE ) || xMBMasterRequestIsBroadcast() )
//     {
// 		vMBMasterGetPDUSndBuf(&ucMBFrame);
//         usRegAddress = ( USHORT )( ucMBFrame[MB_PDU_REQ_WRITE_MUL_ADDR_OFF] << 8 );
//         usRegAddress |= ( USHORT )( ucMBFrame[MB_PDU_REQ_WRITE_MUL_ADDR_OFF + 1] );
//         usRegAddress++;

//         usRegCount = ( USHORT )( ucMBFrame[MB_PDU_REQ_WRITE_MUL_REGCNT_OFF] << 8 );
//         usRegCount |= ( USHORT )( ucMBFrame[MB_PDU_REQ_WRITE_MUL_REGCNT_OFF + 1] );

//         ucRegByteCount = ucMBFrame[MB_PDU_REQ_WRITE_MUL_BYTECNT_OFF];

//         if( ucRegByteCount == 2 * usRegCount )
//         {
//             /* Make callback to update the register values. */
//             eRegStatus =
//                 eMBMasterRegHoldingCB( &ucMBFrame[MB_PDU_REQ_WRITE_MUL_VALUES_OFF],
//                                  usRegAddress, usRegCount, MB_REG_WRITE );

//             /* If an error occured convert it into a Modbus exception. */
//             if( eRegStatus != MB_ENOERR )
//             {
//                 eStatus = prveMBError2Exception( eRegStatus );
//             }
//         }
//         else
//         {
//             eStatus = MB_EX_ILLEGAL_DATA_VALUE;
//         }
//     }
//     else
//     {
//         /* Can't be a valid request because the length is incorrect. */
//         eStatus = MB_EX_ILLEGAL_DATA_VALUE;
//     }
//     return eStatus;
// }
// 
#endif

#if MB_FUNC_READ_HOLDING_ENABLED > 0

/**
 * This function will request read holding register.
 *
 * @param ucPort port used to transfer data
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usNRegs register total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */

// typedef eRS485Exception( *pxRS485MasterFunctionHandler ) ( UCHAR ucPort, UCHAR ucRcvAddress, UCHAR * pucFrame, USHORT * pusLength );
eMBMasterReqErrCode
eMBMasterReqReadHoldingRegister( UCHAR ucPort, UCHAR ucSndAddr, USHORT usRegAddr, USHORT usNRegs, LONG lTimeOut )
{
    UCHAR                 *ucMBFrame;
    eRS485MasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;

    if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = MB_MRE_ILL_ARG;
    else if ( xMasterRunResTake( ucPort, lTimeOut ) == FALSE ) eErrStatus = MB_MRE_MASTER_BUSY;
    else
    {
		vRS485MasterGetPDUSndBuf(ucPort, &ucMBFrame);
		vRS485MasterSetDestAddress(ucPort, ucSndAddr);
		ucMBFrame[MB_PDU_FUNC_OFF]                = MB_FUNC_READ_HOLDING_REGISTER;
		ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF]       = usRegAddr >> 8;
		ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF + 1]   = usRegAddr;
		ucMBFrame[MB_PDU_REQ_READ_REGCNT_OFF]     = usNRegs >> 8;
		ucMBFrame[MB_PDU_REQ_READ_REGCNT_OFF + 1] = usNRegs;
		vRS485MasterSetPDUSndLength( ucPort,MB_PDU_SIZE_MIN + MB_PDU_REQ_READ_SIZE );
		( void ) xMasterPortEventPost( ucPort, EV_MASTER_FRAME_SENT );
		eErrStatus = eMasterWaitRequestFinish(ucPort );
    }
    return eErrStatus;
}
/**
 * This function will request read holding register.
 *
 * @param ucPort port used to transfer data
 * @param ucSndAddr salve address
 * @param usRegAddr register start address
 * @param usNRegs register total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */

// typedef eRS485Exception( *pxRS485MasterFunctionHandler ) ( UCHAR ucPort, UCHAR ucRcvAddress, UCHAR * pucFrame, USHORT * pusLength );
// eMBException
// eMBMasterFuncWriteHoldingRegister( UCHAR ucPort, UCHAR ucRcvAddress, UCHAR * pucFrame, USHORT * usLen )
eMBException
eMBMasterFuncReadHoldingRegister( UCHAR ucPort, UCHAR ucRcvAddress,  UCHAR * pucFrame, USHORT * usLen )
{
    UCHAR          *ucMBFrame;
    USHORT          usRegAddress;
    USHORT          usRegCount;

    eMBException    eStatus = RS485_EX_NONE;
    eMBErrorCode    eRegStatus;
    DBG("eMBMasterFuncReadHoldingRegister is called");
    /* If this request is broadcast, and it's read mode. This request don't need execute. */
    // if ( xMBMasterRequestIsBroadcast() )
    // {
    // 	eStatus = MB_EX_NONE;
    // }
    else if( *usLen >= RS485_PDU_SIZE_MIN + RS485_PDU_FUNC_READ_SIZE_MIN )
    {
		vRS485MasterGetPDUSndBuf(ucPort, &ucMBFrame);
        usRegAddress = ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_ADDR_OFF + 1] );
        usRegAddress++;

        usRegCount = ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_REGCNT_OFF] << 8 );
        usRegCount |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READ_REGCNT_OFF + 1] );

        /* Check if the number of registers to read is valid. If not
         * return Modbus illegal data value exception.
         */
        if( ( usRegCount >= 1 ) && ( 2 * usRegCount == pucFrame[MB_PDU_FUNC_READ_BYTECNT_OFF] ) )
        {
            /* Make callback to fill the buffer. */
            eRegStatus = eMBMasterRegHoldingCB(ucPort, &pucFrame[MB_PDU_FUNC_READ_VALUES_OFF], usRegAddress, usRegCount, MB_REG_READ );
            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != RS485_ENOERR )
            {
                eStatus = prveMBError2Exception( eRegStatus );
            }
        }
        else
        {
            eStatus = RS485_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid request because the length is incorrect. */
        eStatus = RS485_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}


#endif

#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0

/**
 * This function will request read and write holding register.
 *
 * @param ucSndAddr salve address
 * @param usReadRegAddr read register start address
 * @param usNReadRegs read register total number
 * @param pusDataBuffer data to be written
 * @param usWriteRegAddr write register start address
 * @param usNWriteRegs write register total number
 * @param lTimeOut timeout (-1 will waiting forever)
 *
 * @return error code
 */
// eMBMasterReqErrCode
// eMBMasterReqReadWriteMultipleHoldingRegister( UCHAR ucSndAddr,
// 		USHORT usReadRegAddr, USHORT usNReadRegs, USHORT * pusDataBuffer,
// 		USHORT usWriteRegAddr, USHORT usNWriteRegs, LONG lTimeOut )
// {
//     UCHAR                 *ucMBFrame;
//     USHORT                 usRegIndex = 0;
//     eMBMasterReqErrCode    eErrStatus = MB_MRE_NO_ERR;

//     if ( ucSndAddr > MB_MASTER_TOTAL_SLAVE_NUM ) eErrStatus = MB_MRE_ILL_ARG;
//     else if ( xMBMasterRunResTake( lTimeOut ) == FALSE ) eErrStatus = MB_MRE_MASTER_BUSY;
//     else
//     {
// 		vMBMasterGetPDUSndBuf(&ucMBFrame);
// 		vMBMasterSetDestAddress(ucSndAddr);
// 		ucMBFrame[MB_PDU_FUNC_OFF]                           = MB_FUNC_READWRITE_MULTIPLE_REGISTERS;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_READ_ADDR_OFF]        = usReadRegAddr >> 8;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_READ_ADDR_OFF + 1]    = usReadRegAddr;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_READ_REGCNT_OFF]      = usNReadRegs >> 8;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_READ_REGCNT_OFF + 1]  = usNReadRegs ;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF]       = usWriteRegAddr >> 8;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF + 1]   = usWriteRegAddr;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF]     = usNWriteRegs >> 8;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF + 1] = usNWriteRegs ;
// 		ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_BYTECNT_OFF]    = usNWriteRegs * 2;
// 		ucMBFrame += MB_PDU_REQ_READWRITE_WRITE_VALUES_OFF;
// 		while( usNWriteRegs > usRegIndex)
// 		{
// 			*ucMBFrame++ = pusDataBuffer[usRegIndex] >> 8;
// 			*ucMBFrame++ = pusDataBuffer[usRegIndex++] ;
// 		}
// 		vMBMasterSetPDUSndLength( MB_PDU_SIZE_MIN + MB_PDU_REQ_READWRITE_SIZE_MIN + 2*usNWriteRegs );
// 		( void ) xMBMasterPortEventPost( EV_MASTER_FRAME_SENT );
// 		eErrStatus = eMBMasterWaitRequestFinish( );
//     }
//     return eErrStatus;
// }

// eMBException
// eMBMasterFuncReadWriteMultipleHoldingRegister( UCHAR * pucFrame, USHORT * usLen )
// {
//     USHORT          usRegReadAddress;
//     USHORT          usRegReadCount;
//     USHORT          usRegWriteAddress;
//     USHORT          usRegWriteCount;
//     UCHAR          *ucMBFrame;

//     eMBException    eStatus = MB_EX_NONE;
//     eMBErrorCode    eRegStatus;

//     /* If this request is broadcast, and it's read mode. This request don't need execute. */
//     if ( xMBMasterRequestIsBroadcast() )
//     {
//     	eStatus = MB_EX_NONE;
//     }
//     else if( *usLen >= MB_PDU_SIZE_MIN + MB_PDU_FUNC_READWRITE_SIZE_MIN )
//     {
//     	vMBMasterGetPDUSndBuf(&ucMBFrame);
//         usRegReadAddress = ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_READ_ADDR_OFF] << 8U );
//         usRegReadAddress |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_READ_ADDR_OFF + 1] );
//         usRegReadAddress++;

//         usRegReadCount = ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_READ_REGCNT_OFF] << 8U );
//         usRegReadCount |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_READ_REGCNT_OFF + 1] );

//         usRegWriteAddress = ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF] << 8U );
//         usRegWriteAddress |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_ADDR_OFF + 1] );
//         usRegWriteAddress++;

//         usRegWriteCount = ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF] << 8U );
//         usRegWriteCount |= ( USHORT )( ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_REGCNT_OFF + 1] );

//         if( ( 2 * usRegReadCount ) == pucFrame[MB_PDU_FUNC_READWRITE_READ_BYTECNT_OFF] )
//         {
//             /* Make callback to update the register values. */
//             eRegStatus = eMBMasterRegHoldingCB( &ucMBFrame[MB_PDU_REQ_READWRITE_WRITE_VALUES_OFF],
//                                            usRegWriteAddress, usRegWriteCount, MB_REG_WRITE );

//             if( eRegStatus == MB_ENOERR )
//             {
//                 /* Make the read callback. */
// 				eRegStatus = eMBMasterRegHoldingCB(&pucFrame[MB_PDU_FUNC_READWRITE_READ_VALUES_OFF],
// 						                      usRegReadAddress, usRegReadCount, MB_REG_READ);
//             }
//             if( eRegStatus != MB_ENOERR )
//             {
//                 eStatus = prveMBError2Exception( eRegStatus );
//             }
//         }
//         else
//         {
//             eStatus = MB_EX_ILLEGAL_DATA_VALUE;
//         }
//     }
//     return eStatus;
// }

#endif
eRS485Exception    prveMBError2Exception( eRS485ErrorCode eErrorCode )
{
    DBG(eErrorCode);
}
#endif
