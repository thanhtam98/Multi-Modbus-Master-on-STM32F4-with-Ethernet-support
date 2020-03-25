/*
 * RS485bus Libary: Atmel AT91SAM3S Demo Application
 * Copyright (C) 2010 Christian Walter <cwalter@embedded-solutions.at>
 *
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * IF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: portother_m.c,v 1.1 2010/06/05 09:57:48 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include <stm32f4xx_conf.h>
#include "rs485.h"
#include "rs485port.h"

#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
/* define compiler specific symbols */
#if defined   ( __CC_ARM   )
  #define __ASM            __asm           /*!< asm keyword for armcc           */
  #define __INLINE         __inline        /*!< inline keyword for armcc        */

#elif defined ( __ICCARM__ )
  #define __ASM           __asm            /*!< asm keyword for iarcc           */
  #define __INLINE        inline           /*!< inline keyword for iarcc. Only avaiable in High optimization mode! */

#elif defined (  __GNUC__  )
  #define __ASM             __asm          /*!< asm keyword for gcc            */
  #define __INLINE          inline         /*!< inline keyword for gcc         */

#elif defined   (  __TASKING__  )
  #define __ASM            __asm           /*!< asm keyword for TASKING Compiler          */
  #define __INLINE         inline          /*!< inline keyword for TASKING Compiler       */

#endif

/* ----------------------- Static variables ---------------------------------*/
static UCHAR    xOriginalState;
static ULONG    ulNesting;

/* ----------------------- Cortex-M3 PRIMASK enable/disable -----------------*/
/* Gregory Nutt <spudmonkey@racsa.co.cr> */

/* Get/set the primask register to control global DI/EI*/

static __INLINE uint8_t getprimask(void)
{
  uint32_t result=0;
  result = __get_PRIMASK();
  return((uint8_t)result);
}

static __INLINE void setprimask(uint32_t primask)
{
  __set_PRIMASK(primask);
}
/* ----------------------- Critical Section handler -----------------------------*/
/* Check if PRIMASK was set, and disable it on entry. On exit, if it was originally
set then set it back again otherwise leave it unset. */
void
vRS485PortEnterCritical( void )
{
    if(ulNesting == 0)
    {
			xOriginalState = getprimask();
    }
    ulNesting++;
		setprimask(1);
}

void
vRS485PortExitCritical( void )
{
    ulNesting--;
    if(ulNesting == 0)
    {
			setprimask(xOriginalState);
		}
}

/* ----------------------- Close Ports -----------------------------*/
void
vPortClose( UCHAR ucPort )
{
    extern void vMasterPortClose( UCHAR ucPort );
    extern void vMasterPortTimersDisable( UCHAR ucPort );
    vMasterPortClose( ucPort );
    vMasterPortTimersDisable( ucPort );
}

#endif
