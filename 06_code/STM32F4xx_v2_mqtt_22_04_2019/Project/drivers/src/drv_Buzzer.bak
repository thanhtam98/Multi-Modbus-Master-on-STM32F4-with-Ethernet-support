/**
  ******************************************************************************
  * @file    stm3210b_eval.c
  * @author  MCD Application Team
  * @version V4.3.0
  * @date    10/15/2010
  * @brief   This file provides set of firmware functions to manage Leds          
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

#include "stm32f4xx.h"
#include "drv_BUZZER.h"

/** @addtogroup Drivers
 * 	@{
 */


/**
  * @brief  Configures BUZZER.
  * @param  None
  * @retval None
  */
void STM_EVAL_BUZZER_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	uint8_t GPIO_PinSource;
	uint16_t pin_temp;
  /* Enable the GPIO_LED Clock */
  RCC_AHB1PeriphClockCmd(BUZZER_GPIO_CLK, ENABLE);
   RCC_APB2PeriphClockCmd(TIMER_BUZZER_CLOCK , ENABLE);
  /* Configure the GPIO_LED pin */
  GPIO_InitStructure.GPIO_Pin = BUZZER_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);
	
	pin_temp = BUZZER_PIN;
	for(GPIO_PinSource=0;GPIO_PinSource<16;GPIO_PinSource++)
	{
		pin_temp >>=1;
		if(pin_temp==0)
		{
			break;
		}
	}
	GPIO_PinAFConfig(BUZZER_GPIO_PORT, GPIO_PinSource, GPIO_AF_2);
	//GPIO_ResetBits(BUZZER_GPIO_PORT,BUZZER_PIN);
	TIM_TimeBaseStructure.TIM_Period 			= TIMER_BUZZER_PERIOD;     
	TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_BUZZER_PRESCALER;       
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;      
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Down;	 
	TIM_TimeBaseInit(TIMER_BUZZER, &TIM_TimeBaseStructure);
	
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStructure.TIM_Pulse = TIMER_BUZZER_PERIOD-100;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

  TIM_OC1Init(TIMER_BUZZER, &TIM_OCInitStructure);
	TIM_CtrlPWMOutputs(TIMER_BUZZER, ENABLE);
}

/**
  * @brief  Turns selected BUZZER On.
  * @param  None
  * @retval None
  */
void STM_EVAL_BUZZEROn(void)
{
  TIM_Cmd(TIMER_BUZZER, ENABLE);
}

/**
  * @brief  Turns selected BUZZER Off.
  * @param  None
  * @retval None
  */
void STM_EVAL_BUZZEROff(void)
{
  TIM_Cmd(TIMER_BUZZER, DISABLE);  
}
