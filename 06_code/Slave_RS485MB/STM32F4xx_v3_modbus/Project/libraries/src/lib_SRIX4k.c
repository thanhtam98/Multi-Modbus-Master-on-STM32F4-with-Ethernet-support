/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : lib_SRIX4k.c
* Author             : MMY Application Team
* Version            : V1.1
* Date               : 18/07/2011
* Description        : this library provides a set of SRIX4k commands
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
#include "stm32f4xx.h"
#include "lib_SRIX4k.h"

 static int16_t SRIX4K_CRC16 (const uint8_t *DataIn,const uint8_t NbByte);
static int8_t SRIX4K_IsCorrectCRC16Residue (const uint8_t *DataIn,const uint8_t Length);
static int8_t SRIX4K_ExtractChipIDResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);


/** @addtogroup Libraries
 * 	@{
 */

 /** @addtogroup SRIX4k
 * 	@{
 */


 /** @addtogroup SRIX4k_Extern_Variables
 *  @{
 */

extern uint8_t 	ReaderRecBuf[MAX_BUFFER_SIZE+3];

/**
  * @}
  */ 

/** @addtogroup SRIX4k_Functions
*  @{
*/



/** @addtogroup SRIX4k_CRCfunction
*  @{
*/

  
/**  
* @brief  	this function computes the CRC16 as defined by CRC ISO/IEC 13239
* @param  	DataIn		:	input data 
* @param	NbByte 		: 	Number of byte of DataIn
* @retval	ResCrc		: 	CRC16 computed
*/
static int16_t SRIX4K_CRC16 (const uint8_t *DataIn,const uint8_t NbByte) 
{ 
int8_t 	i,
		j; 
int32_t ResCrc = SRIX4K_PRELOADCRC16;
	
	for (i=0;i<NbByte;i++) 
	{ 
		ResCrc=ResCrc ^ DataIn[i];
		for (j=8;j>0;j--) 
		{
			ResCrc = (ResCrc & SRIX4K_MASKCRC16) ? (ResCrc>>1) ^ SRIX4K_POLYCRC16 : (ResCrc>>1); 
		}
	} 

	return ((~ResCrc) & 0xFFFF);
	
} 

/**  
* @brief  	this function returns RESULTOK if the CRC16 residue is correct as defined by CRC ISO/IEC 13239 
* @param  	DataIn		:	input to data 
* @param	Length 		: 	Number of bits of DataIn
* @retval 	RESULTOK  	:   CRC16 residue is correct	
* @retval 	ERRORCODE_GENERIC  	:  CRC16 residue is false
*/
static int8_t SRIX4K_IsCorrectCRC16Residue (const uint8_t *DataIn,const uint8_t Length)
{
int16_t ResCRC=0;

	// check the CRC16 Residue 
	if (Length !=0)
		ResCRC=SRIX4K_CRC16 (DataIn, Length);
	
	if (((~ResCRC) & 0xFFFF) != SRIX4K_RESIDUECRC16)
		return ERRORCODE_GENERIC;
	
	return RESULTOK;
}

/**
  * @}
  */ 


/**  
* @brief  	this function splits reader response and extracts ChipId field. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	ChipID : 	ChipID of contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
static int8_t SRIX4K_ExtractChipIDResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{
int8_t status;
uint8_t	ResultCode,
		NbTagReplyByte,
		NbControlByte,
		TagReplyIndex,
		ControlIndex;

	// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443B,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	
	//  CR95HF returned an error code
	if (ResultCode != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;	
	
	// check CRC16 residue of contacless tag response
	errchk(SRIX4K_IsCorrectCRC16Residue (&ReaderResponse[TagReplyIndex],NbTagReplyByte));

	// extracts data from contacless tag response 
	*ChipID = ReaderResponse[TagReplyIndex+SRIX4K_OFFSET_CHIPID];

	return RESULTOK;
Error:
	return SRIX4K_ERRORCODE_CRCRESIDUE;	
}


/** @addtogroup SRIX4K_ProcolSelectFunction
*  @{
*/

 /**
 * @brief  this function sends a ProtocolSelect command to CR95HF device in order to select ISO14 443B protocol
 * @param  	TxDataRate (1 byte): transmission data rate (CR95HF -> Contactless tag) 
 * @param  	RxDataRate (1 byte): reception data rate (Contactless tag -> CR95HF) 
 * @param 	pResponse : 	pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device. 
 * @return 	SRIX4K_ERRORCODE_PARAMETER : One parameter is erroneous 
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_ProtocolSelect(const uint8_t TxDataRate, uint8_t RxDataRate,uint8_t *pResponse)
{
uint8_t Parameters = 0x00;
int8_t 	status;

	*pResponse = SENDRECV_ERRORCODE_SOFT;
	*(pResponse+1) = 0x00;
	// checks the parameter values
	if (TxDataRate>SRIX4K_TXVALUE_847K)
		return SRIX4K_ERRORCODE_PARAMETER;
	if (RxDataRate>SRIX4K_RXVALUE_847K)
		return SRIX4K_ERRORCODE_PARAMETER;		

	// create the parameter byte of ProtocolSelect command
	Parameters = 	( (TxDataRate<<SRIX4K_OFFSET_TXDATARATE) & SRIX4K_MASK_TXDATARATE) |
					( (RxDataRate<<SRIX4K_OFFSET_RXDATARATE) & SRIX4K_MASK_RXDATARATE);

	// the CR95HF will manage CRC16 of Rf command 
	Parameters |= SRIX4K_APPENDCRC;

	// send ProtocolSelect to CR95HF to configure it
	errchk(CR95HF_ProtocolSelect(	SRIX4K_PROTOCOLSELECT_NBBYTE,
									PROTOCOL_TAG_ISO14443B,
									&Parameters,
									pResponse));

	// checks CR95HF response
	if (CR95HF_IsReaderResultCodeOk (PROTOCOL_SELECT,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;
	
	return RESULTOK;
Error:
	return SRIX4K_ERRORCODE_PARAMETER;
}

/**
  * @}
  */ 




