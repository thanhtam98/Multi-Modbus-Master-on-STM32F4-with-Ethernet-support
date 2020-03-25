#ifndef __TCPMBBUS_TASK_H__
#define __TCPMBBUS_TASK_H__


/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "task.h"
#include "user_rs485_app.h"

/* Exported value ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void vStartTCPMBTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority);


#endif
