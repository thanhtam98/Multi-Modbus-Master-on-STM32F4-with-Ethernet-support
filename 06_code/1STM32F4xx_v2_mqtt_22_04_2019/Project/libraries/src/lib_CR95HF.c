/**
  ******************************************************************************
  * @file    lib_CR95HF.c 
  * @author  MMY Application Team
  * @version V1.2
  * @date    01/07/2011
  * @brief   This file provides set of function to manage CR95HF device. 
  * @brief   The commands as defined in CR95HF datasheet
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------------------ */
#include "lib_CR95HF.h"
#include "hw_config.h"
#include "string.h"
#include "drv_spi.h"
#include "drv_uart.h"
#include "stm32f4_discovery.h"
#include "miscellaneous.h"

/** @addtogroup Libraries
 * 	@{
 */

/** @addtogroup CR95HF
 * 	@{
 */



/** @addtogroup CR95HF_Extern_Variables
 *  @{
 */

 /** @ brief Interuption state come from CR95HF  */
extern volatile bool							CR95HF_DataReady;
/** @ brief Interuption state of internal timeout  */
extern volatile bool							CR95HF_TimeOut;
/** @ brief serial inface choosed by user application  */
extern volatile CR95HF_INTERFACE				CR95HF_Interface;
/** @ brief configuration of CR95HF  */
extern ReaderConfigStruct			ReaderConfig;

/**
  * @}
  */ 

static const uint8_t MCUVERSION[MCUVERSION_LENGTH] =
{	MCUVERSION_FIRSTNUMBER,
  	MCUVERSION_SECONDNUMBER,
	MCUVERSION_THIRDNUMBER
};

static void CR95HF_Send_SPI_Command			(const uint8_t *pData);
static int8_t CR95HF_PollingCommand			(void);
static void CR95HF_Receive_SPI_Response		(uint8_t *pData);
static void CR95HF_Send_UART_Command		(const uint8_t *pData);
static int8_t GetNbControlByte 				(int8_t ProtocolSelected);

static uint8_t IsAnAvailableProtocol 		(uint8_t Protocol);
static uint8_t IsAnAvailableSelectLength 	(const uint8_t Protocol,const uint8_t Length);
static uint8_t IsAnAvailableSelectParameters (const uint8_t Protocol,const uint8_t Length,const uint8_t *parameters) ;
static uint8_t ForceSelectRFUBitsToNull 	(const uint8_t Protocol,const uint8_t Length,uint8_t *parameters);

static int8_t  CR95HF_IsUartMCUAndCR95HFAvailable(const uint8_t *pCommand);
static uint32_t CR95HF_ComputeUARTBaudRate (const uint8_t BaudRateCommandParameter);


/**
 *	@brief  this functions returns CR95HF_SUCCESS_CODE if the protocol is available, otherwise CR95HF_ERRORCODE_PARAMETER
 *  @param  Protocol : RF protocol (ISO 14443 A or B or 15 693 or Fellica)
 *  @retval CR95HF_SUCCESS_CODE	: the protocol is available
 *  @retval CR95HF_ERRORCODE_PARAMETER : the protocol isn't available
 */
static uint8_t IsAnAvailableProtocol (uint8_t Protocol) 
{
	switch(Protocol)
	{
		case PROTOCOL_TAG_FIELDOFF:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO15693:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO14443A:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO14443B:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_FELICA:
			return CR95HF_SUCCESS_CODE;
		default: return CR95HF_ERRORCODE_PARAMETER;
	}	
}

/**
 *	@brief  this functions returns CR95HF_SUCCESS_CODE if length value is allowed, otherwise ERRORCODE_GENERIC
 *	@brief  this functions is used by ProtocolSelect function.
 *  @param  Protocol : RF protocol (ISO 14443 A or B or 15 693 or Fellica)
 *  @param  Length : Number of byte of Protocol Select command
 *  @retval CR95HF_SUCCESS_CODE	: the length value is correct
 *  @retval ERRORCODE_GENERIC : the length value isn't correct
 */
static uint8_t IsAnAvailableSelectLength (uint8_t Protocol,uint8_t Length) 
{
	switch(Protocol)
	{
		case PROTOCOL_TAG_FIELDOFF:
			if (Length == 2 )
				return CR95HF_SUCCESS_CODE;
			else return ERRORCODE_GENERIC;
		case PROTOCOL_TAG_ISO15693:
			if (Length == 2 )
				return CR95HF_SUCCESS_CODE;
			else return ERRORCODE_GENERIC;
		case PROTOCOL_TAG_ISO14443A:
			// length == 2 protocol + parameters 
			// length == 4 protocol + parameters + AFDT (2 bytes)
			if (Length == 2 || Length == 4 )
				return CR95HF_SUCCESS_CODE;
			else return CR95HF_ERRORCODE_PARAMETER;
		case PROTOCOL_TAG_ISO14443B:
			if (Length == 2 || Length == 4 )
				return CR95HF_SUCCESS_CODE;
			else return CR95HF_ERRORCODE_PARAMETER;
		case PROTOCOL_TAG_FELICA:
			if (Length == 2 )
				return CR95HF_SUCCESS_CODE;
			else return CR95HF_ERRORCODE_PARAMETER;
		default: return CR95HF_ERRORCODE_PARAMETER;
	}	
}

/**
 *	@brief  this functions returns CR95HF_SUCCESS_CODE if parameter value is correct, otherwise ERRORCODE_GENERIC
 *	@brief  this functions is used by ProtocolSelect function.
 *  @param  Protocol : RF protocol (ISO 14443 A or B or 15 693 or Fellica)
 *  @param  Length : Number of byte of parameters
 *  @param  parameters : pointer on parameter of ProtocolSelect parameter
 *  @retval CR95HF_SUCCESS_CODE	: the parameter value is correct
 *  @retval CR95HF_ERRORCODE_PARAMETER : the length value isn't correct
 */
static uint8_t IsAnAvailableSelectParameters (const uint8_t Protocol,const uint8_t Length,const uint8_t *parameters) 
{
	switch(Protocol)
	{
		case PROTOCOL_TAG_FIELDOFF:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO15693:
			if ((parameters[0] & 0x30) == 0x30)
				return CR95HF_ERRORCODE_PARAMETER;
			else return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO14443A:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO14443B:
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_FELICA:
			if ( ((parameters[0] & 0x30) == 0x30) || ((parameters[0] & 0x30) == 0x00))
				return CR95HF_ERRORCODE_PARAMETER;
			return CR95HF_SUCCESS_CODE;
		default : return CR95HF_ERRORCODE_PARAMETER;
	}	
}

