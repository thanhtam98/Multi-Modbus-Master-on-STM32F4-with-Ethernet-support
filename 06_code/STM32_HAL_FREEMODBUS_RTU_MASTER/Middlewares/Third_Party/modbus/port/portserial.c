/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */
/*------------------------ Built-in C libraty includes----------------*/
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>


#include "port.h"
#include "main.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static void prvvUARTTxReadyISR( void );
//static void prvvUARTRxISR( void );
UART_HandleTypeDef UARTHandleGet(UCHAR ucPORT);
void RS485SetDir(UCHAR ucPort, ACTION action);
UART_HandleTypeDef*  pUARTHandleGet(UCHAR ucPORT);
void UARTHandleSwitch(UCHAR ucPort);
/* -----------------------    variables     ---------------------------------*/
extern UART_HandleTypeDef huart3	;
extern UART_HandleTypeDef huart2 	;
extern UART_HandleTypeDef huart4  ;
extern UART_HandleTypeDef huart6  ;

UART_HandleTypeDef  			*phuart;

//UART_HandleTypeDef*  uart3 = &huart3;
//UART_HandleTypeDef*  uart2 = &huart2;
//UART_HandleTypeDef*  uart4 = &huart4;
//UART_HandleTypeDef*  uart6 = &huart6;

//UART_HandleTypeDef HUART_RS485[MB_RS485_MAX_PORT] = {&huart3,&huart2,&huart4,&huart6};
/* ----------------------- Start implementation -----------------------------*/
void
vMBPortSerialEnable( UCHAR ucPort, BOOL xRxEnable, BOOL xTxEnable )
{
  /* If xRXEnable enable serial receive interrupts. If xTxENable enable
  * transmitter empty interrupts.
  */
	UARTHandleSwitch(ucPort);
  if (xRxEnable) {
			//HAL_GPIO_WritePin(USART3_RDE_GPIO_Port, USART3_RDE_Pin, 0);
    __HAL_UART_ENABLE_IT(phuart, UART_IT_RXNE);
  } else {    
    __HAL_UART_DISABLE_IT(phuart, UART_IT_RXNE);
  }
  
  if (xTxEnable) {    
		//HAL_GPIO_WritePin(USART3_RDE_GPIO_Port, USART3_RDE_Pin, 1);
    __HAL_UART_ENABLE_IT(phuart, UART_IT_TXE);
  } else {
    __HAL_UART_DISABLE_IT(phuart, UART_IT_TXE);
  }  
}
 
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
  /* 
  Do nothing, Initialization is handled by MX_USART3_UART_Init() 
  Fixed port, baudrate, databit and parity  
  */
  return TRUE;
}
 
BOOL
xMBPortSerialPutByte(UCHAR ucPort, CHAR ucByte )
{
  /* Put a byte in the UARTs transmit buffer. This function is called
  * by the protocol stack if pxMBFrameCBTransmitterEmpty( ) has been
  * called. */
	UARTHandleSwitch(ucPort);
  return (HAL_OK == HAL_UART_Transmit(phuart , (uint8_t*)&ucByte, 1, 10));
}
 
BOOL
xMBPortSerialGetByte( UCHAR ucPort, CHAR * pucByte )
{
  /* Return the byte in the UARTs receive buffer. This function is called
  * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
  */
	UARTHandleSwitch(ucPort);
	
  *pucByte = (uint8_t)((*(phuart)).Instance->DR & (uint8_t)0x00FF);  
  return TRUE;
}
 
/* Create an interrupt handler for the transmit buffer empty interrupt
* (or an equivalent) for your target processor. This function should then
* call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
* a new character can be sent. The protocol stack will then call 
* xMBPortSerialPutByte( ) to send the character.
 
static void prvvUARTTxReadyISR( void )
{
pxMBFrameCBTransmitterEmpty(  );
}
*/
 
/* Create an interrupt handler for the receive interrupt for your target
* processor. This function should then call pxMBFrameCBByteReceived( ). The
* protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
* character.
 
static void prvvUARTRxISR( void )
{
pxMBFrameCBByteReceived(  );
}
*/





/*HW Function Porting -------------------------------------------*/


/*
Brief: Change Direction of MAX485 port
Todo: Add HAL control GPIO HW
*/
void RS485SetDir(UCHAR ucPort, ACTION action)
{

    switch(ucPort)
    {
      case PORT1:
       HAL_GPIO_WritePin(USART3_RDE_GPIO_Port, USART3_RDE_Pin, (GPIO_PinState)action);
        break;
      case PORT2:
				HAL_GPIO_WritePin(USART2_RDE_GPIO_Port, USART2_RDE_Pin, (GPIO_PinState)action);
        break;
      case PORT3:
				HAL_GPIO_WritePin(UART4_RDE_GPIO_Port, UART4_RDE_Pin, (GPIO_PinState)action);
        break;
      case PORT4:
					HAL_GPIO_WritePin(USART6_RDE_GPIO_Port, USART6_RDE_Pin, (GPIO_PinState)action);
        break;
      default:
        break;
    }
}
/*
Brief: Return uart handle 
*/
void UARTHandleSwitch(UCHAR ucPort)
{
	switch (ucPort)
	{
		   case PORT1:
				 phuart = &huart3;
			//	memcpy(phuart, &huart3, sizeof(UART_HandleTypeDef));
        break;
				case PORT2:
					
				memcpy(phuart, &huart2, sizeof(UART_HandleTypeDef));        
					break;
      case PORT3:
				memcpy(phuart, &huart4, sizeof(UART_HandleTypeDef));        
			break;
				case PORT4:
				memcpy(phuart, &huart6, sizeof(UART_HandleTypeDef));        
				break;
      default:
        break;
		}
}
//UART_HandleTypeDef UARTHandleGet(UCHAR ucPORT)
//{
//  switch (ucPORT)
//  {
//  case PORT1/* constant-expression */:
//    return huart3;
//    /* code */
//    break;
//    case PORT2/* constant-expression */:
//    return huart2;
//    /* code */
//    break;
//      case PORT3/* constant-expression */:
//    return huart4;
//    /* code */
//    break;
//      case PORT4/* constant-expression */:
//    return huart6;
//    /* code */
//    break;
//  default:
//    break;
//  }
//}
//UART_HandleTypeDef*  pUARTHandleGet(UCHAR ucPORT)
//{
//	 switch (ucPORT)
//  {
//	  case PORT1/* constant-expression */:
//    return uart3;
//    /* code */
//    break;
//    case PORT2/* constant-expression */:
//    return uart2;
//    /* code */
//    break;
//      case PORT3/* constant-expression */:
//    return uart4;
//    /* code */
//    break;
//      case PORT4/* constant-expression */:
//    return uart6;
//    /* code */
//    break;
//  default:
//    break;
//  }
//}
