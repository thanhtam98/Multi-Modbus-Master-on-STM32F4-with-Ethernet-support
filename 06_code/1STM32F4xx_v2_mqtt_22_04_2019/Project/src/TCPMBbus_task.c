/*
File:	TCPMBbus_task.c
Date: 19 - 03 - 2020
Author: NTT BK
Brief: Modbus tcp handle task
*/

#define MB_TCP_PSEUDO_ADDRESS   1

/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Modbus Task include.*/
#include "main.h"
#include "stm32f4xx.h"
#include "tcp_mb.h"
#include "TaskMessage.h"
#include "TCPMBbus_task.h"

/* Debug include*/
#include "user_debug.h"
#include "stm32f4_discovery.h"

static UCHAR Vendor[14] = "WVC 0.2 - dB14";

extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;

static void prvTCPMBPublishTask(void *pvParaneters)
{
	
	uint8_t SysState;
	xQueueMessage xqueuemessage;
	xqueuemessage.TCPMbMessage.TaskHandle = xTaskGetHandle(TCPMB_SLAVE_TASK);
	do
	{
		vTaskDelay(30);
		xQueuePeek( xQueueState, &SysState, 0 );
	}while(SysState != SYS_TCPMBBUS_START);

	//vTaskDelay(1);
	DBG("\r\n Starting to init TCPMB");
	eMBSetSlaveID( MB_TCP_PSEUDO_ADDRESS, TRUE, Vendor, sizeof(Vendor) );
   eMBTCPInit(0);
	
	
  xqueuemessage.TCPMbMessage.value = TCPMB_START_OK;
	xqueuemessage.TCPMbMessage.Message_type = TASK_EVENT;
	xQueueSend(xQueuemessage,&xqueuemessage,0);
	
	DBG("\r\n Polling");
	
	while (1)
	{
		eMBPoll();
    vTaskDelay(25);
	}
	
}

void vStartTCPMBTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	int rc = 0;
	rc = xTaskCreate(prvTCPMBPublishTask,	/* The function that implements the task. */
										TCPMB_SLAVE_TASK,					/* Just a text name for the task to aid debugging. */
										usTaskStackSize,						/* The stack size is defined in FreeRTOSIPConfig.h. */
										NULL,												/* The task parameter, not used in this case. */
										uxTaskPriority,							/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
										NULL);			
	
}
