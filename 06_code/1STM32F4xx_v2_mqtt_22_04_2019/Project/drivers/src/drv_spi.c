/**
  ******************************************************************************
  * @file    drv_spi.c 
  * @author  MMY Application Team
  * @version V0.1
  * @date    30/03/2011
  * @brief   Handle SPI communications
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

/* Includes ------------------------------------------------------------------*/
#include "drv_spi.h"
#include "hw_config.h"
#include "stm32f4xx_spi.h"

/** @addtogroup Drivers
 * 	@{
 */

/** @addtogroup SPI
 * 	@{
 */

/** @defgroup SPI_Functions
 *  @{
 */


/**
 *	@brief  Sends one byte over SPI
 *  @param  SPIx : where x can be 1, 2 or 3 to select the SPI peripheral
 *  @param  data : data to send	(8 bit)
 *  @return None 
 */
void SPI_SendByte(SPI_TypeDef* SPIx, uint8_t data) 
{	
	/* Wait for SPIx Tx buffer empty */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPIx peripheral */
	SPI_I2S_SendData(SPIx, data);
}

/**  
 *	@brief  Sends one word over SPI
 *  @param  SPIx : where x can be 1, 2 or 3 to select the SPI peripheral
 *  @param  data : data to send	(16 bit)
 *  @return None 
 */
void SPI_SendWord(SPI_TypeDef* SPIx, uint16_t data) 
{	
	/* Wait for SPIx Tx buffer empty */	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPIx peripheral */
	SPI_I2S_SendData(SPIx, data);
}

/**  
 *	@brief  Sends one byte over SPI and recovers a response
 *  @param  SPIx : where x can be 1, 2 or 3 to select the SPI peripheral
 *  @param  data : data to send
 *  @return data response from SPIx 
 */
uint8_t SPI_SendReceiveByte(SPI_TypeDef* SPIx, uint8_t data) 
{	
	/* Wait for SPI1 Tx buffer empty */
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPIx, data);	

	/* Wait for SPI1 data reception	*/
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	/* Read & return SPI1 received data	*/
	return SPI_I2S_ReceiveData(SPIx);
}


/**
 *	@brief  Send a byte array over SPI
 *  @param  SPIx	 	: where x can be 1, 2 or 3 to select the SPI peripheral
 *  @param  pCommand  	: pointer on the buffer to send
 *  @param  length	 	: length of the buffer to send
 *  @param  pResponse 	: pointer on the buffer response
 *  @return None 
 */
void SPI_SendReceiveBuffer(SPI_TypeDef* SPIx, const uint8_t *pCommand, uint8_t length, uint8_t *pResponse) 
{
	uint8_t i;

	for(i=0; i<length; i++)
	{
		pResponse[i] = SPI_SendReceiveByte(SPIx, pCommand[i]);
	}

}

/**
 * @}
 */ 