/**
 *	@brief  this functions reset RFU bits of parameters
 *  @param  Protocol : RF protocol (ISO 14443 A or B or 15 693 or Fellica)
 *  @param  Length : Number of byte of parameters
 *  @param  parameters : pointer on parameter of ProtocolSelect parameter 
 *  @retval CR95HF_SUCCESS_CODE	: RFU bits was reset
 *  @retval CR95HF_ERRORCODE_PARAMETER : the protocol isn't available
 */
static uint8_t ForceSelectRFUBitsToNull (const uint8_t Protocol,const uint8_t Length,uint8_t *parameters) 
{
	
	switch(Protocol)
	{
		case PROTOCOL_TAG_FIELDOFF:
			parameters[0]= 0x00;
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO15693:
			// bit 7:6 RFU
			parameters[0] &= 0x3F;
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO14443A:
			//bits 2:0 of byte 0 defined as RFU
			parameters[Length-2] &= 0xF8;
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_ISO14443B:
			//bits 3:1 of byte 0 defined as RFU
			parameters[Length-2] &= 0xF1;
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_TAG_FELICA:
			// bits 3:1 & 7:6 of byte 0 defined as RFU
			parameters[1] &= 0x31;
			parameters[0] &= 0x1F;
			return CR95HF_SUCCESS_CODE;
		default : return CR95HF_ERRORCODE_PARAMETER;
	}	
}


/**
 *	@brief  this function send a command over SPI bus
 *  @param  *pData : pointer on data to send to the CR95HF
 *  @retval None
 */
static void CR95HF_Send_SPI_Command(const uint8_t *pData)
{
uint8_t DummyBuffer[MAX_BUFFER_SIZE];
	  
	// Select CR95HF over SPI 
	CR95HF_NSS_LOW();

	// Send a sending request to CR95HF 
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_SEND);

	if(*pData == ECHO)
	{
		// Send a sending request to CR95HF
		SPI_SendReceiveByte(CR95HF_SPI, ECHO);
	}
	else
	{
		// Transmit the buffer over SPI
		SPI_SendReceiveBuffer(CR95HF_SPI, pData, pData[CR95HF_LENGTH_OFFSET]+CR95HF_DATA_OFFSET, DummyBuffer);
	}

	//Deselect CR95HF over SPI 
	CR95HF_NSS_HIGH();
}


/**
 *	@brief  this function send polling control byte 
 *  @retval CR95HF_SUCCESS_CODE : Polling sequence is succesful
 *  @retval CR95HF_POLLING_TIMEOUT : Polling sequence was interupted by MCU timeout
 */
static int8_t CR95HF_PollingCommand( void )
{
uint8_t Polling_Status = 0;

	StartTimeOut(10000);		
   
	if (ReaderConfig.SpiMode == SPI_POLLING)
	{
		CR95HF_NSS_LOW();
		while( Polling_Status 	!= CR95HF_FLAG_DATA_READY && 
					 CR95HF_TimeOut != TRUE )
		{
			
			// Send a polling request to CR95HF 
			SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_POLLING);
	
			// poll the CR95HF until he's ready ! 
			Polling_Status  = SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_POLLING);
			Polling_Status &= CR95HF_FLAG_DATA_READY_MASK;
	
			// Pulse on NSS 
					
		}
		CR95HF_NSS_HIGH();
	}	
	else if (ReaderConfig.SpiMode == SPI_INTERRUPT)
	{
		// reset the CR95HF data status 
		CR95HF_DataReady = FALSE;
		
		// Enable Interrupt on the falling edge on the IRQ pin of CR95HF 
		EXTI_ClearITPendingBit(EXTI_CR95HF_LINE);
		EXTI->IMR |= EXTI_CR95HF_LINE;

		// Wait a low level on the IRQ pin or the timeout 
		//while( (CR95HF_DataReady != TRUE) && CR95HF_TimeOut != TRUE );
		while( (CR95HF_DataReady == FALSE) & (CR95HF_TimeOut == FALSE) )
		{	}

		if(CR95HF_TimeOut == TRUE)
		{		
			// Disable CR95HF EXTI 
			EXTI->IMR &= ~EXTI_CR95HF_LINE;
		}
	}

	
	StopTimeOut( );

	if ( CR95HF_TimeOut == TRUE )
		return CR95HF_POLLING_TIMEOUT;

	return CR95HF_SUCCESS_CODE;	
}

/**
 *	@brief  this fucntion recovers a response from CR95HF device
 *  @param  *pData : pointer on data received from CR95HF device
 *  @retval None
 */
static void CR95HF_Receive_SPI_Response(uint8_t *pData)
{
uint8_t DummyBuffer[MAX_BUFFER_SIZE];

	// Select CR95HF over SPI 
	CR95HF_NSS_LOW();

	// Request a response from CR95HF 
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_RECEIVE);

	// Recover the "Command" byte 
	pData[CR95HF_COMMAND_OFFSET] = SPI_SendReceiveByte(CR95HF_SPI, DUMMY_BYTE);

	if(pData[CR95HF_COMMAND_OFFSET] == ECHO)
		 pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	else if(pData[CR95HF_COMMAND_OFFSET] == 0xFF)
		pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	else
	{
		// Recover the "Length" byte 
		pData[CR95HF_LENGTH_OFFSET]  = SPI_SendReceiveByte(CR95HF_SPI, DUMMY_BYTE);
		// Checks the data length 
		if(pData[CR95HF_LENGTH_OFFSET] != 0x00)
			// Recover data 	
			SPI_SendReceiveBuffer(CR95HF_SPI, DummyBuffer, pData[CR95HF_LENGTH_OFFSET], &pData[CR95HF_DATA_OFFSET]);
		
	}

	//. Deselect CR95HF over SPI 
	CR95HF_NSS_HIGH();

}

/**												   
 *	@brief  this function send a command to CR95HF device over SPI bus
 *  @param  *pData : pointer on data to send to the CR95HF
 *  @retval None
 */
static void CR95HF_Send_UART_Command(const uint8_t *pData)
{
	if(pData[0] == ECHO)
		// send Echo 
		UART_SendByte(CR95HF_UART, ECHO);
	
	else
		// send the buffer over UART 
		UART_SendBuffer(CR95HF_UART, pData, pData[CR95HF_LENGTH_OFFSET] + CR95HF_DATA_OFFSET);
	
}

/**
 *	@brief  this functions recovers a response from CR95HF device over SPI bus
 *  @param  *pData : pointer on data received from CR95HF device
 *  @retval None
 */
