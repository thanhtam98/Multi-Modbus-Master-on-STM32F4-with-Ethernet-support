/**
  ******************************************************************************
  * @file    drv_CR95HF.h 
  * @author  MMY Application Team
  * @version V1.2
  * @date    01/07/2011
  * @brief   CR95HF driver header.
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _LIB_CR95HF_H
#define _LIB_CR95HF_H

/* Includes ------------------------------------------------------------------*/
#include "drv_CR95HF.h"

/* CR95HF HEADER command definition ---------------------------------------------- */
#define CR95HF_COMMAND_SEND							0x00
#define CR95HF_COMMAND_RESET						0x01
#define CR95HF_COMMAND_RECEIVE						0x02
#define CR95HF_COMMAND_POLLING						0x03

/* CR95HF command definition */
#define IDN											0x01
#define PROTOCOL_SELECT 							0x02
#define POLL_FIELD 									0x03
#define SEND_RECEIVE								0x04
#define LISTEN										0x05
#define SEND										0x06
#define IDLE										0x07
#define READ_REGISTER								0x08
#define WRITE_REGISTER								0x09
#define BAUD_RATE									0x0A
#define SUB_FREQ_RES								0x0B
#define AC_FILTER									0x0D
#define TEST_MODE									0x0E
#define SLEEP_MODE									0x0F
#define ECHO										0x55
#define ECHORESPONSE								0x55
// customs command
#define CR95HF_GETINTERFACEPINSTATE       			0xBA
#define CR95HF_SETUSBDISCONNECTSTATE      			0xBB
#define CR95HF_GETMCUVERSION						0xBC
#define CR95HF_RESETSEQUENCE						0xBD
#define CR95HF_PULSE_IRQIN							0xBE

						   	
#define SUCCESFUL_SENDS								0x80
#define SUCCESFUL_COMMAND							0x00
//#define COMMAND_FAILED

// CR95HF status
#define CR95HF_SUCCESS_CODE							0x00
#define CR95HF_NOREPLY_CODE							0x01

// CR95HF polling status
#define CR95HF_POLLING_CR95HF						0x00
#define CR95HF_POLLING_TIMEOUT						0x01

// error code
#define	CR95HF_ERRORCODE_DEFAULT					0xFE
#define	CR95HF_ERRORCODE_TIMEOUT					0xFD
#define	CR95HF_ERRORCODE_UARTDATARATEUNCHANGED		0xFC
#define	CR95HF_ERRORCODE_UARTDATARATEPROCESS		0xFB
#define CR95HF_ERROR_CODE							0x40
#define CR95HF_ERRORCODE_PARAMETERLENGTH			0x41
#define CR95HF_ERRORCODE_PARAMETER					0x42
#define CR95HF_ERRORCODE_COMMANDUNKNOWN				0x43

/* Nb of bytes of reader commands */
#define SELECT_BUFFER_SIZE							6
#define SENDRECV_BUFFER_SIZE						257
#define SEND_BUFFER_SIZE							257
#define IDLE_BUFFER_SIZE							16
#define RDREG_BUFFER_SIZE							5
#define WRREG_BUFFER_SIZE							257
#define BAUDRATE_BUFFER_SIZE						3
#define SUBFREQRES_BUFFER_SIZE						2
#define ACFILTER_BUFFER_SIZE						19
#define TESTMODE_BUFFER_SIZE						4
#define SLEEPMODE_BUFFER_SIZE						4
/* Nb of bytes of reader response */
#define CR95HF_RESPONSEBUFFER_SIZE		 			255


/* mask of steclect commnan */
#define CR95HF_SELECTMASK_DATARATE					0x30
#define CR95HF_SELECTMASK_SUBCARRIER				0x02
#define CR95HF_SELECTMASK_WAITORSOF					0x08


