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

/* ----------------------- Platform includes --------------------------------*/
#include <stdlib.h>
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ----------------------- libopencm3 STM32F includes -------------------------------*/
#include <misc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_usart.h>
#include "stm32f4xx_uartstdio.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "rs485.h"
#include "rs485port.h"

/* ----------------------- Defines ------------------------------------------*/
/* serial transmit event */
#define EVENT_SERIAL_TRANS_START    (1<<0)
#define SERIAL_THREAD_PRIO          (configMAX_PRIORITIES - 2)
typedef enum
{
  RS485_IN =0,
  RS485_OUT,
}ACTION;

/* serial event */
typedef struct
{
	UCHAR Port;
	UCHAR EventSerial;
}EventSerialType;
/* ----------------------- static functions ---------------------------------*/
void RS485_RDE_GPIO_Configuration(UCHAR ucPort);
void RS485_RDE_DIR(UCHAR ucPort, ACTION action);
USART_TypeDef * UartPortGet(UCHAR ucPORT);
IRQn_Type IRQPortGet(UCHAR ucPORT);
static void prvvPORTTxReadyISR(UCHAR ucPORT);
static void prvvPORTRxISR(UCHAR Port);
static void serial_soft_trans_irq(void* parameter);

static xQueueHandle event_serial = 0;
/* ----------------------- Enable USART interrupts -----------------------------*/
void
vPortSerialEnable( UCHAR port,BOOL xRxEnable, BOOL xTxEnable )
{
    /* If xRXEnable enable serial receive interrupts. If xTxENable enable
     * transmitter empty interrupts.
     */
	EventSerialType eEvent;
	eEvent.Port = port;
	eEvent.EventSerial = EVENT_SERIAL_TRANS_START;
	if( xRxEnable )
	{
		USART_ITConfig(UartPortGet(port),USART_IT_RXNE,ENABLE);
		// Disable interrupt USART TXE if you used USART TX interrupt.
		//USART_ITConfig(ucPORT,USART_IT_TXE,DISABLE);
   		 RS485_RDE_DIR(port,RS485_IN);
			xQueueReceive(event_serial,&eEvent,0);
	}
	else
	{
		// Disable interrupt USART RXNE
		USART_ITConfig(UartPortGet(port),USART_IT_RXNE,DISABLE);
		// Enable interrupt USART TXE if you used USART TX interrupt.
		//USART_ITConfig(ucPORT,USART_IT_TXE,ENABLE);
		// Else, you are used Poll to send
    	RS485_RDE_DIR(port,RS485_OUT);
		xQueueSend( event_serial, &eEvent, 0 );
	}
}

/* ----------------------- Initialize USART ----------------------------------*/
/* Called with databits = 8 for RTU */

BOOL
xPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eParity eParity )
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* UARTx clock enable */
  UART_RCC_Configuration(UartPortGet(ucPORT));
	
	/* Configure GPIO as alternate function push-pull */
  UART_GPIO_Configuration(UartPortGet(ucPORT));
	
  /* Configure Pin RDE*/
  RS485_RDE_GPIO_Configuration(ucPORT);
  
	/* Enable the USART1 interrupt. */
	NVIC_InitStructure.NVIC_IRQChannel = IRQPortGet(ucPORT);
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = ucPORT;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	/* Setup UART parameters. */
	USART_InitStructure.USART_BaudRate = ulBaudRate;
	switch ( eParity )
	{
	case PORT_PAR_NONE:
			USART_InitStructure.USART_Parity = USART_Parity_No;
			break;
	case PORT_PAR_ODD:
			USART_InitStructure.USART_Parity = USART_Parity_Odd;
			break;
	case PORT_PAR_EVEN:
			USART_InitStructure.USART_Parity = USART_Parity_Even;
			break;
	default:
			return FALSE;
	}
	switch ( ucDataBits )
  {
    case 8:
			if (eParity == PORT_PAR_NONE)
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			else
				USART_InitStructure.USART_WordLength = USART_WordLength_9b;
			break;
    case 7:
			if (eParity == PORT_PAR_NONE)
				return FALSE;
			else
				USART_InitStructure.USART_WordLength = USART_WordLength_8b;
			break;
    default:
        return FALSE;
  }
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(UartPortGet(ucPORT), &USART_InitStructure);
	/* Finally enable the USART. */
	USART_ITConfig(UartPortGet(ucPORT), USART_IT_RXNE,ENABLE);
	USART_Cmd(UartPortGet(ucPORT), ENABLE);
	if(event_serial==0)
	{
		event_serial = xQueueCreate( RS485_PORT_NUMBER, sizeof(EventSerialType) );
		xTaskCreate(serial_soft_trans_irq,	/* The function that implements the task. */
		"serial poll",	/* Just a text name for the task to aid debugging. */
		configMINIMAL_STACK_SIZE,	/* The stack size is defined in FreeRTOSIPConfig.h. */
		NULL,		/* The task parameter, not used in this case. */
		SERIAL_THREAD_PRIO,		/* The priority assigned to the task is defined in FreeRTOSConfig.h. */
		NULL);				/* The task handle is not used. */
	}
  return TRUE;
}

/* -----------------------Send character  ----------------------------------*/
BOOL
xPortSerialPutByte( UCHAR ucPORT,CHAR ucByte )
{
  EventSerialType eEvent;
	/* Put a byte in the UARTs transmit buffer. This function is called
	 * by the protocol stack if pxFrameCBTransmitterEmpty( ) has been
	 * called. */
	USART_SendData(UartPortGet(ucPORT), ucByte);
  while(!USART_GetFlagStatus(UartPortGet(ucPORT),USART_FLAG_TC)){};
  
	eEvent.Port = ucPORT;
	eEvent.EventSerial = EVENT_SERIAL_TRANS_START;
  xQueueSend( event_serial, &eEvent, 0 );
  return TRUE;
}

