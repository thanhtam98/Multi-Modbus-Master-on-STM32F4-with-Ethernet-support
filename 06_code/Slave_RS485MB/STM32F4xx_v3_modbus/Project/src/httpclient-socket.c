/**
  ******************************************************************************
  * @file    httpclient-socket.c
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
#include "lwip/opt.h"
#include "lwip/arch.h"
#include "lwip/api.h"
#include "lwip/inet.h"
#include "lwip/sockets.h"
#include "fs.h"
#include "fsdata.h"
#include "string.h"
#include "rfid_reader.h"
#include "lib_iso14443A.h"
#include "miscellaneous.h"
#include "stm32f4_discovery.h"
#include "tcp_socket.h"
#include "api_server.h"
#include "user_debug.h"
#include "main.h"
/* Private typedef -----------------------------------------------------------*/
//static const char STR_TIMESTAMP_URL[] = "GET /timestamp ";
//static const char STR_CIK_HEADER[] = "X-Exosite-CIK: ";
static const char STR_CONTENT_LENGTH[] = "Content-Length: ";
static const char STR_DEV_ID[] = "DevID=RD01";
//static const char STR_READ_URL[] = "GET /get.php?";
static const char STR_WRITE_POST_URL[] = "POST /doorac/devpost ";
static const char STR_WRITE_UPDATE_URL[] = "POST /doorac/devupdate ";
//static const char STR_ACTIVATE_URL[] = "POST /provision/activate ";
//static const char STR_RPC_URL[] = "POST /onep:v1/rpc/process ";
static const char STR_HTTP[] = "HTTP/1.1";
static const char STR_HOST[] = "Host: ";
//static const char STR_HOST[] = "Host: 192.168.0.5";
//static const char STR_ACCEPT[] = "Accept: application/x-www-form-urlencoded; charset=utf-8";
//static const char STR_ACCEPT_JSON[] = "Accept: application/json; charset=utf-8";
static const char STR_CONTENT[] = "Content-Type: application/x-www-form-urlencoded; charset=utf-8";
//static const char STR_CONTENT_JSON[] = "Content-Type: application/json; charset=utf-8";
static const char STR_CRLF[] = "\r\n";

extern Host_t host;
extern User_t user;
extern int8_t myERROR;
uint8_t CR95HF_Handle;
uint8_t 	ReaderRecBuf[MAX_BUFFER_SIZE+3]; 	// buffer for SPI ou UART reception
/* Private define ------------------------------------------------------------*/
#define RFID_CLIENT_THREAD_PRIO    ( tskIDLE_PRIORITY + 3 )


/**
  * @brief  http client update thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
void update_client_socket_thread(void *arg)
{
	struct sockaddr_in server_address;
	int mysock, recv_data;
	unsigned char data_buffer[100];
	while(1)
	{
		if ((mysock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		{
			myERROR = ERROR_TCP;
			DBG("Can not create TCP socket");
//			return;
		}
		else
		{
			Host_read_flash();
			server_address.sin_family = AF_INET;
			server_address.sin_addr.s_addr = host.IP.idata;
			server_address.sin_port = htons(8000);
			if(connect(mysock,(struct sockaddr*)&server_address,sizeof(server_address)) < 0)
			{
				myERROR = ERROR_TCP;
				close(mysock);
				DBG("connect failed \n");
			}
			else
			{
				myERROR = ERROR_OK;
				recv_data = send(mysock,STR_WRITE_UPDATE_URL,sizeof(STR_WRITE_UPDATE_URL)-1,0);
				recv_data = send(mysock,STR_HTTP, sizeof(STR_HTTP)-1,0);
				recv_data = send(mysock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send Host header
				recv_data = send(mysock,STR_HOST, sizeof(STR_HOST)-1,0);
				recv_data = send(mysock,&host.Host_Name[0].cdata[0], strlen(&host.Host_Name[0].cdata[0]),0);
				recv_data = send(mysock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send content type header
				recv_data = send(mysock,STR_CONTENT, sizeof(STR_CONTENT)-1,0);
				recv_data = send(mysock,STR_CRLF, sizeof(STR_CRLF)-1,0);
			
				recv_data = send(mysock,STR_CONTENT_LENGTH, sizeof(STR_CONTENT_LENGTH)-1,0);
				recv_data = send(mysock,"8", sizeof("8")-1,0);
				recv_data = send(mysock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				recv_data = send(mysock,STR_CRLF, sizeof(STR_CRLF)-1,0);
			
				recv_data = send(mysock, STR_DEV_ID, sizeof(STR_DEV_ID)-1, 0);
				if(recv_data < 0)
				{
					myERROR = ERROR_TCP;
					close(mysock);
					DBG("Send failed \n");
				}
				else
				{
					myERROR = ERROR_OK;
					do
					{
						recv_data = read(mysock,data_buffer,sizeof(data_buffer));
					}
					while(recv_data > 0);
//					data_buffer[recv_data] = '\0';
					if( strstr((char *)data_buffer,"DevID.OK"))
					{
						myERROR = ERROR_OK;
//					STM_EVAL_LEDOn(LEDB2);
//					vTaskDelay(100);               
					}
					else
					{
						myERROR = ERROR_DEV_ID;
//					STM_EVAL_LEDOn(LEDR2);
//					vTaskDelay(100);
					}
					close(mysock);
					DBG("TCP send success \n");
				}
			}
//		STM_EVAL_LEDOff(LEDB2);
//		STM_EVAL_LEDOff(LEDR2);
	  }
		vTaskDelay(20000);
	}
}

/**
  * @brief  http client thread 
  * @param arg: pointer on argument(not used here) 
  * @retval None
  */
