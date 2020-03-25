/**
  ******************************************************************************
  * @file    lib_iso14443B.h
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
#ifndef __ISO14443B_H
#define __ISO14443B_H

#include "lib_CR95HF.h"

#define ATQB_SIZE					13
#define MAX_PUPI_SIZE			4

// command code
#define ISO14443B_CMDCODE_REQB							0x05
#define ISO14443B_CMDCODE_WUPB							0x05
#define ISO14443B_CMDCODE_SLOTNUMBER					0x05
#define ISO14443B_CMDCODE_ATTRIB						0x1B
#define ISO14443B_CMDCODE_HLTB							0x50

#define ISO14443B_CMDCODE_REQB_BIT4						0xEF
#define ISO14443B_CMDCODE_WUPB_BIT4						0x10

/* protocol select parameters 	*/
#define ISO14443B_PROTOCOLSELECT_NBBYTE					0x02


/* command control byte value of CR95HF */
#define ISO14443B_CMD_MAXNBBYTE							0x20
#define ISO14443B_REPLY_MAXNBBYTE						0x20
#define ISO14443B_ATTRIB_MAXNBBYTE						0xFF
#define ISO14443B_CMD_HIGHERLAYERDATA					ISO14443B_ATTRIB_MAXNBBYTE - ISO14443B_NBBYTE_IDENTIFIER- ISO14443B_NBBYTE_PARAMETERS -ISO14443B_NBBYTE_CRC
#define ISO14443B_SLOTNUMBER_MIN						0x01
#define ISO14443B_SLOTNUMBER_MAX						0x0F
#define ISO14443B_PROTOCOLINFO_MIN						0x03
#define ISO14443B_PROTOCOLINFO_MAX						0x04

/* ISO14443_B Length */
#define ISO14443B_NBBYTE_PUPI							0x04
#define ISO14443B_NBBYTE_APPLICATIONDATA				0x04
#define ISO14443B_NBBYTE_CRC							0x02
#define ISO14443B_NBBYTE_AFI							0x01
#define ISO14443B_NBBYTE_IDENTIFIER						0x04
#define ISO14443B_NBBYTE_PARAMETERS						0x04

/* protocol select parameters 	*/
#define ISO14443B_TXVALUE_106K							0x00
#define ISO14443B_TXVALUE_212K							0x01
#define ISO14443B_TXVALUE_424K							0x02
#define ISO14443B_TXVALUE_847K							0x03

#define ISO14443B_RXVALUE_106K							0x00
#define ISO14443B_RXVALUE_212K							0x01
#define ISO14443B_RXVALUE_424K							0x02
#define ISO14443B_RXVALUE_847K							0x03

#define ISO14443B_PROTOCOLSELECT_APPENDCRC				0x01
#define ISO14443B_PROTOCOLSELECT_DONTAPPENDCRC			0x00


/* ISO14443_B offset */
#define ISO14443B_OFFSET_TXDATARATE						0x06
#define ISO14443B_OFFSET_RXDATARATE						0x04
#define ISO14443B_OFFSET_NUMBEROFSLOTS					0x00
#define ISO14443B_OFFSET_REQBATQB						0x03
#define ISO14443B_OFFSET_EXTENDEDBIT					0x04
#define ISO14443B_OFFSET_SLOTNUMBER						0x04
#define ISO14443B_OFFSET_AFI							0x00

#define ISO14443B_OFFSET_BITRATECAPABILITY				0x00
#define ISO14443B_OFFSET_MAXFRAMESIZE					0x01
#define ISO14443B_OFFSET_PROTOCOLBYTE					0x01
#define ISO14443B_OFFSET_FWI							0x02
#define ISO14443B_OFFSET_ADC							0x02
#define ISO14443B_OFFSET_FO								0x02
#define ISO14443B_OFFSET_SFGI							0x03
#define ISO14443B_OFFSET_RFU							0x03

// Protocol info field
#define ISO14443B_OFFSETBIT_MAXFRAMESIZE				0x04
#define ISO14443B_OFFSETBIT_PROTOCOLBYTE				0x00
#define ISO14443B_OFFSETBIT_FWI							0x04
#define ISO14443B_OFFSETBIT_ADC							0x02
#define ISO14443B_OFFSETBIT_FO							0x00
#define ISO14443B_OFFSETBIT_SFGI						0x04
#define ISO14443B_OFFSETBIT_RFU							0x00

/* ISO14443_B mask */
#define ISO14443B_MASK_TXDATARATE						0xC0
#define ISO14443B_MASK_RXDATARATE						0x30
#define ISO14443B_MASK_NUMBEROFSLOTS					0x03
#define ISO14443B_MASK_REQBATQB							0x04
#define ISO14443B_MASK_EXTENDEDBIT						0x10
#define ISO14443B_MASK_SLOTNUMBER						0xF0
	// Protocol info field
#define ISO14443B_MASK_MAXFRAMESIZE						0xF0
#define ISO14443B_MASK_PROTOCOLBYTE						0x0F
#define ISO14443B_MASK_FWI								0xF0
#define ISO14443B_MASK_ADC								0x0C
#define ISO14443B_MASK_FO								0x03
#define ISO14443B_MASK_SFGI								0xF0
#define ISO14443B_MASK_RFU								0x0F

/* ISO14443_B error code */
#define ISO14443B_RESULTOK 								RESULTOK
#define ISO14443B_ERRORGENERIC							0xB0
#define ISO14443B_ERROR_PARAMETERVALUE					0xB1
#define ISO14443B_ERROR_PARAMETERLENGTH					0xB2


int8_t ISO14443B_SetParamByte	( const uint8_t ExtentedBit,const uint8_t REQBWUPBbit,const uint8_t NumberOfSlots );

int8_t ISO14443B_REQB			( const uint8_t AFIin,const uint8_t Param, uint8_t *pResponse );
int8_t ISO14443B_SlotMarker		( const uint8_t SlotNumber, uint8_t *pResponse );
int8_t ISO14443B_Attrib			( const uint8_t *Identifier,const uint8_t *Parameters,const uint8_t NbByteHigherlayerData,const uint8_t *HigherLayersData, uint8_t *pResponse );


int8_t ISO14443B_SplitATQB					( uint8_t *ReaderResponse, uint8_t *PUPIindex, uint8_t *ApplicationDataindex, uint8_t *ProtocolInfoindex);
int8_t ISO14443B_SplitApplicationDataField  ( const uint8_t *ApplicatioDataField, uint8_t *AFIout,uint8_t *CRCBoutIndex, uint8_t *NumberOfAplication);
int8_t ISO14443B_SplitProtocolInfoField		( const uint8_t *ProtocolInfoField, uint8_t *BitRateCapability,uint8_t *MaxFrameSize, uint8_t *ProtocolType, uint8_t *FWIbits, uint8_t *ADCbits, uint8_t *FObits, uint8_t *SFGIbits, uint8_t *RFUbits);


int8_t ISO14443B_IsPresent		( void );


#endif /* __ISO14443B_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
