/*
 * RS485bus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
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
 * File: $Id: portevent.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "rs485port.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ----------------------- Variables ----------------------------------------*/
static xQueueHandle xSlaveOsEvent;

/* ----------------------- Start implementation -----------------------------*/
BOOL xPortEventInit()
{
	xSlaveOsEvent = xQueueCreate(RS485_PORT_NUMBER, sizeof(PortEventType));
	if (xSlaveOsEvent != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL xPortEventPost(UCHAR Port, eEventType event)
{
	PortEventType eEvent;
	eEvent.Port = Port;
	eEvent.Event = event;
	if (xQueueSendFromISR(xSlaveOsEvent, &eEvent, 0) == pdPASS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL xPortEventGet(PortEventType *eEvent)
{
	PortEventType recvedEvent;
	xQueuePeek(xSlaveOsEvent, &recvedEvent, -1);
	eEvent->Port = recvedEvent.Port;
	/* the enum type couldn't convert to int type */
	switch (recvedEvent.Event)
	{
		case EV_READY:
			eEvent->Event = EV_READY;
			break;
		case EV_FRAME_RECEIVED:
			eEvent->Event = EV_FRAME_RECEIVED;
			break;
		case EV_EXECUTE:
			eEvent->Event = EV_EXECUTE;
			break;
		case EV_FRAME_SENT:
			eEvent->Event = EV_FRAME_SENT;
			break;
		default:
			vTaskDelay(30);
			return FALSE;
			break;
	}
	xQueueReceive(xSlaveOsEvent, &recvedEvent, 0);
	return TRUE;
}
