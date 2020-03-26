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
#include "rs485.h"
#include "rs485_m.h"
#include "rs485config.h"
#include "TaskMessage.h"
#include "RS485bus_task_m.h"
/* Debug include*/
#include "debug.h"
#include "stm32f4_discovery.h"

extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;

#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
static void prvRS485MasterPublishTask(void *pvParameters)
{
	UCHAR port;
	uint8_t SysState;
	xQueueMessage xqueuemessage;
	xqueuemessage.RS485Message.TaskHandle = xTaskGetHandle(RS485_SLAVE_TASK);
	do
	{
		vTaskDelay(30);
		xQueuePeek( xQueueState, &SysState, 0 );
	}while(SysState != SYS_RS485BUS_START);
  
	eRS485MasterInit(RS485_RTU, 9600,  PORT_PAR_NONE);
	eRS485MasterEnable(0);	
	eRS485MasterEnable(1);
	eRS485MasterEnable(2);
	eRS485MasterEnable(3);
	
			
  vTaskDelay(100);
  xqueuemessage.RS485Message.value = RS485_START_OK;
	xqueuemessage.RS485Message.Message_type = TASK_EVENT;
	xQueueSend(xQueuemessage,&xqueuemessage,0);
	while (1)
	{
		eRS485MasterPoll();
		vTaskDelay(25);
	}
}

void vStartRS485busMasterTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	int rc = 0;
	rc = xTaskCreate(prvRS485MasterPublishTask,	/* The function that implements the task. */
										RS485_SLAVE_TASK,					/* Just a text name for the task to aid debugging. */
										usTaskStackSize,						/* The stack size is defined in FreeRTOSIPConfig.h. */
										NULL,												/* The task parameter, not used in this case. */
										uxTaskPriority,							/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
										NULL);											/* The task handle is not used. */
}
#endif