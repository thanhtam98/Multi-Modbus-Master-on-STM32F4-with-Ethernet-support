/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
  * @file    lib_iso14443A.h
  * @author  MMY Application Team
  * @version V1.0
  * @date    01/07/2011
  * @brief   This file provides a set of function according to ISO/IEC14443A_3 and ISO/IEC14443A_4 specification.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ISO14443A_H
#define __ISO14443A_H

#include "lib_CR95HF.h"

/* Command code ISO14443_3A protocol control byte field */
#define ISO14443A_CMDCODE_REQA								0x26
#define ISO14443A_CMDCODE_WUPA								0x52
#define ISO14443A_CMDCODE_HLTA								0x50
/* Command code ISO14443_4A protocol control byte field */
#define ISO14443A_CMDCODE_RATS								0xE0
#define ISO14443A_CMDCODE_PPS									0xD0
#define ISO14443_4A_PCB_IBLOCK								0x02
#define ISO14443_4A_PCB_RBLOCK								0xA2
#define ISO14443_4A_PCB_SBLOCK								0xC2
/* Command code Mifare protocol control byte field */
#define ISO14443A_CMDCODE_READ								0x30 //Command = 0x30, Block Address = 0xYY
#define ISO14443A_CMDCODE_WRITE								0xA2 //Command = 0xA2, Block Address = 0xYY, 16 bytes of data

/* Anticollison levels (commands) */
#define ISO14443A_CMDCODE_SELECT_LVL1					0x93
#define ISO14443A_CMDCODE_SELECT_LVL2					0x95
#define ISO14443A_CMDCODE_SELECT_LVL3					0x97

/* UID Sizes */
#define ISO14443A_NBBYTE_UIDSINGLE						4
#define ISO14443A_NBBYTE_UIDDOUBLE						7
#define ISO14443A_NBBYTE_UIDTRIPLE						10
#define ISO14443A_NBBYTE_SELECTRESPONSE				4
#define ISO14443A_NBBYTE_SELECT								5

/* NVB bytes values */
#define ISO14443A_NVB_10											0x10
#define ISO14443A_NVB_20											0x20
#define ISO14443A_NVB_30											0x30
#define ISO14443A_NVB_40											0x40
#define ISO14443A_NVB_50											0x50
#define ISO14443A_NVB_60											0x60
#define ISO14443A_NVB_70											0x70

/* ATQ FLAG */
#define ISO14443A_ATQA_UID_SINGLESIZE					0
#define	ISO14443A_ATQA_UID_DOUBLESIZE					1
#define ISO14443A_ATQA_UID_TRIPLESIZE					2


/* SAK byte constant */
#define ISO14443A_SAK_14443_4_COMPATIBLE			0x20
#define ISO14443A_SAK_UIDNOTCOMPLETE					0x04

/* command control byte value of CR95HF */
#define ISO14443A_NBSIGNIFICANTBIT_7					0x07
#define ISO14443A_NBSIGNIFICANTBIT_8					0x08
#define ISO14443A_APPENDCRC										0x20
#define ISO14443A_SPLITFRAME  								0x40 // vp 181011

/* command control byte value of CR95HF */
#define ISO14443A_MAX_NBBYTE_UID							0x07
#define ISO14443A_CMD_MAXNBBYTE								0x10
#define ISO14443A_ANSWER_MAXNBBYTE						0x20

/* protocol select parameters 	*/
#define ISO14443A_PROTOCOLSELECT_NBBYTE				0x02
#define ISO14443A_MASK_APPENDCRC							0x01
#define ISO14443A_MASK_TXDATARATE							0xC0
#define ISO14443A_MASK_RXDATARATE							0x30
#define ISO14443A_MASK_RFU										0x0C

#define ISO14443A_TXVALUE_106K								0x00
#define ISO14443A_TXVALUE_212K								0x01
#define ISO14443A_TXVALUE_424K								0x02
#define ISO14443A_TXVALUE_847K								0x03

#define ISO14443A_RXVALUE_106K								0x00
#define ISO14443A_RXVALUE_212K								0x01
#define ISO14443A_RXVALUE_424K								0x02
#define ISO14443A_RXVALUE_847K								0x03
												
#define ISO14443A_MASK_SLOTNUMBER							0xF0

