/*
 * RS485bus Libary: RT-Thread Port
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
 * File: $Id: portevent_m.c v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "rs485.h"
#include "rs485_m.h"
#include "rs485config.h"
#include "rs485port.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
/* ----------------------- Defines ------------------------------------------*/
/* ----------------------- Variables ----------------------------------------*/
static xSemaphoreHandle xMasterRunRes[RS485_PORT_NUMBER];
static xQueueHandle     xMasterOsEvent;
/* ----------------------- Start implementation -----------------------------*/
BOOL
xMasterPortEventInit( void )
{
    xMasterOsEvent = xQueueCreate( RS485_PORT_NUMBER, sizeof( PortMasterEventType ) );
    if(xMasterOsEvent != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL
xMasterPortEventPost(UCHAR ucPort, eMasterEventType eEvent )
{
	PortMasterEventType Event;
	Event.Port = ucPort;
	Event.Event = eEvent;
	if(xQueueSendFromISR( xMasterOsEvent, &Event, 0 )==pdPASS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL
xMasterPortEventGet( PortMasterEventType * eEvent )
{
	PortMasterEventType recvedEvent;
	xQueuePeek(xMasterOsEvent,&recvedEvent,-1);
	eEvent->Port = recvedEvent.Port;
	/* the enum type couldn't convert to int type */
	switch (recvedEvent.Event)
	{
		case EV_MASTER_READY:
			eEvent->Event = EV_MASTER_READY;
			break;
		case EV_MASTER_FRAME_RECEIVED:
			eEvent->Event = EV_MASTER_FRAME_RECEIVED;
			break;
		case EV_MASTER_EXECUTE:
			eEvent->Event = EV_MASTER_EXECUTE;
			break;
		case EV_MASTER_FRAME_SENT:
			eEvent->Event = EV_MASTER_FRAME_SENT;
			break;
		case EV_MASTER_ERROR_PROCESS:
			eEvent->Event = EV_MASTER_ERROR_PROCESS;
			break;
		default:
			vTaskDelay(30);
			return FALSE;
			break;
	}
	xQueueReceive(xMasterOsEvent,&recvedEvent,0);
	return TRUE;
}
/**
 * This function is initialize the OS resource for rs485bus master.
 * Note:The resource is define by OS.If you not use OS this function can be empty.
 *
 */
BOOL vMasterOsResInit( UCHAR ucPort )
{
	vSemaphoreCreateBinary(xMasterRunRes[ucPort])
	return TRUE;
}

/**
 * This function is take RS485bus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be just return TRUE.
 *
 * @param lTimeOut the waiting time.
 *
 * @return resource taked result
 */
BOOL xMasterRunResTake(UCHAR ucPort, LONG lTimeOut )
{
    /*If waiting time is -1 .It will wait forever */
  if( xSemaphoreTake(xMasterRunRes[ucPort], ( TickType_t )lTimeOut) == pdFAIL) 
	{
		return FALSE;
	}
	return TRUE;
}

/**
 * This function is release RS485bus Master running resource.
 * Note:The resource is define by Operating System.If you not use OS this function can be empty.
 *
 */
void vMasterRunResRelease( UCHAR ucPort )
{
  /* release resource */
  xSemaphoreGive(xMasterRunRes[ucPort]);
}

/**
 * This is rs485bus master respond timeout error process callback function.
 * @note There functions will block rs485bus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucPort destination transmit
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMasterErrorCBRespondTimeout(UCHAR ucPort, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for rs485bus master protocol stack.
     * If you don't use OS, you can change it.
     */
    xMasterPortEventPost( ucPort, EV_MASTER_ERROR_RESPOND_TIMEOUT);

    /* You can add your code under here. */

}

/**
 * This is rs485bus master receive data error process callback function.
 * @note There functions will block rs485bus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucPort destination transmit
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMasterErrorCBReceiveData(UCHAR ucPort, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for rs485bus master protocol stack.
     * If you don't use OS, you can change it.
     */
		xMasterPortEventPost(ucPort, EV_MASTER_ERROR_RECEIVE_DATA);

    /* You can add your code under here. */

}

/**
 * This is rs485bus master execute function error process callback function.
 * @note There functions will block rs485bus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 * @param ucPort destination transmit
 * @param pucPDUData PDU buffer data
 * @param ucPDULength PDU buffer length
 *
 */
void vMasterErrorCBExecuteFunction(UCHAR ucPort, const UCHAR* pucPDUData,
        USHORT ucPDULength) {
    /**
     * @note This code is use OS's event mechanism for rs485bus master protocol stack.
     * If you don't use OS, you can change it.
     */
		xMasterPortEventPost(ucPort, EV_MASTER_ERROR_EXECUTE_FUNCTION);
    /* You can add your code under here. */

}

/**
 * This is rs485bus master request process success callback function.
 * @note There functions will block rs485bus master poll while execute OS waiting.
 * So,for real-time of system.Do not execute too much waiting process.
 *
 */
void vMasterCBRequestScuuess( UCHAR ucPort ) {
    /**
     * @note This code is use OS's event mechanism for modbus master protocol stack.
     * If you don't use OS, you can change it.
     */
		xMasterPortEventPost(ucPort, EV_MASTER_PROCESS_SUCESS);
    /* You can add your code under here. */

}

/**
 * This function is wait for rs485bus master request finish and return result.
 * Waiting result include request process success, request respond timeout,
 * receive data error and execute function error.You can use the above callback function.
 * @note If you are use OS, you can use OS's event mechanism. Otherwise you have to run
 * much user custom delay for waiting.
 *
 * @return request error code
 */
eRS485MasterReqErrCode eMasterWaitRequestFinish( UCHAR ucPort ) {
	eRS485MasterReqErrCode    eErrStatus = RS485_MRE_NO_ERR;
	PortMasterEventType recvedEvent;
    /* waiting for OS event */
loop:
		
	xQueuePeek(xMasterOsEvent,&recvedEvent,-1);
  if(recvedEvent.Port == ucPort)
  {
    switch (recvedEvent.Event)
    {
      case EV_MASTER_PROCESS_SUCESS:
          break;
      case EV_MASTER_ERROR_RESPOND_TIMEOUT:
          eErrStatus = RS485_MRE_TIMEDOUT;
          break;
      case EV_MASTER_ERROR_RECEIVE_DATA:
          eErrStatus = RS485_MRE_REV_DATA;
          break;
      case EV_MASTER_ERROR_EXECUTE_FUNCTION:
          eErrStatus = RS485_MRE_EXE_FUN;
          break;
      default:
        vTaskDelay(30);
        goto loop;
        break;
    }
  }
  else
  {
    goto loop;
  }
	xQueueReceive(xMasterOsEvent,&recvedEvent,0);
	return eErrStatus;
}
#endif