static void CR95HF_Receive_UART_Response(uint8_t *pData)
{
	// Recover the "Command" byte 
	pData[CR95HF_COMMAND_OFFSET] = UART_ReceiveByte(CR95HF_UART);

	if(pData[CR95HF_COMMAND_OFFSET] == ECHO)
		pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	
	else
	{
		// Recover the "Length" byte 
		pData[CR95HF_LENGTH_OFFSET]  = UART_ReceiveByte(CR95HF_UART);
	
		// Recover data
		if(pData[CR95HF_LENGTH_OFFSET]	!= 0)
			UART_ReceiveBuffer(CR95HF_UART, &pData[CR95HF_DATA_OFFSET], pData[CR95HF_LENGTH_OFFSET]);
	}
}

/**
 *	@brief  This function checks the baud rate is compatible with this MCU and configurates MCU and CR95HF baud rate.
 *  @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 *  @retval CR95HF_SUCCESS_CODE : the baud rate is compatible
 *  @retval CR95HF_ERRORCODE_DEFAULT : the baud rate is not compatible
 */
static int8_t CR95HF_IsUartMCUAndCR95HFAvailable(const uint8_t *pCommand)
{
	if (pCommand[CR95HF_COMMAND_OFFSET] != BAUD_RATE) 
		return CR95HF_ERRORCODE_DEFAULT;
	
	
	switch (pCommand[CR95HF_DATA_OFFSET])
	{
		case 0 : // RFU in data sheet
			return CR95HF_ERRORCODE_DEFAULT;
		case 1 :  // RFU in data sheet
			return CR95HF_ERRORCODE_DEFAULT;
		case 2 :  // not compatible with MCU
			return CR95HF_ERRORCODE_DEFAULT;	
		case 5 :  // not compatible with MCU
			return CR95HF_ERRORCODE_DEFAULT;	
		case 8 :  // not compatible with MCU
			return CR95HF_ERRORCODE_DEFAULT;

		case 0x75 :  
			return CR95HF_SUCCESS_CODE;

		default : return CR95HF_ERRORCODE_DEFAULT; 	
	}  	
}
/**
 *	@brief  This function computes the UART baud rate according to Baudrate command parameter
 *  @param  BaudRateCommandParameter  : Baudrate command parameter (1 byte)
 *  @retval CR95HF_SUCCESS_CODE : the baud rate is compatible
 *  @retval CR95HF_ERRORCODE_DEFAULT : the baud rate is not compatible
 */
static uint32_t CR95HF_ComputeUARTBaudRate (const uint8_t BaudRateCommandParameter)
{
	return (13.56e6/ (2*BaudRateCommandParameter+2));
}

/**  
* @brief  	this function returns the number of control byte according to RF protocol
* @param  	ProtocolSelected : Rf protocol selected
* @retval 	CONTROL_15693_NBBYTE 	: number of control byte for 15 693 protocol
* @retval 	CONTROL_14443A_NBBYTE	: number of control byte for 14 443 A protocol
* @retval 	CONTROL_14443B_NBBYTE  	: number of control byte for 14 443 B protocol
* @retval 	CONTROL_FELICA_NBBYTE  	: number of control byte for Felica protocol
* @retval 	0 : error the protocol in unknown
*/
static int8_t GetNbControlByte (int8_t ProtocolSelected)
{

	switch(ProtocolSelected)
	{
		case PROTOCOL_TAG_ISO15693:
			return CONTROL_15693_NBBYTE;
		case PROTOCOL_TAG_ISO14443A:
			return CONTROL_14443A_NBBYTE;
		case PROTOCOL_TAG_ISO14443B:
			return CONTROL_14443B_NBBYTE;
		case PROTOCOL_TAG_FELICA:
			return CONTROL_FELICA_NBBYTE;
		default :
			return 0;
	}
}

/** @addtogroup CR95HF_Functions
 *  @{
 */
/** @addtogroup CR95HF_CommandFunctions
*  @brief  this set of function send a command defined in CR95HF datasheet to a CR95HF device 
 *  @{
 */

/**
 *	@brief  this function send an IDN command to CR95HF device
 *  @param  pResponse : pointer on CR95HF reply
 *  @retval CR95HF_SUCCESS_CODE : command was succesful sending
 */
int8_t CR95HF_IDN(uint8_t *pResponse)
{
const uint8_t DataToSend[] = {IDN	,0x00};
	
	SPIUART_SendReceive(DataToSend, pResponse);

return CR95HF_SUCCESS_CODE;

}

/**
 *	@brief  this function send an EOF pulse to a contacless tag
 *  @param  pResponse : Reader reply
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 */
int8_t CR95HF_SendEOF(uint8_t *pResponse)
{
const uint8_t DataToSend[] = {SEND_RECEIVE	,0x00};

	SPIUART_SendReceive(DataToSend, pResponse);

return CR95HF_SUCCESS_CODE;
}



/**
 *	@brief  this functions turns the field off
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 */
int8_t CR95HF_FieldOff( void )
{
const uint8_t DataToSend[] = {PROTOCOL_SELECT,0x02,0x00,0x00};
uint8_t pResponse[2];

	SPIUART_SendReceive(DataToSend, pResponse);

	return CR95HF_SUCCESS_CODE;
}



/**
 *	@brief  this function send Echo command to CR95HF 
 *  @param  pResponse : pointer on CR95HF reply
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 */
int8_t CR95HF_Echo(uint8_t *pResponse)
{
const uint8_t command[]= {ECHO};

	SPIUART_SendReceive(command, pResponse);

return CR95HF_SUCCESS_CODE;

}

/**
 *	@brief  this function send a ProtocolSeclect command to CR95HF
 *  @param  Length  : number of byte of protocol select command parameters
 *  @param  Protocol : RF protocol (ISO 14443 A or B or 15 693 or Fellica)
 *  @param  Parameters: prococol parameters (see reader datasheet)
 *  @param  pResponse : pointer on CR95HF response
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 *  @retval CR95HF_ERRORCODE_PARAMETERLENGTH : the Length parameter is erroneous
 *  @retval CR95HF_ERRORCODE_PARAMETER : a parameter is erroneous
 */
