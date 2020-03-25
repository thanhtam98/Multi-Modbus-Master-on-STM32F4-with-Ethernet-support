#include "rfid_reader.h"
#include "lib_iso14443A.h"
#include "lib_iso14443B.h"
#include "lib_SRIX4k.h"
#include "hw_config.h"
#include "miscellaneous.h"
//const uint8_t BITMASK[7] = {0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};

extern CR95HF_INTERFACE		CR95HF_Interface;
extern uint8_t 	ReaderRecBuf[MAX_BUFFER_SIZE+3]; 	// buffer for SPI ou UART reception

int8_t GetControlBytes(uint8_t *ReaderResponse, uint8_t *CR95HFControlByteIndex);
int8_t User_AntiCollision(uint8_t CascadeLevel, uint8_t *pResponse , uint8_t userNVB, uint8_t flagByte);

/**  
* @brief 	this function intialiezes either UART or SPI serial interface of MCU to communication with CR95HF
* @param  	none 	
* @retval 	none
*/
void Rfid_SerialInterfaceInitialization (ReaderConfigStruct ReaderConfig)
{

	if (ReaderConfig.Interface == CR95HF_INTERFACE_UART)
	{
		CR95HF_Interface = CR95HF_INTERFACE_UART;
		UART_Initialization(BAUDRATE_DATARATE_DEFAULT );	 
	}
	else if (ReaderConfig.Interface == CR95HF_INTERFACE_SPI)
	{
		CR95HF_Interface = CR95HF_INTERFACE_SPI;
		ReaderConfig.Interface = CR95HF_INTERFACE_SPI;
		SPI_Initialization( );
		// -- EXTI configuration if the polling mode is not selected
		if (ReaderConfig.SpiMode == SPI_INTERRUPT) //SPI_POLLING_MODE 
		{	// -- An EXTI must be set to catch the IRQ (a edge on the UART RX pin) from the CR95HF
			EXTI_Config( );
		}	
	}
}

/**  
* @brief  	this function returns RESULTOK if an CR95HF is available
* @param  	none 	
* @retval 	status function
*/
int8_t User_IsCR95HF (void)
{
int8_t NbAttempt =0;

	do{
		delay_ms(50);
		CR95HF_Echo(ReaderRecBuf);
	}	while (ReaderRecBuf[0] !=ECHORESPONSE && NbAttempt++ <10);
	
	if (ReaderRecBuf[0] ==0x55)
		return RESULTOK;
	else	
		return ERRORCODE_GENERIC;	
}
/**  
* @brief 	this function read data block from tag
* @param  	pointer for data return
* @retval 	none
*/

int8_t Read_data(uint8_t *pResponse)
{
	uint8_t DataToSend[ISO14443A_CMD_MAXNBBYTE],NthByte=0;
	int8_t 	status;

	// add command code
	DataToSend[NthByte++] = ISO14443A_CMDCODE_READ;
	// append the command control byte 0x07 ( Number of significant bit = 7)
	DataToSend[NthByte++] = 0;
	
	// send command to contacless tag through CR95HF device and received its response
	errchk(CR95HF_SendRecv(NthByte,DataToSend,pResponse));
	return RESULTOK;

Error:
	return CR95HF_ERROR_CODE;
}

/**  
* @brief  this function return control byte index of CR95Hf response
* @param  ReaderResponse : pointer on CR95HF response
* @param  CR95HFControlByteIndex : index of control byte of CR95Hf response (3 bytes)
* @retval RESULTOK : the function has been succesfully executed 
* @retval CR95HF_ERROR_CODE : CR95Hf returned an error code
*/
int8_t GetControlBytes(uint8_t *ReaderResponse, uint8_t *CR95HFControlByteIndex)
{
uint8_t	ResultCode,
			NbTagReplyByte,
			NbControlByte,
			TagReplyIndex,
			ControlIndex;
//int i;
// extracts contacless tag response from reader response
	SplitReaderReply (	SEND_RECEIVE,
						PROTOCOL_TAG_ISO14443A,
						ReaderResponse,
						&ResultCode,
						&NbTagReplyByte,
						&TagReplyIndex,
						&NbControlByte,
						&ControlIndex);
	if (ResultCode == CR95HF_ERROR_CODE)
		return CR95HF_ERROR_CODE;
	*CR95HFControlByteIndex	= TagReplyIndex + NbTagReplyByte;

	return RESULTOK;
}
/**  
* @brief 	this function sends a REQB command and display the result on LCD screen	
* @param  	none 	
* @retval 	none
*/
int8_t User_Get14443ATag (uint8_t* UIDout,uint8_t* NbUIDByte )
{
	int8_t  status;
	uint8_t i;
	uint8_t	SAKByte;
	#define FSDI 8
	uint8_t pResponse [256+5];
	uint8_t CID = 0;
	uint8_t PPS0,PPS1;
	uint32_t SFGT =1;
	ATS_FRAMEWORK ATS_framework;
	
	if(ISO14443A_GetUID(&SAKByte,NbUIDByte,UIDout)==RESULTOK)
	{
		errchk(ISO14443A_Is14443_4Compatible(SAKByte));
		errchk(ISO14443A_RATS(FSDI,CID,pResponse));
		memset(&ATS_framework.TL,0,sizeof(ATS_framework));
		memcpy(&ATS_framework.TL,&pResponse[CR95HF_DATA_OFFSET],pResponse[CR95HF_DATA_OFFSET]+1);
		//errchk(ISO14443A_IsCIDSupport(ATS_framework.TC));
		for(i=0;i<(ATS_framework.TB & ISO14443A_MASK_ATS_TB_SFGI);i++)
		{
			SFGT *=2;
		}
		SFGT *= (256*16/106);
//		if(ISO14443A_IsPPSsuppored(ATS_framework.T0)==RESULTOK)
//		{
//			if(ISO14443A_IsPPSChange(ATS_framework.TA)==RESULTOK)
//			{
//				errchk(ISO14443A_PPS(CID,PPS0,PPS1,pResponse));
//			}
//		}
		/* Send block information*/
		ISO14443_4A_SendBlock(ISO14443_4A_PCB_IBLOCK|ISO14443_4A_PCB_IBLOCK_CID,CID,NULL,"123456",6,255);
Error:
		errchk(ISO14443A_HLTA(pResponse));
		return RESULTOK;		
	}
	return ERRORCODE_GENERIC;
}

/**  
* @brief 	this function sends a REQB command and display the result on LCD screen	
* @param  	none 	
* @retval 	none
*/
int8_t User_Get14443BTag (void)
{
int8_t		status=0;


	// send a ReqB command to detect a 14443_B contacless tag
		status = ISO14443B_IsPresent ();
		/* ===  Display "SR card detected " ===*/		   
		if (status == RESULTOK)
		{
		}
		return status;
}
/**  
* @brief 	this function handles a SR contacless tag and disply its UID.
* @param  	none 	
* @retval 	none
*/
int8_t User_Get14443SRTag (void)
{
int8_t		status=0;
uint8_t 	UIDfield[SRIX4K_SRIX4K_UID_NBBYTE];

	// send an initiate command to detect a SRi product
	status = SRIX4K_IsPresent ();	
	
	// If a SRI product detected, checks if it's a SRIX4k
	if (status == RESULTOK)
	{
		// Get UID of a tag  
		status = SRIX4K_GetUIDadvanced( UIDfield );
		/* ===  Display Contacless tag UID or No Reply === */
	}
	return status;
}

