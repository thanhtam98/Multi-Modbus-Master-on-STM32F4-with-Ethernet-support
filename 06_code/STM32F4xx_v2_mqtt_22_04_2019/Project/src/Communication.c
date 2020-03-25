/**
  ******************************************************************************
  * @file    Communication.c
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
	/* Includes ------------------------------------------------------------------*/
#include "Communication.h"

/**
* @brief  Send Frame function.
* @param  FRAME_FORMAT
* @retval status
*/
char Send_frame(PORT_TypeDef Port,FRAME_FORMAT data)
{
	unsigned char checksum;
	unsigned short i;
	unsigned char *point;
	checksum = Checksum_Caculate(data);
	point = data.Payload;
	send_data(Port,data.Header);
	send_data(Port,data.Length);
	send_data(Port,data.ID);
	send_data(Port,data.Frame_Type);
	send_data(Port,data.CMD);
	for(i=0;i<(data.Length-3);i++)
	{
		send_data(Port,*point++)
	}
	return 1;
}
/**
* @brief  Checksum Caculate function.
* @param  FRAME_FORMAT
* @retval checksum
*/
unsigned short Checksum_Caculate(FRAME_FORMAT data)
{
	unsigned short checksum = 0;
	unsigned char *point;
	unsigned short i;
	checksum += data.Header;
	checksum += data.Length;
	checksum += data.ID;
	checksum += data.Frame_Type;
	checksum += data.CMD;
	point = data.Payload;
	for(i=0;i<(data.Length-3);i++)
	{
		checksum += *(point++);
	}
	return checksum;
}
	