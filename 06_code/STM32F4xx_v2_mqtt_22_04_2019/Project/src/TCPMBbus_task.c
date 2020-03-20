/*
File:	TCPMBbus_task.c
Date: 19 - 03 - 2020
Author: NTT BK
Brief: Modbus tcp handle task
*/



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

extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;

static void prvTCPMBPublishTask(void *pvParaneters)
{
	/*
	uint8_t SysState;
	xQueueMessage xqueuemessage;
	xqueuemessage.TCPMbMessage.TaskHandle = xTaskGetHandle(RS485_SLAVE_TASK);
	do
	{
		vTaskDelay(30);
		xQueuePeek( xQueueState, &SysState, 0 );
	}while(SysState != SYS_TCPMBBUS_START);
*/
	vTaskDelay(10000);
	DBG("\r\n Starting to init TCPMB");
   eMBTCPInit(0);
	/*
  xqueuemessage.RS485Message.value = RS485_START_OK;
	xqueuemessage.RS485Message.Message_type = TASK_EVENT;
	xQueueSend(xQueuemessage,&xqueuemessage,0);
	*/
	DBG("\r\n Polling");
	while (1)
	{
			eMBPoll();
    vTaskDelay(25	);
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
