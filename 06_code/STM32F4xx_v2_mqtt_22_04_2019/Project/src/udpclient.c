/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved. 
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission. 
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING 
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 * 
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
 
#include "main.h"

#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"

#include "udpclient.h"
#include "TaskMessage.h"
#include "stm32f4_discovery.h"
#include "debug.h"

#include "fs.h"
#include "fsdata.h"
#include "string.h"
#include "api_server.h"

#if LWIP_NETCONN

#include "lwip/api.h"
#include "lwip/sys.h"


#define UDPCLIENT_THREAD_PRIO  ( tskIDLE_PRIORITY + 5 )

extern Host_t host;
extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;
/*-----------------------------------------------------------------------------------*/
static void udpclient_thread(void *arg)
{
  struct sockaddr_in broadcast_address;
	socklen_t addrlen = sizeof(broadcast_address);
	int UDPsocket;
	uint8_t Countplush;
	uint8_t Countopen;
	uint8_t udpError;
	uint8_t UdpEvent = UDPCLIENT_NONE;
	xQueueMessage xqueuemessage;
	uint8_t SysState;
	char payload[30];
	
	broadcast_address.sin_family = AF_INET;
	broadcast_address.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	broadcast_address.sin_port = htons(5000);
	xqueuemessage.UdpMessage.TaskHandle = xTaskGetHandle(UDPCLIENT_TASKNAME);
	do
	{
		vTaskDelay(30);
		xQueuePeek( xQueueState, &SysState, 0 );
	}while(SysState != SYS_CONNECTSERVER);
	
	while(1) 
	{
    if (!STM_EVAL_PBGetState(Button_KEY)) 
		{
			if(Countplush <100)
				Countplush++;
			Countopen =0;
			if(Countplush == 10)
			{
				if((UdpEvent == UDPCLIENT_SEND_INCLUSION)||(UdpEvent = UDPCLIENT_SEND_FACTORYRESET))
				{
					if ((UDPsocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
					{
						DBG("Cannot create UDP socket");
						xqueuemessage.UdpMessage.Message_type = TASK_ERROR;
						xqueuemessage.UdpMessage.value = UDPCLIENT_CREATESOCKET_ERROR;
						xQueueSend(xQueuemessage,&xqueuemessage,0);
						Countplush=0;
					}
					else
					{
						memset(payload,0,sizeof(payload));
						Host_read_flash();
						strncat(payload,&host.Device_Name[0].cdata[0],strlen(&host.Device_Name[0].cdata[0]));
						
						if(UdpEvent == UDPCLIENT_SEND_INCLUSION)
						{
							strcat(payload," INCLUSION");
							if(sendto(UDPsocket,payload,strlen(payload), 0, (struct sockaddr *)&broadcast_address, addrlen) < 0) 
							{
								DBG("UDP broadcast failed \n");
								xqueuemessage.UdpMessage.Message_type = TASK_ERROR;
								xqueuemessage.UdpMessage.value = UDPCLIENT_SEND_ERROR;
								xQueueSend(xQueuemessage,&xqueuemessage,0);
								Countplush=0;
							}
							else 
							{
								DBG("UDP broadcast success \n");
								xqueuemessage.UdpMessage.Message_type = TASK_EVENT;
								xqueuemessage.UdpMessage.value = UDPCLIENT_SEND_INCLUSION;
								xQueueSend(xQueuemessage,&xqueuemessage,0);
							}
						}
						else
						{
							strcat(payload," FACTORY RESET");
							if(sendto(UDPsocket, payload,strlen(payload), 0, (struct sockaddr *)&broadcast_address, addrlen) < 0) 
							{
								DBG("UDP broadcast failed \n");
								xqueuemessage.UdpMessage.Message_type = TASK_ERROR;
								xqueuemessage.UdpMessage.value = UDPCLIENT_SEND_ERROR;
								xQueueSend(xQueuemessage,&xqueuemessage,0);
								Countplush=0;
							}
							else 
							{
								DBG("UDP broadcast success \n");
								xqueuemessage.UdpMessage.Message_type = TASK_EVENT;
								xqueuemessage.UdpMessage.value = UDPCLIENT_SEND_FACTORYRESET;
								xQueueSend(xQueuemessage,&xqueuemessage,0);
							}
						}
						UdpEvent = UDPCLIENT_NONE;
						close(UDPsocket);
					}
				}
			}
			else if(Countplush == 20)
			{
				UdpEvent = UDPCLIENT_SEND_INCLUSION;
				xqueuemessage.UdpMessage.Message_type = TASK_EVENT;
				xqueuemessage.UdpMessage.value = UDPCLIENT_SELECT_INCLUSION;
				xQueueSend(xQueuemessage,&xqueuemessage,0);
			}
			else if(Countplush == 60)
			{
				UdpEvent = UDPCLIENT_SEND_FACTORYRESET;
				xqueuemessage.UdpMessage.Message_type = TASK_EVENT;
				xqueuemessage.UdpMessage.value = UDPCLIENT_SELECT_FACTORYRESET;
				xQueueSend(xQueuemessage,&xqueuemessage,0);
			}
		}
		else 
		{
			if(Countopen<50)
			{
				Countopen++;
			}
			else if(UdpEvent != UDPCLIENT_NONE)
			{
				UdpEvent = UDPCLIENT_NONE;
				xqueuemessage.UdpMessage.Message_type = TASK_EVENT;
				xqueuemessage.UdpMessage.value = UDPCLIENT_NONE;
				xQueueSend(xQueuemessage,&xqueuemessage,0);
			}
			Countplush = 0;
		}
	  vTaskDelay(100);	
	}
}

/*-----------------------------------------------------------------------------------*/
void udpclient_init(void)
{
	sys_thread_new(UDPCLIENT_TASKNAME, udpclient_thread, NULL, DEFAULT_THREAD_STACKSIZE ,UDPCLIENT_THREAD_PRIO );
//	xTaskCreate(udpclient_thread, "udpclient", DEFAULT_THREAD_STACKSIZE , NULL, UDPCLIENT_THREAD_PRIO, NULL);
}

#endif /* LWIP_NETCONN */
