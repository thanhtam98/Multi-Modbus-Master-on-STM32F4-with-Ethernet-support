/**
  ******************************************************************************
  * @file    ldrv_LED.h 
  * @author  MMY Application Team
  * @version V0.1
  * @date    15/03/2011
  * @brief   
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#ifdef __cplusplus
 extern "C" {
#endif

#define LEDn                             3
#define LED1_PIN                         GPIO_Pin_6
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOC  
  
#define LED2_PIN                         GPIO_Pin_7
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOC  

#define LED3_PIN                         GPIO_Pin_8  
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOC  
  

typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
} Led_TypeDef;


void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_LIB_H */


/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */   
  
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
