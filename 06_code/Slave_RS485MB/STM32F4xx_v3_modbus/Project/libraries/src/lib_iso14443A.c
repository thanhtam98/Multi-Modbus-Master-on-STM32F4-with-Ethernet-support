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

/**
 * <!-- ------------------------------------------------------------------------------------------------------------ -->
 * @mainpage
 * @section lib14443AMainIntroduction Introduction
 * This document describes the 144443_3 (included type A &B ) libraries and the  SRIX4k library. 
 * This firmware provides a set functions according to ISO/IEC14443_3 usable with CR95Hf device. 
 * <!-- ------------------------------------------------------------------------------------------------------------ -->
 * @subsection XorroMainTerms Terms and conventions
 * The table provides a list of terms used in this document and their meaning.
 *
 * <TABLE>
  *	<TR>
 *		<TD><b>Term</b></TD>
 *		<TD><b>Description</b></TD>
 *  </TR>  
 *	<TR>
 *		<TD>AFI</TD>
 *		<TD>Application Family Identifier</TD>
 *  </TR>  
 *	<TR>
 *		<TD>ATQA</TD>
 *		<TD>Answer To Request type A</TD>
 *  </TR>  
 *	<TR>
 *		<TD>ATQB</TD>
 *		<TD>Answer To Request type B</TD>
 *  </TR>  
 *	<TR>
 *		<TD>HLTA</TD>
 *		<TD>Halt command type A</TD>
 *  </TR> 
 *	<TR>
 *		<TD>HLTB</TD>
 *		<TD>Halt command type B</TD>
 *  </TR> 
  *	<TR>
 *		<TD>PCD</TD>
 *		<TD>Proximity coupling device</TD>
 *  </TR>  
 *	<TR>
 *		<TD>REQA</TD>
 *		<TD>Request command type A</TD>
 *  </TR>  
  *	<TR>
 *		<TD>REQB</TD>
 *		<TD>Request command type B</TD>
 *  </TR>  
 *	<TR>
 *		<TD>RFU</TD>
 *		<TD>Reserved for Future Use</TD>
 *  </TR>  
 *	<TR>
 *		<TD>UID</TD>
 *		<TD>Unique IDentifier</TD>
 *  </TR>  
 *	<TR>
 *		<TD>WUPA</TD>
 *		<TD>Wake-Up A command type A</TD>
 *  </TR>
 *	<TR>
 *		<TD>WUPA</TD>
 *		<TD>Wake-Up A command type B</TD>
 *  </TR>
 * </TABLE>
 *
 *
 * @page 14443AGroupChangeLog Revision History
 * <TABLE>
 *		<CAPTION>Document revision history</CAPTION>
 *		<TR><TD><B>Date</B></TD><TD><B>Revision</B></TD><TD><B>Changes</B></TD></TR>
 *		<TR><TD>01-July-2011</TD><TD>1.0</TD><TD>Initial release</TD></TR>
 *		<TR><TD>17-July-2011</TD><TD>1.1</TD><TD>substitution of SRIX4K function header</TD></TR>
 * </TABLE>
*******************************************************************************/


#include "lib_iso14443A.h"
#include "string.h"
#include "stdio.h"
#include "hw_config.h" 
#include "miscellaneous.h"

/** @addtogroup Libraries
 * 	@{
 */

 /** @addtogroup ISO14443_A
 * 	@{
 */


/** @addtogroup ISO14443A_Functions
*  @{
*/
/** @addtogroup ISO14443A_ProcolSelectFunction
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
int8_t ISO14443A_ProtocolSelect(const uint8_t TxDataRate, const uint8_t RxDataRate,uint8_t *pResponse)
{
uint8_t Parameters = 0x00;
int8_t 	status;

	*pResponse = SENDRECV_ERRORCODE_SOFT;
	*(pResponse+1) = 0x00;
	// checks the parameter values
	if (TxDataRate>ISO14443A_TXVALUE_847K)
		return ISO14443A_ERRORCODE_PARAMETER;
	if (RxDataRate>ISO14443A_RXVALUE_847K)
		return ISO14443A_ERRORCODE_PARAMETER;		

	// create the parameter byte of ProtocolSelect command
	Parameters = 	( (TxDataRate<<ISO14443A_OFFSET_TXDATARATE) & ISO14443A_MASK_TXDATARATE) |
					( (RxDataRate<<ISO14443A_OFFSET_RXDATARATE) & ISO14443A_MASK_RXDATARATE);

	// send ProtocolSelect to CR95HF to configure it
	errchk(CR95HF_ProtocolSelect(	ISO14443A_PROTOCOLSELECT_NBBYTE,
									PROTOCOL_TAG_ISO14443A,
									&Parameters,
									pResponse));

	// checks CR95HF response
	if (CR95HF_IsReaderResultCodeOk (PROTOCOL_SELECT,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;
	
	return RESULTOK;
Error:
	return ISO14443A_ERRORCODE_PARAMETER;
}

/**
  * @}
  */ 

  /** @addtogroup ISO14443A_CommandFunctions
*  @brief  this set of function send a command defined ISO/IEC 14443-3 document
*  @{
*/


