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
 * File: $Id: porttimer.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- static functions ---------------------------------*/
//static void prvvTIMERExpiredISR( void );

/* -----------------------    variables     ---------------------------------*/
extern TIM_HandleTypeDef htim7;
uint16_t timeout = 0;
// uint16_t downcounter = 0;

enum
{
  EVENT_TIMENONE = 0,
  EVENT_TIMEEXPIRED_START,
} Timer_Event;

TimerExpiredType TimerExpired[MB_RS485_MAX_PORT];

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
  TIM_MasterConfigTypeDef sMasterConfig;

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 50 - 1;

  timeout = usTim1Timerout50us;

  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    return FALSE;
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    return FALSE;
  }
  HAL_TIM_Base_Start_IT(&htim7);
  return TRUE;
}

void vMBPortTimersEnable(UCHAR ucPort)
{
  /* Enable the timer with the timeout passed to xMBPortTimersInit( ) */
  TimerExpired[ucPort].TimeOut = timeout;
  TimerExpired[ucPort].Event = EVENT_TIMEEXPIRED_START;
  /* Timer will always run ports */
  //HAL_TIM_Base_Start_IT(&htim7);
}

void vMBPortTimersDisable(UCHAR ucPort)
{
  /* Disable any pending timers. */

  TimerExpired[ucPort].Event = EVENT_TIMENONE;
  /* Timer will always run ports */
 // HAL_TIM_Base_Stop_IT(&htim7);
}

/* Create an ISR which is called whenever the timer has expired. This function
* must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
* the timer has expired.

*/

void prvvTIMERExpiredISR(void)
{
  UCHAR ucPort = 0;
  for (ucPort = 0; ucPort < MB_RS485_MAX_PORT; ucPort++)
  {
    if (TimerExpired[ucPort].Event == EVENT_TIMEEXPIRED_START)
    {
      if ((--TimerExpired[ucPort].TimeOut) == 0)
        {
          TimerExpired[ucPort].Event = EVENT_TIMEEXPIRED_START;
          (void) pxMBPortCBTimerExpired(ucPort);
        }
    }
  }
}