int8_t CR95HF_ProtocolSelect(const uint8_t Length,const uint8_t Protocol,const uint8_t *Parameters,uint8_t *pResponse)
{
uint8_t DataToSend[SELECT_BUFFER_SIZE],
		SelectParameters[SELECT_BUFFER_SIZE];
int8_t	status,
		i=0; 

	if (CHECKVAL(Length,1,SELECT_BUFFER_SIZE) == FALSE)
		return CR95HF_ERRORCODE_PARAMETERLENGTH;

	// check the function parameters
	errchk(IsAnAvailableProtocol(Protocol));
	errchk(IsAnAvailableSelectLength(Protocol,Length));
	errchk(IsAnAvailableSelectParameters(Protocol,Length,Parameters));

	memcpy(SelectParameters,Parameters,Length);

	// force the RFU bits to 0
	errchk(ForceSelectRFUBitsToNull	(Protocol,Length,SelectParameters));

	DataToSend[CR95HF_COMMAND_OFFSET ] = PROTOCOL_SELECT;
	DataToSend[CR95HF_LENGTH_OFFSET  ]	= Length;
	DataToSend[CR95HF_DATA_OFFSET    ]	= Protocol;

	// DataToSend CodeCmd Length Data
	// Parameters[0] first byte to emmit
	for (i = 0; i< Length -1  ;i++)
		DataToSend[CR95HF_DATA_OFFSET +1 + i ] = SelectParameters[i];

  	SPIUART_SendReceive(DataToSend, pResponse);

	return CR95HF_SUCCESS_CODE;

Error:
	// initialize the result code to 0xFF and length to in case of error 
	*pResponse = CR95HF_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;
	return CR95HF_ERRORCODE_PARAMETER;	
}

/**
 *	@brief  this function send a SendRecv command to CR95HF. the command to contacless device is embeded in Parameters.
 *  @param  Length 		: Number of bytes
 *  @param	Parameters 	: data depenps on protocl selected
 *  @param  pResponse : pointer on CR95HF response
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 *  @retval CR95HF_ERROR_CODE : CR95HF returned an error code
 *  @retval CR95HF_ERRORCODE_PARAMETERLENGTH : Length parameter is erroneous
 */