/**
 * @brief  this function emits a ReqA command to a contactless tag
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443A_REQA( uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_REQA;
	// append the command control byte 0x07 ( Number of significant bit = 7)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_7;
	
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  this function emits a WUPA command to a contactless tag
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443A_WUPA( uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_WUPA;
	// append the command control byte 0x07 ( Number of significant bit = 7)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_7;
	
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}


/**
 * @brief  this function emits a HLTA command to a contactless tag
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443A_HLTA( uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_HLTA;
	DataToSend[NthByte++] = 0x00;
	// append the command control byte 0x28 ( append CRC & Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;
		
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  this function emits an Anticollision command  (Level 1) to a contactless tag
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443A_AntiCollisionLevel1( uint8_t *pResponse )
{
	uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
	NthByte = 0;
	int8_t status;
	uint8_t PointCollision =0;
	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL1;
	DataToSend[NthByte++] = ISO14443A_NVB_20;
	// append the command control byte 0x08 ( Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8;
CollisionLoop:
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	if(PointCollision)
	{
		memcpy(&pResponse[ISO14443A_OFFSET_UID_SELECT],&DataToSend[ISO14443A_OFFSET_UID_SELECT],PointCollision>>4);
		pResponse[ISO14443A_OFFSET_UID_SELECT +(PointCollision>>4)] |= (DataToSend[ISO14443A_OFFSET_UID_SELECT+(PointCollision>>4)]&(0xFF >>(8-(PointCollision&0x0F))));
	}
	if(ISO14443A_IsCollisionDetected(&pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]-1])==RESULTOK) //Bit Collision is detected
	{
		NthByte = 0;
		DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL1;
		PointCollision = (pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]]<<4) | (pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]+1]&0x0F);
		DataToSend[NthByte++] = ISO14443A_NVB_20 + PointCollision + 1;
		memcpy(&DataToSend[NthByte],&pResponse[ISO14443A_OFFSET_UID_SELECT],(PointCollision>>4)+1);
		NthByte += PointCollision >>4;
		DataToSend[NthByte++] &= (0xFF>>(8- (PointCollision &0x0F))); 
		DataToSend[NthByte++] = ISO14443A_SPLITFRAME | ((PointCollision & 0x0F)+1);
		goto CollisionLoop;
	}
	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  this function emits an Anticollision command  (Level 2) to a contactless tag
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443A_AntiCollisionLevel2( uint8_t *pResponse )
{
	uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
	NthByte = 0;
	int8_t status;
	uint8_t PointCollision =0;
	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL2;
	DataToSend[NthByte++] = ISO14443A_NVB_20;
	// append the command control byte 0x08 ( Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8;
CollisionLoop:
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	if(PointCollision)
	{
		memcpy(&pResponse[ISO14443A_OFFSET_UID_SELECT],&DataToSend[ISO14443A_OFFSET_UID_SELECT],PointCollision>>4);
		pResponse[ISO14443A_OFFSET_UID_SELECT +(PointCollision>>4)] |= (DataToSend[ISO14443A_OFFSET_UID_SELECT+(PointCollision>>4)]&(0xFF >>(8-(PointCollision&0x0F))));
	}
	if(ISO14443A_IsCollisionDetected(&pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]-1])==RESULTOK) //Bit Collision is detected
	{
		NthByte = 0;
		DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL2;
		PointCollision = (pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]]<<4) | (pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]+1]&0x0F);
		DataToSend[NthByte++] = ISO14443A_NVB_20 + PointCollision + 1;
		memcpy(&DataToSend[NthByte],&pResponse[ISO14443A_OFFSET_UID_SELECT],(PointCollision>>4)+1);
		NthByte += PointCollision >>4;
		DataToSend[NthByte++] &= (0xFF>>(8- (PointCollision &0x0F))); 
		DataToSend[NthByte++] = ISO14443A_SPLITFRAME | ((PointCollision & 0x0F)+1);
		goto CollisionLoop;
	}
	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  this function emits an Anticollision command  (Level 3) to a contactless tag
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443A_AntiCollisionLevel3( uint8_t *pResponse )
{
	uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
	NthByte = 0;
	int8_t status;
	uint8_t PointCollision =0;
	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL3;
	DataToSend[NthByte++] = ISO14443A_NVB_20;
	// append the command control byte 0x08 ( Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8;
CollisionLoop:
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	if(PointCollision)
	{
		memcpy(&pResponse[ISO14443A_OFFSET_UID_SELECT],&DataToSend[ISO14443A_OFFSET_UID_SELECT],PointCollision>>4);
		pResponse[ISO14443A_OFFSET_UID_SELECT +(PointCollision>>4)] |= (DataToSend[ISO14443A_OFFSET_UID_SELECT+(PointCollision>>4)]&(0xFF >>(8-(PointCollision&0x0F))));
	}
	if(ISO14443A_IsCollisionDetected(&pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]-1])==RESULTOK) //Bit Collision is detected
	{
		NthByte = 0;
		DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL3;
		PointCollision = (pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]]<<4) | (pResponse[pResponse[ISO14443A_OFFSET_UID_SELECT-1]+1]&0x0F);
		DataToSend[NthByte++] = ISO14443A_NVB_20 + PointCollision + 1;
		memcpy(&DataToSend[NthByte],&pResponse[ISO14443A_OFFSET_UID_SELECT],(PointCollision>>4)+1);
		NthByte += PointCollision >>4;
		DataToSend[NthByte++] &= (0xFF>>(8- (PointCollision &0x0F))); 
		DataToSend[NthByte++] = ISO14443A_SPLITFRAME | ((PointCollision & 0x0F)+1);
		goto CollisionLoop;
	}
	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}


/**							  
 * @brief  this function emits a select command  (Level 1) to a contactless tag
 * @param 	NbByteUIDin : Number of byte of UIDin parameters
 * @param 	UIDin : pointer on UID append to Select command
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 * @retval 	ISO14443A_ERRORCODE_PARAMETER : One parameter is erroneous
 */
