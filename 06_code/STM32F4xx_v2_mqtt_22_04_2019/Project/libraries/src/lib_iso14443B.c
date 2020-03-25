/**
  ******************************************************************************
  * @file    lib_iso14443B.c
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
#include "lib_iso14443B.h"
#include "string.h"
#include "stdio.h"
#include "hw_config.h" 
#include "miscellaneous.h"

/** @addtogroup Libraries
 * 	@{
 */

 /** @addtogroup ISO14443_B
 * 	@{
 */


/** @addtogroup ISO14443B_Functions
*  @{
*/

/** @addtogroup ISO14443B_ProcolSelectFunction
*  @{
*/

   /**
 * @brief  this function sends a ProtocolSelect command to CR95HF device in order to select ISO14443B protocol
 * @param  	TxDataRate (1 byte): transmission data rate (CR95HF -> Contactless tag) 
 * @param  	RxDataRate (1 byte): reception data rate (Contactless tag -> CR95HF) 
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful send to CR95HF device. 
 * @retval 	ISO14443B_ERRORCODE_PARAMETER : One parameter is erroneous 
 * @retval 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t ISO14443B_ProtocolSelect(const uint8_t TxDataRate, uint8_t RxDataRate,uint8_t *pResponse)
{
uint8_t Parameters = 0x00;
int8_t 	status;

	*pResponse = SENDRECV_ERRORCODE_SOFT;
	*(pResponse+1) = 0x00;
	// checks the parameter values
	if (TxDataRate>ISO14443B_TXVALUE_847K)
		return ISO14443B_ERROR_PARAMETERVALUE;
	if (RxDataRate>ISO14443B_RXVALUE_847K)
		return ISO14443B_ERROR_PARAMETERVALUE;		

	// create the parameter byte of ProtocolSelect command
	Parameters = 	( (TxDataRate<<ISO14443B_OFFSET_TXDATARATE) & ISO14443B_MASK_TXDATARATE) |
					( (RxDataRate<<ISO14443B_OFFSET_RXDATARATE) & ISO14443B_MASK_RXDATARATE);

	// the CR95HF will manage CRC16 of RF command 
	Parameters |= ISO14443B_PROTOCOLSELECT_APPENDCRC;

	// send ProtocolSelect to CR95HF to configure it
	errchk(CR95HF_ProtocolSelect(	ISO14443B_PROTOCOLSELECT_NBBYTE,
							PROTOCOL_TAG_ISO14443B,
							&Parameters,
							pResponse));

	// checks CR95HF response
	if (CR95HF_IsReaderResultCodeOk (PROTOCOL_SELECT,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;
	
	return RESULTOK;
Error:
	return ISO14443B_ERROR_PARAMETERVALUE;
}

/**
  * @}
  */ 

  /** @addtogroup ISO14443B_SetFunctions
*  @{
*/


/**
 * @brief  this function returns PARAM byte 
 * @param 	ExtentedBit : 	indicated the PCD capability to support extended ATQB response
 * @param 	REQBWUPBbit :  one bit to define either Reqb (0) or WupB (1)
 * @param 	NumberOfSlots : number of slots 
 * @retval 	PARAM : PARAM byte
 */
int8_t ISO14443B_SetParamByte(const uint8_t ExtentedBit,const uint8_t REQBWUPBbit,const uint8_t NumberOfSlots )
{
int8_t ParamByte = 0;

  	ParamByte = ( 	(( NumberOfSlots << ISO14443B_OFFSET_NUMBEROFSLOTS) & ISO14443B_MASK_NUMBEROFSLOTS) |
					(( REQBWUPBbit  	<< ISO14443B_OFFSET_REQBATQB	)  & ISO14443B_MASK_REQBATQB ) |
					(( ExtentedBit 	<< ISO14443B_OFFSET_EXTENDEDBIT )  & ISO14443B_MASK_EXTENDEDBIT) );
	return ParamByte;

}

