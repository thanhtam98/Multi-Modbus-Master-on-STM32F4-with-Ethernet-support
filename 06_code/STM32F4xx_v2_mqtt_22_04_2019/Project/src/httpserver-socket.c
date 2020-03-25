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
#include "httpserver-socket.h"
#include "stm32f4xx_flash.h"
#include "api_server.h"
#include "rtc.h"
#include "main.h"
#include "hw_config.h"
#include "user_debug.h"
#include "stm32f4_discovery.h"
#include "TaskMessage.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

typedef enum
{
    HTTP_RESPONSE_NONE,
    HTTP_RESPONSE_OK,
    HTTP_RESPONSE_ERRO
}HTTP_RESPONSE;
static const char STR_HTTP_RESPONSE[] = "HTTP/1.1 200 ";
static const char STR_HTTP_RESPONSE_OK[] = "HTTP/1.1 200 OK";
static const char STR_HTTP_RESPONSE_ERR[] = "HTTP/1.1 400 ERR";
static const char STR_CRLF[] = "\r\n";
//extern Device_Lan_t ip_lan;
u32_t nPageHits = 0;
portCHAR PAGE_BODY[512];

extern Device_Lan_t ip_lan;
extern User_t user;
extern Host_t host;
extern CTPORT_TypeDef CTport;

extern xQueueHandle xQueueState;
extern xQueueHandle xQueuemessage;
/**
  * @brief serve tcp connection  
  * @param conn: connection socket 
  * @retval None
  */

