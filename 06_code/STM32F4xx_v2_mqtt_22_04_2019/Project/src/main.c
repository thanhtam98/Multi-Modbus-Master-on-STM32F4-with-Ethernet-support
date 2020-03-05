/**
  ******************************************************************************
  * @file    main.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Main program body
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
  * @file    main.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Main program body      
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
/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include "stm32f4xx.h"
#include "stm32f4x7_eth.h"
#include "netconf.h"
#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tcpip.h"
#include "ip_addr.h"

#include "httpserver-socket.h"
#include "httpclient-socket.h"
//#include "tcp_socket.h"
#include "udpclient.h"
#include "stm32f4xx_uartstdio.h"
#include "stm32f4xx_flash.h"
#include "api_server.h"
#include "user_debug.h"
#include "hw_config.h"
#include "stm32f4x7_eth_bsp.h"
#include "MQTTClientTask.h"
#include "RS485bus_task.h"
#include "keyscan_task.h"
#include "TaskMessage.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

const uint8_t BITMASK[7] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
/*--------------- LCD Messages ---------------*/
#define MESSAGE1 "     STM32F4x7      "
#define MESSAGE2 "  STM32F-4 Series   "
#define MESSAGE3 "Basic WebServer Demo"
#define MESSAGE4 "                    "

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void ControlTask(void *pvParameters);
void Port_read_config(void);
void RTCUserInit(void);
void RTC_set_time(void);
void Factory_reset(void);
void modbus_test(void *pvParameters);
/* Private functions ---------------------------------------------------------*/

Device_Lan_t ip_lan;
User_t user;
Host_t host;
CTPORT_TypeDef CTport;
unsigned char ucFlashUpdate = 0;

xQueueHandle xQueuemessage;
xQueueHandle xQueueState;

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/*!< At this stage the microcontroller clock setting is already configured to 
       144 MHz, this is done through SystemInit() function which is called from
       startup file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f4xx.c file
     */

	unsigned char i;
	unsigned int j;

	//	SystemInit();
	RTCUserInit();
	if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
	{
		RTC_set_time();
	}

	UARTstdio_Configuration(USART1, 115200);

	for (i = 0; i < 20; i++)
	{
		DBG("\r\n");
	}

	Port_read_config();
	Lan_read_flash();
	if ((ip_lan.IP.idata == 0xFFFFFFFF) | (ip_lan.IP.idata == 0))
	{
		DBG("\r\nSystem default setting");
		Factory_reset();
	}
	/* Initialize LEDs */
	STM_EVAL_LEDInit(LED1);
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDInit(LED4);
	/* Init Button */
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);

	/* Initialize Output Control*/
	STM_EVAL_OutCTLInit();

	/* Initialize Buzzer */
	STM_EVAL_BuzzerInit();
	/* Turn off Buzzer */
	STM_Buzzer_OFF();

	/* Configure ethernet (GPIOs, clocks, MAC, DMA) */
	ETH_BSP_Config();

	xQueueState = xQueueCreate(1, sizeof(uint8_t));
	xQueuemessage = xQueueCreate(5, sizeof(xQueueMessage));
	if ((xQueuemessage != NULL) && (xQueueState != NULL))
	{
		/* Initilaize the LwIP stack */
		//LwIP_Init(ip_lan.IP.cdata, ip_lan.Subnetmask.cdata, ip_lan.gateway.cdata, ip_lan.dhcp);

		/* Configure the Ethernet */
		//xTaskCreate(ETH_ConfigTask, ETH_CONFIG_TASK, configMINIMAL_STACK_SIZE, NULL, ETH_TASK_PRIO, NULL);

		/* start task key scan */
		//vStartKeyScanTasks(configMINIMAL_STACK_SIZE, KEYSCAN_TASK_PRIO);

		/* Initialize HTTP socket-server */
		//http_server_socket_init(HTTP_SERVER_TASK_PRIO);

		/* Create RS485 Task*/
		vStartRS485busSlaveTasks(configMINIMAL_STACK_SIZE * 2, RS485_SLAVE_TASK_PRIO);
		/* Create RGB LED Task */
		xTaskCreate(ControlTask, "Control Task", configMINIMAL_STACK_SIZE * 2, NULL, CONTROL_TASK_PRIO, NULL);
		
		xTaskCreate(modbus_test, "Test modbus Task", configMINIMAL_STACK_SIZE * 2 , NULL, CONTROL_TASK_PRIO+2,NULL);
		/* Start scheduler */
		vTaskStartScheduler();
	}
	/* We should never get here as control is now taken by the scheduler */
	for (;;)
		;
}
/**
  * @brief  Modbus frame test 
  * @param  pvParameters not used
  * @retval None
  */
