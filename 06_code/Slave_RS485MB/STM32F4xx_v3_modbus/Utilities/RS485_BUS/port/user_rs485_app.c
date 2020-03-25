/*
 * RS485bus Libary: user callback functions and buffer define in slave mode
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
 * File: $Id: user_rs485_app.c,v 1.60 2013/11/23 11:49:05 Armink $
 */
 /* ----------------------- RS485bus includes ----------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "user_rs485_app.h"

/* ----------------------- Slave mode Defines ----------------------------------*/


/*------------------------Slave mode use these variables------------------------*/
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
eRS485ErrorCode eResponseChecksCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort + RS485_PORT_OFFSET].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort + RS485_PORT_OFFSET].status = PORT_OK;
	if(pucBuffer[0] == CHECK_STATUS)
  {
    if(CTport.Port[ucPort + RS485_PORT_OFFSET].active == PORT_ENABLE)
    {
      pucBuffer[1] = 0x00;
    }
    else
    {
      pucBuffer[1] = 0x01;
    }
  }
  else if((pucBuffer[0] == CHECK_UID_AUTHENTICATION)|(pucBuffer[0] == CHECK_KEY_AUTHENTICATION))
  {
    if(CTport.Port[ucPort + RS485_PORT_OFFSET].event & PORT_TRANSMIT_DATA)
    {
      if((CTport.Port[ucPort + RS485_PORT_OFFSET].event & 0x0F) == PORT_AUTH_PASS)
      {
        pucBuffer[1] = 0x00;
      }
      else
      {
        pucBuffer[1] = 0x01;
      }
      CTport.Port[ucPort + RS485_PORT_OFFSET].event = PORT_EVENT_NONE;
    }
    else
    {
       pucBuffer[1] = 0x02;
    }
  }
	*ucLen = 2;
  return eStatus;
}

/**
 * RS485bus slave Periodic Ping callback function.
 *
 * @param ucPort Port Periodic Ping
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode ePeriodicPingCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	CTport.Port[ucPort + RS485_PORT_OFFSET].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort + RS485_PORT_OFFSET].status = PORT_OK;
  if(CTport.Port[ucPort + RS485_PORT_OFFSET].active == PORT_ENABLE)
  {
    pucBuffer[0] = RS485_EX_ACK;
  }
  else
  {
    pucBuffer[0] = RS485_EX_NAK;
  }
	*ucLen = 1;
  return eStatus;
}


/**
 * RS485bus slave function NFC Detect callback function.
 *
 * @param ucPort Port Periodic Ping
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eFuncNFCDetectCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	UCHAR i;
	
	CTport.Port[ucPort + RS485_PORT_OFFSET].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort + RS485_PORT_OFFSET].status = PORT_OK;
	if(CTport.Port[ucPort + RS485_PORT_OFFSET].active == PORT_ENABLE)
	{
    if((CTport.Port[ucPort + RS485_PORT_OFFSET].event & PORT_RECEIVE_DATA) ==0 )
    {
      if(pucBuffer[0]< PORT_BUFFER_NUMBER)
      {
        for(i=0;i<=pucBuffer[0];i++)
        {
          CTport.Port[ucPort + RS485_PORT_OFFSET].Buffer[i] = pucBuffer[i];
        }
        CTport.Port[ucPort + RS485_PORT_OFFSET].event = PORT_RECEIVE_DATA;
        pucBuffer[0] = RS485_EX_ACK;
      }
      else
      {
        eStatus = RS485_ENORES;
      }
    }
    else
    {
      eStatus = RS485_ENORES;
    }
	}
	else
	{
		pucBuffer[0] = RS485_EX_NAK;
	}
	*ucLen = 1;
  return eStatus;
}

/**
 * RS485bus slave function key enter callback function.
 *
 * @param ucPort Port Periodic Ping
 * @param pucBuffer Check buffer
 * @param usLen Length
 *
 * @return result
 */
eRS485ErrorCode eFuncKeyEnterCB( UCHAR ucPort,UCHAR * pucBuffer, UCHAR * ucLen)
{
  eRS485ErrorCode    eStatus = RS485_ENOERR;
	UCHAR i;
	
	CTport.Port[ucPort + RS485_PORT_OFFSET].timeout = PORT_TIMEOUT;
	CTport.Port[ucPort + RS485_PORT_OFFSET].status = PORT_OK;
	if(CTport.Port[ucPort + RS485_PORT_OFFSET].active == PORT_ENABLE)
	{
    if((CTport.Port[ucPort + RS485_PORT_OFFSET].event & PORT_RECEIVE_DATA) ==0 )
    {
      if(pucBuffer[0]< PORT_BUFFER_NUMBER)
      {
        for(i=0;i<=pucBuffer[0];i++)
        {
          CTport.Port[ucPort + RS485_PORT_OFFSET].Buffer[i] = pucBuffer[i];
        }
        CTport.Port[ucPort + RS485_PORT_OFFSET].event = PORT_RECEIVE_DATA;
        pucBuffer[0] = RS485_EX_ACK;
      }
      else
      {
        eStatus = RS485_ENORES;
      }
    }
    else
    {
      eStatus = RS485_ENORES;
    }
	}
	else
	{
		pucBuffer[0] = RS485_EX_NAK;
	}
	*ucLen = 1;
  return eStatus;
}