int8_t ISO14443A_SelectLevel1( const uint8_t NbByteUIDin, const uint8_t *UIDin, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	if (NbByteUIDin > ISO14443A_MAX_NBBYTE_UID)
		return ISO14443A_ERRORCODE_PARAMETER;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL1;
	DataToSend[NthByte++] = ISO14443A_NVB_70;
	memcpy(&(DataToSend[NthByte]),UIDin,NbByteUIDin);
	NthByte += NbByteUIDin;

	// append the command control byte 0x28 ( append CRC & Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  this function emits a select command  (Level 2) to a contactless tag
 * @param 	NbByteUIDin : Number of byte of UIDin parameters
 * @param 	UIDin : pointer on UID append to Select command
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 * @retval 	ISO14443A_ERRORCODE_PARAMETER : One parameter is erroneous
 */
int8_t ISO14443A_SelectLevel2 ( const uint8_t NbByteUIDin, const uint8_t *UIDin, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	
	if (NbByteUIDin > ISO14443A_MAX_NBBYTE_UID)
		return ISO14443A_ERRORCODE_PARAMETER;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL2;
	DataToSend[NthByte++] = ISO14443A_NVB_70;
	memcpy(&(DataToSend[NthByte]),UIDin,NbByteUIDin);
	NthByte += NbByteUIDin;

	// append the command control byte 0x28 ( append CRC & Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  	this function emits a select command  (Level 3) to a contactless tag
 * @param 	NbByteUIDin : Number of byte of UIDin parameters
 * @param 	UIDin : pointer on UID append to Select command
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 * @retval 	ISO14443A_ERRORCODE_PARAMETER : One parameter is erroneous
 */
int8_t ISO14443A_SelectLevel3 ( const uint8_t NbByteUIDin, const uint8_t *UIDin, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	
	if (NbByteUIDin > ISO14443A_MAX_NBBYTE_UID)
		return ISO14443A_ERRORCODE_PARAMETER;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_SELECT_LVL3;
	DataToSend[NthByte++] = ISO14443A_NVB_70;
	memcpy(&(DataToSend[NthByte]),UIDin,NbByteUIDin);
	NthByte += NbByteUIDin;

	// append the command control byte 0x28 ( append CRC & Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  	this function emits a RATS command  (Iso 14443-4) to a contactless tag
 * @param 	FSDI : Frame size for proximity coupling device integer ( 4 bits) 
 * @param 	CID : Card Identifier (4 bits)
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
  */
int8_t ISO14443A_RATS ( const uint8_t FSDI, const uint8_t CID, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_RATS;
	DataToSend[NthByte++] = ((FSDI<<ISO14443A_OFFSET_RATS_FSDI)& ISO14443A_MASK_RATS_FSDI) |
							(CID & ISO14443A_MASK_RATS_CID) ;
		// append the command control byte 0x28 ( append CRC & Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**							  
 * @brief  	this function emits a PPS command (Iso 14443-4) to a contactless tag
 * @param 	CID : Card Identifier (4 bits)
 * @param 	PPSbyte0 : First byte parameter of PPS command
 * @param 	PPSbyte1 : Second byte parameter of PPS command
 * @param 	pResponse : pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
  */
int8_t ISO14443A_PPS (const uint8_t CID, const uint8_t PPSbyte0, const uint8_t PPSbyte1, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_PPS | (CID & ISO14443A_MASK_PPS_CID) ;
	DataToSend[NthByte++] = PPSbyte0;
	DataToSend[NthByte++] = PPSbyte1;
		// append the command control byte 0x28 ( append CRC & Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}


 /**
  * @}
  */

/** @addtogroup ISO14443A_SplitFunctions
*  @brief  this set of function split CR95HF response in order to extract contacless tag field. 
*  @brief  Each command defined in ISO/IEC 14 443_A has its own split function. 
*  @{
*/

/**
 * @brief  this function splits ATQA response ( ex of CR95HF response 80 05 0400 280000)
 * @param 	ReaderResponse : 	pointer on CR95HF response
 * @param 	ProprietaryCoding : Proprietary coding (4 bits)
 * @param 	UIDsizeframe : UID size bit frame (2  bits) (0 for single UID size, 1 for double UID size, 2 for triple UID size)
 * @param 	BitFrameAnticol : Bit frame anticollision ( 5 bits) 
 * @param 	CR95HFControlByteIndex : index on control byte of CR95Hf response (3 bytes)
 * @retval 	RESULTOK : the functionbas been succesful executed 
 * @retval 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t ISO14443A_SplitATQA( uint8_t *ReaderResponse, uint8_t *ProprietaryCoding, uint8_t *UIDsizeframe, uint8_t *BitFrameAnticol,uint8_t *CR95HFControlByteIndex)
{
uint8_t	ResultCode,
		NbTagReplyByte,
		NbControlByte,
		TagReplyIndex,
		ControlIndex;

	// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443A,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	
	//  CR95HF returned an error code
	if (ResultCode == CR95HF_ERROR_CODE)
		return CR95HF_ERROR_CODE;	
	
	*ProprietaryCoding = ReaderResponse[TagReplyIndex] & ISO14443A_MASK_PROPRIETARYCODING;
	*UIDsizeframe = (ReaderResponse[TagReplyIndex+1] & ISO14443A_MASK_UIDSIZEBITFRAME) >> ISO14443A_OFFSET_UIDSIZEBITFRAME;
	*BitFrameAnticol = (ReaderResponse[TagReplyIndex+1] & ISO14443A_MASK_BITFRAMEANTICOL);
	
	*CR95HFControlByteIndex	= TagReplyIndex + NbTagReplyByte;

	return RESULTOK;
}

 /**
  * @}
  */

/** @addtogroup ISO14443A_IsFunctions
*  @{
*/

/**
 * @brief  this function returns RESULTOK if the bit "Collosion detected" of control byte is set, ERRORCODE_GENERIC otherwise
 * @param 	CR95HFControlByteIndex : pointer on CR95HF control byte (three bytes). CR95HF add three bytes at the end of its response.
 * @retval 	RESULTOK : Collosion detected bit is set
 * @retval 	ERRORCODE_GENERIC : Collosion detected bit is reset
 */
int8_t ISO14443A_IsCollisionDetected(const uint8_t *CR95HFControlByteIndex)
{
	if ((CR95HFControlByteIndex[ISO14443A_INDEX_CONTROLBYTE_GENERIC] & ISO14443A_MASK_COLLISIONBIT) !=0)
		return RESULTOK;
	else 
		return ERRORCODE_GENERIC; 
}

/**
 * @brief  this function returns RESULTOK if the bit "CRC Error" of control byte is set, ERRORCODE_GENERIC otherwise
 * @param 	CR95HFControlByteIndex : pointer on CR95HF control byte (three bytes). CR95HF add three bytes at the end of its response.
 * @retval 	RESULTOK : "CRC Error" bit is set
 * @retval 	ERRORCODE_GENERIC : "CRC Error" bit is reset
 */
int8_t ISO14443A_IsCRCError(const uint8_t *CR95HFControlByteIndex)
{
	if ((CR95HFControlByteIndex[ISO14443A_INDEX_CONTROLBYTE_GENERIC] & ISO14443A_MASK_CRCBIT) !=0)
		return RESULTOK;
	else 
		return ERRORCODE_GENERIC; 
}


/**
 * @brief  this function returns RESULTOK if the bit "Parity Error" of control byte is set, ERRORCODE_GENERIC otherwise
 * @param 	CR95HFControlByteIndex : pointer on CR95HF control byte (three bytes). CR95HF add three bytes at the end of its response.
 * @retval 	RESULTOK : "Parity Error" bit is set
 * @retval 	ERRORCODE_GENERIC : "Parity Error" bit is reset
 */
int8_t ISO14443A_IsParityError(const uint8_t *CR95HFControlByteIndex)
{
	if ((CR95HFControlByteIndex[ISO14443A_INDEX_CONTROLBYTE_GENERIC] & ISO14443A_MASK_PARITYBIT) !=0)
		return RESULTOK;
	else 
		return ERRORCODE_GENERIC; 
}

/**
 * @brief  	this function returns RESULTOK if Contacless Tag is compliant with  ISO/IEC 14443-4, ERRORCODE_GENERIC otherwise. 
 * @brief  	This check is carry out on SAK byte.
 * @param 	SAKbyte : Contacless tag response to Select command
 * @retval 	RESULTOK : Contacless tag is compatible with ISO/IEC 14443-4 
 * @retval 	ERRORCODE_GENERIC : Contacless tag isn't compatible with ISO/IEC 14443-4 
 */
int8_t ISO14443A_Is14443_4Compatible(const uint8_t SAKbyte)
{
	if ((SAKbyte & ISO14443A_MASK_14443_4_COMPATIBLE) == ISO14443A_SAK_14443_4_COMPATIBLE)
		return RESULTOK;
	else 
		return ERRORCODE_GENERIC; 
}

/**
 * @brief  	this function returns RESULTOK if UID is complete, ERRORCODE_GENERIC otherwise. 
 * @brief  	This check is carry out on SAK byte.
 * @param 	SAKbyte : Contacless tag response to Select command
 * @retval 	RESULTOK : UID is complete. the anticollision loop is over
 * @retval 	ERRORCODE_GENERIC : UID isn't complete
 */
int8_t ISO14443A_IsUIDComplete(const uint8_t SAKbyte)
{
	if ((SAKbyte & ISO14443A_MASK_SAK_UIDNOTCOMPLETE) == ISO14443A_SAK_UIDNOTCOMPLETE)
		return ERRORCODE_GENERIC;
	else 
		return RESULTOK; 
}

/**
 * @brief  	this function returns RESULTOK if UID is a 14 443_A is present into RF field, ERRORCODE_GENERIC otherwise. 
 * @retval 	RESULTOK : A 14 4443_4 contacless tag is present
 * @retval 	ERRORCODE_GENERIC :  no 14 4443_4 contacless tag available
 */
int8_t ISO14443A_IsPresent ( void )
{
uint8_t pResponse [ISO14443A_ANSWER_MAXNBBYTE];
int8_t	status;

	errchk (ISO14443A_ProtocolSelect( ISO14443A_TXVALUE_106K, 
																		ISO14443A_RXVALUE_106K,
																		pResponse));
	delay_ms(10);
	errchk ( ISO14443A_REQA( pResponse ));

	return RESULTOK;
Error: 
	return ERRORCODE_GENERIC;
}

/**
 * @brief  	this function returns RESULTOK if XOr of Data is egual to BCC, ERRORCODE_GENERIC otherwise 
 * @param 	NbByte : NbByte of Data
 * @param 	Data : Data to check (without BCC)
  * @param 	BCC : BCC byte of contacless tag response
 * @retval 	RESULTOK : BCC  = XOR (Data)
 * @retval 	ERRORCODE_GENERIC :  BCC  != XOR (Data)
 */
int8_t ISO14443A_IsCorrectBCC ( uint8_t NbByte,uint8_t *Data, uint8_t BCC )
{
uint8_t Xor=0,
		i=0;

	for (i=0;i<NbByte;i++)
		Xor	^= Data[i];
	
	if ((Xor	^ BCC) ==0)	
		return RESULTOK;
	else 
		return ERRORCODE_GENERIC;
}


int8_t ISO14443A_IsCIDSupport(uint8_t TCByte)
{
	if((TCByte & ISO14443A_MASK_ATS_TC_CID)== ISO14443A_MASK_ATS_TC_CID)
		return RESULTOK;
	else
		return ERRORCODE_GENERIC;
}

int8_t ISO14443A_IsPPSsuppored(const uint8_t T0Byte)
{
	if((T0Byte & ISO14443A_MASK_ATS_TA)== ISO14443A_MASK_ATS_TA)
		return RESULTOK;
	else
		return ERRORCODE_GENERIC;
}

int8_t ISO14443A_IsPPSChange(const uint8_t PPSByte)
{
	if((PPSByte & ISO14443A_MASK_ATS_TA_PPSCHANGE)== ISO14443A_MASK_ATS_TA_PPSCHANGE)
		return ERRORCODE_GENERIC;
	else
		return RESULTOK;
}


 /**
  * @}
  */

/** @addtogroup ISO14443A_GetFunctions
*  @{
*/


/**
 * @brief  	this function returns the number of significant bit of the first byte. This information is extracted from Control byte of CR95HF 
 * @param 	CR95HFControlByteIndex : pointer on CR95HF control byte (three bytes). CR95HF add three bytes at the end of its response.
 * @retval 	number of significant bit of the first byte
  */
int8_t ISO14443A_GetSignificantBit (const uint8_t *CR95HFControlByteIndex)
{
	return (CR95HFControlByteIndex[ISO14443A_INDEX_CONTROLBYTE_GENERIC] & ISO14443A_MASK_SIGNIFICANTBITS);
}

/**
 * @brief  	this function returns the index of the first byte where the collision is detected. This information is extracted from Control byte of CR95HF 
 * @param 	CR95HFControlByteIndex : pointer on CR95HF control byte (three bytes). CR95HF add three bytes at the end of its response.
 * @retval 	index of the first byte where the collision is detected
  */
int8_t ISO14443A_GetFirstCollisionByte (const uint8_t *CR95HFControlByteIndex)
{
	return (CR95HFControlByteIndex[ISO14443A_INDEX_CONTROLBYTE_FIRSTBYTECOLLISION] & ISO14443A_MASK_FIRSTBYTECOLLISION);
}

/**
 * @brief  	this function returns the index of the first bit where the collision is detected. This information is extracted from Control byte of CR95HF 
 * @param 	CR95HFControlByteIndex : pointer on CR95HF control byte (three bytes). CR95HF add three bytes at the end of its response.
 * @retval 	index of the first bit where the collision is detected
  */
int8_t ISO14443A_GetFirstCollisionBit (const uint8_t *CR95HFControlByteIndex)
{
	return (CR95HFControlByteIndex[ISO14443A_INDEX_CONTROLBYTE_FIRSTBITCOLLISION] & ISO14443A_MASK_FIRSTBITCOLLISION);
}


 /**
  * @}
  */

/** @addtogroup ISO14443A_AdvancedFunctions
*  @{
*/

/**
 * @brief  	this function runs a Select sequence as defined in ISO/IEC 14443_3 document.
 * @param 	SAKByte : lasr SAK byte received from contacless tag duting Select sequence
 * @retval 	RESULTOK : the select sequence is completed 
 * @retval 	ERRORCODE_GENERIC :  an error occured during select sequence
 */
int8_t ISO14443A_SelectSequence ( uint8_t *SAKByte )
{
uint8_t pResponse [ISO14443A_ANSWER_MAXNBBYTE];
int8_t  status;

	*SAKByte = 0;
	errchk (ISO14443A_ProtocolSelect( 	ISO14443A_TXVALUE_106K , 
										ISO14443A_RXVALUE_106K,
										pResponse));
	
	status = ISO14443A_REQA( pResponse );
	if (status != RESULTOK )
	{
		errchk(ISO14443A_REQA( pResponse ));
	}

	// === Select cascade level 1 ===
	errchk(ISO14443A_AntiCollisionLevel1 ( pResponse ));
	errchk(ISO14443A_SelectLevel1( ISO14443A_NBBYTE_SELECT, &pResponse[ISO14443A_OFFSET_UID_SELECT],  pResponse ));
	if (ISO14443A_IsUIDComplete(pResponse[ISO14443A_OFFSET_SAKBYTE])== RESULTOK)
	{	*SAKByte = pResponse[ISO14443A_OFFSET_SAKBYTE];
		return RESULTOK;
	}

	// === Select cascade level 2 ===
	errchk(ISO14443A_AntiCollisionLevel2 ( pResponse ));
	errchk(ISO14443A_SelectLevel2( ISO14443A_NBBYTE_SELECT, &pResponse[ISO14443A_OFFSET_UID_SELECT],  pResponse ));
	if (ISO14443A_IsUIDComplete(pResponse[ISO14443A_OFFSET_SAKBYTE])== RESULTOK)
	{	*SAKByte = pResponse[ISO14443A_OFFSET_SAKBYTE];
		return RESULTOK;
	}

   // === Select cascade level 3 ===
	errchk(ISO14443A_AntiCollisionLevel3 ( pResponse ));
	errchk(ISO14443A_SelectLevel3( ISO14443A_NBBYTE_SELECT, &pResponse[ISO14443A_OFFSET_UID_SELECT],  pResponse ));
	if (ISO14443A_IsUIDComplete(pResponse[ISO14443A_OFFSET_SAKBYTE])== RESULTOK)
	{	*SAKByte = pResponse[ISO14443A_OFFSET_SAKBYTE];
		return RESULTOK;
	}
    
	return ERRORCODE_GENERIC;

Error: 
	return ERRORCODE_GENERIC;
}


/**
 * @brief  	this function emit a REQA command to a contacless tag and returns UID size 
 * @param 	UIDsize : (2 bits) value available 0 for single, 1 for double,  2 for triple and 3 for RFU
 * @retval 	RESULTOK : the function is succesful
 * @retval 	ERRORCODE_GENERIC :  the function failed
 */
int8_t ISO14443A_GetUIDsize ( uint8_t *UIDsize )
{
uint8_t pResponse [ISO14443A_ANSWER_MAXNBBYTE];
int8_t  status;

	errchk (ISO14443A_ProtocolSelect( 	ISO14443A_TXVALUE_106K , 
										ISO14443A_RXVALUE_106K,
										pResponse));
	
	status  = ISO14443A_REQA( pResponse );
	if (status != RESULTOK )
	{
		errchk(ISO14443A_REQA( pResponse ));
	}

	*UIDsize = (pResponse[ISO14443A_OFFSET_ATQAFIRSTBYTE] >> ISO14443A_OFFSET_UIDSIZE) & ISO14443A_MASK_UIDSIZE;

	return 	RESULTOK;

Error: 
	return ERRORCODE_GENERIC;

}



/**
 * @brief  	this function returns UID of a contacless tag 
 * @param 	SAKByte : SAK byte
 * @param 	NbUIDByte : NbByte of UID
 * @param 	UIDout : last SAK byte received from contacless tag duting Select sequence
 * @retval 	RESULTOK : the select sequence is completed 
 * @retval 	ERRORCODE_GENERIC :  an error occured during select sequence
 * @retval 	ISO14443A_ERRORCODE_BCC :  a BCC byte are erroneous. the received data from Contacless tag was corupted.
 */
int8_t ISO14443A_GetUID ( uint8_t *SAKByte, uint8_t *NbUIDByte, uint8_t *UIDout )
{
uint8_t pResponse [ISO14443A_ANSWER_MAXNBBYTE],
		UIDsize;
int8_t  status;

	*SAKByte = 0;
	*NbUIDByte = 0;
	// UIDsize : (2 bits) value available 0 for single, 1 for double,  2 for triple and 3 for RFU
	errchk(ISO14443A_GetUIDsize ( &UIDsize ));
	
	// === Select cascade level 1 ===
	errchk(ISO14443A_AntiCollisionLevel1 ( pResponse ));
	// Check BCC 
	if (ISO14443A_IsCorrectBCC ( 4 ,&pResponse[CR95HF_DATA_OFFSET],  pResponse[CR95HF_DATA_OFFSET + 4 ] ) != RESULTOK)
		return ISO14443A_ERRORCODE_BCC	;
			
	// copy UID from CR95Hf response
	if (UIDsize == ISO14443A_ATQA_UID_SINGLESIZE)
		memcpy(UIDout,&pResponse[CR95HF_DATA_OFFSET],ISO14443A_NBBYTE_UIDSINGLE );
	else 
		memcpy(UIDout,&pResponse[CR95HF_DATA_OFFSET+1],ISO14443A_NBBYTE_UIDSINGLE-1 );

	//emit Select command	
	errchk(ISO14443A_SelectLevel1( ISO14443A_NBBYTE_SELECT, &pResponse[ISO14443A_OFFSET_UID_SELECT],  pResponse ));
	if (ISO14443A_IsUIDComplete(pResponse[ISO14443A_OFFSET_SAKBYTE]) == RESULTOK)
	{	*NbUIDByte = ISO14443A_NBBYTE_UIDSINGLE;	
		*SAKByte = pResponse[ISO14443A_OFFSET_SAKBYTE];	
		return RESULTOK;
	}
	// === Select cascade level 2 ===
	errchk(ISO14443A_AntiCollisionLevel2 ( pResponse ));
	// Check BCC 
	if (ISO14443A_IsCorrectBCC ( 4 ,&pResponse[CR95HF_DATA_OFFSET],  pResponse[CR95HF_DATA_OFFSET + 4 ] ) != RESULTOK)
		return ISO14443A_ERRORCODE_BCC	;

	if (UIDsize == ISO14443A_ATQA_UID_DOUBLESIZE)
		memcpy(&(UIDout[ISO14443A_NBBYTE_UIDSINGLE-1]),&pResponse[CR95HF_DATA_OFFSET],ISO14443A_NBBYTE_UIDSINGLE );
	else 
		memcpy(&(UIDout[ISO14443A_NBBYTE_UIDSINGLE-1]),&pResponse[CR95HF_DATA_OFFSET+1],ISO14443A_NBBYTE_UIDSINGLE-1 );

	//emit Select command	 
	errchk(ISO14443A_SelectLevel2( ISO14443A_NBBYTE_SELECT, &pResponse[ISO14443A_OFFSET_UID_SELECT],  pResponse ));
	if (ISO14443A_IsUIDComplete(pResponse[ISO14443A_OFFSET_SAKBYTE])== RESULTOK)
	{	*NbUIDByte = ISO14443A_NBBYTE_UIDDOUBLE;
		*SAKByte = pResponse[ISO14443A_OFFSET_SAKBYTE];
		return RESULTOK;
	}
   // === Select cascade level 3 ===
	errchk(ISO14443A_AntiCollisionLevel3 ( pResponse ));
	// Check BCC 
	if (ISO14443A_IsCorrectBCC ( 4 ,&pResponse[CR95HF_DATA_OFFSET],  pResponse[CR95HF_DATA_OFFSET + 4 ] ) != RESULTOK)
		return ISO14443A_ERRORCODE_BCC	;
	if (UIDsize == ISO14443A_ATQA_UID_TRIPLESIZE)	
		memcpy(&(UIDout[ISO14443A_NBBYTE_UIDDOUBLE-1]),&pResponse[CR95HF_DATA_OFFSET],ISO14443A_NBBYTE_UIDSINGLE );
	
   	//emit Select command	
	errchk(ISO14443A_SelectLevel3( ISO14443A_NBBYTE_SELECT, &pResponse[ISO14443A_OFFSET_UID_SELECT],  pResponse ));
	if (ISO14443A_IsUIDComplete(pResponse[ISO14443A_OFFSET_SAKBYTE])== RESULTOK)
	{	*NbUIDByte = ISO14443A_NBBYTE_UIDTRIPLE;
		*SAKByte = pResponse[ISO14443A_OFFSET_SAKBYTE];
		return RESULTOK;
	}
    
	return ERRORCODE_GENERIC;

Error: 
	return ERRORCODE_GENERIC;
}

/**
 * @brief  	this function send Block data 
 * @param 	PCBByte : Protocol control byte
 * @param 	CIDByte : Card IDentifier
 * @param 	NADByte : Node ADdress
* @param		Playload : Data send
 * @retval 	RESULTOK : the send Block Data is completed 
 * @retval 	ERRORCODE_GENERIC :  an error occured during select sequence
 * @retval 	ISO14443A_ERRORCODE_BCC :  a BCC byte are erroneous. the received data from Contacless tag was corupted.
 */

int8_t ISO14443_4A_SendBlock(uint8_t PCBByte, uint8_t CIDByte, uint8_t NADByte, uint8_t *Playload, uint8_t NBPlayload, uint8_t FSC)
{
	uint8_t pResponse [256];
	uint8_t DataToSend[256],
	NthByte,i = 0;
	int8_t  status;
	PCBByte &= ~ISO14443_4A_PCB_IBLOCK_BLOCKNUMBER;
	if((PCBByte & ISO14443_4A_PCB_IBLOCK)== ISO14443_4A_PCB_IBLOCK)
	{
		PCBByte |= ISO14443_4A_PCB_IBLOCK_CHAINING;
	}
	Sendnext:
	NthByte=0;
	DataToSend[NthByte++] = PCBByte;
	if(PCBByte & ISO14443_4A_PCB_IBLOCK_CID)
	{
		DataToSend[NthByte++] = CIDByte;
	}
	if(PCBByte & ISO14443_4A_PCB_IBLOCK_NAD)
	{
		DataToSend[NthByte++] = NADByte;
	}
	for(;NthByte<FSC;NthByte++,i++)
	{
		if(i>= NBPlayload)
		{
			break;
		}
		DataToSend[NthByte] = Playload[i];	
	}
//	if((PCBByte & ISO14443_4A_PCB_IBLOCK)== ISO14443_4A_PCB_IBLOCK)
//	{
//		if(i>=NBPlayload)
//		{
//			DataToSend[0] &= ~ISO14443_4A_PCB_IBLOCK_CHAINING;
//		}
//	}
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8 | ISO14443A_APPENDCRC;
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	if(i<NBPlayload)
	{
		PCBByte ^= ISO14443_4A_PCB_IBLOCK_BLOCKNUMBER;
		goto Sendnext;
	}
	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}

int8_t Authentication_Tag(int8_t Command_key,int8_t Add,uint8_t* Key,uint8_t* UID)
{
	uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE];
	uint8_t pResponse [ISO14443A_ANSWER_MAXNBBYTE],
	NthByte=0;
	uint8_t i;
	int8_t 	status;

	// add command code
	memset(DataToSend, 0,sizeof(DataToSend));	
	memset(pResponse, 0,sizeof(pResponse));
	DataToSend[NthByte++] = Command_key;
	DataToSend[NthByte++] = Add;
	for(i=0;i<6;i++)
	{
		DataToSend[NthByte++] = Key[i];
	}
	for(i=0;i<4;i++)
	{
		DataToSend[NthByte++] = UID[i];
	}
	
	// append the command control byte 0x08 ( Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8|ISO14443A_APPENDCRC ;
		
	// send command to contacless tag through CR95HF device and received its response
// 	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
// 	NthByte=0;
// 	for(i=0;i<4;i++)
// 	{
// 		DataToSend[NthByte++] = UID[i];
// 	}
// 	for(i=0;i<4;i++)
// 	{
// 		DataToSend[NthByte++] = pResponse[i+2]^0xFF;
// 	}
// 	DataToSend[NthByte++] = ISO14443A_NBSIGNIFICANTBIT_8|ISO14443A_APPENDCRC;
	memset(pResponse, 0,sizeof(pResponse));
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}
 /**
  * @}
  */
int8_t ISO14443A_Read(int8_t Add, uint8_t *Data,int8_t leght)
{
	uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE];
	uint8_t pResponse [ISO14443A_ANSWER_MAXNBBYTE],
	NthByte=0;
	int8_t 	status;

	// add command code
	//DataToSend[NthByte++] = ISO14443A_CMDCODE_READ;
	DataToSend[NthByte++] = 78;
	//DataToSend[NthByte++] = Add+leght;
	// append the command control byte 0x08 ( Number of significant bit = 8)
	DataToSend[NthByte++] = ISO14443A_APPENDCRC|ISO14443A_NBSIGNIFICANTBIT_8 ;
		
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
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
