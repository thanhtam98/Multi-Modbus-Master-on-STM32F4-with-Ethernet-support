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
#include "mbport.h"
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ----------------------- Variables ----------------------------------------*/
static xQueueHandle xSlaveOsEvent;

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortEventInit()
{
	xSlaveOsEvent = xQueueCreate(1, sizeof(PortEventType));
	if (xSlaveOsEvent != NULL)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL xMBPortEventPost( eMBEventType event)
{
	PortEventType eEvent;
	eEvent = event;
	if (xQueueSendFromISR(xSlaveOsEvent, &eEvent, 0) == pdPASS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL xMBPortEventGet(PortEventType *eEvent)
{
	PortEventType recvedEvent;
	xQueuePeek(xSlaveOsEvent, &recvedEvent, -1);
	*eEvent = recvedEvent;
	/* the enum type couldn't convert to int type */
	switch (recvedEvent)
	{
		case EV_READY:
			*eEvent = EV_READY;
			break;
		case EV_FRAME_RECEIVED:
			*eEvent = EV_FRAME_RECEIVED;
			break;
		case EV_EXECUTE:
			*eEvent= EV_EXECUTE;
			break;
		case EV_FRAME_SENT:
			*eEvent = EV_FRAME_SENT;
			break;
		default:
			vTaskDelay(30);
			return FALSE;
			break;
	}
	xQueueReceive(xSlaveOsEvent, &recvedEvent, 0);
	return TRUE;
}
