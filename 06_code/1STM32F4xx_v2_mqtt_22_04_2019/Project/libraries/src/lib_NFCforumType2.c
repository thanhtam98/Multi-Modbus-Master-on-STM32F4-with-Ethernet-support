/**
  ******************************************************************************
  * @file    lib_iso14443A.c
  * @author  MMY Application Team
  * @version V1.0
  * @date    01/07/2011
  * @brief   This file provides a set of function according to ISO/IEC14443A_3 and ISO/IEC14443A_4 specification.
  ******************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/


#include "lib_iso14443A.h"
#include "lib_NFCforumType2.h"
#include "string.h"
#include "stdio.h"
#include "hw_config.h" 
#include "miscellaneous.h"

/** @addtogroup Libraries
 * 	@{
 */

 /** @addtogroup NFCforumType2
 * 	@{
 */


/** @addtogroup NFCforumType2_Functions
*  @{
*/
/** @addtogroup NFCforumType2_ProcolSelectFunction
*  @{
*/


/**
 * @brief  this function sends a ProtocolSelect command to CR95HF device in order to select ISO 14443A protocol
 * @param  	TxDataRate (1 byte): transmission data rate (CR95HF -> Contactless tag) 
 * @param  	RxDataRate (1 byte): reception data rate (Contactless tag -> CR95HF) 
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful send to CR95HF device. 
 * @retval 	ISO14443A_ERRORCODE_PARAMETER : One parameter is erroneous 
 * @retval 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t NFCforumType2_ProtocolSelect(uc8 TxDataRate, uint8_t RxDataRate,uint8_t *pResponse)
{
	
	errchk(ISO14443A_ProtocolSelect(TxDataRate, RxDataRate,*pResponse));
		
	return NFCFORUMT2_RESULTOK;
Error:
	return NFCFORUMT2_ERRORCODE_PARAMETER;
}

/**
  * @}
  */ 

  /** @addtogroup NFCforumType2_CommandFunctions
*  @brief  this set of function send a command defined NFC forum Type2 Tag operation
*  @{
*/


/**
 * @brief  this function emits a Read command to a contactless tag
 * @param 	NthBlock : 	index of block to read
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t  NFCforumType2_Read(uc8 NthBlock,uint8_t *pResponse )
{
uint8_t DataToSend[NFCFORUMT2_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = NFCFORUMT2_CMDCODE_READ;
	DataToSend[NthByte++] = NthBlock;
	
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  this function emits a write command to a contactless tag
 * @param 	NthBlock : 	index of block to write
 * @param 	DataTowrite : 	pointer of data to write (4 bytes)
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t NFCforumType2_Write(uc8 NthBlock,uc8 *DataTowrite, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = NFCFORUMT2_CMDCODE_WRITE;
	DataToSend[NthByte++] = NthBlock;
	memcpy(&(DataToSend[NthByte]),DataTowrite,NFCFORUMT2_NBBYTE_WRITE);
   	NthByte += NFCFORUMT2_NBBYTE_WRITE;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  this function emits a SelectoSelect command to a contactless tag
 * @param 	NthSelector : 	index of sector to select
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t NFCforumType2_SelectoSelect(uc8 NthSelector, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		RFUToSend[NFCFORUMT2_NBBYTE_RFUSELECTORSELECT]={NFCFORUMT2_SELECTORSELECT_RFU,NFCFORUMT2_SELECTORSELECT_RFU,NFCFORUMT2_SELECTORSELECT_RFU},
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = NFCFORUMT2_CMDCODE_SELECTORSELECT;
	DataToSend[NthByte++] = NFCFORUMT2_SELECTORSELECT_PARAMETER;
	// send the first part of selector select command
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	NthByte = 0;
	DataToSend[NthByte++] = NthSelector;
	memcpy(&(DataToSend[NthByte]),DataTowrite,NFCFORUMT2_NBBYTE_RFUSELECTORSELECT);
   	NthByte += NFCFORUMT2_NBBYTE_RFUSELECTORSELECT;

	// send the second part of selector select command
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}


 /**
  * @}
  */

/** @addtogroup NFCforumType2_AdvancedFunctions
*  @{
*/

/**
 * @brief  	this function runs a Select sequence as defined in ISO/IEC 14443_3 document.
 * @param 	SAKByte : lasr SAK byte received from contacless tag duting Select sequence
 * @retval 	RESULTOK : the select sequence is completed 
 * @retval 	ERRORCODE_GENERIC :  an error occured during select sequence
 */
 int8_t NFCforumType2_SelectSequence ( uint8_t *SAKByte )
 {
 	errchk( ISO14443A_SelectSequence ( SAKByte ));

	return RESULTOK;

Error: 
	return ERRORCODE_GENERIC;
 }


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
 /**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