/* protocol allowed */
#define PROTOCOL_TAG_FIELDOFF						0x00
#define PROTOCOL_TAG_ISO15693						0x01
#define PROTOCOL_TAG_ISO14443A				  0x02
#define PROTOCOL_TAG_ISO14443B					0x03
#define PROTOCOL_TAG_FELICA							0x04
#define PROTOCOL_CARD_ISO14443A					0x12
#define PROTOCOL_CARD_ISO14443B					0x13
#define PROTOCOL_CARD_FELICA						0x14


/* CR95HF command name definition */
#define IDN_CMDNAME									    "IDN"
#define PROTOCOL_SELECT_CMDNAME					"Protocol select"
#define SEND_RECEIVE_CMDNAME						"Send/Receive"
#define IDLE_CMDNAME								    "Idle"
#define READ_REGISTER_CMDNAME						"Read register"
#define WRITE_REGISTER_CMDNAME					"Write register"
#define BAUD_RATE_CMDNAME							  "Baud rate"
#define ECHO_CMDNAME								    "echo"
#define ECHO_CMDUNKOWN								  "Cmd unknown"
#define MAXLENGTH_CMDNAME							  16


// === error code description ===
#define PROTOCOLSELECT_ERRORDESC_MAXLENGTH			48
	// protocol select status
#define PROTOCOLSELECT_ERRORDESC_CMDLENGTH			"Invalid command length"
#define PROTOCOLSELECT_ERRORDESC_INVALID			  "Invalid protocol"
//  send receive field status
#define SENDRECV_ERRORDESC_COMERROR					    "communication error"
#define SENDRECV_ERRORDESC_FRAMEWAIT				    "No tag"
#define SENDRECV_ERRORDESC_SOF						      "Invalid SOF"
#define SENDRECV_ERRORDESC_OVERFLOW					    "Receive buffer overflow"
#define SENDRECV_ERRORDESC_FRAMING					    "framing error"
#define SENDRECV_ERRORDESC_EGT						      "EGT time out"
#define SENDRECV_ERRORDESC_LENGTH					      "Invalid length (Felica case)"
#define SENDRECV_ERRORDESC_CRC				       		"CRC error (Felica case)"
#define SENDRECV_ERRORDESC_RECEPTIONLOST			  "Reception lost without EOF"
//  Listen command field status
#define LISTEN_ERRORDESC_CMDLENGTH	    				"Invalid command length"
#define LISTEN_ERRORDESC_PROTOCOL				      	"Invalid protocol"
#define LISTEN_ERRORDESC_NOFIELD					      "No field"
#define LISTEN_ERRORDESC_COM						        "Communication error"
#define LISTEN_ERRORDESC_CANCELED					      "listening mode was canceled by the application"
#define LISTEN_ERRORDESC_SOF						        "Invalid SOF"
#define LISTEN_ERRORDESC_OVERFLOW					      "Receive buffer overflow"
#define LISTEN_ERRORDESC_FRAMING					      "Framing error"
#define LISTEN_ERRORDESC_EGT						        "EGT time out"
#define LISTEN_ERRORDESC_CRC						        "CRC error (Felica case)"
#define LISTEN_ERRORDESC_RECEPTIONLOST				  "Reception lost without EOF"
//  Send command field status
#define SEND_ERRORDESC_LENGTH					         	"Invalid length"
#define SEND_ERRORDESC_PROTOCOL						      "Invalid protocol"
//  Idle command field status
#define IDLE_ERRORDESC_LENGTH	        					"Invalid command length"
//  read register command field status
#define READREG_ERRORDESC_LENGTH					      "Invalid command length"
//  AC filter command field status
#define ACFILTER_ERRORDESC_LENGTH					      "Invalid command length"

	
/* Offset definitions for global buffers */
#define CR95HF_COMMAND_OFFSET						0x00
#define CR95HF_LENGTH_OFFSET						0x01
#define CR95HF_DATA_OFFSET							0x02

#define TAGREPPLY_OFFSET_UID						DATA_OFFSET+0x02

#define ECHOREPLY_OFFSET							0x00
#define ECHOREPLY_LENGTH							0x01

#define READERREPLY_STATUSOFFSET					0x00