/**
  * @}
  */ 

  /** @addtogroup ISO14443B_CommandFunctions
*  @brief  this set of function send a command defined ISO/IEC 14443-3 document
*  @{
*/


/**
 * @brief  this function emits a ReqB command to a contactless tag
 * @param 	AFIin : 	AFI byte (application Family identifier) represents the type of application targeted
 * @param 	Param : 	PARAM byte
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443B_REQB(const uint8_t AFIin,const uint8_t Param, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443B_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443B_CMDCODE_REQB;
	DataToSend[NthByte++] = AFIin;
	// reset b4 
	DataToSend[NthByte++] = Param & ISO14443B_CMDCODE_REQB_BIT4;
		
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  this function emits a WupB command to a contactless tag
 * @param 	AFIin : 	AFI byte (application Family identifier) represents the type of application targeted
 * @param 	Param : 	PARAM bytee
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443B_WUPB(const uint8_t AFIin,const uint8_t Param, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443B_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443B_CMDCODE_WUPB;
	DataToSend[NthByte++] = AFIin ;
	// set b4
	DataToSend[NthByte++] = Param | ISO14443B_CMDCODE_WUPB_BIT4 ;
		
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  this function emits a Slot-Marker command to a contactless tag
 * @param 	SlotNumber : 	Slot number value (4 bits) between 1 and 15
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443B_SlotMarker(const uint8_t SlotNumber, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443B_CMD_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	errchk(CHECKVAL(SlotNumber,ISO14443B_SLOTNUMBER_MIN,ISO14443B_SLOTNUMBER_MAX));
	// add command code
	DataToSend[NthByte++] = ISO14443B_CMDCODE_SLOTNUMBER | 
							( SlotNumber << ISO14443B_OFFSET_SLOTNUMBER) & ISO14443B_MASK_SLOTNUMBER;

	// send command to contacless tag through CR95HF device and received its response
	if (CR95HF_SendRecv(NthByte,DataToSend,pResponse)!=RESULTOK)
		return CR95HF_ERROR_CODE;

	return RESULTOK;
Error:
	return ISO14443B_ERROR_PARAMETERVALUE;
}

/**
 * @brief  this function emits an Attrib command to a contactless tag
 * @param 	Identifier :  pointer on identifier parameter (4 bytes). It's  the value off the PUPI send by Contacless tag in the ATQB
 * @param 	Parameters :  pointer on Parameters parameter (4 bytes).
 * @param 	NbByteHigherlayerData : Number of byte of HigherLayersData parameter
 * @param 	HigherLayersData : pointer on Parameters parameter (NbByteHigherlayerData bytes) (Optional).
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443B_Attrib(const uint8_t *Identifier,const uint8_t *Parameters,const uint8_t NbByteHigherlayerData,const uint8_t *HigherLayersData, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443B_ATTRIB_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	if (NbByteHigherlayerData > ISO14443B_CMD_HIGHERLAYERDATA)
		return ISO14443B_ERROR_PARAMETERLENGTH;	

	// add command code
	DataToSend[NthByte++] = ISO14443B_CMDCODE_ATTRIB;

	memcpy(&(DataToSend[NthByte]),Identifier,ISO14443B_NBBYTE_IDENTIFIER);
	NthByte += ISO14443B_NBBYTE_IDENTIFIER;
	memcpy(&(DataToSend[NthByte]),Parameters,ISO14443B_NBBYTE_PARAMETERS);
	NthByte += ISO14443B_NBBYTE_PARAMETERS;

	if (NbByteHigherlayerData > 0)
	{
		memcpy(&(DataToSend[NthByte]),HigherLayersData,NbByteHigherlayerData);
		NthByte += NbByteHigherlayerData;
	}

	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}

/**
 * @brief  this function emits an HLTB command to a contactless tag
 * @param 	Identifier :  pointer on identifier parameter (4 bytes). It's  the value off the PUPI send by Contacless tag in the ATQB
 * @param 	pResponse : 	pointer on CR95HF response
 * @retval 	RESULTOK : the command has been succesful emited to CR95HF device.  
 * @retval 	CR95HF_ERROR_CODE : CR95HF returned an error code
 */