void modbus_test(void *pvParameters)
{
	eRS485ErrorCode    eStatus = RS485_ENOERR;
	 UCHAR *ucRS485Frame;
	 UCHAR ucFunctionCode;
	 USHORT usLength;
	 eRS485Exception eException;
	vTaskDelay(500);
	while (1)
	{
		CTport.Port[2].active = PORT_ENABLE;
		 CTport.Port[2].event |= PORT_TRANSMIT_DATA;
	
		DBG("Send data to Dest");
				vTaskDelay(500);
	//	eStatus = peRS485FrameSendCur( 1, 0x00 ,ucRS485Frame, usLength);
	}
}

/**
  * @brief  RTC init 
  * @param  pvParameters not used
  * @retval None
  */

void RTCUserInit(void)
{

	RTC_InitTypeDef RTC_InitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable the PWR clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	/* Allow access to RTC */
	PWR_BackupAccessCmd(ENABLE);

	/* Reset RTC Backup Domain */
	RCC_RTCCLKCmd(DISABLE);
	RCC_LSEConfig(RCC_LSE_ON);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	/* Wait till LSE is ready */
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
	{
	}
	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	// 	RCC_LSICmd(ENABLE);

	//   /* Wait till LSI is ready */
	//   while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	//   {
	//   }

	//   /* Select the RTC Clock Source */
	//   RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
	/* Enable the RTC Clock */
	RCC_RTCCLKCmd(ENABLE);
	/* Wait for RTC APB registers synchronisation */
	RTC_WaitForSynchro();

	/* Configure the RTC data register and RTC prescaler */
	RTC_InitStruct.RTC_AsynchPrediv = 127;
	RTC_InitStruct.RTC_SynchPrediv = 255;
	RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_12;
	RTC_Init(&RTC_InitStruct);
}
void RTC_set_time(void)
{
	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;
	RTC_WriteProtectionCmd(DISABLE);
	/* Set the date: Friday January 11th 2017 */
	RTC_DateStructure.RTC_Year = 17;
	RTC_DateStructure.RTC_Month = RTC_Month_January;
	RTC_DateStructure.RTC_Date = 1;
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Monday;
	RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
	/* Set the time to 05h 20mn 00s AM */
	RTC_TimeStructure.RTC_H12 = RTC_H12_PM;
	RTC_TimeStructure.RTC_Hours = 0;
	RTC_TimeStructure.RTC_Minutes = 0;
	RTC_TimeStructure.RTC_Seconds = 0;
	RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure);
	RTC_WriteProtectionCmd(ENABLE);
	RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}

/**
  * @brief  Control task
  * @param  pvParameters not used
  * @retval None
  */