int8_t CR95HF_SendRecv(const uint8_t Length,const uint8_t *Parameters,uint8_t *pResponse)
{
uint8_t DataToSend[SENDRECV_BUFFER_SIZE],							  
		i=0;

	// initialize the result code to 0xFF and length to 0
	*pResponse = CR95HF_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;
	

	// check the function parameters
	if (CHECKVAL (Length,1,255)==FALSE)
		return CR95HF_ERRORCODE_PARAMETERLENGTH; 

	DataToSend[CR95HF_COMMAND_OFFSET ] = SEND_RECEIVE;
	DataToSend[CR95HF_LENGTH_OFFSET  ]	= Length;

	// DataToSend CodeCmd Length Data
	// Parameters[0] first byte to emmit
	for (i =0 ; i< Length ;i++)
		DataToSend[CR95HF_DATA_OFFSET + i ] = Parameters[i];

	SPIUART_SendReceive(DataToSend, pResponse);

	if (CR95HF_IsReaderResultCodeOk (SEND_RECEIVE,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;

	return CR95HF_SUCCESS_CODE;
}

/**
 *	@brief  this function send a Idle command to CR95HF
 *  @param  Length 	: Number of bytes
 *  @param	Data	: pointer on data. Data depends on protocl selected
 *  @param  pResponse : pointer on CR95HF response
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 *  @retval CR95HF_ERROR_CODE : CR95HF returned an error code
 *  @retval CR95HF_ERRORCODE_PARAMETERLENGTH : Length parameter is erroneous
 */
int8_t CR95HF_Idle(const uint8_t Length,const uint8_t *Data,uint8_t *pResponse)
{
uint8_t DataToSend[IDLE_BUFFER_SIZE],
		i=0;

	// check the function parameters
	if (Length != 0x0E)
		return CR95HF_ERRORCODE_PARAMETERLENGTH; 

	DataToSend[CR95HF_COMMAND_OFFSET ] = IDLE;
	DataToSend[CR95HF_LENGTH_OFFSET  ]	= Length;

	// Parameters[0] first byte to emmit
	for (i =0 ; i< Length ;i++)
		DataToSend[CR95HF_DATA_OFFSET + i ] = Data[i];

	SPIUART_SendReceive(DataToSend, pResponse);

	// in case of succes CR95HF might not reply
	if (pResponse[READERREPLY_STATUSOFFSET] == CR95HF_NOREPLY_CODE)
		return CR95HF_SUCCESS_CODE;	
			
	if (CR95HF_IsReaderResultCodeOk (IDLE,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;

	return CR95HF_SUCCESS_CODE;	
}

/**
 *	@brief  this function send a RdReg command to CR95HF
 *  @param  Length 		: Number of bytes
 *  @param	Address		: address of first register to read
 *  @param	RegCount	: number of register to read
 *  @param	Flags		: whether to increment address after register read
 *  @param  pResponse : pointer on CR95HF response
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 *  @retval CR95HF_ERROR_CODE : CR95HF returned an error code
 *  @retval CR95HF_ERRORCODE_PARAMETERLENGTH : Length parameter is erroneous
 */
int8_t CR95HF_RdReg(const uint8_t Length,const uint8_t Address,const uint8_t RegCount,const uint8_t Flags,uint8_t *pResponse)
{
uint8_t DataToSend[RDREG_BUFFER_SIZE],
		FlagsMasqued;

	// check the function parameters
	if (Length != 3)
		return CR95HF_ERRORCODE_PARAMETERLENGTH; 

	FlagsMasqued = Flags & 0x03;

	DataToSend[CR95HF_COMMAND_OFFSET ] = READ_REGISTER;
	DataToSend[CR95HF_LENGTH_OFFSET  ]	= Length;
	DataToSend[CR95HF_DATA_OFFSET  ]	= Address;
	DataToSend[CR95HF_DATA_OFFSET +1 ]	= RegCount;
	DataToSend[CR95HF_DATA_OFFSET +2 ]	= FlagsMasqued;

	SPIUART_SendReceive(DataToSend, pResponse);
	
	if (CR95HF_IsReaderResultCodeOk (IDLE,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;

	return CR95HF_SUCCESS_CODE;

}

/**
 *	@brief  this function send a WrReg command to CR95HF
 *  @param  Length 		: Number of bytes of WrReg parameters
 *  @param	Address		: address of first register to write
 *  @param	pData 		: pointer data to be write
 *  @param	Flags		: whether to increment address after register read
 *  @param  pResponse : pointer on CR95HF response
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 *  @retval CR95HF_ERRORCODE_PARAMETERLENGTH : Length parameter is erroneous
 */
int8_t CR95HF_WrReg (const uint8_t Length,const uint8_t Address,const uint8_t Flags,const uint8_t *pData,uint8_t *pResponse)
{
uint8_t DataToSend[WRREG_BUFFER_SIZE],
		FlagsMasqued,
		i=0;

	// check the function parameters
	if (Length != 3)
		return CR95HF_ERRORCODE_PARAMETERLENGTH; 

	FlagsMasqued = Flags & 0x03;

	DataToSend[CR95HF_COMMAND_OFFSET ] = WRITE_REGISTER;
	DataToSend[CR95HF_LENGTH_OFFSET  ]	= Length;
	DataToSend[CR95HF_DATA_OFFSET  ]	= Address;
	DataToSend[CR95HF_DATA_OFFSET +1 ]	= FlagsMasqued;

	// Parameters[0] first byte to emmit
	for (i =0 ; i< Length - 2 ;i++)
		DataToSend[CR95HF_DATA_OFFSET + 2 + i ] = pData[i];

	SPIUART_SendReceive(DataToSend, pResponse);
	return CR95HF_SUCCESS_CODE;
}

/**
 *	@brief  this function send a BaudRate command to CR95HF
 *  @param	BaudRate	: new baudrate
 *  @param	pResponse 	: pseudo reply (shall be 0x55)
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 */
int8_t CR95HF_BaudRate (const uint8_t BaudRate,uint8_t *pResponse)
{
uint8_t DataToSend[BAUDRATE_BUFFER_SIZE];

	DataToSend[CR95HF_COMMAND_OFFSET ] = BAUD_RATE;
	DataToSend[CR95HF_LENGTH_OFFSET  ]	= BAUDRATE_LENGTH;
	DataToSend[CR95HF_DATA_OFFSET  ]	= BaudRate;

	SPIUART_SendReceive(DataToSend, pResponse);
	return CR95HF_SUCCESS_CODE;
}
/**
 * @}
 */


/** @addtogroup CR95HF_CustomsFunctions
*  @brief  this set of specific function
 *  @{
 */


/**
 *	@brief  this function send a command to CR95HF device over SPI or UART bus and reveive its response
 *  @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 *  @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 *  @retval  none
 */
void CR95HF_SendCustomCommand(const uint8_t *pCommand, uint8_t *pResponse)
{
	
	*pResponse = 0x00;
	*(pResponse+1) = 0x00;
		
	switch (pCommand[CR95HF_COMMAND_OFFSET])
	{
		case CR95HF_RESETSEQUENCE: 		
			CR95HF_Send_SPI_ResetSequence();
		break;
		case CR95HF_PULSE_IRQIN: 
			CR95HF_Send_IRQIN_NegativePulse();
		break;
		case CR95HF_GETMCUVERSION: 
		 	CR95HF_GetMCUversion(pResponse);
		break;
		case CR95HF_GETINTERFACEPINSTATE: 
		 	CR95HF_GetInterfaceBus (pResponse);
		break;
		case CR95HF_SETUSBDISCONNECTSTATE: 
			CR95HF_SetUSBDisconnectPin (pCommand);		
		break;
	}
	
}

/**
 *	@brief  this function returns the inferface bus choosed 
 *  @param  *pResponse : pointer on the CR95HF response
 *  @param  : 0x00 : status ok
 *  @param  : 0x01 : length
 *  @param  : 0x0X : with X = 0 => UART Or X = 0 => SPI Or X = 2 => TWI
 *  @retval  none
 */
void CR95HF_GetInterfaceBus (uint8_t *pResponse)
{
   	*pResponse 		= 0x00;
	*(pResponse+1) 	= 0x01;
	*(pResponse+2) 	= CR95HF_GetInterfacePinState ();
}

/**
 *	@brief  this function returns the inferface bus choosed 
 *  @param  *pCommand : command received 
 *  @retval none
 */
void CR95HF_SetUSBDisconnectPin (const uint8_t *pCommand)
{
//   	if (pCommand[CR95HF_DATA_OFFSET] == DISABLE)
//		USB_Cable_Config (DISABLE);
//	else 
//		USB_Cable_Config (ENABLE);

}


/**
 *	@brief  this function returns the MCU version 
 *  @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 *  @retval  none
 */
void CR95HF_GetMCUversion(uint8_t *pResponse)
{
	*pResponse = MCUVERSION_STATUS;
	*(pResponse+1) = MCUVERSION_LENGTH;
	memcpy((pResponse+2),MCUVERSION ,MCUVERSION_LENGTH);	
}

/**
 *	@brief  This function run a process to change UARt baud rate. It'll check the baud rate is
 *	@brief   compatible with this MCU and configurates MCU and CR95HF baud rate.
 *  @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 *  @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 *  @retval  CR95HF_SUCCESS_CODE : the UART baud rate has been succesfuly changed 
 *  @retval  CR95HF_ERRORCODE_UARTDATARATEUNCHANGED : the UART baud rate has not changed 
 *  @retval  CR95HF_ERRORCODE_UARTDATARATEPROCESS : command DaubRate was send to CR95HF but the MCU was not able to comunicatewith CR95HF
 */
int8_t CR95HF_ChangeUARTBaudRate (const uint8_t *pCommand, uint8_t *pResponse)
{
	*pResponse =CR95HF_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;

	//if(CR95HF_Interface == SPI)
	if(ReaderConfig.Interface != CR95HF_INTERFACE_UART)
	{	*pResponse =CR95HF_ERRORCODE_UARTDATARATEUNCHANGED;
		return CR95HF_ERRORCODE_UARTDATARATEUNCHANGED; 
	}

	// some UART data rates are not compatible between MCU and CR95HF
	if (CR95HF_IsUartMCUAndCR95HFAvailable(pCommand) != CR95HF_SUCCESS_CODE)
	{	*pResponse =CR95HF_ERRORCODE_UARTDATARATEUNCHANGED;
		return CR95HF_ERRORCODE_UARTDATARATEUNCHANGED; 
	}

	// send baud rate command to CR95HF
	SPIUART_SendReceive(pCommand,pResponse);

	// udpate UART config of MCU
	UART_Initialization(CR95HF_ComputeUARTBaudRate (pCommand[CR95HF_DATA_OFFSET]));

    // check if the MCU can communicate with CR95HF
	CR95HF_Echo(pResponse);
	if (pResponse[ECHOREPLY_OFFSET]!=0x55)
	{	*pResponse =CR95HF_ERRORCODE_UARTDATARATEPROCESS;
		return CR95HF_ERRORCODE_UARTDATARATEPROCESS; 
	}
	// the baud rate has been succesful modified
	*pResponse =CR95HF_SUCCESS_CODE;
	return CR95HF_SUCCESS_CODE; 
}

/**
 *	@brief  this function send a command to CR95HF device over SPI or UART bus and receive its response.
 *	@brief  the returned value is CR95HF_SUCCESS_CODE 	
 *  @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 *  @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 */
int8_t CR95HF_CheckSendReceive(const uint8_t *pCommand, uint8_t *pResponse) 
{
		
	SPIUART_SendReceive(pCommand, pResponse);

	if (CR95HF_IsReaderResultCodeOk (SEND_RECEIVE,pResponse) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERROR_CODE;

	return CR95HF_SUCCESS_CODE;
}


/**
 * @}
 */


/** @addtogroup CR95HF_SPIorUARTFunctions
*  @brief  this set of function manage the communication to CR95HF device. the bus can be either SPI or UART. 
 *  @{
*/


/**
 *	@brief  this function send a command to CR95HF device over SPI or UART bus and reveive its response
 *  @param  *pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 *  @param  *pResponse : pointer on the CR95HF response ( Command | Length | Data)
 *  @retval CR95HF_SUCCESS_CODE : the command was succedfully send
 */
int8_t SPIUART_SendReceive(const uint8_t *pCommand, uint8_t *pResponse)
{
int8_t i=0;

	*pResponse =CR95HF_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;

	//if(CR95HF_Interface == SPI)
	if(ReaderConfig.Interface == CR95HF_INTERFACE_SPI)
	{
		// First step  - Sending command 
		CR95HF_Send_SPI_Command(pCommand);
		for (i=0;i<50;i++)
			;
		// Second step - Polling
		if (CR95HF_PollingCommand( ) != CR95HF_SUCCESS_CODE)
		{	*pResponse =CR95HF_ERRORCODE_TIMEOUT;
			return CR95HF_POLLING_CR95HF;	
		}
		for (i=0;i<50;i++)
			;
		// Third step  - Receiving bytes 
		CR95HF_Receive_SPI_Response(pResponse);
	}
	else if(ReaderConfig.Interface == CR95HF_INTERFACE_UART)
	{
		// First step  - Sending command
		CR95HF_Send_UART_Command(pCommand);
		// Second step - Receiving bytes 
		CR95HF_Receive_UART_Response(pResponse);
	}

	return CR95HF_SUCCESS_CODE; 
}


/**
 *	@brief  this function send a command to CR95HF device over SPI or UART bus
 *  @param  pCommand  : pointer on the buffer to send to the CR95HF ( Command | Length | Data)
 *  @retval none
 */
void SPIUART_Send(const uint8_t *pCommand)
{
	if(ReaderConfig.Interface == CR95HF_INTERFACE_SPI)
		// First step  - Sending command 
		CR95HF_Send_SPI_Command(pCommand);

	else if(ReaderConfig.Interface == CR95HF_INTERFACE_UART)
		// First step  - Sending command
		CR95HF_Send_UART_Command(pCommand);
}

/**
 *	@brief  this functions captures the reader command and updates the global variables (case of shadow mode).
 *  @param  pCommand  : pointer on commamd send to CR95HF
 *  @param  pResponse : pointer on CR95Hf response
 *  @retval none
 */
void SpyTransaction(const uint8_t *pCommand, uint8_t *pResponse)
{
	switch (pCommand[CR95HF_COMMAND_OFFSET])
	{
		case PROTOCOL_SELECT: 
			if (CR95HF_IsReaderResultCodeOk (pCommand[CR95HF_COMMAND_OFFSET],pResponse) == TRUE)
				ReaderConfig.CurrentProtocol=pCommand[CR95HF_DATA_OFFSET];	
	}
}

/**
 * @}
 */


/** @addtogroup CR95HF_TranslationFunctions
 *  @{
 */


/**
 *	@brief  converts the command code to a command name
 *  @param  HexBuffer  : command code
 *  @param  StringCommand : ASCII string of command name
 *  @param  CmdNameNbByte : number of byte of StringCommand
 *  @retval CR95HF_SUCCESS_CODE
 */
int8_t HexCommandToStringCommand(uint8_t HexBuffer,uint8_t *StringCommand,uint8_t *CmdNameNbByte)
{
	memset(StringCommand,0x00,MAXLENGTH_CMDNAME);

	switch (HexBuffer)
	{
		case ECHO: 
			memcpy(StringCommand,ECHO_CMDNAME,strlen(ECHO_CMDNAME)+1);
			*CmdNameNbByte = strlen(ECHO_CMDNAME);  
		break;
		case IDN: 
			memcpy(StringCommand,IDN_CMDNAME,strlen(IDN_CMDNAME)+1);
			*CmdNameNbByte = strlen(IDN_CMDNAME);
		break;
		case PROTOCOL_SELECT: 
			memcpy(StringCommand,PROTOCOL_SELECT_CMDNAME,strlen(PROTOCOL_SELECT_CMDNAME)+1);
			*CmdNameNbByte = strlen(PROTOCOL_SELECT_CMDNAME);
		break;
		case SEND_RECEIVE: 
			memcpy(StringCommand,SEND_RECEIVE_CMDNAME,strlen(SEND_RECEIVE_CMDNAME)+1);
			*CmdNameNbByte = strlen(SEND_RECEIVE_CMDNAME);
		break;
		case IDLE: 
			memcpy(StringCommand,IDLE_CMDNAME,strlen(IDLE_CMDNAME)+1);
			*CmdNameNbByte = strlen(IDLE_CMDNAME);
		break;
		case READ_REGISTER: 
			memcpy(StringCommand,READ_REGISTER_CMDNAME,strlen(READ_REGISTER_CMDNAME)+1);
			*CmdNameNbByte = strlen(READ_REGISTER_CMDNAME);
		break;
		case WRITE_REGISTER: 
			memcpy(StringCommand,WRITE_REGISTER_CMDNAME,strlen(WRITE_REGISTER_CMDNAME)+1);
			*CmdNameNbByte = strlen(WRITE_REGISTER_CMDNAME);
		break;
		case BAUD_RATE: 
			memcpy(StringCommand,BAUD_RATE_CMDNAME,strlen(BAUD_RATE_CMDNAME)+1);
			*CmdNameNbByte = strlen(BAUD_RATE_CMDNAME);
		break;
		default: 
			memcpy(StringCommand,ECHO_CMDUNKOWN,strlen(ECHO_CMDUNKOWN)+1);
			*CmdNameNbByte = strlen(ECHO_CMDUNKOWN);
		break;
	
	}
	return CR95HF_SUCCESS_CODE;
}

/**  
* @brief  	this function returns an ASCII string wich descibed the error code returned by CR95HF
* @param  	CmdCode	: code command send to the reader
* @param  	ReaderReply	: pointer on CR95HF response		
* @param	ErrorDescription : 	ASCII string description of the error
* @retval 	ERRORCODE_GENERIC :  CR95HF didn't return a error code
* @retval 	CR95HF_SUCCESS_CODE : CR95HF returned a error code
*/
int8_t CR95HF_GetReaderErrorCode (const uint8_t CmdCode,const uint8_t *ReaderReply,char *ErrorDescription)
{
 uint8_t CmdCodeMasqued= CmdCode & 0xFF; 
	
	memset (ErrorDescription,0x00,PROTOCOLSELECT_ERRORDESC_MAXLENGTH);
  	switch (CmdCodeMasqued)
	{
		case ECHO: 
			if (ReaderReply[ECHOREPLY_OFFSET] == ECHO)
				return ERRORCODE_GENERIC;
			else 
				return CR95HF_SUCCESS_CODE;
		case IDN: 
			if (ReaderReply[READERREPLY_STATUSOFFSET] == IDN_RESULTSCODE_OK)
				return ERRORCODE_GENERIC;
			else 
				return ERRORCODE_GENERIC;
		case PROTOCOL_SELECT: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDN_RESULTSCODE_OK :
					return ERRORCODE_GENERIC;
				case PROTOCOLSELECT_ERRORCODE_CMDLENGTH :
					memcpy (ErrorDescription,PROTOCOLSELECT_ERRORDESC_CMDLENGTH,strlen(PROTOCOLSELECT_ERRORDESC_CMDLENGTH));
					return CR95HF_SUCCESS_CODE;
				case PROTOCOLSELECT_ERRORCODE_INVALID :
					memcpy (ErrorDescription,PROTOCOLSELECT_ERRORDESC_INVALID,strlen(PROTOCOLSELECT_ERRORDESC_INVALID));
					return CR95HF_SUCCESS_CODE;
				default : return ERRORCODE_GENERIC;
			}
		case SEND_RECEIVE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case SENDRECV_RESULTSCODE_OK :
					return ERRORCODE_GENERIC;
				case SENDRECV_ERRORCODE_COMERROR :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_COMERROR,strlen(SENDRECV_ERRORDESC_COMERROR));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_FRAMEWAIT :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_FRAMEWAIT,strlen(SENDRECV_ERRORDESC_FRAMEWAIT));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_SOF :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_SOF,strlen(SENDRECV_ERRORDESC_SOF));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_OVERFLOW :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_OVERFLOW,strlen(SENDRECV_ERRORDESC_OVERFLOW));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_FRAMING :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_FRAMING,strlen(SENDRECV_ERRORDESC_FRAMING));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_EGT :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_EGT,strlen(SENDRECV_ERRORDESC_EGT));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_LENGTH :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_LENGTH,strlen(SENDRECV_ERRORDESC_LENGTH));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_CRC :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_CRC,strlen(SENDRECV_ERRORDESC_CRC));
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_RECEPTIONLOST :
					memcpy (ErrorDescription,SENDRECV_ERRORDESC_RECEPTIONLOST,strlen(SENDRECV_ERRORDESC_RECEPTIONLOST));
					return CR95HF_SUCCESS_CODE;
				default :
					return ERRORCODE_GENERIC;
			}			

		case IDLE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDLE_RESULTSCODE_OK :
					return ERRORCODE_GENERIC;
				case IDLE_ERRORCODE_LENGTH :
					memcpy (ErrorDescription,IDLE_ERRORDESC_LENGTH,strlen(IDLE_ERRORDESC_LENGTH));
					return CR95HF_SUCCESS_CODE;
				default : return ERRORCODE_GENERIC;
		
			}
		case READ_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case READREG_RESULTSCODE_OK :
					return ERRORCODE_GENERIC;
				case READREG_ERRORCODE_LENGTH :
					memcpy (ErrorDescription,READREG_ERRORDESC_LENGTH,strlen(READREG_ERRORDESC_LENGTH));
					return CR95HF_SUCCESS_CODE;
				default : return ERRORCODE_GENERIC;
			}
		case WRITE_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case WRITEREG_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_SUCCESS_CODE;
			}
		default: 
			return ERRORCODE_GENERIC;
	}
}