#define IDN_RESULTSCODE_OK							0x00
// protocol select status
#define PROTOCOLSELECT_LENGTH						0x02
#define PROTOCOLSELECT_RESULTSCODE_OK				0x00
#define PROTOCOLSELECT_ERRORCODE_CMDLENGTH			0x82
#define PROTOCOLSELECT_ERRORCODE_INVALID			0x83
// baud rate command
#define	BAUDRATE_LENGTH						  		0x01
#define	BAUDRATE_DATARATE_DEFAULT			  		57600
//  poll field status
#define POLLFIELD_RESULTSCODE_OK					0x00
//  send receive field status
#define SENDRECV_RESULTSCODE_OK						0x80
#define SENDRECV_ERRORCODE_COMERROR					0x86
#define SENDRECV_ERRORCODE_FRAMEWAIT				0x87
#define SENDRECV_ERRORCODE_SOF						0x88
#define SENDRECV_ERRORCODE_OVERFLOW					0x89
#define SENDRECV_ERRORCODE_FRAMING					0x8A
#define SENDRECV_ERRORCODE_EGT						0x8B
#define SENDRECV_ERRORCODE_LENGTH					0x8C
#define SENDRECV_ERRORCODE_CRC						0x8D
#define SENDRECV_ERRORCODE_RECEPTIONLOST			0x8E


#define SENDRECV_ERRORCODE_SOFT						0xFF


//  Listen command field status
#define LISTEN_RESULTSCODE_OK						0x00
#define LISTEN_ERRORCODE_CMDLENGTH					0x82
#define LISTEN_ERRORCODE_PROTOCOL					0x83
#define LISTEN_ERRORCODE_NOFIELD					0x8F
#define LISTEN_ERRORCODE_COM						0x86
#define LISTEN_ERRORCODE_CANCELED					0x87
#define LISTEN_ERRORCODE_SOF						0x88
#define LISTEN_ERRORCODE_OVERFLOW					0x89
#define LISTEN_ERRORCODE_FRAMING					0x8A
#define LISTEN_ERRORCODE_EGT						0x8B
#define LISTEN_ERRORCODE_CRC						0x8D
#define LISTEN_ERRORCODE_RECEPTIONLOST				0x8E
//  Send command field status
#define SEND_RESULTSCODE_OK							0x00
#define SEND_ERRORCODE_LENGTH						0x82
#define SEND_ERRORCODE_PROTOCOL						0x83
//  Idle command field status
#define IDLE_RESULTSCODE_OK							0x00
#define IDLE_ERRORCODE_LENGTH						0x82
//  read register command field status
#define READREG_RESULTSCODE_OK						0x00
#define READREG_ERRORCODE_LENGTH					0x82
//  write register command field status
#define WRITEREG_RESULTSCODE_OK						0x00
//  Baud rate command field status
#define BAUDRATE_RESULTSCODE_OK						0x55
//  AC filter command field status
#define ACFILTER_RESULTSCODE_OK						0x00
#define ACFILTER_ERRORCODE_LENGTH					0x82
//  sub freq command field status
#define SUBFREQ_RESULTSCODE_OK						0x00
//  Test mode command field status
#define TESTMODE_RESULTSCODE_OK						0x00

/* control byte according to protocl*/
#define CONTROL_MAX_NBBYTE							0x03
#define CONTROL_15693_NBBYTE						0x01
#define CONTROL_15693_CRCMASK						0x02
#define CONTROL_15693_COLISIONMASK					0x01

#define CONTROL_14443A_NBBYTE						0x03
#define CONTROL_14443A_COLISIONMASK					0x80
#define CONTROL_14443A_CRCMASK						0x20
#define CONTROL_14443A_PARITYMASK					0x10
#define CONTROL_14443A_NBSIGNIFICANTBITMASK			0x0F
#define CONTROL_14443A_FIRSTCOLISIONBITMASK			0x0F

#define CONTROL_14443B_NBBYTE						0x01
#define CONTROL_14443B_CRCMASK						0x02
#define CONTROL_14443B_COLISIONMASK					0x01