void ControlTask(void *pvParameters)
{
	uint8_t port;
	uint8_t ledstatus = 0;
	uint8_t doorstatus = 0;
	uint8_t SysState_update = TRUE;
	uint8_t SysState = SYS_START;
	uint8_t SysState_pre = SYS_START;
	uint8_t MqttError = MQTTCLIENT_NONE;
	uint8_t SysEvent;
	xQueueMessage SysMessage;

	for (;;)
	{
		if (SysState != SYS_IDE)
		{
			if (ledstatus)
			{
				STM_EVAL_LEDOn(LED1);
				STM_EVAL_LEDOn(LED2);
				STM_EVAL_LEDOn(LED3);
				STM_EVAL_LEDOn(LED4);
				ledstatus = 0;
			}
			else
			{
				STM_EVAL_LEDOff(LED1);
				STM_EVAL_LEDOff(LED2);
				STM_EVAL_LEDOff(LED3);
				STM_EVAL_LEDOff(LED4);
				ledstatus = 1;
			}
		}
		else
		{
			for (port = RS485_PORT_OFFSET; port < PORT_NUMBER; port++)
				if (CTport.Port[port].status == PORT_OK)
				{
					STM_EVAL_LEDOn(port - RS485_PORT_OFFSET);
				}
				else
				{
					STM_EVAL_LEDOff(port - RS485_PORT_OFFSET);
				}
		}
		if (ucFlashUpdate)
		{
			if (ETH_Disable())
			{
				__disable_irq();
				Flash_Write_Information();
				__enable_irq();
				ETH_Enable();
				ucFlashUpdate = 0;
				DBG("\r\nFlash update");
			}
		}
		switch (SysState)
		{
		case SYS_START:
			DBG("\r\nSystem start");
			SysState = SYS_GET_IP;
			SysState_update = TRUE;
			DBG("\r\nSystem connect Ethernet");
			break;
		case SYS_GET_IP:
			DBG(".");
			if (xQueueReceive(xQueuemessage, &SysMessage, 0) == pdPASS)
			{
				if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(DHCP_TASKNAME))
				{
					if (SysMessage.DhcpMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.DhcpMessage.value == DHCP_GETIP_OK)
						{
							DBG("\r\n\tEthernet adapter Ethernet:");
							DBG("\r\n\t\tConnection-specific DNS Suffix  . :");
							DBG("\r\n\t\tIPv4 Address. . . . . . . . . . . : %d.%d.%d.%d", ip_lan.IP.cdata[0], ip_lan.IP.cdata[1], ip_lan.IP.cdata[2], ip_lan.IP.cdata[3]);
							DBG("\r\n\t\tSubnet Mask . . . . . . . . . . . : %d.%d.%d.%d", ip_lan.Subnetmask.cdata[0], ip_lan.Subnetmask.cdata[1], ip_lan.Subnetmask.cdata[2], ip_lan.Subnetmask.cdata[3]);
							DBG("\r\n\t\tDefault Gateway . . . . . . . . . : %d.%d.%d.%d", ip_lan.gateway.cdata[0], ip_lan.gateway.cdata[1], ip_lan.gateway.cdata[2], ip_lan.gateway.cdata[3]);
							SysState = SYS_SERVER_START;
							SysState_update = TRUE;
							DBG("\r\nSystem start webserver");
						}
					}
				}
			}
			break;
		case SYS_SERVER_START:
			DBG(".");
			if (xQueueReceive(xQueuemessage, &SysMessage, 0) == pdPASS)
			{
				if (SysMessage.HttpServerMessage.TaskHandle == xTaskGetHandle(HTTPSERVER_TASKNAME))
				{
					if (SysMessage.HttpServerMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.HttpServerMessage.value == HTTPSERVER_OK)
						{
							SysState = SYS_POWER_ON;
							SysState_update = TRUE;
						}
					}
				}
				else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(KEYSCAN_TASK))
				{
					if (SysMessage.MqttMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.MqttMessage.value == KEYSCAN_PRESS)
						{
							SysState = SYS_FACTORY_RESET;
						}
					}
				}
			}
			break;
		case SYS_POWER_ON:
			Host_read_flash();
			if ((host.IP.idata == 0xFFFFFFFF) || (host.IP.idata == 0x00000000))
			{
				DBG("\r\nYou need to configure the host IP, host name, username and password");
				SysState = SYS_INCLUSION;
				SysState_update = TRUE;
			}
			else
			{
				SysState = SYS_RS485BUS_START;
				DBG("\r\nSystem start port");
				SysState_update = TRUE;
			}
			break;
		case SYS_RS485BUS_START:
			DBG(".");
			if (xQueueReceive(xQueuemessage, &SysMessage, 0) == pdPASS)
			{
				if (SysMessage.RS485Message.TaskHandle == xTaskGetHandle(RS485_SLAVE_TASK))
				{
					if (SysMessage.RS485Message.Message_type == TASK_EVENT)
					{
						if (SysMessage.RS485Message.value == RS485_START_OK)
						{
							SysState = SYS_CONNECTSERVER;
							SysState_update = TRUE;
							DBG("\r\nSystem connect to server");
							vStartMQTTTasks(DEFAULT_THREAD_STACKSIZE * 2, MQTT_TASK_PRIO);
						}
					}
					else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(KEYSCAN_TASK))
					{
						if (SysMessage.MqttMessage.Message_type == TASK_EVENT)
						{
							if (SysMessage.MqttMessage.value == KEYSCAN_PRESS)
							{
								SysState = SYS_FACTORY_RESET;
							}
						}
					}
				}
			}
			break;
		case SYS_CONNECTSERVER:
			if (xQueueReceive(xQueuemessage, &SysMessage, 0) == pdPASS)
			{
				if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(MQTTCLIENT_TASKNAME))
				{
					if (SysMessage.MqttMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.MqttMessage.value == MQTTCLIENT_CONNECT_OK)
						{
							SysState = SYS_IDE;
							SysState_update = TRUE;
						}
					}
				}
				else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(KEYSCAN_TASK))
				{
					if (SysMessage.MqttMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.MqttMessage.value == KEYSCAN_PRESS)
						{
							SysState = SYS_FACTORY_RESET;
						}
					}
				}
				else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(UDPCLIENT_TASKNAME))
				{
					if (SysMessage.UdpMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.UdpMessage.value == UDPCLIENT_SELECT_INCLUSION)
						{
							SysState = SYS_INCLUSION;
							SysState_update = TRUE;
						}
					}
				}
			}
			break;
		case SYS_INCLUSION:
			if (xQueueReceive(xQueuemessage, &SysMessage, 0) == pdPASS)
			{
				if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(HTTPSERVER_TASKNAME))
				{
					if (SysMessage.HttpServerMessage.Message_type == TASK_EVENT)
					{
						if (SysMessage.HttpServerMessage.value == HTTPSERVER_INCUSION_OK)
						{
							STM_Buzzer_ON();
							vTaskDelete(xTaskGetHandle(MQTTCLIENT_TASKNAME));
							SysState = SYS_POWER_ON;
							SysState_update = TRUE;
						}
					}
					else if (SysMessage.HttpServerMessage.Message_type == TASK_ERROR)
					{
						if (SysMessage.HttpServerMessage.value == HTTPSERVER_INCLUSION_ERROR)
						{
							STM_Buzzer_ON();
							SysState = SysState_pre;
							SysState_update = TRUE;
						}
					}
				}
			}
			break;
		case SYS_FACTORY_RESET:
			Factory_reset();
			NVIC_SystemReset();
			//SysState = SYS_IDE;
			break;
		case SYS_IDE:
			if (xQueueReceive(xQueuemessage, &SysMessage, 0) == pdPASS)
			{
				if (SysMessage.TaskMessage.Message_type == TASK_ERROR)
				{
					if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(DHCP_TASKNAME))
					{
					}
					else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(HTTPSERVER_TASKNAME))
					{
					}
					else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(MQTTCLIENT_TASKNAME))
					{
						MqttError = SysMessage.MqttMessage.value;
					}
				}
				else if (SysMessage.TaskMessage.Message_type == TASK_EVENT)
				{
					if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(DHCP_TASKNAME))
					{
					}
					else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(HTTPSERVER_TASKNAME))
					{
					}
					else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(KEYSCAN_TASK))
					{
						if (SysMessage.MqttMessage.value == KEYSCAN_PRESS)
						{
							SysState = SYS_FACTORY_RESET;
						}
					}
					else if (SysMessage.TaskMessage.TaskHandle == xTaskGetHandle(MQTTCLIENT_TASKNAME))
					{
						if (SysMessage.MqttMessage.value == MQTTCLIENT_BUTTON_PRESS)
						{
							STM_Buzzer_ON();
						}
						else if (SysMessage.MqttMessage.value == MQTTCLIENT_BUTTON_OPEN)
						{
							STM_Buzzer_ON();
						}
						else if (SysMessage.MqttMessage.value == MQTTCLIENT_PORT_ERROR)
						{
							STM_Buzzer_ON();
						}
						else if (SysMessage.MqttMessage.value == MQTTCLIENT_DOORLOCK_DISCONNECT)
						{
							STM_Buzzer_ON();
						}
						else if (SysMessage.MqttMessage.value == MQTTCLIENT_DOOR_OPEN)
						{
							STM_Buzzer_ON();
							STM_Output_HIGH();
						}
						else if (SysMessage.MqttMessage.value == MQTTCLIENT_CONNECT_OK)
						{
							MqttError = MQTTCLIENT_NONE;
						}
					}
				}
			}
			else
			{
				if (MqttError != MQTTCLIENT_NONE)
				{
					if (SysMessage.MqttMessage.value == MQTTNETWORK_CONNECT_ERROR)
					{
					}
					else if (SysMessage.MqttMessage.value == MQTTCLIENT_CONNECT_ERROR)
					{
					}
					else if (SysMessage.MqttMessage.value == MQTTSUBSCRIBE_ERROR)
					{
					}
					else if (SysMessage.MqttMessage.value == MQTTPUSLISH_ERROR)
					{
					}
				}
				else
				{
				}
			}
			break;
		default:
			break;
		}
		if (SysState_update == TRUE)
		{
			xQueueReceive(xQueueState, &SysState_pre, 0);
			xQueueSend(xQueueState, &SysState, 0);
			SysState_update = FALSE;
		}
		vTaskDelay(250);
		STM_Output_LOW();
		STM_Buzzer_OFF();
	}
}