/**
 * @}
 */


/** @addtogroup CR95HF_ISFunctions
*  @brief  this set of function checks a parameter and returns either a succesful code or an error code.
 *  @{
 */


/**  
* @brief  this function returns CR95HF_SUCCESS_CODE is the command code exists, CR95HF_ERRORCODE_COMMANDUNKNOWN otherwise
* @param  CmdCode :  	code command send to CR95HF		
* @retval CR95HF_SUCCESS_CODE : the command code is available
* @retval CR95HF_ERRORCODE_COMMANDUNKNOWN : the command code isn't available
*/
int8_t CR95HF_IsCommandExists(uint8_t CmdCode)
{

   CmdCode = CmdCode & 0x0F;
  	switch (CmdCode)
	{
		case ECHO: 
			return CR95HF_SUCCESS_CODE;
		case IDN: 
			return CR95HF_SUCCESS_CODE;
		case PROTOCOL_SELECT: 
			return CR95HF_SUCCESS_CODE;
		case SEND_RECEIVE: 
			return CR95HF_SUCCESS_CODE;
		case IDLE: 
			return CR95HF_SUCCESS_CODE;
		case READ_REGISTER: 
			return CR95HF_SUCCESS_CODE;
		case WRITE_REGISTER: 
			return CR95HF_SUCCESS_CODE;
		case BAUD_RATE: 
			return CR95HF_SUCCESS_CODE;
		default: 
			return CR95HF_ERRORCODE_COMMANDUNKNOWN;
	}
}


