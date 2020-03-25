/**
  ******************************************************************************
  * @file    key_scan.c
  * @author  Tuan Nguyen
  * @version V1.0.0
  * @date    04/26/2019
  * @brief   key scan hardware configuration
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
  */

/* Includes ------------------------------------------------------------------*/
#include "key_scan.h"
#include "stm32f4xx.h"
#include "stm32f4xx_adc.h"
/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_ADC_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIOC clock */
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

/**
  * @brief  Configures the ADC.
  * @param  None
  * @retval None
  */
void ADC_Configuration(void)
{
  ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	
	/* Enable ADC3 clocks ****************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	GPIO_ADC_Configuration();
	

	/* ADC1 Configuration ------------------------------------------------------*/
  ADC_InitStructure.ADC_Resolution = ADC_Mode_Independent;
	
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
  ADC_InitStructure.ADC_NbrOfConversion = 1;
	
  ADC_Init(ADC1, &ADC_InitStructure);
	
	 /* ADC1 regular channel0 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 1, ADC_SampleTime_15Cycles);
	
	/* Enable ADC1 */
  ADC_Cmd(ADC1, ENABLE);
}

uint16_t ADC_read(uint8_t ADC_Channel)
{
  /* ADC1 regular channel0 configuration */
  ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_15Cycles);
	ADC_SoftwareStartConv(ADC1);
	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);
	return ADC_GetConversionValue(ADC1);
}

uint8_t ADCChannelGet(uint8_t channel)
{
  switch(channel)
  {
    case 0:
      return ADC_Channel_12;
    case 1: 
      return ADC_Channel_13;
    default:
      return ADC_Channel_12;
  }
}


void Key_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	/* Enable GPIOC clock */
	RCC_APB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_Get(uint8_t key)
{
  return GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5);
}
