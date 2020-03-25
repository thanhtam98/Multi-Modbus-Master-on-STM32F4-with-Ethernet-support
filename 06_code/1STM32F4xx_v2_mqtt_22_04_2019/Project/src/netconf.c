/**
  ******************************************************************************
  * @file    netconf.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    31-October-2011
  * @brief   Network connection configuration
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; Portions COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */
/**
  ******************************************************************************
  * <h2><center>&copy; Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.</center></h2>
  * @file    netconf.c
  * @author  CMP Team
  * @version V1.0.0
  * @date    28-December-2012
  * @brief   Network connection configuration     
  *          Modified to support the STM32F4DISCOVERY, STM32F4DIS-BB and
  *          STM32F4DIS-LCD modules. 
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, Embest SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
  * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
  * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "lwip/mem.h"
#include "lwip/memp.h"
#include "lwip/dhcp.h"
#include "ethernetif.h"
#include "main.h"
#include "netconf.h"
#include "tcpip.h"
#include "TaskMessage.h"

/* Private typedef -----------------------------------------------------------*/
typedef enum 
{ 
  DHCP_START=0,
  DHCP_WAIT_ADDRESS,
  DHCP_ADDRESS_ASSIGNED,
  DHCP_TIMEOUT
} 
DHCP_State_TypeDef;
/* Private define ------------------------------------------------------------*/
#define MAX_DHCP_TRIES 5

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif xnetif; /* network interface structure */
char *ucIP,*ucGW, *ucNM;
extern xQueueHandle xQueuemessage;
extern xQueueHandle xQueueState;
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes the lwIP stack
  * @param  None
  * @retval None
  */
void LwIP_Init(char *Ip,char *Netmask,char *Gateway,char Dhcp)
{
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  
  ucIP = Ip;
  ucGW = Gateway;
  ucNM = Netmask;
	if( Dhcp==0)
	{	
		uint8_t iptab[4];
		uint8_t iptxt[20];
	}
  /* Create tcp_ip stack thread */
  tcpip_init( NULL, NULL );	

  /* IP address setting & display on STM32_evalboard LCD*/
	if( Dhcp)
	{
		ipaddr.addr = 0;
		netmask.addr = 0;
		gw.addr = 0;
	}
	else
	{
		IP4_ADDR(&ipaddr, Ip[0], Ip[1], Ip[2], Ip[3]);
		IP4_ADDR(&netmask, Netmask[0], Netmask[1] , Netmask[2], Netmask[3]);
		IP4_ADDR(&gw, Gateway[0], Gateway[1], Gateway[2], Gateway[3]);
	}

  /* - netif_add(struct netif *netif, struct ip_addr *ipaddr,
            struct ip_addr *netmask, struct ip_addr *gw,
            void *state, err_t (* init)(struct netif *netif),
            err_t (* input)(struct pbuf *p, struct netif *netif))
    
   Adds your network interface to the netif_list. Allocate a struct
  netif and pass a pointer to this structure as the first argument.
  Give pointers to cleared ip_addr structures when using DHCP,
  or fill them with sane numbers otherwise. The state pointer may be NULL.

  The init function pointer must point to a initialization function for
  your ethernet netif interface. The following code illustrates it's use.*/

  netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);

 /*  Registers the default network interface. */
  netif_set_default(&xnetif);

 /*  When the netif is fully configured this function must be called.*/
  netif_set_up(&xnetif); 
}

/**
  * @brief  LwIP_DHCP_Process_Handle
  * @param  None
  * @retval None
  */
void LwIP_DHCP_task(void * pvParameters)
{
	xQueueMessage xqueuemessage;
  struct ip_addr ipaddr;
  struct ip_addr netmask;
  struct ip_addr gw;
  uint32_t IPaddress;
	uint32_t uDhcp_timeout =0;
  uint8_t DHCP_state;
  uint8_t SysState;
	
	xqueuemessage.DhcpMessage.TaskHandle = xTaskGetHandle(DHCP_TASKNAME);
	xqueuemessage.DhcpMessage.Message_type = TASK_EVENT;
  DHCP_state = DHCP_START;
	
  for (;;)
  {
		xQueuePeek( xQueueState, &SysState, 0 );
    switch (DHCP_state)
    {
      case DHCP_START:
      {
        if(dhcp_start(&xnetif)==ERR_OK)
				{
					xnetif.ip_addr.addr = 0;
					IPaddress = 0;
					DHCP_state = DHCP_WAIT_ADDRESS;
				}
				xqueuemessage.DhcpMessage.value = DHCP_GETIP_REQ;
				xQueueSend(xQueuemessage,&xqueuemessage,0);
      }
      break;

      case DHCP_WAIT_ADDRESS:
      {
        /* Read the new IP address */
        IPaddress = xnetif.ip_addr.addr;
				uDhcp_timeout =0;
        if (IPaddress!=0) 
				{
          ucIP[3] = IPaddress>>24;
          ucIP[2] = IPaddress>>16;
          ucIP[1] = IPaddress>>8;
          ucIP[0] = IPaddress&0xFF;
          ucGW[3] = xnetif.gw.addr >> 24;
          ucGW[2] = xnetif.gw.addr >> 16;
          ucGW[1] = xnetif.gw.addr >> 8;
          ucGW[0] = xnetif.gw.addr & 0xFF;
          ucNM[3] = xnetif.netmask.addr >> 24;
          ucNM[2] = xnetif.netmask.addr >> 16;
          ucNM[1] = xnetif.netmask.addr >> 8;
          ucNM[0] = xnetif.netmask.addr & 0xFF;
          DHCP_state = DHCP_ADDRESS_ASSIGNED;	
					uDhcp_timeout = xnetif.dhcp->offered_t1_renew;
          /* Stop DHCP */
          dhcp_stop(&xnetif);					
					xqueuemessage.DhcpMessage.value = DHCP_GETIP_OK;
					if(SysState == SYS_GET_IP)
						xQueueSend(xQueuemessage,&xqueuemessage,0);
        } 
				else 
				{
          /* DHCP timeout */
          if (xnetif.dhcp->tries > MAX_DHCP_TRIES) 
					{
            DHCP_state = DHCP_TIMEOUT;
            /* Stop DHCP */
            dhcp_stop(&xnetif);
						xqueuemessage.DhcpMessage.value = DHCP_GETIP_TIMEOUT;
						xQueueSend(xQueuemessage,&xqueuemessage,0);
          }
        }
      }
      break;
			case DHCP_ADDRESS_ASSIGNED:
			{
				if(uDhcp_timeout)
				{
					uDhcp_timeout --;
					/* wait 750 ms */
					vTaskDelay(700);
				}
				else
				{
					DHCP_state = DHCP_START;
				}
			}
			break;
			case DHCP_TIMEOUT:
			{
				/* wait 750 ms */
				vTaskDelay(2000);
				DHCP_state = DHCP_START;
			}
			break;
      default: break;
    }
    /* wait 250 ms */
    vTaskDelay(250);
  }   
}

/*********** Portions COPYRIGHT 2012 Embest Tech. Co., Ltd.*****END OF FILE****/