/**  
* @brief  	this function returns CR95HF_SUCCESS_CODE is the reader reply is a succesful code.
* @param  	CmdCode		:  	code command send to the reader
* @param  	ReaderReply	:  	pointer on CR95HF response	
* @retval  	CR95HF_SUCCESS_CODE :  CR95HF returned a succesful code
* @retval  	CR95HF_ERROR_CODE  :  CR95HF didn't return a succesful code
* @retval  	CR95HF_NOREPLY_CODE : no CR95HF response
*/
int8_t CR95HF_IsReaderResultCodeOk (uint8_t CmdCode,const uint8_t *ReaderReply)
{

   CmdCode = CmdCode & 0xFF;

   if (ReaderReply[READERREPLY_STATUSOFFSET] == CR95HF_ERRORCODE_DEFAULT)
		return CR95HF_NOREPLY_CODE;

  	switch (CmdCode)
	{
		case ECHO: 
			if (ReaderReply[ECHOREPLY_OFFSET] == ECHO)
				return CR95HF_SUCCESS_CODE;
			else 
				return CR95HF_ERROR_CODE;
		case IDN: 
			if (ReaderReply[READERREPLY_STATUSOFFSET] == IDN_RESULTSCODE_OK)
				return CR95HF_SUCCESS_CODE;
			else 
				return CR95HF_ERROR_CODE;
		case PROTOCOL_SELECT: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDN_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case PROTOCOLSELECT_ERRORCODE_CMDLENGTH :
					return CR95HF_ERROR_CODE;
				case PROTOCOLSELECT_ERRORCODE_INVALID :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case SEND_RECEIVE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case SENDRECV_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_COMERROR :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_FRAMEWAIT :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_SOF :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_OVERFLOW :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_FRAMING :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_EGT :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_LENGTH :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_CRC :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_RECEPTIONLOST :
					return CR95HF_ERROR_CODE;
				default :
					return CR95HF_ERROR_CODE;
			}			
		case IDLE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDLE_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case IDLE_ERRORCODE_LENGTH :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case READ_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case READREG_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case READREG_ERRORCODE_LENGTH :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case WRITE_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case WRITEREG_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case BAUD_RATE: 
			return CR95HF_ERROR_CODE;
		default: 
			return ERRORCODE_GENERIC;
	}
}