#define CONTROL_FELICA_NBBYTE						0x01
#define CONTROL_FELICA_CRCMASK						0x02
#define CONTROL_FELICA_COLISIONMASK					0x01

// get MCU verion
#define MCUVERSION_STATUS							0x00
#define MCUVERSION_LENGTH							0x03
#define MCUVERSION_FIRSTNUMBER						1	// stand for 1.2.0
#define MCUVERSION_SECONDNUMBER						2
#define MCUVERSION_THIRDNUMBER						0


#define ERRORCODE_LENGTH							0x01

#define DUMMY_BYTE									0xFF
#define MAX_BUFFER_SIZE  							256 

#define CR95HF_FLAG_DATA_READY						0x08
#define CR95HF_FLAG_DATA_READY_MASK					0xF8


typedef struct {
	CR95HF_INTERFACE 		Interface;
	SPI_MODE 				SpiMode;
	int8_t					CurrentProtocol;
}ReaderConfigStruct;


/* Functions ---------------------------------------------------------------- */

//  command prototypes
int8_t HexCommandToStringCommand		(uint8_t HexBuffer,uint8_t *StringCommand,uint8_t *CmdNameNbByte);
int8_t CR95HF_IsReaderResultCodeOk 			(uint8_t CmdCode,const uint8_t *ReaderReply);
int8_t CR95HF_IsReaderErrorCode 				(uint8_t CmdCode,uint8_t *ReaderReply);
int8_t CR95HF_IsCommandExists					(uint8_t CmdCode);
int8_t CR95HF_GetReaderErrorCode 		(const uint8_t CmdCode,const uint8_t *ReaderReply,char *ErrorDescription);

int8_t SplitReaderReply (uint8_t CmdCodeToReader,uint8_t ProtocolSelected,const uint8_t *ReaderReply,uint8_t *ResultCode,uint8_t *NbTagByte,uint8_t *TagReplyData,uint8_t *NbControlByte,uint8_t *ControlByte);

int8_t 	SPIUART_SendReceive				(const uint8_t *pCommand, uint8_t *pResponse);
void 	SPIUART_Send					(const uint8_t *pCommand);
int8_t 	CR95HF_CheckSendReceive(const uint8_t *pCommand, uint8_t *pResponse);

int8_t InitCR95HFConfigStructure (ReaderConfigStruct *ReaderConfig);

int8_t CR95HF_FieldOff					(void);


int8_t CR95HF_IDN			(uint8_t *pResponse);
int8_t CR95HF_Echo			(uint8_t *pResponse);
int8_t CR95HF_SendEOF		(uint8_t *pResponse);
int8_t CR95HF_ProtocolSelect(const uint8_t Length,const uint8_t Protocol,const uint8_t *Parameters,uint8_t *pResponse);
int8_t CR95HF_SendRecv		(const uint8_t Length,const uint8_t *Parameters,uint8_t *pResponse);
int8_t CR95HF_Idle			(const uint8_t Length,const uint8_t *Data,uint8_t *pResponse);
int8_t CR95HF_RdReg			(const uint8_t Length,const uint8_t Address,const uint8_t RegCount,const uint8_t Flags,uint8_t *pResponse);
int8_t CR95HF_WrReg 		(const uint8_t Length,const uint8_t Address,const uint8_t Flags,const uint8_t *pData,uint8_t *pResponse);
int8_t CR95HF_BaudRate 		(const uint8_t BaudRate,uint8_t *pResponse);

void CR95HF_SendCustomCommand	(const uint8_t *pCommand, uint8_t *pResponse);
void CR95HF_GetMCUversion		(uint8_t *pResponse);
void CR95HF_GetInterfaceBus 	(uint8_t *pResponse);
void CR95HF_SetUSBDisconnectPin (const uint8_t *pCommand);

int8_t CR95HF_ChangeUARTBaudRate (const uint8_t *pCommand, uint8_t *pResponse);
void SpyTransaction(const uint8_t *pCommand, uint8_t *pResponse);
/**
  * @}
  */

#endif /* __CR95HF_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/



