/**
  ******************************************************************************
  * @file    miscellaneous.c 
  * @author  MMY Application Team
  * @version V0.1
  * @date    15/03/2011
  * @brief   general function
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

#include "miscellaneous.h"


/** @addtogroup MISC
 * 	@{
 */


/** @defgroup Misc_Functions
 * @{
 */

/**  
 * @brief  returns the char correspondint to the int value
 * @retval ASCII char
 * @retval None
 */
uint8_t HexIntToChar (uint8_t HexInt)
{
	
   HexInt = HexInt & 0x0F;
   switch (HexInt)
   {
   		case 0 : 
			return '0';
		case 1 : 
			return '1'; 
		case 2 : 
			return '2';
		case 3 : 
			return '3';
		case 4 : 
			return '4'; 
		case 5 : 
			return '5';
		case 6 : 
			return '6'; 
		case 7 : 
			return '7';
		case 8 : 
			return '8'; 
		case 9 : 
			return '9';
		case 10 : 
			return 'A'; 
		case 11 : 
			return 'B';
		case 12 : 
			return 'C'; 
		case 13 : 
			return 'D'; 
		case 14 : 
			return 'E';
		case 15 : 
			return 'F'; 
		default : return 'x';
	}
}  

/**  
 * @brief  returns the char correspondint to the int value
 * @retval ASCII char
 */
uint8_t DecIntToChar (uint8_t HexInt)
{
	
   HexInt = HexInt & 0x0F;
   switch (HexInt)
   {
   		case 0 : 
			return '0';
		case 1 : 
			return '1'; 
		case 2 : 
			return '2';
		case 3 : 
			return '3';
		case 4 : 
			return '4'; 
		case 5 : 
			return '5';
		case 6 : 
			return '6'; 
		case 7 : 
			return '7';
		case 8 : 
			return '8'; 
		case 9 : 
			return '9';
		default : return 'x';
	}
}

/**  
 * @brief  returns the char correspondint to the int value
 * @param  Line : the Line to be cleared.
 *   This parameter can be @e LineX: where X can be 0..9
 * @retval None
 */
int8_t HexToString (uint8_t *HexString,uint8_t NbVal,char *ASCIIString)
{
uint8_t NthVal;
 	
	ASCIIString[0]=0x00;
	if (NbVal==0)
		return ERRORCODE_GENERIC;
		
	for (NthVal=0;NthVal<NbVal;NthVal++)
	{
		ASCIIString[2*NthVal+1] = HexIntToChar (HexString[NthVal] & 0x0F);
		ASCIIString[2*NthVal] = HexIntToChar ((HexString[NthVal]& 0xF0)>>4 );
	}	

	return RESULTOK;

}

/**  
 * @brief  
 * @param  
 * @retval None
 */
uint8_t* itoa(uint32_t val, uint8_t base )
{            
static uint8_t buf[32] = {0};  // 32 bits
int i = 30;
	if(val == 0)
    buf[i--] = '0';
	for(; val && i ; --i, val /= base)
	 	buf[i] = "0123456789abcdef"[val % base];

	return &buf[i+1];
}

/**  
 * @brief  this function return a 2 byte word from a array of 1 byte
 * @param  data : 1 byte word
 * @retval value on 2 bytes
 */
uint16_t Uint8ToUint16 (const uint8_t *data)
{
 	
   	return ( (data [1]<<8) | data [0] );

}

/**  
 * @brief  this function return a 2 byte word from a array of 1 byte
 * @param  data : 1 byte word
 * @retval value on 2 bytes
 */
void Uint16ToUint8 (const uint16_t data,uint8_t *uint8tab)
{
 	
   	uint8tab [0] = data & 0xFF;
	uint8tab [1] = (data>>8) & 0xFF;

}

/**  
 * @brief  this function return a 4 byte word from a array of 1 byte
 * @param  data : 1 byte word
 * @retval value on 4 bytes
 */
uint32_t Uint8ToUint32 (const uint8_t *data)
{
 	
   	return ( (data [3]<<24) | (data [2]<<16) | (data [1]<<8) | data [0] );

}

/**  
 * @brief  this function return a 2 byte word from a array of 1 byte
 * @param  data : 1 byte word
 * @retval value on 2 bytes
 */
void Uint32ToUint8 (const uint32_t data,uint8_t *uint8tab)
{
 	
   	uint8tab [0] = data & 0xFF;
	uint8tab [1] = (data>>8) & 0xFF;
	uint8tab [2] = (data>>16) & 0xFF;
	uint8tab [3] = (data>>24) & 0xFF;

}

/**
 * @}
 */
/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