int8_t ISO14443B_HLTB(const uint8_t *Identifier, uint8_t *pResponse )
{
uint8_t DataToSend[ISO14443B_ATTRIB_MAXNBBYTE],
		NthByte=0;
int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443B_CMDCODE_HLTB;

	memcpy(&(DataToSend[NthByte]),Identifier,ISO14443B_NBBYTE_IDENTIFIER);
	NthByte += ISO14443B_NBBYTE_IDENTIFIER;
	
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));

	return RESULTOK;
Error:
	return CR95HF_ERROR_CODE;
}



 /**
  * @}
  */

/** @addtogroup ISO14443B_SplitFunctions
*  @brief  this set of function split CR95HF response in order to extract contacless tag field. 
*  @brief  Each command defined in ISO/IEC 14 443_A has its own split function. 
*  @{
*/

/**
 * @brief  	this function splits ATQB response 
 * @brief  	CR95HF response example : 80 0F 5077FE01B3000000000071718EBA 00
 * @brief  	with 80 : response ok & 0F : Nb byte
 * @brief  		 50 : first byte
 * @brief  		 77 FE 01 B3  : PUPI field
 * @brief  		 00 00 00 00  : Application data 	
 * @brief  		 00 71 71  : Protocol Info 	
 * @brief  		 8E BA  : CRCB	
 * @brief  		 00     : CRC95HF control byte ( CRC validated)
 * @param 	ReaderResponse : 	pointer on CR95HF response
 * @param 	PUPIindex : index of PUPI field (4 bytes)
 * @param 	ApplicationDataindex : index of ApplicationData field (4 bytes)
 * @param 	ProtocolInfoindex : index of Protocol field (3 or 4 bytes it's depends on Extended ATQBsupported bit (see REQB or WUPB command) )
 * @retval 	RESULTOK : the function has been succesful executed 
 * @retval 	CR95HF_ERROR_CODE : CR95Hf returned an error code
 */
int8_t ISO14443B_SplitATQB( uint8_t *ReaderResponse, uint8_t *PUPIindex, uint8_t *ApplicationDataindex, uint8_t *ProtocolInfoindex)
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
	
	*PUPIindex 				= TagReplyIndex ;
	*ApplicationDataindex 	= TagReplyIndex + ISO14443B_NBBYTE_PUPI;
	*ProtocolInfoindex 		= TagReplyIndex + ISO14443B_NBBYTE_PUPI + ISO14443B_NBBYTE_APPLICATIONDATA;
	
	return RESULTOK;
}

/**
 * @brief  	this function splits Application Data field 
 * @param 	ApplicatioDataField : 	Application Data field ( part of ATQB response) (4bytes)
 * @param 	AFIout : AFI byte
 * @param 	CRC_BAIDoutIndex : index of CRC field (2 bytes)
 * @param 	NumberOfAplication : Number of application byte
 * @retval 	RESULTOK : the function has been succesful executed 
  */
int8_t ISO14443B_SplitApplicationDataField ( const uint8_t *ApplicatioDataField, uint8_t *AFIout,uint8_t *CRC_BAIDoutIndex, uint8_t *NumberOfAplication)
{

	*AFIout 				= ApplicatioDataField[ISO14443B_OFFSET_AFI] ;
	*CRC_BAIDoutIndex		= ISO14443B_NBBYTE_AFI;
	*NumberOfAplication		= ApplicatioDataField[ISO14443B_NBBYTE_AFI + ISO14443B_NBBYTE_CRC] ;
		
	return RESULTOK;
}

