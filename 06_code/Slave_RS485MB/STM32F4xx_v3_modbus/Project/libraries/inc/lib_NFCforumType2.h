/******************** (C) COPYRIGHT 2011 STMicroelectronics ********************
  * @file    lib_NFCforumType2.h
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
#ifndef __NFCFORUMT2_H
#define __NFCFORUMT2_H

#include "lib_CR95HF.h"

// command code
#define NFCFORUMT2_CMDCODE_READ							0x30
#define NFCFORUMT2_CMDCODE_WRITE						0xA2
#define NFCFORUMT2_CMDCODE_SELECTORSELECT				0xC2

#define NFCFORUMT2_NBBYTE_WRITE							0x04
#define NFCFORUMT2_NBBYTE_READ							0x10
#define NFCFORUMT2_NBBYTE_CRC							0x02
#define NFCFORUMT2_NBBYTE_RFUSELECTORSELECT				0x03

#define NFCFORUMT2_CMD_MAXNBBYTE 						0x10
#define NFCFORUMT2_SELECTORSELECT_PARAMETER				0xFF
#define NFCFORUMT2_SELECTORSELECT_RFU					0x00

/* ISO14443_A error code */
#define NFCFORUMT2_RESULTOK 							RESULTOK
#define NFCFORUMT2_ERRORGENERIC							0xC0
#define NFCFORUMT2_ERRORCODE_PROTOCOLSELECT				0xC1
#define NFCFORUMT2_ERRORCODE_PARAMETER					0xC2
#define NFCFORUMT2_ERRORCODE_CRCRESIDUE					0xC3
#define NFCFORUMT2_ERRORCODE_BCC						0xC4




/* ---------------------------------------------------------------------------------
 * --- Local Functions  
 * --------------------------------------------------------------------------------- */
int8_t NFCforumType2_ProtocolSelect			( uc8 TxDataRate, uint8_t RxDataRate,uint8_t *pResponse );
int8_t  NFCforumType2_Read					( uc8 NthBlock, uint8_t *pResponse );
int8_t NFCforumType2_Write					( uc8 NthBlock,uc8 *DataTowrite, uint8_t *pResponse );

int8_t NFCforumType2_SelectSequence 		( uint8_t *SAKByte );


#endif /* __ISO14443A_H */

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
