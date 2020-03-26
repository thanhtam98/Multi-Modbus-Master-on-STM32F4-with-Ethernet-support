/*
 * RS485bus Libary: BARE Port
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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
/* ----------------------- RS485bus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- libopencm3 STM32F includes -------------------------------*/
#include <misc.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_tim.h>
/* Time Expired event start*/
typedef enum
{
	EVENT_TIMENONE = 0,
	EVENT_TIMEEXPIRED_START  = (1<<0),
}EVENT_TIME;

static xQueueHandle TimerExpired;
typedef struct
{
	UCHAR Event;
	USHORT TimeOut;
}TimerExpiredType;
static USHORT usT35TimeOut50us;
/* ----------------------- static functions ---------------------------------*/
static void prvvTIMERExpiredISR(void);

/* ----------------------- Initialize Timer -----------------------------*/
BOOL
xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	NVIC_InitTypeDef   NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStructure;
	
  
	static BOOL bConfigStatus =0;
	usT35TimeOut50us = usTim1Timerout50us;
	
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

	}
	if(TimerExpired ==0)
	{
		TimerExpired= xQueueCreate( 4, sizeof(TimerExpiredType) );
//    eEvent.Event = EVENT_TIMENONE;
//    eEvent.TimeOut = 0;
//    xQueueSend(TimerExpired,&eEvent,-1);
	}
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2,ENABLE);
  return TRUE;
}

/* ----------------------- Enable Timer -----------------------------*/
void vMBPortTimersEnable(void)
{
  BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	TimerExpiredType eEvent;
	eEvent.Event = EVENT_TIMEEXPIRED_START;
	eEvent.TimeOut = 100;
  /* Restart the timer with the period value set in xPortTimersInit( ) */
	xQueueOverwriteFromISR( TimerExpired, &eEvent, &xHigherPriorityTaskWoken);
	
//	TIM_SetCounter(TIM2,0);
//	TIM_Cmd(TIM2,ENABLE);
}

/* ----------------------- Disable timer -----------------------------*/
void vMBPortTimersDisable(void)
{
	TimerExpiredType eEvent;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	eEvent.TimeOut = 0;
	eEvent.Event = EVENT_TIMENONE;
	xQueueOverwriteFromISR( TimerExpired, &eEvent,&xHigherPriorityTaskWoken);
}

static void prvvTIMERExpiredISR(void)
{
	TimerExpiredType eEvent;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;

		xQueuePeekFromISR(TimerExpired,&eEvent);
		if(eEvent.Event == EVENT_TIMEEXPIRED_START)
		{
			if((--eEvent.TimeOut) == 0)
			{
				eEvent.Event = EVENT_TIMENONE;
				(void) pxMBPortCBTimerExpired();
			}
			xQueueOverwriteFromISR( TimerExpired, &eEvent,&xHigherPriorityTaskWoken);
		}
	
}


/* ----------------------- Timer ISR -----------------------------*/
/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void TIM2_IRQHandler(void)
{
	prvvTIMERExpiredISR();
  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}

