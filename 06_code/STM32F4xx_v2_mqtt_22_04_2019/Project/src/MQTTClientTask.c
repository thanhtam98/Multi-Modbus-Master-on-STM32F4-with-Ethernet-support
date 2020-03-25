/* Standard includes. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "TaskMessage.h"

/* TCP includes. */
#include "ip.h"
#include "Sockets.h"

/* MQTT Client include.*/
#include "MQTTClient.h"
#include "MQTTClientTask.h"
#include "api_server.h"
#include "main.h"
#include "miscellaneous.h"
#include "key_scan.h"
/* Debug include*/
#include "user_debug.h"

//#ifndef __cplusplus
//typedef enum {FALSE = 0, TRUE = !FALSE} bool;
//#endif

/* Private typedef -----------------------------------------------------------*/
extern Host_t host;
extern User_t user;
extern CTPORT_TypeDef CTport;

extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;

uint16_t numberpackage[PORT_NUMBER];

void messageArrived(MessageData *data)
{
	char Topic_Receive[30];
	char *point;
	char port = 0;
	xQueueMessage xqueuemessage;
	DBG("\r\nMessage arrived on topic %.*s: %.*s", data->topicName->lenstring.len, data->topicName->lenstring.data,
		data->message->payloadlen, data->message->payload);
	for (port = 0; port < PORT_NUMBER; port++)
	{
		if (CTport.Port[port].active == PORT_ENABLE)
		{
			memset(Topic_Receive, 0, sizeof(Topic_Receive));
			strcat(Topic_Receive, &host.Device_ID[0].cdata[0]);
			strcat(Topic_Receive, "/ports/");
			strcat(Topic_Receive, (char *)itoa(port, 10));
			strcat(Topic_Receive, "/access_auth");
			point = strstr(data->topicName->lenstring.data, Topic_Receive);
			if (point != NULL)
			{
				if (strstr(data->message->payload, itoa(numberpackage[port], 10)))
				{
					if (strstr(point, "forceopen"))
					{
						//                      xqueuemessage.MqttMessage.TaskHandle = xTaskGetHandle(MQTTCLIENT_TASKNAME);
						//                      xqueuemessage.MqttMessage.Message_type = TASK_EVENT;
						//                      xqueuemessage.MqttMessage.value = MQTTCLIENT_DOOR_OPEN;
						//                      xQueueSend(xQueuemessage,&xqueuemessage,0);
						CTport.Port[port].event = (CTport.Port[port].event & 0xF0) | PORT_AUTH_PASS;
						numberpackage[port] = 0;
					}
					else if (strstr(point, "pass"))
					{
						CTport.Port[port].event = (CTport.Port[port].event & 0xF0) | PORT_AUTH_PASS;
					}
					else if (strstr(point, "fail"))
					{
						CTport.Port[port].event = (CTport.Port[port].event & 0xF0) | PORT_AUTH_FAIL;
						numberpackage[port] = 0;
					}
					else if (strstr(point, "error"))
					{
						CTport.Port[port].event = (CTport.Port[port].event & 0xF0) | PORT_AUTH_ERROR;
						numberpackage[port] = 0;
					}
					CTport.Port[port].event |= PORT_TRANSMIT_DATA;
				}
			}
		}
	}
}

