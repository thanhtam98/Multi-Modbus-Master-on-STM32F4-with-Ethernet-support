/* 
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: rs485config.h,v 1.14 2006/12/07 22:10:34 wolti Exp $
 *       $Id: rs485config.h,v 1.60 2013/08/13 21:19:55 Armink Add Master Functions $
 */

#ifndef _RS485_CONFIG_H
#define _RS485_CONFIG_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
/* ----------------------- Defines ------------------------------------------*/
/*! \defgroup rs485bus_cfg RS485 Configuration
 *
 * Most modules in the protocol stack are completly optional and can be
 * excluded. This is specially important if target resources are very small
 * and program memory space should be saved.<br>
 *
 * All of these settings are available in the file <code>rs485config.h</code>
 */
/*! \addtogroup rs485bus_cfg
 *  @{
 */
 /*! \brief If RS485 Master ASCII support is enabled. */
#define RS485_MASTER_ASCII_ENABLED              (  0 )
/*! \brief If RS485 Master RTU support is enabled. */
#define RS485_MASTER_RTU_ENABLED                (  0 ) 
/*! \brief If RS485 Slave ASCII support is enabled. */
#define RS485_SLAVE_ASCII_ENABLED               (  0 )
/*! \brief If RS485 Slave RTU support is enabled. */
#define RS485_SLAVE_RTU_ENABLED                 (  1 )
/*! \brief Maximum number of RS485 Bus functions codes the protocol stack
 *    should support.
 *
 * The maximum number of supported RS485 Bus functions must be greater than
 * the sum of all enabled functions in this file and custom function
 * handlers. If set to small adding more functions will fail.
 */
 /*! \brief If the <em>Request check</em> function should be enabled. */
#define RS485_FUNC_CHECK              ( 1 )
/*! \brief If the <em>Request Ping</em> function should be enabled. */
#define RS485_FUNC_PERIODIC_PING      ( 0 )
/*! \brief If the <em>Tamper Detect Request</em> function should be enabled. */
#define RS485_FUNC_TAMPER_DETECT      ( 0 )
/*! \brief If the <em>NFC Detect Request</em> function should be enabled. */
#define RS485_FUNC_NFC_DETECT      		( 1 )
/*! \brief If the <em>Key Enter</em> function should be enabled. */
#define RS485_FUNC_KEY_ENTER          ( 1 )

/*! @} */
#ifdef __cplusplus
    PR_END_EXTERN_C
#endif
#if RS485_MASTER_RTU_ENABLED > 0 || RS485_MASTER_ASCII_ENABLED > 0
/*! \brief If master send a broadcast frame,the master will wait time of convert to delay,
 * then master can send other frame */
#define RS485_MASTER_DELAY_MS_CONVERT              (200 )
/*! \brief If master send the frame,the master will wait sometime for slave.
 * And if slave is not respond in this time,the master will process this timeout error.
 * Then master can send other frame */
#define RS485_MASTER_TIMEOUT_MS_RESPOND            (100 )


#endif

#define RS485_ASCII_TIMEOUT_SEC                     (1)
#endif