void Port_read_config(void)
{
	unsigned char port;
	for (port = 0; port < PORT_NUMBER; port++)
	{
		CTport.Port[port].active = PORT_DISABLE;
		CTport.Port[port].event = PORT_EVENT_NONE;
		CTport.Port[port].status = PORT_ERROR;
		CTport.Port[port].timeout = PORT_TIMEOUT;
	}
	Port_read_flash();
}

void Factory_reset(void)
{
	char i;
	ip_lan.IP.cdata[0] = IP_ADDR0;
	ip_lan.IP.cdata[1] = IP_ADDR1;
	ip_lan.IP.cdata[2] = IP_ADDR2;
	ip_lan.IP.cdata[3] = IP_ADDR3;
	ip_lan.Subnetmask.cdata[0] = NETMASK_ADDR0;
	ip_lan.Subnetmask.cdata[1] = NETMASK_ADDR1;
	ip_lan.Subnetmask.cdata[2] = NETMASK_ADDR2;
	ip_lan.Subnetmask.cdata[3] = NETMASK_ADDR3;
	ip_lan.gateway.cdata[0] = GW_ADDR0;
	ip_lan.gateway.cdata[1] = GW_ADDR1;
	ip_lan.gateway.cdata[2] = GW_ADDR2;
	ip_lan.gateway.cdata[3] = GW_ADDR3;
	ip_lan.dhcp = FALSE;
	strcpy(&user.UserName[0].cdata[0], "admin");
	strcpy(&user.PassWord[0].cdata[0], "admin");
	host.IP.cdata[0] = HOSTIP_ADDR0;
	host.IP.cdata[1] = HOSTIP_ADDR1;
	host.IP.cdata[2] = HOSTIP_ADDR2;
	host.IP.cdata[3] = HOSTIP_ADDR3;
	strcpy(&host.Host_Name[0].cdata[0], "localhost");
	strcpy(&host.Device_ID[0].cdata[0], "DC02");
	strcpy(&host.Host_Username[0].cdata[0], "test");
	strcpy(&host.Host_Password[0].cdata[0], "1234");

	CTport.number = PORT_NUMBER;
	CTport.Port[0].type = PORT_EXIT;
	CTport.Port[0].active = PORT_ENABLE;
	CTport.Port[1].type = PORT_DOOR_DETECT;
	CTport.Port[1].active = PORT_DISABLE;
	CTport.Port[2].type = PORT_ENTRY_READER;
	CTport.Port[2].active = PORT_ENABLE;
	for (i = 3; i < CTport.number; i++)
	{
		CTport.Port[i].type = PORT_NONE;
		CTport.Port[i].active = PORT_DISABLE;
	}
	Flash_Write_Information();
}

int Bt_debounce(void)
{
	static uint16_t state = 0;
	state = (state << 1) | !STM_EVAL_PBGetState(BUTTON_USER) | 0xe000;
	//	if (state == 0xf000)
	if (state == 0xffff)
		return 1;
	return 0;
}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

	/* Infinite loop */
	while (1)
	{
	}
}
#endif

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	DBG("\r\nfreeHeapsize = %d", xPortGetFreeHeapSize());
	for (;;)
		;
}

void vApplicationStackOverflowHook(void)
{
	taskDISABLE_INTERRUPTS();
	DBG("\r\nStack Overflow Hook");
	for (;;)
		;
}
/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
