/**
  ******************************************************************************
  * @file    httpserver-socket.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011  
  * @brief   Basic http server implementation using LwIP socket API   
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
#include <string.h>
#include "miscellaneous.h"
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "fs.h"
#include "fsdata.h"
#include "httpserver-socket.h"
#include "stm32f4xx_flash.h"
#include "api_server.h"
#include "debug.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define TCPSERVER_THREAD_PRIO    ( tskIDLE_PRIORITY + 4 )

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/**
  * @brief serve tcp connection  
  * @param conn: connection socket 
  * @retval None
  */

void http_server_serve(int conn) 
{
  int buflen = 1500;
  int ret;
  struct fs_file * file;
  unsigned char recv_buffer[1500];
	unsigned char send_buffer[100];
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	
  /* Read in the request */
  ret = read(conn, recv_buffer, buflen); 
	
  if(ret < 0) return;
		recv_buffer[ret] = '\0';
	DBG("Received data to client: %s\n",recv_buffer);
	memset(send_buffer,0,sizeof(send_buffer));
	send_buffer[0] = COMMAND_SUCCESS;
	// Config network for device
	if(strncmp((char *)recv_buffer, POST_NETWORK_CONFIG, sizeof(POST_NETWORK_CONFIG)-1)==0)
	{
			network_config((char *)recv_buffer);
	}
  // Confgure host server for device
  else if(strncmp((char *)recv_buffer, POST_HOST_CONFIG, sizeof(POST_HOST_CONFIG)-1)==0)
  {
    host_config((char *)recv_buffer);
  }    
	// Change username and password for device
	else if(strncmp((char *)recv_buffer, POST_CHANGE_USERNAME_PASSWORD, sizeof(POST_CHANGE_USERNAME_PASSWORD)-1)==0)
	{
			if(username_password_config((char *)recv_buffer))
			{
				memset(send_buffer,0,sizeof(send_buffer));
				send_buffer[0] =COMMAND_ERRO;
			}
	}
	// Set time
	else if(strncmp((char *)recv_buffer, POST_SET_TIME, sizeof(POST_SET_TIME)-1)==0)
	{
			set_time((char *)recv_buffer);
	}
	// Get time
	else if(strncmp((char *)recv_buffer, GET_DATA_TIME, sizeof(GET_DATA_TIME)-1)==0)
	{
		// Get time from RTC
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		// Get date from RTC
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
		memset(send_buffer,0,sizeof(send_buffer));
		if(RTC_TimeStruct.RTC_Hours<10)
		{
			strcat((char*)send_buffer,"0");
		}
		if (RTC_TimeStruct.RTC_Hours==0)
		{
			strcat((char*)send_buffer,"0");
		}
		else
		{
			strcat((char*)send_buffer,(char*)itoa(RTC_TimeStruct.RTC_Hours,10));
		}
		strcat((char*)send_buffer,":");
		if(RTC_TimeStruct.RTC_Minutes<10)
		{
			strcat((char*)send_buffer,"0");
		}
		if (RTC_TimeStruct.RTC_Minutes==0)
		{
			strcat((char*)send_buffer,"0");
		}
		else
		{
			strcat((char*)send_buffer,(char*)itoa(RTC_TimeStruct.RTC_Minutes,10));
		}
		strcat((char*)send_buffer,":");
		if(RTC_TimeStruct.RTC_Seconds<10)
		{
			strcat((char*)send_buffer,"0");
		}
		if (RTC_TimeStruct.RTC_Seconds==0)
		{
			strcat((char*)send_buffer,"0");
		}
		else
		{
			strcat((char*)send_buffer,(char*)itoa(RTC_TimeStruct.RTC_Seconds,10));
		}
		if(RTC_TimeStruct.RTC_H12)
		{
			strcat((char*)send_buffer," PM ");
		}
		else
		{
			strcat((char*)send_buffer," AM ");
		}
		switch(RTC_DateStruct.RTC_WeekDay)
		{
			case RTC_Weekday_Monday: 
				strcat((char*)send_buffer,"Monday");
			break;
			case RTC_Weekday_Tuesday: 
				strcat((char*)send_buffer,"Tuesday");
			break;
			case RTC_Weekday_Wednesday: 
				strcat((char*)send_buffer,"Wednesday");
			break;
			case RTC_Weekday_Thursday: 
				strcat((char*)send_buffer,"Thursday");
			break;
			case RTC_Weekday_Friday: 
				strcat((char*)send_buffer,"Friday");
			break;
			case RTC_Weekday_Saturday: 
				strcat((char*)send_buffer,"Saturday");
			break;
			case RTC_Weekday_Sunday: 
				strcat((char*)send_buffer,"Sunday");
			break;
			default:
				break;
		}
		strcat((char*)send_buffer,", ");
		if(RTC_DateStruct.RTC_Date<10)
		{
			strcat((char*)send_buffer,"0");
		}
		if (RTC_DateStruct.RTC_Date==0)
		{
			strcat((char*)send_buffer,"0");
		}
		else
		{
			strcat((char*)send_buffer,(char*)itoa(RTC_DateStruct.RTC_Date,10));
		}
		strcat((char*)send_buffer,"/");
		if(RTC_DateStruct.RTC_Month<10)
		{
			strcat((char*)send_buffer,"0");
		}
		if (RTC_DateStruct.RTC_Month==0)
		{
			strcat((char*)send_buffer,"0");
		}
		else
		{
			strcat((char*)send_buffer,(char*)itoa(RTC_DateStruct.RTC_Month,10));
		}
		strcat((char*)send_buffer,"/");
		if(RTC_DateStruct.RTC_Year<10)
		{
			strcat((char*)send_buffer,"0");
		}
		if (RTC_DateStruct.RTC_Year==0)
		{
			strcat((char*)send_buffer,"0");
		}
		else
		{
			strcat((char*)send_buffer,(char*)itoa(RTC_DateStruct.RTC_Year,10));
		}
	}
	// Reboot device
	else if(strncmp((char *)recv_buffer,GET_REBOOT, sizeof(GET_REBOOT)-1)==0)
	{
		write(conn, (const unsigned char*)(send_buffer), (size_t)strlen((char*)send_buffer));
		close(conn);
		vTaskDelay(10);
		NVIC_SystemReset();
	}
	// Add or modify card
	else if(strncmp((char *)recv_buffer, POST_ADD_OR_MODIFY_CARD, sizeof(POST_ADD_OR_MODIFY_CARD)-1)==0)
	{
		
	}
	// The API is not support
	else
	{
		memset(send_buffer,0,sizeof(send_buffer));
		send_buffer[0] =COMMAND_ERRO; 
	}
	write(conn, (const unsigned char*)(send_buffer), (size_t)strlen((char*)send_buffer));
	close(conn);
	return;
}

/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_socket_thread(void *arg)
{
  int sock, newconn, size;
  struct sockaddr_in address, remotehost;

 /* create a TCP socket */
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
  {
    DBG("can not create socket");
    return;
  }
  
  /* bind to port 80 at any interface */
  address.sin_family = AF_INET;
  address.sin_port = htons(8000);
  address.sin_addr.s_addr = INADDR_ANY;

  if (bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
  {
    DBG("can not bind socket");
    return;
  }
  
  /* listen for incoming connections (TCP listen backlog = 5) */
  listen(sock, 5);
  
  size = sizeof(remotehost);
  
  while (1) 
  {
    newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
    http_server_serve(newconn);
  }
}
/**
  * @brief  Initialize the TCP server (start its thread) 
  * @param  none
  * @retval None
  */
void tcp_server_socket_init()
{
  sys_thread_new("TCPSERVER", http_server_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE, TCPSERVER_THREAD_PRIO);
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
