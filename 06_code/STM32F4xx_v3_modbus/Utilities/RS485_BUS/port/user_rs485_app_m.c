/*
 * RS485bus Libary: user callback functions and buffer define in master mode
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: user_rs485_app_m.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
#include "user_rs485_app.h"

/*-----------------------Master mode use these variables----------------------*/
#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
extern CTPORT_TypeDef CTport;

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterResponseChecksCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort].status = PORT_OK;
  if(pucBuffer[0] == RS485_EX_ACK)
  {
    CTport.Port[ucPort].active = PORT_ENABLE;
  }
  else
  {
    CTport.Port[ucPort].active = PORT_DISABLE;
  }
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterFuncPeriodicPingCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort].status = PORT_OK;
  if(pucBuffer[0] == RS485_EX_ACK)
  {
    CTport.Port[ucPort].active = PORT_ENABLE;
  }
  else
  {
    CTport.Port[ucPort].active = PORT_DISABLE;
  }
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterFuncNFCDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort].status = PORT_OK;
  if(pucBuffer[0] == RS485_EX_ACK)
  {
    CTport.Port[ucPort].event = PORT_AUTH_PASS | PORT_RECEIVE_DATA;
  }
  else
  {
    CTport.Port[ucPort].event = PORT_AUTH_FAIL | PORT_RECEIVE_DATA;
  }
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave Check callback function.
 *
 * @param ucPort Port check
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eMasterFuncErrorDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
  return eStatus;
}
#endif
