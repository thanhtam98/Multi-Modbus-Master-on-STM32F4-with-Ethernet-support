/*
 * RS485bus Libary: RTOS Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
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
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ----------------------- RS485bus includes ----------------------------------*/
#include "rs485.h"
#include "rs485_m.h"
#include "rs485port.h"
#include "rs485config.h"
/* ----------------------- libopencm3 STM32F includes -------------------------------*/
#include <misc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>

#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
static USHORT PortTimeOut;
/* Time Expired event start*/
typedef enum
{
	EVENT_TIMENONE = 0,
	EVENT_TIMEEXPIRED_START  = (1<<0),
}EVENT_TIME;

static xQueueHandle TimerExpired[RS485_PORT_NUMBER];
typedef struct
{
	UCHAR Event;
	USHORT TimeOut;
}TimerExpiredType;

/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);
static void timer_timeout_ind(void);

/* ----------------------- Start implementation -----------------------------*/
BOOL xMasterPortTimersInit(UCHAR ucPort, USHORT usTimeOut50us)
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	static BOOL bConfigStatus =0;
	/* backup T35 ticks */
	PortTimeOut = usTimeOut50us;

	if(bConfigStatus==0)
	{
		/* Enable TIM2 clock. */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		
		NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4;
		TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		TIM_TimeBaseInitStructure.TIM_Period = 100;
		TIM_TimeBaseInitStructure.TIM_Prescaler = 40;
		TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0x00;
		TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
		TIM_UpdateRequestConfig(TIM2, TIM_UpdateSource_Global);
		TIM_ARRPreloadConfig(TIM2,ENABLE);
		TIM_SetCounter(TIM2,5);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);	

		bConfigStatus =1;
	}
	if(TimerExpired[ucPort] ==0)
	{
			TimerExpired[ucPort] = xQueueCreate( 1, sizeof(TimerExpiredType) );
	}
	if (ucPort == 3)
	{
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2,ENABLE);
	}
	return TRUE;
}

void vMasterPortTimersT35Enable(UCHAR ucPort)
{
		TimerExpiredType eEvent;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	eEvent.Event = EVENT_TIMEEXPIRED_START;
	eEvent.TimeOut = 100;
	
	xQueueOverwriteFromISR( TimerExpired[ucPort], &eEvent, &xHigherPriorityTaskWoken);
	//xQueueSend( TimerExpired[ucPort], &eEvent, -1);
	/* Set current timer mode, don't change it.*/
	vMasterSetCurTimerMode(ucPort,RS485_TMODE_T35);
//	TIM_SetCounter(TIM2,0);
//	TIM_Cmd(TIM2,ENABLE);
}

void vMasterPortTimersConvertDelayEnable(UCHAR ucPort)
{
	TimerExpiredType eEvent;
	eEvent.Event = EVENT_TIMEEXPIRED_START;
	eEvent.TimeOut = RS485_MASTER_DELAY_MS_CONVERT*1000/(50*PortTimeOut);
	xQueueSend( TimerExpired[ucPort], &eEvent, -1);
	/* Set current timer mode, don't change it.*/
	vMasterSetCurTimerMode(ucPort,RS485_TMODE_CONVERT_DELAY);

//	TIM_SetCounter(TIM2,0);
//	TIM_ITConfig(TIM2, TIM_DIER_UIE,ENABLE);
//	TIM_Cmd(TIM2,ENABLE); 
}

void vMasterPortTimersRespondTimeoutEnable(UCHAR ucPort)
{
	TimerExpiredType eEvent;
	eEvent.Event = EVENT_TIMEEXPIRED_START;
	eEvent.TimeOut = RS485_MASTER_TIMEOUT_MS_RESPOND * 1000 /(50*PortTimeOut);
	xQueueSend( TimerExpired[ucPort], &eEvent, -1);
	/* Set current timer mode, don't change it.*/
	vMasterSetCurTimerMode(ucPort,RS485_TMODE_RESPOND_TIMEOUT);

	TIM_SetCounter(TIM2,0);

	TIM_Cmd(TIM2,ENABLE); 
	TIM_ITConfig(TIM2, TIM_DIER_UIE,ENABLE);
}

void vMasterPortTimersDisable(UCHAR ucPort)
{
//	TimerExpiredType eEvent;
//	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
//	
//	eEvent.Event = EVENT_TIMENONE;
//	xQueuePeekFromISR(TimerExpired[ucPort],&eEvent);
////	TIM_ITConfig(TIM2, TIM_DIER_UIE,DISABLE);
////	TIM_Cmd(TIM2,DISABLE);
	
	TimerExpiredType eEvent;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	eEvent.TimeOut = 0;
	eEvent.Event = EVENT_TIMENONE;
	xQueueOverwriteFromISR( TimerExpired[ucPort], &eEvent,&xHigherPriorityTaskWoken);
}

void prvvTIMERExpiredISR(void)
{
	TimerExpiredType eEvent;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	UCHAR port;
	for(port = 0; port<RS485_PORT_NUMBER;port++)
	{
		xQueuePeekFromISR(TimerExpired[port],&eEvent);
		if(eEvent.Event == EVENT_TIMEEXPIRED_START)
		{
			if((--eEvent.TimeOut) == 0)
			{
				eEvent.Event = EVENT_TIMENONE;
				(void) pxMasterPortCBTimerExpired(port);
			}
			xQueueOverwriteFromISR( TimerExpired[port], &eEvent,&xHigherPriorityTaskWoken);
		}
	}
}

void TIM2_IRQHandler(void)
{
	if (TimerExpired[3] != 0)
	{
	prvvTIMERExpiredISR();
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	}
}
#endif
