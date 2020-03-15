/**
  ******************************************************************************
  * @file    main.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    main.h
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   This file contains all the functions prototypes for the main.c 
  *          file.      
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB and
  *          STM32F4DIS-LCD modules. 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
 
/* MAC ADDRESS*/
#define MAC_ADDR0   02
#define MAC_ADDR1   00
#define MAC_ADDR2   00
#define MAC_ADDR3   00
#define MAC_ADDR4   00
#define MAC_ADDR5   07
 
/*Static IP ADDRESS*/
#define IP_ADDR0   192
#define IP_ADDR1   168
#define IP_ADDR2   1
#define IP_ADDR3   50

   
/*NETMASK*/
#define NETMASK_ADDR0   255
#define NETMASK_ADDR1   255
#define NETMASK_ADDR2   255
#define NETMASK_ADDR3   0

/*Gateway Address*/
#define GW_ADDR0   192
#define GW_ADDR1   168
#define GW_ADDR2   1
#define GW_ADDR3   1

/*Broadcast IP ADDRESS*/
#define BROADCAST_ADDR0   192
#define BROADCAST_ADDR1   168
#define BROADCAST_ADDR2   1
#define BROADCAST_ADDR3   255

/*HOST IP ADDRESS*/
#define HOSTIP_ADDR0   192
#define HOSTIP_ADDR1   168
#define HOSTIP_ADDR2   1
#define HOSTIP_ADDR3   199

/* MII and RMII mode selection */
#define RMII_MODE

// ports define
#define TCP_PORT 8000
#define UDP_PORT 5000

// errors define
#define ERROR_DEV_ID     -3
#define ERROR_UID        -2
#define ERROR_TCP        -1
#define ERROR_OK          0
#define ERROR_UDP_OK      1
#define ERROR_UID_OK      2
#define ERROR_UID_DETECT  3

/*--------------- Tasks Priority -------------*/
#define KEYSCAN_TASK_PRIO           (tskIDLE_PRIORITY + 6)
#define CONTROL_TASK_PRIO   				(tskIDLE_PRIORITY + 5)
#define RS485_SLAVE_TASK_PRIO				(tskIDLE_PRIORITY + 5)
#define RS485_MASTER_TASK_PRIO			(tskIDLE_PRIORITY + 5)
#define ETH_TASK_PRIO								(tskIDLE_PRIORITY + 4)
#define HTTP_SERVER_TASK_PRIO    		(tskIDLE_PRIORITY + 3)
#define MQTT_TASK_PRIO   						(tskIDLE_PRIORITY + 2)
#define CLIENT_TASK_PRIO						(tskIDLE_PRIORITY + 2)

typedef enum
{
	ERROR_NONE =0,
	ERROR_DHCP,
	ERROR_SERVER,
	ERROR_CLIENT,
	ERROR_SEND_UDP,
	ERROR_UNDETEMINED,
}ERROR_STATUS;

typedef enum
{
	STATE_OK =0,
	STATE_ERROR
}STATE;

typedef enum
{
	SYS_START = 0,
	SYS_GET_IP,
	SYS_SERVER_START,
	SYS_POWER_ON,
	SYS_INCLUSION,
	SYS_FACTORY_RESET,
	SYS_CONNECTSERVER,
	SYS_RS485BUS_START,
	SYS_IDE,
}SYSTEMSTATE;
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */  
void Time_Update(void);
void Delay(uint32_t nCount);
int Bt_debounce(void);


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */


/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/