/**
 * @brief  	this function splits ProtocolInfo field 
 * @param 	ProtocolInfoField : 	protocol info field ( part of ATQB response) (4 bytes)
 * @param 	BitRateCapability : bit rate supported by the contacless tag (8 bits)
 * @param 	MaxFrameSize : Max frame size (4 bits)
 * @param 	ProtocolType : (4 bits) b1 = 1 compliant with 14 443_4  b1 = 0 not compliant with 14 443_4. b2&b3 minimum TR2 coding b4: RFU
 * @param 	FWIbits : Frame waiting time integer (4 bits)
 * @param 	ADCbits : Application data coding (2 bits) (00b  : application is proprietary  01b : Application is coded as described in clause 7.9.3 (see ISO/IEC 14443_3 spec others: RFU)
 * @param 	FObits : Frame option (2 bits) (1xb : NAD supported x1b : CID supported)
 * @param 	SFGIbits : SFGI bits (4 bits) (optional it's depends on Extended ATQBsupported bit (see REQB or WUPB command))
 * @param 	RFUbits : RFU bits (4 bits) (optional it's depends on Extended ATQBsupported bit (see REQB or WUPB command))
 * @retval 	RESULTOK : the function has been succesful executed 
  */
int8_t ISO14443B_SplitProtocolInfoField ( const uint8_t *ProtocolInfoField, uint8_t *BitRateCapability,uint8_t *MaxFrameSize, uint8_t *ProtocolType, uint8_t *FWIbits, uint8_t *ADCbits, uint8_t *FObits, uint8_t *SFGIbits, uint8_t *RFUbits)
{

	*BitRateCapability 	= ProtocolInfoField[ISO14443B_OFFSET_BITRATECAPABILITY] ;
	*MaxFrameSize 		= (ProtocolInfoField[ISO14443B_OFFSET_MAXFRAMESIZE] & ISO14443B_MASK_MAXFRAMESIZE) >> ISO14443B_OFFSETBIT_MAXFRAMESIZE;
	*ProtocolType 		= (ProtocolInfoField[ISO14443B_OFFSET_PROTOCOLBYTE] & ISO14443B_MASK_PROTOCOLBYTE) >> ISO14443B_OFFSETBIT_PROTOCOLBYTE;
	*FWIbits 			= (ProtocolInfoField[ISO14443B_OFFSET_FWI] & ISO14443B_MASK_FWI) >> ISO14443B_OFFSETBIT_FWI;
	*ADCbits 			= (ProtocolInfoField[ISO14443B_OFFSET_ADC] & ISO14443B_MASK_ADC) >> ISO14443B_OFFSETBIT_ADC;
	*FObits 			= (ProtocolInfoField[ISO14443B_OFFSET_FO] & ISO14443B_MASK_FO) >> ISO14443B_OFFSETBIT_FO;
	*SFGIbits 			= (ProtocolInfoField[ISO14443B_OFFSET_SFGI] & ISO14443B_MASK_SFGI) >> ISO14443B_OFFSETBIT_SFGI;
	*RFUbits 			= (ProtocolInfoField[ISO14443B_OFFSET_RFU] & ISO14443B_MASK_RFU) >> ISO14443B_OFFSETBIT_RFU;

	return RESULTOK;
}


 /**
  * @}
  */

/** @addtogroup ISO14443A_IsFunctions
*  @{
*/



/**
 * @brief  	this function returns RESULTOK if UID is a 14 443_B is present into RF field, ERRORCODE_GENERIC otherwise. 
 * @retval 	RESULTOK : A 14 4443_B contacless tag is present
 * @retval 	ERRORCODE_GENERIC :  no 14 4443_4 contacless tag available
 */
int8_t ISO14443B_IsPresent ( void )
{
uint8_t pResponse [ISO14443B_REPLY_MAXNBBYTE],
		AFIin = 0 ,
		Param =0;
int8_t	status;

	errchk (ISO14443B_ProtocolSelect( 	ISO14443B_TXVALUE_106K , 
										ISO14443B_RXVALUE_106K,
										pResponse));
	delay_ms(10);
	errchk ( ISO14443B_REQB(  AFIin, Param,pResponse ));
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/