void http_client_socket_thread(void *arg)
{
	struct sockaddr_in server_address;
	int sock, recv_data;	
	unsigned char data_buffer[500];
	char *point;
	uint8_t NbUIDByte,UIDout[ISO14443A_MAX_NBBYTE_UID];
	int8_t status =0,i;
	uint16_t count=0;
	do
	{
		CR95HF_IDN(ReaderRecBuf);
		CR95HF_Handle = User_IsCR95HF();
	}while(CR95HF_Handle !=RESULTOK);
	
	while(1)
	{
		count++;
		
		Host_read_flash();
		User_read_flash();
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = host.IP.idata;
		server_address.sin_port = htons(TCP_PORT);
		
		// catch a 14 4443_A contacless tag
		memset(UIDout, 0,sizeof(UIDout));	
		if((status =User_Get14443ATag (UIDout,&NbUIDByte))==RESULTOK)
		{
			myERROR = ERROR_UID_DETECT;
//			STM_EVAL_LEDOn(BUZZER);
//			vTaskDelay(20);
//			STM_EVAL_LEDOff(BUZZER);
		}
//		else if((status = User_Get14443BTag ())==RESULTOK)
//		{
// 			
//		}
//		else if((status =User_Get14443SRTag ())==RESULTOK)
//		{
// 			
//		}
		CR95HF_FieldOff();


		if(status==RESULTOK)
		{
			 /* create a TCP socket */
			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
			{
				DBG("can not create socket");
				myERROR = ERROR_TCP;
//				return;
			}
			if(connect(sock,(struct sockaddr*)&server_address,sizeof(server_address)) < 0)
			{
				DBG("connect failed \n");
				myERROR = ERROR_TCP;
				close(sock);
			}
			else
			{
				myERROR = ERROR_OK;
				DBG("connect OK \n");
				memset(data_buffer, 0,500);	
				// send request
				recv_data = send(sock,STR_WRITE_POST_URL,sizeof(STR_WRITE_POST_URL)-1,0);
				recv_data = send(sock,STR_HTTP, sizeof(STR_HTTP)-1,0);
				recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send Host header
				recv_data = send(sock,STR_HOST, sizeof(STR_HOST)-1,0);
				recv_data = send(sock,&host.Host_Name[0].cdata[0], strlen(&host.Host_Name[0].cdata[0]),0);
				recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send content type header
				recv_data = send(sock,STR_CONTENT, sizeof(STR_CONTENT)-1,0);
				recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// add username
//				strcat((char *)data_buffer,"username=");
//				strcat((char *)data_buffer,&user.UserName[0].cdata[0]);
				// add password
//				strcat((char *)data_buffer,"&password=");
//				strcat((char *)data_buffer,&user.PassWord[0].cdata[0]);
				//strcat((char *)data_buffer,Flash_Host.PassWord.cdata);
				strcat((char *)data_buffer,STR_DEV_ID);
				// add Tag Uid
				strcat((char *)data_buffer,"&UID=");
				for(i=0;i<NbUIDByte;i++)
				{
					point = (char*) itoa(UIDout[i],16);
					if(strlen(point)==1)
					{
						strcat((char *)data_buffer,"0");
					}
					strcat((char *)data_buffer,point);
				}
				point = (char*)itoa(strlen(data_buffer),10);
				
				// send content length header
				recv_data = send(sock,STR_CONTENT_LENGTH, sizeof(STR_CONTENT_LENGTH)-1,0);
				recv_data = send(sock,point, strlen(point),0);
				recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send content data
				recv_data = send(sock,data_buffer,strlen((const char *)data_buffer),0);
//				recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				if(recv_data < 0)
				{
					DBG("Send failed \n");
					close(sock);
					myERROR = ERROR_TCP;
				}
				else
				{
					myERROR = ERROR_OK;
					DBG("Received data: ");
					do {
						recv_data = read(sock,data_buffer,sizeof(data_buffer));
					}
					while(recv_data > 0);
					data_buffer[recv_data] = '\0';
					point = strstr((char *)data_buffer,"UID.OK");
					if(point != NULL)
					{
            myERROR = ERROR_UID_OK;						
					}
					else
					{
						myERROR = ERROR_UID;
					}
					DBG("%s",data_buffer);
          close(sock);
				}
			}
		}
		
		if (count > 300)
		{
		  count = 0;
			if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	    {
		  	myERROR = ERROR_TCP;
		  	DBG("Can not create TCP socket");
			  return;
		  }		
		  if(connect(sock,(struct sockaddr*)&server_address,sizeof(server_address)) < 0)
		  {
			  myERROR = ERROR_TCP;
			  close(sock);
			  DBG("connect failed \n");
//				return;
		  }
		  else
		  {
			  myERROR = ERROR_OK;
			  recv_data = send(sock,STR_WRITE_UPDATE_URL,sizeof(STR_WRITE_UPDATE_URL)-1,0);
			  recv_data = send(sock,STR_HTTP, sizeof(STR_HTTP)-1,0);
			  recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send Host header
			  recv_data = send(sock,STR_HOST, sizeof(STR_HOST)-1,0);
			  recv_data = send(sock,&host.Host_Name[0].cdata[0], strlen(&host.Host_Name[0].cdata[0]),0);
			  recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
				
				// send content type header
			  recv_data = send(sock,STR_CONTENT, sizeof(STR_CONTENT)-1,0);
			  recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
			
			  recv_data = send(sock,STR_CONTENT_LENGTH, sizeof(STR_CONTENT_LENGTH)-1,0);
			  recv_data = send(sock,"10", sizeof("10")-1,0);
			  recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
			  recv_data = send(sock,STR_CRLF, sizeof(STR_CRLF)-1,0);
			
			  recv_data = send(sock, STR_DEV_ID, sizeof(STR_DEV_ID)-1, 0);
			  if(recv_data < 0)
			  {
				  myERROR = ERROR_TCP;
				  close(sock);
				  DBG("Send failed \n");
			  }
			  else
			  {
				  myERROR = ERROR_OK;
				  do
				  {
				    recv_data = read(sock,data_buffer,sizeof(data_buffer));
				  }
				  while(recv_data > 0);
					data_buffer[recv_data] = '\0';
				  if( strstr((char *)data_buffer,"DevID.OK"))
				  {
					  myERROR = ERROR_OK;             
				  }
				  else
				  {
					  myERROR = ERROR_DEV_ID;
				  }
				  close(sock);
				  DBG("TCP send success \n");
			  }
			}
		}
		vTaskDelay(100);
	}
}

/**
  * @brief  Initialize the HTTP Client (start its thread) 
  * @param  none
  * @retval None
  */
void http_client_socket_init()
{
//	sys_thread_new("RFID detect tcp-client Thread", rfid_client_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, RFID_CLIENT_THREAD_PRIO);
//	sys_thread_new("update http-client thread", update_client_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, RFID_CLIENT_THREAD_PRIO -1);
	sys_thread_new("http-client thread", http_client_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE * 3, RFID_CLIENT_THREAD_PRIO);
}