/** @addtogroup SRIX4k_CommandFunctions
*  @brief  this set of function send a command defined in SRIX4k datasheet to a conctact less tag trough CR95HF
*  @{
*/

/**
 * @brief  	this function sends an initiate command to a contactless tag through CR95HF device
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_Initiate(uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// copy command code
	DataToSend[NthByte++] = SRIX4K_CMDCODE_INITIATE_BYTE0;
	DataToSend[NthByte++] = SRIX4K_CMDCODE_INITIATE_BYTE1;
	
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  	this function sends an Pcall16 command to a contactless tag through CR95HF device
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @return 	SRIX4K_ERRORCODE_PARAMETER : One parameter is erroneous 
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_Pcall16(  uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// copy command code
	DataToSend[NthByte++] = SRIX4K_CMDCODE_PCALL16_BYTE0;
	DataToSend[NthByte++] = SRIX4K_CMDCODE_PCALL16_BYTE1;
	
   	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}


/**
 * @brief  	this function sends an SlotMarker command to a contactless tag through CR95HF device
 * @param  	SlotNumber (1 byte) : slot number between 1 and 15 (4 LSB bits)
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_SlotMarker(const uint8_t SlotNumber, uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// copy command code and chip ID
	DataToSend[NthByte++] = ((SlotNumber <<SRIX4K_OFFSET_SLOTNUMBER) & SRIX4K_MASK_SLOTNUMBER)|
							SRIX4K_CMDCODE_SLOTMARKER ;
							

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}


/**
 * @brief  	this function sends an SelectChipID command to a contactless tag through CR95HF device
 * @param  	ChipID	:  	ChipId of a contacless tag
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_SelectChipID ( const uint8_t ChipID, uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// copy command code and chip ID
	DataToSend[NthByte++] = SRIX4K_CMDCODE_SELECTCHIPID;
	DataToSend[NthByte++] = ChipID;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  	this function sends a Completion command to a contactless tag through CR95HF device. 
 * @brief  	contaclesss tag didn't reply to this command. 
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 */
int8_t SRIX4K_Completion(  uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;

	// copy command code 
	DataToSend[NthByte++] = SRIX4K_CMDCODE_COMPLETION;

	// send command to contacless tag through CR95HF device and received its response
	CR95HF_SendRecv(NthByte,DataToSend,pResponse);

	return RESULTOK;

}

/**
 * @brief  	this function sends a ResetToInventory command to a contactless tag through CR95HF device
 * @brief  	contaclesss tag didn't reply to this command. 
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 */
int8_t SRIX4K_ResetToInventory(uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;

	// copy command code 
	DataToSend[NthByte++] = SRIX4K_CMDCODE_RESETTOINVENTORY;

	// send command to contacless tag through CR95HF device and received its response
	CR95HF_SendRecv(NthByte,DataToSend,pResponse);

	return RESULTOK;
}

