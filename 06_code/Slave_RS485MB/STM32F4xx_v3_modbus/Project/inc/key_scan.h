/**
  ******************************************************************************
  * @file    key_scan.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __KEY_SCAN_H
#define __KEY_SCAN_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

void GPIO_ADC_Configuration(void);
void ADC_Configuration(void);
uint16_t ADC_read(uint8_t ADC_Channel);
uint8_t ADCChannelGet(uint8_t channel);
void Key_Configuration(void);
uint8_t Key_Get(uint8_t key);
  
#ifdef __cplusplus
}
#endif
#endif
