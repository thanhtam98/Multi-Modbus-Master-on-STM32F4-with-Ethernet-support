/**
  ******************************************************************************
  * @file    stm32f4x7_eth_bsp.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011 
  * @brief   Header for stm32f4x7_eth_bsp.c file.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    stm32f4x7_eth_bsp.h
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Header for stm32f4x7_eth_bsp.c file.
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB and
  *          STM32F4DIS-LCD modules. 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F4x7_ETH_BSP_H
#define __STM32F4x7_ETH_BSP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
//#include "stm32f4_discovery_lcd.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
//#define LAN8720_PHY_ADDRESS     0 /* Relative to STM324xG-EVAL Board */
//#define PHY_SR    				((uint16_t)31)         /*!< Tranceiver Status Register  for LAN8700*/
//#define PHY_INT					((uint16_t)29)
//
///* The Speed and Duplex mask values change from a PHY to another, so the user
//   have to update this value depending on the used external PHY */
////#define PHY_SPEED_STATUS            ((uint16_t)0x2000) /* Value for DP83848 PHY */
////#define PHY_DUPLEX_STATUS           ((uint16_t)0x0100) /* Value for DP83848 PHY */
//
////for LAN8700
//#define PHY_SPEED_STATUS        ((u16)0x0004)       /*!< Configured information of Speed: 10Mbps */
//#define PHY_DUPLEX_STATUS       ((u16)0x0010)       /*!< Configured information of Duplex: Full-duplex */
//
//
//
///* The Speed and Duplex mask values change from a PHY to another, so the user
//   have to update this value depending on the used external PHY */
//#define PHY_DUPLEX_SPEED_STATUS_MASK  	((uint16_t)0x001C)
//#define PHY_100BTX_FULL               	0x0018
//#define PHY_100BTX_HALF					0x0008
//#define PHY_10M_FULL					0x0014
//#define PHY_10M_HALF					0x0004

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void  ETH_BSP_Config(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F4x7_ETH_BSP_H */


/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
