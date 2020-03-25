/**
  ******************************************************************************
  * @file    rfid_reader.h 
  * @author  Biogen Application Team
  * @version V0.1
  * @date    15/03/2017
  * @brief   API for Rfid reader
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
*/ 
#ifndef __RFID_READER__
#define __RFID_READER__
#include "lib_CR95HF.h"
#include "lib_iso14443A.h"
#include "lib_iso14443B.h"

void Rfid_SerialInterfaceInitialization (ReaderConfigStruct ReaderConfig);
int8_t User_IsCR95HF (void);
int8_t User_SelectSequence(uint8_t *SAKByte);
int8_t User_Get14443ATag (uint8_t* UIDout,uint8_t* NbUIDByte );
int8_t User_Get14443BTag (void);
int8_t User_Get14443SRTag (void);
int8_t ISO14443A_AntiCollisionLevel1Loop( uint8_t *pResponse, uint8_t NVBByte,  const uint8_t NbByteUIDvalid, const uint8_t *UIDvalid, uint8_t NbSignificantBit );
#endif
