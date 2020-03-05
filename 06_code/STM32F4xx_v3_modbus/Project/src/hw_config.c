/**
  ******************************************************************************
  * @file    hw_config.c 
  * @author  MMY Application Team
  * @version V0.1
  * @date    15/03/2011
  * @brief   Hardware Configuration & Setup
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
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
*/ 
 
#include "hw_config.h"
#include "string.h"
#include "stdio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_flash.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_tim.h"

/** @addtogroup User
 * 	@{
 */

/** @addtogroup HardwareConfig
  * @{
  */



/** @defgroup HwConfig_Private_Variables
 * @{
 */
ErrorStatus HSEStartUpStatus;

/** @defgroup HwConfig_Extern_Variables
 * @{
 */

//static __IO uint16_t					counter_delay_ms;



/**
 *	@brief  Configures the Extern Interrupt for the IRQ coming from the CR95HF
 *  @param  None.
 *  @return None.
 */
void EXTI_Config( void )
{
	EXTI_DeInit( );
	EXTI_RCC_Config( );
	// GPIOC pin8 (as floating input)
	EXTI_GPIO_Config( );
	// the pin 14 of CR95HF can be configurate as interupt output
	EXTI_Structure_Config( ); 
}

/**
 *	@brief  RCC configuration for the EXTI
 *  @param  None.
 *  @return None.
 */
void EXTI_RCC_Config( void )
{
	/* Enable the Clock */
	RCC_AHB1PeriphClockCmd(	EXTI_CR95HF_GPIO_CLOCK ,ENABLE);
}

/**
 *	@brief  GPIO configuration for the EXTI
 *  @param  None.
 *  @return None.
 */
void EXTI_GPIO_Config( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	/* Configure IRQ pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin  = EXTI_CR95HF_PIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* Connect IRQ EXTI Line to GPIO Pin */
	SYSCFG_EXTILineConfig(EXTI_CR95HF_GPIO_PORT_SOURCE, EXTI_CR95HF_PIN_SOURCE);
	EXTI_ClearITPendingBit(EXTI_CR95HF_LINE);
}

/**
 *	@brief  Structure configuration for the EXTI
 *  @param  None.
 *  @return None.
 */
void EXTI_Structure_Config( void )
{
EXTI_InitTypeDef EXTI_InitStructure;

	/* Configure CR95HF IRQ EXTI line */
	EXTI_InitStructure.EXTI_Line 		= EXTI_CR95HF_LINE;
	EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger 	= EXTI_Trigger_Falling;    
	EXTI_InitStructure.EXTI_LineCmd 	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}


/**
 *	@brief  Configures the Timer 2 (for standalone demo)
 *  @param  None.
 *  @return None.
 */
void Timer_Config( void )
{
	Timer_RCC_Config( );
	Timer_Structure_Config( );
}

/**
 *	@brief  RCC configuration for the Timer2
 *  @param  None.
 *  @return None.
 */
void Timer_RCC_Config( void )
{
	//enable TIM2 & TIM3 & TIM4
	RCC_APB1PeriphClockCmd(TIMER_TIMEOUT_CLOCK , ENABLE);
	RCC_APB1PeriphClockCmd(TIMER_DELAY_CLOCK , ENABLE);
}

/**
 *	@brief  Structure configuration for the Timer2
 *  @param  None.
 *  @return None.
 */
void Timer_Structure_Config( void )
{
 TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;


	/* -------------------------------------------------------------------------- 
	 * TimeOut TIMER configuration
	 * -------------------------------------------------------------------------- 
	 * 72 MHz / 7200 = 10KHz (100탎)
	 * 100탎 * 300 + 100탎 ~= 30ms	
	 * -------------------------------------------------------------------------- */
	TIM_TimeBaseStructure.TIM_Period 			= TIMER_TIMEOUT_PERIOD;     
	TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_TIMEOUT_PRESCALER;       
	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;      
	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Down;	  
	// Update the timeout timer (TIM3) 
	TIM_TimeBaseInit(TIMER_TIMEOUT, &TIM_TimeBaseStructure);
	
	TIM_UpdateRequestConfig(TIMER_TIMEOUT, TIM_UpdateSource_Global);
	
	TIM_ClearITPendingBit(TIMER_TIMEOUT, TIM_IT_Update);
		
	/* Enable TIMER Update interrupt */
	TIM_ITConfig(TIMER_TIMEOUT, TIM_IT_Update, ENABLE);
	
	
	/* -------------------------------------------------------------------------- 
	 * Delay TIMER configuration
	 * --------------------------------------------------------------------------
	 * 72 MHz / 72 = 1MHz (1탎)
	 * 1탎 * 1000 + 1탎 ~= 1ms	
	 * -------------------------------------------------------------------------- */ 
//	TIM_TimeBaseStructure.TIM_Period 			= TIMER_DELAY_PERIOD;      
//	TIM_TimeBaseStructure.TIM_Prescaler 		= TIMER_DELAY_PRESCALER;       
//	TIM_TimeBaseStructure.TIM_ClockDivision 	= TIM_CKD_DIV1;    
//	TIM_TimeBaseStructure.TIM_CounterMode 		= TIM_CounterMode_Up;
//	TIM_TimeBaseInit(TIMER_DELAY, &TIM_TimeBaseStructure);
//	
//	TIM_UpdateRequestConfig(TIMER_DELAY, TIM_UpdateSource_Global);
//	
//	TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);
		
	/* Enable TIMER Update interrupt */
//	TIM_ITConfig(TIMER_DELAY, TIM_IT_Update, ENABLE);


	// Disable timers
	TIM_Cmd(TIMER_TIMEOUT, DISABLE);
	TIM_Cmd(TIMER_DELAY, DISABLE);
	TIM_Cmd(TIMER_STANDALONE, DISABLE);
}