/**
 * @brief  	this function sends a ReadBlock command to a contactless tag through CR95HF device
 * @param  	BlockAddress (1 byte): block address to read into contacless tag memory.
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_ReadSingleBlock ( const uint8_t BlockAddress, uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// copy command code and block address
	DataToSend[NthByte++] = SRIX4K_CMDCODE_READBLOCK;
	DataToSend[NthByte++] = BlockAddress;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  	this function sends a WriteBlock command to a contactless tag through CR95HF device
 * @brief  	contaclesss tag didn't reply to this command. 
 * @param  	BlockAddress (1 byte): block address to read into contacless tag memory.
 * @param  	DataToWrite (4 bytes): Data to write into contacless tag memory.
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.  
 */
int8_t SRIX4K_WriteSingleBlock ( const uint8_t BlockAddress,const uint8_t *DataToWrite, uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_WRITECMD_MAXNBBYTE],
		NthByte=0;

	// copy command code and block address
	DataToSend[NthByte++] = SRIX4K_CMDCODE_WRITEBLOCK;
	DataToSend[NthByte++] = BlockAddress;
	// append 4 bytes of DataToWrite to RF command 
	memcpy(&(DataToSend[NthByte]),DataToWrite,SRIX4K_SRIX4K_BLOCK_NBBYTE);
	NthByte += SRIX4K_SRIX4K_BLOCK_NBBYTE;

	// send command to contacless tag through CR95HF device and received its response
	CR95HF_SendRecv(NthByte,DataToSend,pResponse);

	return RESULTOK;
}

/**
 * @brief  	this function sends a GetUID command to a contactless tag through CR95HF device
 * @param  	pResponse : pointer on CR95HF response
 * @return 	RESULTOK : the command has been succesful emited to CR95HF device.   
 * @return 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t SRIX4K_GetUID (uint8_t *pResponse )
{
uint8_t DataToSend[SRIX4K_SRIX4K_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// copy command code and block address
	DataToSend[NthByte++] = SRIX4K_CMDCODE_GETUID;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;

Error:
	return SRIX4K_ERRORCODE_PARAMETER;
}


/**
  * @}
  */

/** @addtogroup SRIX4k_SplitFunctions
*  @brief  this set of function split CR95HF response in order to extract contacless tag field. 
*  @brief  Each command defined in SRIX4k has its oww plit function. 
*  @{
*/


/**  
* @brief  	this function splits initiate response and extracts information field. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	ChipID : 	ChipID of contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
int8_t SRIX4K_SplitInitiateResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{

	return(SRIX4K_ExtractChipIDResponse( ReaderResponse,
											Length,
											ChipID));

}

/**  
* @brief  	this function splits PCall16 response and extracts information field. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	ChipID : 	ChipID of contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
int8_t SRIX4K_SplitPCall16Response( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{
	return(SRIX4K_ExtractChipIDResponse( ReaderResponse,
											Length,
											ChipID));	
}

/**  
* @brief  	this function splits SlotMarker response and extracts information field. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	ChipID : 	ChipID of contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
int8_t SRIX4K_SplitSlotMarkerResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{
	return(SRIX4K_ExtractChipIDResponse( ReaderResponse,
											Length,
											ChipID));	
}

/**  
* @brief  	this function splits SlotMarker response and extracts information field. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	ChipID : 	ChipID of contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
int8_t SRIX4K_SplitSelectChipIDResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{
	return(SRIX4K_ExtractChipIDResponse( ReaderResponse,
											Length,
											ChipID));
}

/**  
* @brief  	this function splits Completion response. Contacless tag doesn't reply to Completion command. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param  	ChipID	:  	ChipId of a contacless tag
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
*/
int8_t SRIX4K_SplitCompletionResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{
uint8_t	ResultCode,
		NbTagReplyByte,
		NbControlByte,
		TagReplyIndex,
		ControlIndex;

	// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443B,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	
	//  CR95HF returned an error code
	if (ResultCode == CR95HF_ERROR_CODE)
		return CR95HF_ERROR_CODE;	

return RESULTOK;

}

