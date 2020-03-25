/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Modbus Task include.*/
#include "main.h"
#include "stm32f4xx.h"
#include "TaskMessage.h"
#include "keyscan_task.h"
/* Debug include*/
#include "user_debug.h"
#include "stm32f4_discovery.h"

extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;

static void prvkeyscanSlavePublishTask(void *pvParameters)
{
	uint8_t SysState;
	xQueueMessage xqueuemessage;
	uint8_t keycount = 0;

	xqueuemessage.KeyscanMessage.TaskHandle = xTaskGetHandle(KEYSCAN_TASK);
	do
	{
		vTaskDelay(30);
		xQueuePeek(xQueueState, &SysState, 0);
	} while (SysState != SYS_GET_IP);

	Key_Configuration();

	xqueuemessage.KeyscanMessage.value = KEYSCAN_START_OK;
	xqueuemessage.KeyscanMessage.Message_type = TASK_EVENT;
	xQueueSend(xQueuemessage, &xqueuemessage, 0);
	while (1)
	{
		if (Key_Get(0) == RESET)
		{
			keycount++;
		}
		else
		{
			keycount = 0;
		}
		if (keycount == 100)
		{
			xqueuemessage.KeyscanMessage.value = KEYSCAN_PRESS;
			xqueuemessage.KeyscanMessage.Message_type = TASK_EVENT;
			xQueueSend(xQueuemessage, &xqueuemessage, 0);
			DBG("\r\nSystem is factory reset");
		}
		vTaskDelay(100);
	}
}

void vStartKeyScanTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	int rc = 0;
	rc = xTaskCreate(prvkeyscanSlavePublishTask, /* The function that implements the task. */
					 KEYSCAN_TASK,				 /* Just a text name for the task to aid debugging. */
					 usTaskStackSize,			 /* The stack size is defined in FreeRTOSIPConfig.h. */
					 NULL,						 /* The task parameter, not used in this case. */
					 uxTaskPriority,			 /* The priority assigned to the task is defined in FreeRTOSConfig.h. */
					 NULL);						 /* The task handle is not used. */
}
