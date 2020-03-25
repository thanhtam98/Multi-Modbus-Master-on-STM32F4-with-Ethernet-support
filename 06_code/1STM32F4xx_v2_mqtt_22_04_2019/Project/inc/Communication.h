/**
  ******************************************************************************
  * @file    Communication.h
  * @author  Tuan Nguyen
  * @version V1.1.0
  * @date    24-April-2019
  * @brief   
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************  
  */ 

#ifndef __COMMUNICATION_H
#define __COMMUNICATION_H
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_usart.h"
#include "port.h"
/* Exported define -----------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	FRAME_INVALID = 0x00,
	FRAME_REQUEST = 0x01,
	FRAME_RESPONSE = 0x02,
	FRAME_PERIODIC_PING = 0x03
}FRAME_TYPE;

typedef enum
{
	COMMAND_CHECK = 0x01,
	COMMAND_PERIODIC_PING = 0x02,
	COMMAND_TAMPER_DETECT = 0x03,
	COMMAND_NFC_DETECT = 0x04,
	COMMAND_UNKNOWN_ERROR = 0x05,
}COMMAND_CODE;


typedef struct FRAME_FORMAT_
{
	unsigned char Header;
	unsigned short Length;
	unsigned char ID;
	unsigned char Frame_Type;
	unsigned char CMD;
	unsigned char *Payload;
	unsigned short Checksum;
}FRAME_FORMAT;
/* Exported constants --------------------------------------------------------*/
#define PORT_TX_MAX_PACKET_SIZE 256
#define PORT_RX_MAX_PACKET_SIZE 256
/* Exported macro ------------------------------------------------------------*/
#define send_data(port,val)	USART_SendData(port,val); while(USART_GetFlagStatus(port, USART_FLAG_TXE) == RESET);

/* Exported functions ------------------------------------------------------- */
char Send_frame(PORT_TypeDef Port,FRAME_FORMAT data);
unsigned short Checksum_Caculate(FRAME_FORMAT data);
#endif