/* ISO14443_A Mask */
#define ISO14443A_MASK_PROPRIETARYCODING			0x0F
#define ISO14443A_MASK_UIDSIZEBITFRAME				0xC0
#define ISO14443A_MASK_BITFRAMEANTICOL				0x1F
#define ISO14443A_MASK_COLLISIONBIT						0x80
#define ISO14443A_MASK_CRCBIT									0x20
#define ISO14443A_MASK_PARITYBIT							0x10
#define ISO14443A_MASK_SIGNIFICANTBITS				0x0F
#define ISO14443A_MASK_FIRSTBYTECOLLISION			0xFF
#define ISO14443A_MASK_FIRSTBITCOLLISION			0x0F
#define ISO14443A_MASK_RATS_FSDI							0xF0
#define ISO14443A_MASK_RATS_CID								0x0F
#define ISO14443A_MASK_ATS_FSCI								0x0F
#define ISO14443A_MASK_ATS_TA									0x10
#define ISO14443A_MASK_ATS_TB									0x20
#define ISO14443A_MASK_ATS_TC									0x40
#define ISO14443A_MASK_ATS_TA_DR							0x07
#define ISO14443A_MASK_ATS_TA_DS							0x70
#define ISO14443A_MASK_ATS_TA_PPSCHANGE				0x80
#define ISO14443A_MASK_ATS_TB_SFGI						0x0F
#define ISO14443A_MASK_ATS_TB_FWI							0xF0
#define ISO14443A_MASK_ATS_TC_NAD							0x01
#define ISO14443A_MASK_ATS_TC_CID							0x02
#define ISO14443A_MASK_PPS_CID								0x0F
#define ISO14443A_MASK_14443_4_COMPATIBLE			0x24
#define ISO14443A_MASK_SAK_UIDNOTCOMPLETE			0x04
#define ISO14443A_MASK_UIDSIZE						    0x03



// ISO14443_4A I-Block
#define ISO14443_4A_PCB_IBLOCK_CHAINING				0x10
#define ISO14443_4A_PCB_IBLOCK_CID						0x08
#define ISO14443_4A_PCB_IBLOCK_NAD						0x04
#define ISO14443_4A_PCB_IBLOCK_BLOCKNUMBER		0x01
// ISO14443_4A R-Block
#define ISO14443_4A_PCB_RBLOCK_NAK						0x10
#define ISO14443_4A_PCB_RBLOCK_CID						0x08
#define ISO14443_4A_PCB_RBLOCK_BLOCKNUMBER		0x01
// ISO14443_4A S-Block
#define ISO14443_4A_PCB_SBLOCK_WTX						0x30
#define ISO14443_4A_PCB_SBLOCK_CID						0x08

/* ISO14443_A index of CR95HF control byte	 */
#define ISO14443A_INDEX_CONTROLBYTE_GENERIC							0x00
#define ISO14443A_INDEX_CONTROLBYTE_FIRSTBYTECOLLISION	0x01
#define ISO14443A_INDEX_CONTROLBYTE_FIRSTBITCOLLISION		0x02

/* ISO14443_A offset */
#define ISO14443A_OFFSET_APPENDCRC						0x00
#define ISO14443A_OFFSET_TXDATARATE						0x06
#define ISO14443A_OFFSET_RXDATARATE						0x04
#define ISO14443A_OFFSET_SLOTNUMBER						0x04
#define ISO14443A_OFFSET_NVB									0x04
#define ISO14443A_OFFSET_UIDSIZEBITFRAME			0x06
#define ISO14443A_OFFSET_RATS_FSDI						0x04
#define ISO14443A_OFFSET_UID_SELECT						2
#define ISO14443A_OFFSET_SAKBYTE						 	CR95HF_DATA_OFFSET
#define ISO14443A_OFFSET_ATQAFIRSTBYTE				CR95HF_DATA_OFFSET
#define ISO14443A_OFFSET_UIDSIZE							6

/* ISO14443_A error code */
#define ISO14443A_RESULTOK 										RESULTOK
#define ISO14443A_ERRORGENERIC								0xA0
#define ISO14443A_ERRORCODE_PROTOCOLSELECT		0xA1
#define ISO14443A_ERRORCODE_PARAMETER					0xA2
#define ISO14443A_ERRORCODE_CRCRESIDUE				0xA3
#define ISO14443A_ERRORCODE_BCC								0xA4

/* ISO14443_4A ATS framework*/
typedef struct _ATS_FRAMEWORK
{
	uint8_t TL;
	uint8_t T0;
	uint8_t TA;
	uint8_t TB;
	uint8_t TC;
	uint8_t T1;
	uint8_t TK;
	uint8_t CRC1;
	uint8_t CRC2;
}ATS_FRAMEWORK;