/**
 *	@brief  Interrupts configuration
|---------------|-----------------------|-------------------|-------------------|
|	 name		|	preemption priority	|	sub proiority	|	channel			|
|---------------|-----------------------|-------------------|-------------------|
| CR95HF		|		0				|		0			|	EXT9_5_IRQn		|
|---------------|-----------------------|-------------------|-------------------|
| timeout		|		2				|		0			|	TIM3_IRQn		|
|---------------|-----------------------|-------------------|-------------------|
| delay			|		2				|		2			|	TIM2_IRQn		|
|---------------|-----------------------|-------------------|-------------------|
 *  @param  None.
 *  @return None.
 */
void Interrupts_Config( void )
{
NVIC_InitTypeDef NVIC_InitStructure;

//NVIC_PriorityGroupConfig(NVIC_PRIORITY_GROUP);
	// NVIC : Nested vectored interrupt controller
	// EXTI : external interupt/event controller

	/* Enable and set TIMER IRQ used for timeout */
	NVIC_InitStructure.NVIC_IRQChannel 							= TIMER_TIMEOUT_IRQ_CHANNEL;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_TIMEOUT_PREEMPTION_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 		= TIMER_TIMEOUT_SUB_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelCmd 						= ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	/* Enable and set TIMER IRQ used for delays */ 
//	NVIC_InitStructure.NVIC_IRQChannel 					 		= TIMER_DELAY_IRQ_CHANNEL;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 		= TIMER_DELAY_SUB_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelCmd 						= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);						 	
}


/**
 *	@brief  Time delay in millisecond
 *  @param  delay : delay in ms.
 *  @return None.
 */
//void delay_ms(uint16_t delay)
//{
//	counter_delay_ms = delay;

//	TIM_SetCounter(TIMER_DELAY, 0);
//	/* TIM4 enable counter */
//  TIM_Cmd(TIMER_DELAY, ENABLE);
//	/* Wait for 'delay' milliseconds */
//	while(counter_delay_ms != 0);
//	/* TIM4 disable counter */
//	TIM_Cmd(TIMER_DELAY, DISABLE);
//}

/**
 *	@brief 	: Time delay in millisecond. The default prirrity are changed for this function.
 	@brief	: this function is used into void CR95HF_Send_SPI_ResetSequence(void).
 *  @param  delay : delay in ms.
 *  @return None.
 */
//void delayHighPriority_ms(uint16_t delay)
//{
//NVIC_InitTypeDef NVIC_InitStructure;

//	counter_delay_ms = delay;

//	/* Enable and set TIMER IRQ used for delays. High priority*/ 
//	NVIC_InitStructure.NVIC_IRQChannel 					 		= TIMER_DELAY_IRQ_CHANNEL;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= 6;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 		= 0;
//	NVIC_InitStructure.NVIC_IRQChannelCmd 						= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	

//	TIM_ClearITPendingBit(TIMER_DELAY, TIM_IT_Update);
//	TIM_SetCounter(TIMER_DELAY, 0);
//	/* TIM4 enable counter */
//  	TIM_Cmd(TIMER_DELAY, ENABLE);
//	/* Wait for 'delay' milliseconds */
//	while(counter_delay_ms != 0);
//	/* TIM4 disable counter */
//	TIM_Cmd(TIMER_DELAY, DISABLE);

//	/* Enable and set TIMER IRQ used for delays. Default priority */ 
//	NVIC_InitStructure.NVIC_IRQChannel 					 		= TIMER_DELAY_IRQ_CHANNEL;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 		= TIMER_DELAY_PREEMPTION_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 		= TIMER_DELAY_SUB_PRIORITY;
//	NVIC_InitStructure.NVIC_IRQChannelCmd 						= ENABLE;
//	NVIC_Init(&NVIC_InitStructure);	


//}


/**
 *	@brief  decrement the counter every millisecond used by the function delay_ms
 *  @param  delay : delay in ms.
 *  @return None.
 */
//void decrement_delay(void)
//{
//	if(counter_delay_ms != 0)
//	{
//		/* Decrement the counter */ 
//		counter_delay_ms--;
//	}
//}

/**
 *	@brief  Configures the CR95HF interface pin
 *  @param  None.
 *  @return None.
 */
void Interface_Pin_Config( void )
{
GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(INTERFACE_GPIO_CLK, ENABLE);
	
	/* Configure SELECT_COM pin as input floating */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = INTERFACE_PIN;
	// GPIO C pin 7
	GPIO_Init(INTERFACE_GPIO_PORT, &GPIO_InitStructure);
}

/**
 *	@brief  Starts the time out used to avoid the STM32 freeze
 *  @param  delay : delay in tenth of milliseconds (100탎).
 *  @return None.
 */
void StartTimeOut( uint16_t delay )
{
	/* Set the TimeOut flag to false */
	/* Clear the IT flag */
	TIM_ClearITPendingBit(TIMER_TIMEOUT, TIM_IT_Update);
	/* Set the timer counter */
	TIM_SetCounter(TIMER_TIMEOUT, delay);
  /* Enable the Time out timer */
	TIM_Cmd(TIMER_TIMEOUT, ENABLE);
}

/**
 *	@brief  Stop the timer used for the time out
 *  @param  None.
 *  @return None.
 */
void StopTimeOut( void )
{	
  /* Disable the Time out timer */
	TIM_Cmd(TIMER_TIMEOUT, DISABLE);	
}


/**
 *	@brief  Stop the timer used for the time out
 *  @param  None.
 *  @return None.
 */
void stm32_systick_disable(void)
{
    SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}

/**
 * @}
 */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