/**  
* @brief  	this function returns CR95HF_SUCCESS_CODE is the reader reply is an error code.
* @param  	CmdCode		:  	code command send to the reader
* @param  	ReaderReply	:  	pointer on CR95HF response	
* @retval  	CR95HF_SUCCESS_CODE :  CR95HF returned a error code
* @retval  	CR95HF_ERROR_CODE  :  CR95HF didn't return a error code
*/
int8_t CR95HF_IsReaderErrorCode (uint8_t CmdCode,uint8_t *ReaderReply)
{
   CmdCode = CmdCode & 0xFF;
  	switch (CmdCode)
	{
		case ECHO: 
			if (ReaderReply[ECHOREPLY_OFFSET] == ECHO)
				return CR95HF_ERROR_CODE;
			else 
				return CR95HF_SUCCESS_CODE;
		case IDN: 
			if (ReaderReply[READERREPLY_STATUSOFFSET] == IDN_RESULTSCODE_OK)
				return CR95HF_ERROR_CODE;
			else 
				return CR95HF_ERROR_CODE;
		case PROTOCOL_SELECT: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDN_RESULTSCODE_OK :
					return CR95HF_ERROR_CODE;
				case PROTOCOLSELECT_ERRORCODE_CMDLENGTH :
					return CR95HF_SUCCESS_CODE;
				case PROTOCOLSELECT_ERRORCODE_INVALID :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case SEND_RECEIVE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case SENDRECV_RESULTSCODE_OK :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_COMERROR :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_FRAMEWAIT :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_SOF :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_OVERFLOW :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_FRAMING :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_EGT :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_LENGTH :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_CRC :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_RECEPTIONLOST :
					return CR95HF_SUCCESS_CODE;
				default :
					return CR95HF_ERROR_CODE;
			}			
		case IDLE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDLE_RESULTSCODE_OK :
					return CR95HF_ERROR_CODE;
				case IDLE_ERRORCODE_LENGTH :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case READ_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case READREG_RESULTSCODE_OK :
					return CR95HF_ERROR_CODE;
				case READREG_ERRORCODE_LENGTH :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case WRITE_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case WRITEREG_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_SUCCESS_CODE;
			}
		case BAUD_RATE: 
			return CR95HF_SUCCESS_CODE;
		case SUB_FREQ_RES: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{	case SUBFREQ_RESULTSCODE_OK :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}	

		default: 
			return ERRORCODE_GENERIC;
	}
}




/**
 * @}
 */


/** @addtogroup CR95HF_SplitFunctions
 *  @{
 */



/**  
* @brief    this function split a CR95HF response and extract the different fields
* @param  	CmdCodeToReader : code command send to the reader
* @param	ProtocolSelected : protocol selected (select command)
* @param 	ReaderReply 	: reader reply
* @param 	ResultCode		: TRUE is reponse is Ok, false otherwise 
* @param 	NbTagByte		: Number of byte of tag reply
* @param 	TagReplyDataIndex : data of the reader reply is exists
* @param 	NbControlByte	: Number of control byte
* @param 	ControlIndex	: control byte(s)
* @retval 	CR95HF_SUCCESS_CODE : CR95HF was succesful splitted
* @retval 	CR95HF_ERRORCODE_COMMANDUNKNOWN : The command code is unknown
*/
int8_t SplitReaderReply (uint8_t CmdCodeToReader,uint8_t ProtocolSelected,const uint8_t *ReaderReply,uint8_t *ResultCode,uint8_t *NbTagByte,uint8_t *TagReplyDataIndex,uint8_t *NbControlByte,uint8_t *ControlIndex)
{	
	// output parameters initialization
	*NbTagByte= 0;
	*ResultCode = CR95HF_ERROR_CODE;

	if (  CR95HF_IsCommandExists(CmdCodeToReader) != CR95HF_SUCCESS_CODE)
		return CR95HF_ERRORCODE_COMMANDUNKNOWN;
	
	*ResultCode = CR95HF_SUCCESS_CODE;

	// the ECHO and Baud rate commands reply with a pseudo reponse (0x55)
	if ((CmdCodeToReader == ECHO) || (CmdCodeToReader == BAUD_RATE))
	{
		//memcpy(TagReplyData,&ReaderReply[PSEUDOREPLY_OFFSET],PSEUDOREPLY_LENGTH);
		*TagReplyDataIndex = ECHOREPLY_OFFSET;
		*NbTagByte = ECHOREPLY_LENGTH;
	}
	else if (CR95HF_IsReaderResultCodeOk (CmdCodeToReader,ReaderReply)== CR95HF_SUCCESS_CODE)
	{
		// reply 00 00 (Length = 0)
		if ( ( ReaderReply[READERREPLY_STATUSOFFSET] & 0x80 == 0) &&
			 (ReaderReply[READERREPLY_STATUSOFFSET+1] == 0x00))
		{
			//memcpy(TagReplyData,&ReaderReply[CR95HF_DATA_OFFSET-1],1);
			*TagReplyDataIndex = CR95HF_DATA_OFFSET;
			*NbTagByte = 1;
		}
		// case (Length > 0)
		else		
		{
			*NbControlByte = GetNbControlByte (ProtocolSelected);
			*TagReplyDataIndex = CR95HF_DATA_OFFSET;
			*ControlIndex = CR95HF_DATA_OFFSET+*NbTagByte;
			*NbTagByte = ReaderReply[CR95HF_LENGTH_OFFSET]-*NbControlByte;
		}
	}
	// Error Code
	else
	{
	 	//memcpy(TagReplyData,&ReaderReply[CR95HF_DATA_OFFSET-2],ERRORCODE_LENGTH);
		*TagReplyDataIndex = CR95HF_DATA_OFFSET-2;
		*NbTagByte = ERRORCODE_LENGTH;
		*ResultCode = CR95HF_ERROR_CODE;
	}
	
	return CR95HF_SUCCESS_CODE;
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

