/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : lib_SRX4k.h
* Author             : MMY Application Team
* Version            : V1.0
* Date               : 03/01/2010
* Description        : Send commands SR
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LIBSRIX4K_H
#define __LIBSRIX4K_H

#include "string.h"
#include "stdio.h"
#include "lib_CR95HF.h"
#include "miscellaneous.h"


#define SRIX4K_SRIX4K_BLOCK_NBBYTE						0x04
#define SRIX4K_SRIX4K_UID_NBBYTE						0x08
#define SRIX4K_SRIX4K_CMD_MAXNBBYTE					0x04
#define SRIX4K_SRIX4K_WRITECMD_MAXNBBYTE				0x04+0x04
#define SRIX4K_NBSLOT									16

// value of UID field for SRIX4k
#define SRIX4K_VALUE_UID_MSB							0xD0
#define SRIX4K_MASK_UID_ICREF							0xFC
#define SRIX4K_OFFSET_UID_ICREF						0x02

// Address mappinf of SRIX4k
#define SRIX4K_MEMORY_NBBLOCK							0x7F
#define SRIX4K_MEMORY_OTP								0x00
#define SRIX4K_MEMORY_LOCKABLE_EEPROM					0x07
#define SRIX4K_MEMORY_EEPROM							0x10

// ICref value									
#define ISO144443SR_ICREF_STM	 							0x02
// ICref value									
#define ISO144443SR_ICREF_SRI2K 							0x0F
#define ISO144443SR_ICREF_SRI4K 							0x07
#define ISO144443SR_ICREF_SRIX4K 							0x03
#define ISO144443SR_ICREF_SRI512 							0x06
#define ISO144443SR_ICREF_SRT512 							0x0C



// CRC 16 constants
#define SRIX4K_PRELOADCRC16 							0xFFFF 
#define SRIX4K_POLYCRC16 								0x8408 
#define SRIX4K_MASKCRC16 								0x0001
#define SRIX4K_RESIDUECRC16 							0xF0B8


// protocol select parameters
#define SRIX4K_PROTOCOLSELECT_NBBYTE					0x02
#define SRIX4K_MASK_APPENDCRC							0x01
#define SRIX4K_MASK_TXDATARATE							0xC0
#define SRIX4K_MASK_RXDATARATE							0x30
#define SRIX4K_MASK_RFU									0x0C

#define SRIX4K_TXVALUE_106K								0x00
#define SRIX4K_TXVALUE_212K								0x01
#define SRIX4K_TXVALUE_424K								0x02
#define SRIX4K_TXVALUE_847K								0x03

#define SRIX4K_RXVALUE_106K								0x00
#define SRIX4K_RXVALUE_212K								0x01
#define SRIX4K_RXVALUE_424K								0x02
#define SRIX4K_RXVALUE_847K								0x03
												
#define SRIX4K_APPENDCRC								0x01
#define SRIX4K_DONTAPPENDCRC							0x00


#define SRIX4K_OFFSET_APPENDCRC							0x00
#define SRIX4K_OFFSET_TXDATARATE						0x06
#define SRIX4K_OFFSET_RXDATARATE						0x04
#define SRIX4K_OFFSET_SLOTNUMBER						0x04
#define SRIX4K_MASK_SLOTNUMBER							0xF0
// offset in UID field 
#define SRIX4K_INDEX_UID_MSB							0x07
#define SRIX4K_INDEX_UID_ICREF							0x06
#define SRIX4K_INDEX_UID_PRODUCTREF						0x05
// field offset in contactless tag offset
#define SRIX4K_OFFSET_CHIPID 							0x00 
#define SRIX4K_OFFSET_READBLOCK							0x00 
#define SRIX4K_OFFSET_UID								0x00 


// ISO14443_SR error code
#define SRIX4K_RESULTOK 								RESULTOK
#define SRIX4K_ERRORGENERIC								0xB0
#define SRIX4K_ERRORCODE_PROTOCOLSELECT					0xB1
#define SRIX4K_ERRORCODE_PARAMETER						0xB2
#define SRIX4K_ERRORCODE_CRCRESIDUE						0xB3

// ISO14443_SR command code
#define SRIX4K_CMDCODE_INITIATE_BYTE0 					0x06
#define SRIX4K_CMDCODE_INITIATE_BYTE1 					0x00
#define SRIX4K_CMDCODE_PCALL16_BYTE0 					0x06
#define SRIX4K_CMDCODE_PCALL16_BYTE1 					0x04
#define SRIX4K_CMDCODE_SLOTMARKER		 				0x06
#define SRIX4K_CMDCODE_SELECTCHIPID		 				0x0E
#define SRIX4K_CMDCODE_COMPLETION		 				0x0F
#define SRIX4K_CMDCODE_RESETTOINVENTORY	 				0x0C
#define SRIX4K_CMDCODE_READBLOCK			 			0x08
#define SRIX4K_CMDCODE_WRITEBLOCK		 				0x09
#define SRIX4K_CMDCODE_GETUID			 				0x0B
				   
		

// --- this function configures CR95HF to communicate with ISO14 443 B contacless tag
int8_t SRIX4K_ProtocolSelect	( const uint8_t TxDataRate, uint8_t RxDataRate,uint8_t *pResponse);
// --- these functions emit a command to contacless tag through CR95HF
int8_t SRIX4K_Initiate			( uint8_t *pResponse );
int8_t SRIX4K_Pcall16			( uint8_t *pResponse );
int8_t SRIX4K_SlotMarker		( const uint8_t SlotNumber, uint8_t *pResponse );
int8_t SRIX4K_SelectChipID		( const uint8_t ChipID, uint8_t *pResponse );
int8_t SRIX4K_Completion		( uint8_t *pResponse );
int8_t SRIX4K_ResetToInventory	( uint8_t *pResponse );
int8_t SRIX4K_ReadSingleBlock	( const uint8_t BlockAddress, uint8_t *pResponse );
int8_t SRIX4K_WriteSingleBlock  ( const uint8_t BlockAddress,const uint8_t *DataToWrite, uint8_t *pResponse );
int8_t SRIX4K_GetUID 			( uint8_t *pResponse );

// --- these functions extract contactless tag response from reader response
int8_t SRIX4K_SplitInitiateResponse		( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);
int8_t SRIX4K_SplitPCall16Response		( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);
int8_t SRIX4K_SplitSlotMarkerResponse	( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);
int8_t SRIX4K_SplitSelectChipIDResponse	( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);
int8_t SRIX4K_SplitCompletionResponse	( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);
int8_t SRIX4K_SplitWriteBlockResponse	( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *ChipID);
int8_t SRIX4K_SplitReadBlockResponse		( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *DataReadIndex);
int8_t SRIX4K_SplitGetUIDResponse		( const uint8_t *ReaderResponse,const uint8_t Length,uint8_t *UIDindex);

// --- Advanced Functions  
int8_t SRIX4K_IsPresent					( void );
int8_t SRIX4K_GetUIDadvanced			( uint8_t *UIDfield );
int8_t SRIX4K_IsSRIX4kPresent			( void );
int8_t SRIX4K_GetChipIDAndSelectIt		(uint8_t *ChipID);
int8_t SRIX4K_Anticollision				( uint8_t *NbTagInventoried,uint8_t *ChipIdArray );

#endif /* __SRIX4K_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