/**  
* @brief  	this function splits ReadBlock response and returned data index field. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	DataReadIndex : index on ReaderResponse of the data read into contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
int8_t SRIX4K_SplitReadBlockResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *DataReadIndex)
{
int8_t status;
uint8_t	ResultCode,
		NbTagReplyByte,
		NbControlByte,
		TagReplyIndex,
		ControlIndex;

	// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443B,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	
	//  CR95HF returned an error code
	if (ResultCode == CR95HF_ERROR_CODE)
		return CR95HF_ERROR_CODE;	
	
	// check CRC16 residue of contacless tag response
	errchk(SRIX4K_IsCorrectCRC16Residue (&ReaderResponse[TagReplyIndex],NbTagReplyByte));

	// extracts data from contacless tag response 
	*DataReadIndex = TagReplyIndex+SRIX4K_OFFSET_READBLOCK;

	return RESULTOK;
Error:
	return SRIX4K_ERRORCODE_CRCRESIDUE;	
}

/**  
* @brief  	this function splits WriteBlock response. Contacless tag doesn't reply to WriteBlock command. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param  	ChipID	:  	ChipId of a contacless tag
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
*/
int8_t SRIX4K_SplitWriteBlockResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID)
{
uint8_t	ResultCode,
		NbTagReplyByte,
		NbControlByte,
		TagReplyIndex,
		ControlIndex;

	// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443B,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	
	//  CR95HF returned an error code
	if (ResultCode == CR95HF_ERROR_CODE)
		return CR95HF_ERROR_CODE;	

return RESULTOK;

}

/**  
* @brief  	this function splits GetUID response and returns UID index. 
* @brief  	If the residue of tag response is incorrect the function returns ERRORCODE_GENERIC, otherwise RESULTOK
* @param  	ReaderResponse	:  	pointer on CR95HF response
* @param	Length :	Number of byte of Reader response
* @param	UIDindex : index on ReaderResponse of the UID into contactless tag 
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
* @retval 	SRIX4K_ERRORCODE_CRCRESIDUE : CRC16 residue is erroneous
*/
int8_t SRIX4K_SplitGetUIDResponse( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *UIDindex)
{
int8_t status;
uint8_t	ResultCode,
		NbTagReplyByte,
		NbControlByte,
		TagReplyIndex,
		ControlIndex;

	// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443B,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	
	//  CR95HF returned an error code
	if (ResultCode == CR95HF_ERROR_CODE)
		return CR95HF_ERROR_CODE;	
	
	// check CRC16 residue of contacless tag response
	errchk(SRIX4K_IsCorrectCRC16Residue (&ReaderResponse[TagReplyIndex],NbTagReplyByte));

	// extracts UID index from contacless tag response 
	*UIDindex = TagReplyIndex+SRIX4K_OFFSET_UID;

	return RESULTOK;
Error:
	return SRIX4K_ERRORCODE_CRCRESIDUE;	
}

/**
  * @}
  */

/** @addtogroup SRIX4k_AdvancedFunctions
* @brief  this set of function carries out advanced functionlities. They use the function defined in others group. 
*  @{
*/


/**  
* @brief  	this function returns RESULTOK if a SRIx product is present, ERRORCODE_GENERIC otherwise
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	ERRORCODE_GENERIC : CR95HF returned an error code
*/
int8_t SRIX4K_IsPresent( void )
{
int8_t 	status;

	// configure CR95HF to exchange with 14 443B 
	errchk(SRIX4K_ProtocolSelect(	SRIX4K_TXVALUE_106K,
										SRIX4K_TXVALUE_106K,
										ReaderRecBuf));
	// send initiate command to contacless tag
	errchk(SRIX4K_Initiate(ReaderRecBuf));

	// a SRx product is present in RF field
	return RESULTOK;

Error:
	return ERRORCODE_GENERIC;
}


/**  
* @brief  	this function returns UID field of a contactless tag
* @param  	UIDfield	:  	pointer UID field of a contacless tag
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	ERRORCODE_GENERIC : CR95HF returned an error code
*/
int8_t SRIX4K_GetUIDadvanced( uint8_t *UIDfield )
{
int8_t 	status;
uint8_t UIDindex,
		ChipID=0;

	memset(UIDfield,0x00,SRIX4K_SRIX4K_UID_NBBYTE);
	// configure CR95HF to exchange with 14 443B 
	errchk(SRIX4K_ProtocolSelect(	SRIX4K_TXVALUE_106K,
										SRIX4K_TXVALUE_106K,
										ReaderRecBuf));
	// send initiate command to contacless tag
	errchk(SRIX4K_Initiate(ReaderRecBuf));
	SRIX4K_SplitInitiateResponse(ReaderRecBuf,ReaderRecBuf[CR95HF_LENGTH_OFFSET],&ChipID);
	
	errchk(SRIX4K_SelectChipID ( ChipID, ReaderRecBuf));
		
	// send initiate command to contacless tag
	errchk(SRIX4K_GetUID (ReaderRecBuf));

	// analyse and get UID index
	errchk(SRIX4K_SplitGetUIDResponse(ReaderRecBuf,ReaderRecBuf[CR95HF_LENGTH_OFFSET],&UIDindex));
	
	memcpy(UIDfield,&(ReaderRecBuf[UIDindex]),SRIX4K_SRIX4K_UID_NBBYTE);

	return RESULTOK;

Error:
	return ERRORCODE_GENERIC;
}