void http_server_serve(int conn,struct sockaddr_in *addr) 
{
	#define NUMBER_LOGIN 2
	int8_t http_resp = HTTP_RESPONSE_NONE;
	volatile int ret;
	struct fs_file * file;
	unsigned char recv_buffer[1500];
	unsigned char send_buffer[100];
	struct in_addr struct_ip;
	char *Pointer_User,*Pointer_Pass,*point,User_Pass[50];
	char i;
	static uint32_t S_addr_login[NUMBER_LOGIN] = {0,0};
	static uint32_t Time_login[NUMBER_LOGIN] ={0,0};
	uint8_t status_login = FALSE;
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	for(i=0;i<NUMBER_LOGIN;i++)
	{
			Time_login[i]++;
	}
	for(i=0;i<NUMBER_LOGIN;i++)
	{
			if(S_addr_login[i] == addr->sin_addr.s_addr)
			{
					status_login = TRUE;
					Time_login[i] =0;
			} 
			if(Time_login[i]>200)
			{
					Time_login[i] =0;
					S_addr_login[i] =0;
			} 
	}
	
	if(conn>=0)
	{
			/* Read in the request */
			ret = read(conn, recv_buffer, sizeof(recv_buffer)); 	
			if(ret <= 0)
			{	
					close(conn);
					return;
			}
			
			recv_buffer[ret] = '\0';
			//DBG("\r\nReceived data to client: %s\n",recv_buffer);
			memset(send_buffer,0,sizeof(send_buffer));
			send_buffer[0] = COMMAND_SUCCESS;
			User_read_flash();   
			Pointer_User = strstr((char *)recv_buffer,"username");
			memset(User_Pass,0,sizeof(User_Pass));
			strcat(User_Pass,"username=");
			strcat(User_Pass,user.UserName[0].cdata);
			strcat(User_Pass,"&password=");
			strcat(User_Pass,user.PassWord[0].cdata);
			
			// Config network for device
			if(strncmp((char *)recv_buffer, POST_NETWORK_CONFIG, sizeof(POST_NETWORK_CONFIG)-1)==0)
			{
					if(status_login)
					{
							http_resp = HTTP_RESPONSE_OK;
							Api_lan_config((char *)recv_buffer);
					}
			}
			// Confgure host server for device
			else if(strncmp((char *)recv_buffer, POST_HOST_CONFIG, sizeof(POST_HOST_CONFIG)-1)==0)
			{ 
					if(status_login)
					{
							Api_host_config((char *)recv_buffer);
							http_resp = HTTP_RESPONSE_OK;
					}
			}    
			// Change username and password for device
			else if(strncmp((char *)recv_buffer, POST_CHANGE_USERNAME_PASSWORD, sizeof(POST_CHANGE_USERNAME_PASSWORD)-1)==0)
			{
					if(status_login)
					{
							http_resp = HTTP_RESPONSE_OK;
							username_password_config((char *)recv_buffer);
					}
			}
			// Set time
			else if(strncmp((char *)recv_buffer, POST_SET_TIME, sizeof(POST_SET_TIME)-1)==0)
			{
					if(status_login)
					{
							http_resp = HTTP_RESPONSE_OK;
              set_time((char *)recv_buffer);
					}
			}
			// Get time
			else if(strncmp((char *)recv_buffer, GET_DATA_TIME, sizeof(GET_DATA_TIME)-1)==0)
			{
					// Get time from RTC
					if(status_login)
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
							http_resp = HTTP_RESPONSE_OK;
					}
			}
			// Reboot device
			else if(strncmp((char *)recv_buffer,GET_REBOOT, sizeof(GET_REBOOT)-1)==0)
			{
					if(status_login)
					{
							write(conn, STR_HTTP_RESPONSE_OK, sizeof(STR_HTTP_RESPONSE_OK));
							write(conn, STR_CRLF, sizeof(STR_CRLF));
							write(conn, STR_CRLF, sizeof(STR_CRLF));
							write(conn, (const unsigned char*)(send_buffer), (size_t)strlen((char*)send_buffer));
							close(conn);
							vTaskDelay(10);
							NVIC_SystemReset();
					}
			}
			// Get port config
			else if(strncmp((char *)recv_buffer,GET_PORT_CONFIG, sizeof(GET_PORT_CONFIG)-1)==0)
			{
					if(status_login)
					{
							memset(recv_buffer,0,sizeof(recv_buffer));
							Port_read_flash();
							strcat((char *)recv_buffer, STR_HTTP_RESPONSE);
							strcat((char *)recv_buffer, STR_CRLF);
							strcat((char *)recv_buffer, STR_CRLF);
							
							strcat((char *)recv_buffer,"deviceID=");
							strcat((char *)recv_buffer,&host.Device_ID[0].cdata[0]);
							for(i=0;i<CTport.number;i++)
							{
									strcat((char *)recv_buffer,"&port");
									strcat((char *)recv_buffer,(char *)itoa(i,10));
									strcat((char *)recv_buffer,"_type=");
									strcat((char *)recv_buffer,(char *)itoa(CTport.Port[i].type,10));
									strcat((char *)recv_buffer,"&");
									strcat((char *)recv_buffer,"port");
									strcat((char *)recv_buffer,(char *)itoa(i,10));
									strcat((char *)recv_buffer,"_active=");
									strcat((char *)recv_buffer,(char *)itoa(CTport.Port[i].active,10));
							}
							write(conn, (const unsigned char*)(recv_buffer), strlen((const char *)recv_buffer));
					}
					else
					{
							http_resp = HTTP_RESPONSE_ERRO;
					}
			}
			// Post port config
			else if(strncmp((char *)recv_buffer,POST_PORT_CONFIG, sizeof(POST_PORT_CONFIG)-1)==0)
			{
					if(status_login)
					{
							port_config((char *)recv_buffer);
							http_resp = HTTP_RESPONSE_OK;
					}
					else
					{
							http_resp = HTTP_RESPONSE_ERRO;
					}
			}
	/*----Webserver-------------------------------------------------------------------------------------------*/
			else if(strncmp((char *)recv_buffer, "GET /style/style.css", 20) == 0)
			{
					/* Load Login page */
					file = fs_open("/style/style.css"); 
					write(conn, (const unsigned char*)(file->data), (size_t)file->len);
					if(file) fs_close(file);
			}
			else if(strncmp((char *)recv_buffer, "GET /script/common.js", 21) == 0)
			{
					/* Load Login page */
					file = fs_open("/script/common.js"); 
					write(conn, (const unsigned char*)(file->data), (size_t)file->len);
					if(file) fs_close(file);
			}
			else if(strncmp((char *)recv_buffer, "GET /script/datahost.js", 23) == 0)
			{
					if(status_login)
					{
							Host_read_flash();
							memset(recv_buffer,0,sizeof(recv_buffer));
							strcat((char *)recv_buffer,"var DataString = new Object();\n");
							
							strcat((char *)recv_buffer,"DataString.username = \"");		
							strcat((char *)recv_buffer,host.Host_Username[0].cdata);
							
							strcat((char *)recv_buffer,"\";\nDataString.password = \"");
							strcat((char *)recv_buffer,host.Host_Password[0].cdata);
							
							strcat((char *)recv_buffer,"\";\nDataString.hostip = \"");
							struct_ip.s_addr = host.IP.idata;
							strcat((char *)recv_buffer,inet_ntoa(struct_ip));
							
							strcat((char *)recv_buffer,"\";\nDataString.hostname = \"");
							strcat((char *)recv_buffer,host.Host_Name[0].cdata);
							
							strcat((char *)recv_buffer,"\";\nDataString.Device_Name = \"");
							strcat((char *)recv_buffer,host.Device_ID[0].cdata);
							strcat((char *)recv_buffer,"\";\n");
							write(conn, (const unsigned char*)(recv_buffer), strlen((const char *)recv_buffer));
					}
			}
			else if(strncmp((char *)recv_buffer, "GET /script/datalan.js", 22) == 0)
			{
					if(status_login)
					{
							memset(recv_buffer,0,sizeof(recv_buffer));
							Lan_read_flash();
							strcat((char *)recv_buffer,"var DataString = new Object();\n");
							strcat((char *)recv_buffer,"DataString.deviceid = \"");
							strcat((char *)recv_buffer,host.Device_ID[0].cdata);
							
							strcat((char *)recv_buffer,"\";\nDataString.ip = \"");
							struct_ip.s_addr = ip_lan.IP.idata;
							strcat((char *)recv_buffer,inet_ntoa(struct_ip));
							
							strcat((char *)recv_buffer,"\";\nDataString.subnetmask = \"");
							struct_ip.s_addr = ip_lan.Subnetmask.idata;
							strcat((char *)recv_buffer,inet_ntoa(struct_ip));
							
							strcat((char *)recv_buffer,"\";\nDataString.gateway = \"");
							struct_ip.s_addr = ip_lan.gateway.idata;
							strcat((char *)recv_buffer,inet_ntoa(struct_ip));
							
							strcat((char *)recv_buffer,"\";\nDataString.dhcp = \"");
							strcat((char *)recv_buffer,(char*)itoa(ip_lan.dhcp,10));        
							strcat((char *)recv_buffer,"\";\n");
							write(conn, (const unsigned char*)(recv_buffer), strlen((const char *)recv_buffer));
					}
			}
			else if(strncmp((char *)recv_buffer, "GET /script/dataport.js", 23) == 0)
			{
					if(status_login)
					{
							memset(recv_buffer,0,sizeof(recv_buffer));
							Port_read_flash();
							strcat((char *)recv_buffer,"var DataString = new Object();\n");
							for(i=0;i<CTport.number;i++)
							{
									strcat((char *)recv_buffer,"DataString.port");
									strcat((char *)recv_buffer,(char *)itoa(i,10));
									strcat((char *)recv_buffer,"_type = \"");
									strcat((char *)recv_buffer,(char *)itoa(CTport.Port[i].type,10));
									strcat((char *)recv_buffer,"\";\n");
									strcat((char *)recv_buffer,"DataString.port");
									strcat((char *)recv_buffer,(char *)itoa(i,10));
									strcat((char *)recv_buffer,"_active = \"");
									strcat((char *)recv_buffer,(char *)itoa(CTport.Port[i].active,10));
									strcat((char *)recv_buffer,"\";\n");
							}
							write(conn, (const unsigned char*)(recv_buffer), strlen((const char *)recv_buffer));
					}
			}
			else if(strncmp((char *)recv_buffer, "GET /lanconfig", 14)==0)
			{
					if(status_login)
					{
							/* Load Login page */
							file = fs_open("/lanconfig.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			else if(strncmp((char *)recv_buffer, POST_WEB_NETWORK_CONFIG, sizeof(POST_WEB_NETWORK_CONFIG)-1)==0)
			{
					if(status_login)
					{
							Web_lan_config((char *)recv_buffer);
							file = fs_open("/lanconfig.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			else if(strncmp((char *)recv_buffer, "GET /portconfig", 15)==0)
			{
					if(status_login)
					{
							/* Load port config page */
							file = fs_open("/portconfig.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			else if(strncmp((char *)recv_buffer, POST_WEB_PORT_CONFIG, sizeof(POST_WEB_PORT_CONFIG)-1)==0)
			{
					if(status_login)
					{
							port_config((char *)recv_buffer);
							file = fs_open("/portconfig.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
              
					}
			}
			else if(strncmp((char *)recv_buffer, "GET /hostconfig", 15)==0)
			{
					if(status_login)
					{
							/* Load Login page */   
							file = fs_open("/hostconfig.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			 else if(strncmp((char *)recv_buffer, POST_WEB_HOST_CONFIG, sizeof(POST_WEB_HOST_CONFIG)-1)==0)
			{
					if(status_login)
					{
							Web_host_config((char *)recv_buffer);
							file = fs_open("/hostconfig.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			else if(strncmp((char *)recv_buffer, POST_USERNAME_PASSWORD, sizeof(POST_USERNAME_PASSWORD)-1)==0)
			{
					if(status_login)
					{
							username_password_config((char *)recv_buffer);
              file = fs_open("/maintain.html");
							//file = fs_open("/login.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			else if(strncmp((char *)recv_buffer, "GET /maintain", 13)==0)
			{
					if(status_login)
					{
							/* Load maintain page */
							file = fs_open("/maintain.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
					}
			}
			else if((strncmp((char *)recv_buffer, "GET /Reboot", 11)==0)|(strncmp((char *)recv_buffer, "POST /Reboot", 12)==0))
			{
					if(status_login)
					{
							/* Load Login page */
              ApiSystemReboot();
							file = fs_open("/reboot.html"); 
							write(conn, (const unsigned char*)(file->data), (size_t)file->len);
							if(file) fs_close(file);
              
					}
			}
			else if((strncmp((char *)recv_buffer, "GET /login", 10) == 0)||(strncmp((char *)recv_buffer, "GET / ", 6) == 0))
			{
					/* Load Login page */
					file = fs_open("/login.html"); 
					write(conn, (const unsigned char*)(file->data), (size_t)file->len);
					if(file) fs_close(file);
			}
			else if(strncmp((char *)recv_buffer, "GET /logout", 11) == 0)
			{
					for(i=0;i<NUMBER_LOGIN;i++)
					{
							if(S_addr_login[i] == addr->sin_addr.s_addr)
							{
									/* Load Login page */
									S_addr_login[i] = 0;
									file = fs_open("/login.html"); 
									write(conn, (const unsigned char*)(file->data), (size_t)file->len);
									if(file) fs_close(file);
									break;
									
							}
					}
			}
			else if(strncmp((char *)recv_buffer, "POST /login", 11)==0)
			{
					for(i=0;i<NUMBER_LOGIN;i++)
					{
							if(S_addr_login[i] == 0)
							{
									if(strcmp(Pointer_User,User_Pass)==0)
									{
                    if(strcmp(User_Pass,Pointer_User)==0)
                    {
											S_addr_login[i] = addr->sin_addr.s_addr;
											file = fs_open("/index.html"); 
											write(conn, (const unsigned char*)(file->data), (size_t)file->len);
											if(file) fs_close(file);
											break;
                    }
									}
									else
									{
											/* Load Login page */
											file = fs_open("/login.html"); 
											write(conn, (const unsigned char*)(file->data), (size_t)file->len);
											if(file) fs_close(file);
                      break;
									}
							}
					}
          if(i==NUMBER_LOGIN)
          {
              /* Load Login page */
              file = fs_open("/login.html"); 
              write(conn, (const unsigned char*)(file->data), (size_t)file->len);
              if(file) fs_close(file);
          }
			}
			// The API is not support
			else
			{
					http_resp = HTTP_RESPONSE_ERRO;
					send_buffer[0] =COMMAND_ERRO; 
			}
			if (http_resp == HTTP_RESPONSE_OK)
					write(conn, STR_HTTP_RESPONSE_OK, sizeof(STR_HTTP_RESPONSE_OK));
			else if(http_resp == HTTP_RESPONSE_ERRO)
					write(conn, STR_HTTP_RESPONSE_ERR, sizeof(STR_HTTP_RESPONSE_ERR));
			if(http_resp != HTTP_RESPONSE_NONE)
			{
					write(conn, STR_CRLF, sizeof(STR_CRLF));
					write(conn, STR_CRLF, sizeof(STR_CRLF));
					write(conn, (const unsigned char*)(send_buffer), (size_t)strlen((char*)send_buffer));
			}
      close(conn);
      //DBG("\r\nClose connect\r\n");
      ApiProcessComplete();
	}
	return;
}

/**
  * @brief  http server thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
static void http_server_socket_thread(void *arg)
{
    xQueueMessage xqueuemessage;
    volatile int sock, newconn, size;
    volatile struct sockaddr_in address, remotehost;
    uint8_t SysState;
    TickType_t xTicksToWait = 1000 / portTICK_PERIOD_MS; /* convert milliseconds to ticks */

    xqueuemessage.HttpServerMessage.TaskHandle = xTaskGetHandle(HTTPSERVER_TASKNAME);
    do
    {
        vTaskDelay(30);
        xQueuePeek( xQueueState, &SysState, 0 );
    }while(SysState != SYS_SERVER_START);

    /* create a TCP socket */
    while((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        DBG("can not create socket");
        xqueuemessage.HttpServerMessage.value = HTTPSERVER_CREATESOCKET_ERROR;
        xqueuemessage.HttpServerMessage.Message_type = TASK_ERROR;
        xQueueSend(xQueuemessage,&xqueuemessage,0);
        vTaskDelay(300);
    }

    /* bind to port 8080 at any interface */
    address.sin_family = AF_INET;
    address.sin_port = htons(80);
    address.sin_addr.s_addr = INADDR_ANY;

    while(bind(sock, (struct sockaddr *)&address, sizeof (address)) < 0)
    {
        DBG("can not bind socket");
        xqueuemessage.HttpServerMessage.value = HTTPSERVER_BIND_ERROR;
        xqueuemessage.HttpServerMessage.Message_type = TASK_ERROR;
        xQueueSend(xQueuemessage,&xqueuemessage,0);
        vTaskDelay(300);
    }

    /* listen for incoming connections (TCP listen backlog = 5) */
    listen(sock, 4);

    size = sizeof(remotehost);
    
    xqueuemessage.HttpServerMessage.value = HTTPSERVER_OK;
    xqueuemessage.HttpServerMessage.Message_type = TASK_EVENT;
    xQueueSend(xQueuemessage,&xqueuemessage,0);
    while (1) 
    {
				vTaskDelay(10);
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &xTicksToWait, sizeof(xTicksToWait));
        newconn = accept(sock, (struct sockaddr *)&remotehost, (socklen_t *)&size);
        http_server_serve(newconn, (struct sockaddr_in *)&remotehost);
    }
}
/**
  * @brief  Initialize the HTTP server (start its thread) 
  * @param  none
  * @retval None
  */
void http_server_socket_init(UBaseType_t uxTaskPriority)
{
  sys_thread_new(HTTPSERVER_TASKNAME, http_server_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, uxTaskPriority);
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
