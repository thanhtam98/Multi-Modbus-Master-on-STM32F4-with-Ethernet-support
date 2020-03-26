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
#include "mb.h"
#include "TaskMessage.h"
#include "RS485bus_task.h"
#include "mbport.h"
/* Debug include*/
#include "debug.h"
#include "stm32f4_discovery.h"

#define ucIvtDeltaAdr 0x08
extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;


static void prvRS485SlavePublishTask(void *pvParameters)
{
	uint8_t SysState;
	xQueueMessage xqueuemessage;
	xqueuemessage.RS485Message.TaskHandle = xTaskGetHandle(RS485_SLAVE_TASK);
	do
	{
		vTaskDelay(30);
		xQueuePeek( xQueueState, &SysState, 0 );
	}while(SysState != SYS_RS485BUS_START);
    
	eMBInit(MB_RTU, ucIvtDeltaAdr, 9600 ,MB_PAR_NONE);
	eMBEnable();
    
  xqueuemessage.RS485Message.value = RS485_START_OK;
	xqueuemessage.RS485Message.Message_type = TASK_EVENT;
	xQueueSend(xQueuemessage,&xqueuemessage,0);
	vTaskDelay(100);
	while (1)
	{
	eMBPoll();
  vTaskDelay(250);
	}
}

void vStartRS485busSlaveTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	int rc = 0;
	rc = xTaskCreate(prvRS485SlavePublishTask,	/* The function that implements the task. */
										RS485_SLAVE_TASK,					/* Just a text name for the task to aid debugging. */
										usTaskStackSize,						/* The stack size is defined in FreeRTOSIPConfig.h. */
										NULL,												/* The task parameter, not used in this case. */
										uxTaskPriority,							/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
										NULL);											/* The task handle is not used. */
}