/* ----------------------- Get character ----------------------------------*/
BOOL
xPortSerialGetByte( UCHAR ucPORT, CHAR * pucByte )
{
	/* Return the byte in the UARTs receive buffer. This function is called
	 * by the protocol stack after pxMBFrameCBByteReceived( ) has been called.
	 */
	*pucByte = (CHAR) USART_ReceiveData(UartPortGet(ucPORT));
   return TRUE;
}

/* ----------------------- Close Serial Port ----------------------------------*/
void
vPortSerialClose( UCHAR ucPORT )
{
  NVIC_InitTypeDef   NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = IRQPortGet(ucPORT);
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_Cmd(UartPortGet(ucPORT), DISABLE);
}

/* 
 * Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xPortSerialPutByte( ) to send the character.
 */
static void prvvPORTTxReadyISR(UCHAR ucPORT)
{
    pxFrameCBTransmitterEmpty(ucPORT);
}


/* 
 * Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxFrameCBByteReceived( ). The
 * protocol stack will then call xPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvPORTRxISR(UCHAR Port)
{
    pxFrameCBByteReceived(Port);
}

/**
 * Software simulation serial transmit IRQ handler.
 *
 * @param parameter parameter
 */
static void serial_soft_trans_irq(void* parameter) {
	EventSerialType eEvent;
	while (1)
	{
			/* waiting for serial transmit start */
			xQueueReceive(event_serial,&eEvent,-1);
			/* execute RS485bus callback */
			prvvPORTTxReadyISR(eEvent.Port);
	}
}

/* ----------------------- USART ISR ----------------------------------*/
/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
/* Find out what interrupted and get or send data as appropriate */
void USART2_IRQHandler(void)
{
	/* Check if we were called because of RXNE. */
	if (USART_GetITStatus(USART2,USART_IT_RXNE))
	{
	    prvvPORTRxISR(PORT4);
	}
}

void USART3_IRQHandler(void)
{
	/* Check if we were called because of RXNE. */
	if (USART_GetITStatus(USART3,USART_IT_RXNE))
	{
	    prvvPORTRxISR(PORT1);
	}
}

void UART4_IRQHandler(void)
{
	/* Check if we were called because of RXNE. */
	if (USART_GetITStatus(UART4,USART_IT_RXNE))
	{
	    prvvPORTRxISR(PORT3);
	}
}

void USART6_IRQHandler(void)
{
	/* Check if we were called because of RXNE. */
	if (USART_GetITStatus(USART6,USART_IT_RXNE))
	{
	    prvvPORTRxISR(PORT2);
	}
}

USART_TypeDef * UartPortGet(UCHAR ucPORT)
{
	switch(ucPORT)
	{
		case PORT1:
			return USART3;
		case PORT2:
			return USART6;
		case PORT3:
			return UART4;
		case PORT4:
			return USART2;
		default:
			break;		
	}
}

IRQn_Type IRQPortGet(UCHAR ucPORT)
{
	switch(ucPORT)
	{
		case PORT1:
			return USART3_IRQn;
		case PORT2:
			return USART6_IRQn;
		case PORT3:
			return UART4_IRQn;
		case PORT4:
			return USART2_IRQn;
		default:
			break;		
	}
}

void RS485_RDE_GPIO_Configuration(UCHAR ucPort)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  
  switch(ucPort)
  {
    case PORT1:
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
      /* Enable the GPIO_LED Clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
      /* Configure the GPIO_LED pin */
      GPIO_Init(GPIOD, &GPIO_InitStructure);
      GPIO_ResetBits(GPIOD,GPIO_Pin_10);
      break;
    case PORT2:
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
      /* Enable the GPIO_LED Clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
      /* Configure the GPIO_LED pin */
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      GPIO_ResetBits(GPIOC,GPIO_Pin_8);
      break;
    case PORT3:
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
      /* Enable the GPIO_LED Clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
      /* Configure the GPIO_LED pin */
      GPIO_Init(GPIOC, &GPIO_InitStructure);
      GPIO_ResetBits(GPIOC,GPIO_Pin_12);
      break;
    case PORT4:
      GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
      /* Enable the GPIO_LED Clock */
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
      /* Configure the GPIO_LED pin */
      GPIO_Init(GPIOD, &GPIO_InitStructure);
      GPIO_ResetBits(GPIOD,GPIO_Pin_4);
      break;
    default:
      break;
  }
}

void RS485_RDE_DIR(UCHAR ucPort, ACTION action)
{
  if(action == RS485_IN)
  {
    switch(ucPort)
    {
      case PORT1:
        GPIO_ResetBits(GPIOD,GPIO_Pin_10);
        break;
      case PORT2:
        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
        break;
      case PORT3:
        GPIO_ResetBits(GPIOC,GPIO_Pin_12);
        break;
      case PORT4:
        GPIO_ResetBits(GPIOD,GPIO_Pin_4);
        break;
      default:
        break;
    }
  }
  else
  {
    switch(ucPort)
    {
      case PORT1:
        GPIO_SetBits(GPIOD,GPIO_Pin_10);
        break;
      case PORT2:
        GPIO_SetBits(GPIOC,GPIO_Pin_8);
        break;
      case PORT3:
        GPIO_SetBits(GPIOC,GPIO_Pin_12);
        break;
      case PORT4:
        GPIO_SetBits(GPIOD,GPIO_Pin_4);
        break;
      default:
        break;
    }
  }
}