static void prvMQTTPublishTask(void *pvParameters)
{
	uint8_t SysState;
	xQueueMessage xqueuemessage;
	MQTTClient client;
	Network network;
	char topic_sub[30];
	unsigned char sendbuf[80], readbuf[80];
	MQTTPacket_connectData connectData = MQTTPacket_connectData_initializer;
	MQTTMessage message;
	char payload[30];
	char topic_pub[30];
	int rc = 0, i, port;
	uint8_t Count = 0, port_error = 0;
	uint8_t ButtonEvent;
	uint8_t DoorLock_Event = EVENT_NONE;
	char Status_button = 0;
	char Status_doorlock = 0;
	uint8_t ctport_change = FALSE;
	uint8_t ADC_Channel = 0;
	uint16_t ADC_value;

	xqueuemessage.MqttMessage.TaskHandle = xTaskGetHandle(MQTTCLIENT_TASKNAME);
	do
	{
		/* Waiting for getting Ip*/
		vTaskDelay(30);
		xQueuePeek(xQueueState, &SysState, 0);
	} while (SysState != SYS_CONNECTSERVER);

	/* ADC configuration */
	ADC_Configuration();

Connect:

	NetworkInit(&network);
	MQTTClientInit(&client, &network, 30000, sendbuf, sizeof(sendbuf), readbuf, sizeof(readbuf));

	/* Read ip of the Server*/
	Host_read_flash();
	DBG("\r\n\tConnect to the Server %d.%d.%d.%d", host.IP.cdata[0], host.IP.cdata[1], host.IP.cdata[2], host.IP.cdata[3]);
	{
	while ((rc = NetworkConnect(&network, host.IP.idata, 1883)) != 0)
		DBG("\tConnect Fail\r\n\tConnect to the Server %d.%d.%d.%d", host.IP.cdata[0], host.IP.cdata[1], host.IP.cdata[2], host.IP.cdata[3]);
		xqueuemessage.MqttMessage.value = MQTTNETWORK_CONNECT_ERROR;
		xqueuemessage.MqttMessage.Message_type = TASK_ERROR;
		xQueueSend(xQueuemessage, &xqueuemessage, 0);
		/* Read ip of the Server*/
		Host_read_flash();
	}

#if defined(MQTT_TASK)
	while ((rc = MQTTStartTask(&client)) != pdPASS)
	{
		vTaskDelay(200);
	}
#endif

	connectData.MQTTVersion = 3;
	connectData.clientID.cstring = &host.Device_ID[0].cdata[0];
	connectData.username.cstring = &host.Host_Username[0].cdata[0];
	connectData.password.cstring = &host.Host_Password[0].cdata[0];
	connectData.willFlag = 1;
	connectData.will.topicName.cstring = "devicewill/";
	connectData.will.message.cstring = &host.Device_ID[0].cdata[0];
	Count = 0;
	while ((rc = MQTTConnect(&client, &connectData)) != 0)
	{
		DBG("\r\n\tPlease check Username and Password again");
		xqueuemessage.MqttMessage.value = MQTTCLIENT_CONNECT_ERROR;
		xqueuemessage.MqttMessage.Message_type = TASK_ERROR;
		xQueueSend(xQueuemessage, &xqueuemessage, 0);
		Count++;
		if (Count > 5)
		{
			close(network.my_socket);
			goto Connect;
		}
		vTaskDelay(200);
	}
	Count = 0;
	// MQTT Subcribe topic Ports/<x>/access_auth
	memset(topic_sub, 0, sizeof(topic_sub));
	strncat(topic_sub, &host.Device_ID[0].cdata[0], strlen(&host.Device_ID[0].cdata[0]));
	strcat(topic_sub, "/ports/#");
	while ((rc = MQTTSubscribe(&client, topic_sub, QOS0, messageArrived)) != 0)
	{
		xqueuemessage.MqttMessage.value = MQTTSUBSCRIBE_ERROR;
		xqueuemessage.MqttMessage.Message_type = TASK_ERROR;
		xQueueSend(xQueuemessage, &xqueuemessage, 0);
		Count++;
		if (Count > 10)
		{
			close(network.my_socket);
			goto Connect;
		}
		vTaskDelay(200);
	}
	xqueuemessage.MqttMessage.value = MQTTCLIENT_CONNECT_OK;
	xqueuemessage.MqttMessage.Message_type = TASK_EVENT;
	xQueueSend(xQueuemessage, &xqueuemessage, 0);
	DBG("\tConnect Successful\r\n");
	for (;;)
	{
		ADC_value = ADC_read(ADCChannelGet(ADC_Channel));
		if ((CTport.Port[0].active == PORT_ENABLE) && (ADC_Channel == 0))
		{
			if (ADC_value < 100)
			{
				ButtonEvent = MQTTCLIENT_BUTTON_OPEN;
			}
			else if (ADC_value < 1700)
			{
				ButtonEvent = MQTTCLIENT_BUTTON_CONNECT;
				CTport.Port[0].status = PORT_OK;
				CTport.Port[0].timeout = PORT_TIMEOUT;
				Status_button = 1;
			}
			else
			{
				CTport.Port[0].status = PORT_OK;
				CTport.Port[0].timeout = PORT_TIMEOUT;
				ButtonEvent = MQTTCLIENT_BUTTON_PRESS;
			}

			if (ButtonEvent == MQTTCLIENT_BUTTON_PRESS)
			{
				if (Status_button == 1)
				{
					Status_button = 0;
					message.qos = 1;
					message.retained = 0;
					message.payload = payload;
					memset(payload, 0, sizeof(payload));
					numberpackage[0]++;
					strcat(payload, itoa(numberpackage[0], 10));
					strcat(payload, "/btn_pressed");
					message.payloadlen = strlen(payload);

					memset(topic_pub, 0, sizeof(topic_pub));
					strncat(topic_pub, &host.Device_ID[0].cdata[0], strlen(&host.Device_ID[0].cdata[0]));
					strcat(topic_pub, "/ports/0");
					if ((rc = MQTTPublish(&client, topic_pub, &message)) != 0)
					{
						xqueuemessage.MqttMessage.value = MQTTPUSLISH_ERROR;
						xqueuemessage.MqttMessage.Message_type = TASK_ERROR;
						xQueueSend(xQueuemessage, &xqueuemessage, 0);
					}
					else
					{
						xqueuemessage.MqttMessage.value = MQTTCLIENT_BUTTON_PRESS;
						xqueuemessage.MqttMessage.Message_type = TASK_EVENT;
						xQueueSend(xQueuemessage, &xqueuemessage, 0);
					}
					DBG("\r\nThe Exit Button is press");
					
				}
			}
		}
		if ((CTport.Port[1].active == PORT_ENABLE) && (ADC_Channel == 1))
		{
			if (ADC_value < 100)
			{
				DoorLock_Event = MQTTCLIENT_DOORLOCK_DISCONNECT;
				Status_doorlock = 0;
			}
			else if (ADC_value < 1700)
			{
				if (Status_doorlock != 1)
				{
					DoorLock_Event = MQTTCLIENT_DOORLOCK_CLOSE;
					CTport.Port[1].status = PORT_OK;
					CTport.Port[1].timeout = PORT_TIMEOUT;
					Status_doorlock = 1;
				}
			}
			else
			{
				if (Status_doorlock != 2)
				{
					DoorLock_Event = MQTTCLIENT_DOORLOCK_OPEN;
					CTport.Port[1].status = PORT_OK;
					CTport.Port[1].timeout = PORT_TIMEOUT;
					Status_doorlock = 2;
				}
			}
			if ((DoorLock_Event == MQTTCLIENT_DOORLOCK_CLOSE) | (DoorLock_Event == MQTTCLIENT_DOORLOCK_OPEN))
			{
				if (Status_doorlock)
				{
					message.qos = 1;
					message.retained = 0;
					message.payload = payload;
					memset(payload, 0, sizeof(payload));
					if (DoorLock_Event == MQTTCLIENT_DOORLOCK_OPEN)
					{
						sprintf(payload, "Door_open");
					}
					else
					{
						sprintf(payload, "Door_close");
					}
					message.payloadlen = strlen(payload);

					memset(topic_pub, 0, sizeof(topic_pub));
					strncat(topic_pub, &host.Device_ID[0].cdata[0], strlen(&host.Device_ID[0].cdata[0]));
					strcat(topic_pub, "/ports/1");
					if ((rc = MQTTPublish(&client, topic_pub, &message)) != 0)
					{
						xqueuemessage.MqttMessage.value = MQTTPUSLISH_ERROR;
						xqueuemessage.MqttMessage.Message_type = TASK_ERROR;
						xQueueSend(xQueuemessage, &xqueuemessage, 0);
					}
					else
					{
						if (DoorLock_Event == MQTTCLIENT_DOORLOCK_OPEN)
						{
							xqueuemessage.MqttMessage.value = MQTTCLIENT_DOORLOCK_OPEN;
						}
						else
						{
							xqueuemessage.MqttMessage.value = MQTTCLIENT_DOORLOCK_CLOSE;
						}
						xqueuemessage.MqttMessage.Message_type = TASK_EVENT;
						xQueueSend(xQueuemessage, &xqueuemessage, 0);
						if (DoorLock_Event == MQTTCLIENT_DOORLOCK_OPEN)
						{
							DBG("\r\nThe Door is open");
						}
						else
						{
							DBG("\r\nThe Door is close");
						}
					}
					DoorLock_Event = EVENT_NONE;
				}
			}
		}
		if (ADC_Channel == 0)
		{
			ADC_Channel = 1;
		}
		else
		{
			ADC_Channel = 0;
		}
		for (port = 2; port < PORT_NUMBER; port++)
		{
			if (CTport.Port[port].event & PORT_CHANGE_CONFIG)
			{
				ctport_change = TRUE;
				CTport.Port[port].event &= ~PORT_CHANGE_CONFIG;
			}
			if (CTport.Port[port].active == PORT_ENABLE)
			{
				if (CTport.Port[port].status == PORT_OK)
				{
					if (CTport.Port[port].event & PORT_RECEIVE_DATA)
					{
						message.qos = 1;
						message.retained = 0;
						message.payload = payload;
						numberpackage[port]++;
						memset(payload, 0, sizeof(payload));
						strcat(payload, itoa(numberpackage[port], 10));
						message.payloadlen = strlen(payload);
						payload[message.payloadlen++] = '/';

						if ((CTport.Port[port].type == PORT_ENTRY_READER) || ((CTport.Port[port].type == PORT_EXIT_READER)))
						{
							for (i = 0; i < CTport.Port[port].Buffer[0]; i++)
							{
								payload[2 * i + message.payloadlen] = HexIntToChar((CTport.Port[port].Buffer[i + 1] & 0xF0) >> 4);
								payload[2 * i + 1 + message.payloadlen] = HexIntToChar((CTport.Port[port].Buffer[i + 1] & 0x0F));
							}
						}
						CTport.Port[port].event &= ~PORT_RECEIVE_DATA;
						message.payloadlen = strlen(payload);

						memset(topic_pub, 0, sizeof(topic_pub));
						strncat(topic_pub, &host.Device_ID[0].cdata[0], strlen(&host.Device_ID[0].cdata[0]));
						strcat(topic_pub, "/ports/");
						strcat(topic_pub, (char *)itoa(port, 10));
						do
						{
							rc = MQTTPublish(&client, topic_pub, &message);
							if (rc != 0)
								Count++;
							else
								Count = 0;
							if (Count > 5)
							{
								Count = 0;
								break;
							}
						} while (rc != 0);
						DBG("\r\nReceived Data from port %d", port);
					}
				}
			}
		}
		for (port = 0; port < PORT_NUMBER; port++)
		{
			if (CTport.Port[port].timeout)
			{
				CTport.Port[port].timeout--;
			}
			else
			{
				CTport.Port[port].status = PORT_ERROR;
				CTport.Port[port].timeout = PORT_TIMEOUT;
				if (CTport.Port[port].active == PORT_ENABLE)
				{
					message.qos = 1;
					message.retained = 0;
					message.payload = payload;

					memset(payload, 0, sizeof(payload));
					strcat(payload, "ports/");
					strcat(payload, (char *)itoa(port, 10));
					if (port == 0)
					{
						strcat(payload, " btn_open");
					}
					else
					{
						strcat(payload, " error");
					}
					message.payloadlen = strlen(payload);

					memset(topic_pub, 0, sizeof(topic_pub));
					strncat(topic_pub, &host.Device_ID[0].cdata[0], strlen(&host.Device_ID[0].cdata[0]));
					strcat(topic_pub, "/status");
					rc = MQTTPublish(&client, topic_pub, &message);

					if (port == 0)
					{
						DBG("\r\nThe Exit Button is lost connect");
					}
					else if (port == 1)
					{
						DBG("\r\nThe Door Sensor is lost connect");
					}
					else
					{
						DBG("\r\nThe Port %d is lost connect", port);
					}
					port_error = 1;
				}
			}
		}
		if (port_error)
		{
			port_error = 0;
			xqueuemessage.MqttMessage.value = MQTTCLIENT_PORT_ERROR;
			xqueuemessage.MqttMessage.Message_type = TASK_EVENT;
			xQueueSend(xQueuemessage, &xqueuemessage, 0);
		}
		if (ctport_change == TRUE)
		{
			ctport_change = FALSE;
			message.qos = 1;
			message.retained = 0;
			message.payload = payload;

			memset(payload, 0, sizeof(payload));
			strcat(payload, "config_change");
			message.payloadlen = strlen(payload);

			memset(topic_pub, 0, sizeof(topic_pub));
			strncat(topic_pub, &host.Device_ID[0].cdata[0], strlen(&host.Device_ID[0].cdata[0]));
			strcat(topic_pub, "/status");
			rc = MQTTPublish(&client, topic_pub, &message);
		}
		if (MQTTIsConnected(&client) == 0)
		{
			xqueuemessage.MqttMessage.value = MQTTCLIENT_CONNECT_ERROR;
			xqueuemessage.MqttMessage.Message_type = TASK_ERROR;
			xQueueSend(xQueuemessage, &xqueuemessage, 0);
			close(network.my_socket);
			goto Connect;
		}
#if !defined(MQTT_TASK)
		if ((rc = MQTTYield(&client, 200)) != 0)
		{
			DBG("\r\nReturn code from yield is %d", rc);
		}
		else
#endif
			vTaskDelay(10);
	}
}

void vStartMQTTTasks(uint16_t usTaskStackSize, UBaseType_t uxTaskPriority)
{
	BaseType_t x = 0L;
	int rc = 0;
	rc = xTaskCreate(prvMQTTPublishTask,  /* The function that implements the task. */
					 MQTTCLIENT_TASKNAME, /* Just a text name for the task to aid debugging. */
					 usTaskStackSize,	 /* The stack size is defined in FreeRTOSIPConfig.h. */
					 (void *)x,			  /* The task parameter, not used in this case. */
					 uxTaskPriority,	  /* The priority assigned to the task is defined in FreeRTOSConfig.h. */
					 NULL);				  /* The task handle is not used. */
}