/**  
* @brief  	this function returns a ChipId of a contacless tag and send a SelectChipId Command
* @param  	ChipID	:  	ChipId of a contacless tag
* @retval 	RESULTOK : a Contactless is in the field		
* @retval 	ERRORCODE_GENERIC : CR95HF returned an error code
*/
int8_t SRIX4K_GetChipIDAndSelectIt(uint8_t *ChipID)
{
int8_t 	status;

	*ChipID = 0;
	// send initiate command to contacless tag
	errchk(SRIX4K_Initiate(ReaderRecBuf));
	SRIX4K_SplitInitiateResponse(ReaderRecBuf,ReaderRecBuf[CR95HF_LENGTH_OFFSET],ChipID);
	
	errchk(SRIX4K_SelectChipID ( *ChipID, ReaderRecBuf));
		
	return RESULTOK;

Error:
	return ERRORCODE_GENERIC;
}

/**  
* @brief  	this function returns RESULTOK if a SRIX4k product is present, ERRORCODE_GENERIC otherwise
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
*/
int8_t SRIX4K_IsSRIX4kPresent( void )
{
int8_t 	status;
uint8_t UIDfield[SRIX4K_SRIX4K_UID_NBBYTE];

	// configure CR95HF to exchange with 14 443B 
	errchk(SRIX4K_GetUIDadvanced( UIDfield ));

	// a SRx product is present in RF field ( check D0h on UID MSB byte and Icref)
	if ((UIDfield[SRIX4K_INDEX_UID_MSB] == SRIX4K_VALUE_UID_MSB) &&
		(UIDfield[SRIX4K_INDEX_UID_ICREF] == ISO144443SR_ICREF_STM) &&
		(UIDfield[SRIX4K_INDEX_UID_PRODUCTREF] == ((ISO144443SR_ICREF_SRIX4K << SRIX4K_OFFSET_UID_ICREF) & SRIX4K_MASK_UID_ICREF)))
		return RESULTOK;

	return ERRORCODE_GENERIC;

Error:
	return ERRORCODE_GENERIC;
}



/**  
* @brief  	this function runs an anticollission sequence as defined in SRIX4k datasheet
* @param  	NbTagInventoried	:  number of tag inventoried during anticollision sequence
* @param 	ChipIdArray : pointer on table whiich contains the ChipId of the tag inventoried (the length of ChipId is one byte)
* @retval 	RESULTOK : Contactless tag response validated			
* @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
*/
int8_t SRIX4K_Anticollision(uint8_t *NbTagInventoried,uint8_t *ChipIdArray )
{
int8_t 	status;
uint8_t ChipID,NthSlot=1;
	
	*NbTagInventoried = 0;		 
	// configure CR95HF to exchange with 14 443B 
	errchk(SRIX4K_ProtocolSelect(	SRIX4K_TXVALUE_106K,
										SRIX4K_TXVALUE_106K,
										ReaderRecBuf));

	SRIX4K_ResetToInventory(ReaderRecBuf );
	// start anticollision sequence
	SRIX4K_Initiate(ReaderRecBuf);
	SRIX4K_Pcall16(ReaderRecBuf);
	// Get ChipID in first Slot
	status = SRIX4K_SplitPCall16Response(ReaderRecBuf,ReaderRecBuf[CR95HF_LENGTH_OFFSET],&ChipID);
	// a contactless tag has reply, save its ChipID and select it
	if (status == CR95HF_SUCCESS_CODE)
	{	SRIX4K_SelectChipID ( ChipID, ReaderRecBuf);	
		ChipIdArray[(*NbTagInventoried)]=ChipID;
		(*NbTagInventoried)++;
	}
    // detect SRIX4k products an the next 15 slots
	for (NthSlot = 1;NthSlot <SRIX4K_NBSLOT;NthSlot++)
	{
		// Get ChipID in NthSlot 		
		SRIX4K_SlotMarker(NthSlot, ReaderRecBuf);
		status = SRIX4K_SplitSlotMarkerResponse(ReaderRecBuf,ReaderRecBuf[CR95HF_LENGTH_OFFSET],&ChipID);
		// a contactless tag has reply, save its ChipID and select it
		if (status == CR95HF_SUCCESS_CODE)
		{	SRIX4K_SelectChipID ( ChipID, ReaderRecBuf);	
			ChipIdArray[(*NbTagInventoried)]=ChipID;
			(*NbTagInventoried)++;
		}
	}
	
  	return RESULTOK;
Error :
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/