typedef struct _BLOCK_TRANSMISSION_PROTOCOL
{
	uint8_t PCB; // Protocol Control Byte (mandatory), 
	uint8_t CID; // Card IDentifier (optional), 
	uint8_t NAD; // Node ADdress (optional)
	uint8_t *Information;  //Information field
	uint16_t EDC; // Epilogue field
}BLOCK_TRANSMISSION_PROTOCOL;


/* ---------------------------------------------------------------------------------
 * --- Local Functions  
 * --------------------------------------------------------------------------------- */
int8_t ISO14443A_ProtocolSelect( const uint8_t TxDataRate, const uint8_t RxDataRate,uint8_t *pResponse );
int8_t ISO14443A_IsPresent( void );
int8_t ISO14443A_Anticollision( void );
int8_t ISO14443A_CardTest( void );

int8_t ISO14443A_REQA( uint8_t *pResponse );
int8_t ISO14443A_WUPA( uint8_t *pResponse );
int8_t ISO14443A_HLTA( uint8_t *pResponse );
int8_t ISO14443A_RATS( const uint8_t FSDI, const uint8_t CID, uint8_t *pResponse );
int8_t ISO14443A_PPS( const uint8_t CID, const uint8_t PPSbyte0, const uint8_t PPSbyte1, uint8_t *pResponse );


int8_t ISO14443A_AntiCollisionLevel1( uint8_t *pResponse );
int8_t ISO14443A_AntiCollisionLevel2( uint8_t *pResponse );
int8_t ISO14443A_AntiCollisionLevel3( uint8_t *pResponse );
									
int8_t ISO14443A_SelectLevel1( const uint8_t NbByteUIDin, const uint8_t *UIDin, uint8_t *pResponse );
int8_t ISO14443A_SelectLevel2( const uint8_t NbByteUIDin, const uint8_t *UIDin, uint8_t *pResponse );
int8_t ISO14443A_SelectLevel3( const uint8_t NbByteUIDin, const uint8_t *UIDin, uint8_t *pResponse );

int8_t ISO14443A_SplitATQA( uint8_t *ReaderResponse, uint8_t *ProprietaryCoding, uint8_t *UIDsizeframe, uint8_t *BitFrameAnticol,uint8_t *CR95HFControlByteIndex);

int8_t ISO14443A_IsCollisionDetected( const uint8_t *CR95HFControlByteIndex );
int8_t ISO14443A_IsCRCError( const uint8_t *CR95HFControlByteIndex );
int8_t ISO14443A_IsParityError( const uint8_t *CR95HFControlByteIndex );
int8_t ISO14443A_IsUIDComplete( const uint8_t SAKbyte );
int8_t ISO14443A_Is14443_4Compatible( const uint8_t SAKbyte );
int8_t ISO14443A_IsPresent( void );
int8_t ISO14443A_IsCorrectBCC( uint8_t NbByte,uint8_t *Data, uint8_t BCC );
int8_t ISO14443A_IsCIDSupport(uint8_t TCByte);
int8_t ISO14443A_IsPPSsuppored(const uint8_t T0Byte);
int8_t ISO14443A_IsPPSChange(const uint8_t PPSByte);


int8_t ISO14443A_GetSignificantBit( const uint8_t *CR95HFControlByteIndex );
int8_t ISO14443A_GetFirstCollisionByte( const uint8_t *CR95HFControlByteIndex );
int8_t ISO14443A_GetFirstCollisionBit( const uint8_t *CR95HFControlByteIndex );


int8_t ISO14443A_SelectSequence( uint8_t *SAKByte );
int8_t ISO14443A_GetUIDsize( uint8_t *UIDsize );
int8_t ISO14443A_GetUID( uint8_t *SAKByte, uint8_t *NbUIDByte, uint8_t *UIDout );
int8_t ISO14443_4A_SendBlock(uint8_t PCBByte, uint8_t CIDByte, uint8_t NADByte, uint8_t *Playload, uint8_t NBPlayload, uint8_t FSC);

int8_t Authentication_Tag(int8_t Command_key,int8_t Add,uint8_t* Key,uint8_t* UID);
int8_t ISO14443A_Read(int8_t Add, uint8_t *Data,int8_t leght);

#endif /* __ISO14443A_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
