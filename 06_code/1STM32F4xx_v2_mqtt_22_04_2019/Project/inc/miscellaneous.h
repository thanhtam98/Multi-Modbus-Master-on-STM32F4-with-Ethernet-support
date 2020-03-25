/**
  ******************************************************************************
  * @file    miscellaneous.h
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


/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f10x
  * @{
  */
    
#ifndef __CR95HF_MISC_H
#define __CR95HF_MISC_H

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup Library_configuration_section
  * @{
  */
#include "stdint.h"

#define MAX(x,y) 				((x > y)? x : y)
#define MIN(x,y) 				((x < y)? x : y)  
#define ABS(x) 					((x)>0 ? (x) : -(x))  
#define CHECKVAL(val, min,max) 	((val < min || val > max) ? FALSE : TRUE) 
 
#define RESULTOK 				0 
#define ERRORCODE_GENERIC 		1 

//#define TRUE	 				1
//#define FALSE			 		0 

#ifndef errchk
#define errchk(fCall) if (status = (fCall), status != CR95HF_SUCCESS_CODE) \
	{goto Error;} else
#endif


uint8_t HexIntToChar (uint8_t HexInt);
uint8_t DecIntToChar (uint8_t HexInt);
int8_t HexToString (uint8_t *HexString,uint8_t NbVal,char *ASCIIString);
uint8_t* itoa(uint32_t val, uint8_t base );

uint16_t Uint8ToUint16 (const uint8_t *data);
void Uint16ToUint8 (const uint16_t data,uint8_t *uint8tab);
uint32_t Uint8ToUint32 (const uint8_t *data);
void Uint32ToUint8 (const uint32_t data,uint8_t *uint8tab);

/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
