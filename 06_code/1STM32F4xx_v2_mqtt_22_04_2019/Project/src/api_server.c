#include <string.h>
#include <stdlib.h>
#include "lwip/inet.h"
#include "api_server.h"
#include "stm32f4xx_flash.h"
#include "rtc.h"
#include "hw_config.h"


extern Device_Lan_t ip_lan;
extern User_t user;
extern Host_t host;
extern CTPORT_TypeDef CTport;
extern unsigned char ucFlashUpdate;
unsigned char ucReadProgram = 0;
unsigned char ucWriteProgram =0;
unsigned char ucSystemReboot =0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief Write to Flash  
  * @param None
  * @retval None
  */

char Flash_Write_Information(void)
{
	unsigned int Address;
	uint16_t data;
	uint32_t i;
  
	FLASH_Unlock();
	FLASH_ClearFlag( FLASH_FLAG_EOP |  FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);
	while(FLASH_EraseSector(FLASH_SECTOR_INFORMATION,VoltageRange_3)!= FLASH_COMPLETE);
	//write to flash static ip of lan
	Address = ADD_STATIC_IP;
	FLASH_ProgramWord(Address, ip_lan.IP.idata);
	// write to flash subnetmask
	Address = ADD_SUBNETMASK;
	FLASH_ProgramWord(Address, ip_lan.Subnetmask.idata);
	// write to flash gateway
	Address = ADD_GATEWAY;
	FLASH_ProgramWord(Address, ip_lan.gateway.idata);
	// write to flash the dhcp define
	Address = ADD_DHCP;
	FLASH_ProgramWord(Address, ip_lan.dhcp);
	// write to flash username
	Address = ADD_USER_NAME;
	for(i=0;i<3;i++)
	{
		FLASH_ProgramWord(Address, user.UserName[i].idata);
		Address = Address+4;
	}
	// write to flash password
	Address = ADD_PASS_WORD;
	for(i=0;i<3;i++)
	{
		FLASH_ProgramWord(Address, user.PassWord[i].idata);
		Address = Address+4;
	}
	// write to flash host ip
	Address = ADD_HOST_IP;
	FLASH_ProgramWord(Address,host.IP.idata);
	// write to flash host name
	Address = ADD_HOST_NAME;
	for(i=0;i<16;i++)
	{
		FLASH_ProgramWord(Address, host.Host_Name[i].idata);
		Address = Address+4;
	}
	// write to flash the device name
	Address = ADD_DEVICE_NAME;
	for(i=0;i<3;i++)
	{
		FLASH_ProgramWord(Address, host.Device_ID[i].idata);
		Address = Address+4;
	}
    // write to flash the hot username
	Address = ADD_HOST_USERNAME;
	for(i=0;i<3;i++)
	{
		FLASH_ProgramWord(Address, host.Host_Username[i].idata);
		Address = Address+4;
	}
     // write to flash the hot password
	Address = ADD_HOST_PASSWORD;
	for(i=0;i<3;i++)
	{
		FLASH_ProgramWord(Address, host.Host_Password[i].idata);
		Address = Address+4;
	}
    
  // write to flash the port config
  for(i=0;i<CTport.number;i++)
	{
		Address = (unsigned int) (&CONTROL_PORT->Port[i].type);
		data = (CTport.Port[i].active<<8)|CTport.Port[i].type;
		FLASH_ProgramHalfWord(Address,data);
	}
	FLASH_Lock();
	return TRUE;
}

/**
  * @brief Read from Flash  
  * @param None
  * @retval None
  */
void Host_read_flash(void)
{
	char i;
	uint32_t *point;
  
  if(ucWriteProgram)
  {
    ucReadProgram =1;
    return;
  }
	// Read from flash the ip of host
	point = (uint32_t*) ADD_HOST_IP;
	host.IP.idata = *point;
	// Read from flash the host name
	point = (uint32_t*) ADD_HOST_NAME;
	for(i=0;i<16;i++)
	{
		host.Host_Name[i].idata = *point;
		point++;
	}
	// Read from flash the device name
	point =  (uint32_t*)ADD_DEVICE_NAME;
	for(i=0;i<3;i++)
	{
		host.Device_ID[i].idata = *point;
		point++;
	}
    // Read from flash the username
	point =  (uint32_t*)ADD_HOST_USERNAME;
	for(i=0;i<3;i++)
	{
		host.Host_Username[i].idata = *point;
		point++;
	}
    // Read from flash the password
	point =  (uint32_t*)ADD_HOST_PASSWORD;
	for(i=0;i<3;i++)
	{
		host.Host_Password[i].idata = *point;
		point++;
	}
}

void User_read_flash (void)
{
	char i;
	uint32_t *point;
  
  if(ucWriteProgram)
  {
    ucReadProgram =1;
    return;
  }
  
	point =(uint32_t*) ADD_USER_NAME;
	// read from flash the username
	for(i=0;i<3;i++)
	{
		user.UserName[i].idata = *point;
		point++;
	}
	// read from flash the password
	point =  (uint32_t*)ADD_PASS_WORD;
	for(i=0;i<3;i++)
	{
		user.PassWord[i].idata = *point;
		point++;
	}
}

void Lan_read_flash(void)
{
  
  if(ucWriteProgram)
  {
    ucReadProgram =1;
    return;
  }
	// Read from flash the ip of lan
	ip_lan.IP.idata = *((uint32_t*)ADD_STATIC_IP);
	// Read from flash the subnetmask
	ip_lan.Subnetmask.idata= *((uint32_t*)ADD_SUBNETMASK);
	// Read from flash the gateway
	ip_lan.gateway.idata = *((uint32_t*)ADD_GATEWAY);
	// Read from flash the DHCP define
	ip_lan.dhcp = *((uint32_t*)ADD_DHCP);
}

void Port_read_flash(void)
{
	char i;
  
  if(ucWriteProgram)
  {
    ucReadProgram =1;
    return;
  }
	// write to flash the port config
	CTport.number = PORT_NUMBER;
	for(i=0;i<CTport.number;i++)
	{
			CTport.Port[i].type = CONTROL_PORT->Port[i].type;
			CTport.Port[i].active = CONTROL_PORT->Port[i].active;
	}
}

void Api_lan_config(char *recv_buffer)
{
    char *Pointer;
    // read flash
    Host_read_flash();
    Lan_read_flash();
    User_read_flash();
    Port_read_flash();
    // Check config device ip1
    Pointer = strstr((char *)recv_buffer,DEVICE_IP1);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.IP.cdata[0] = atoi(Pointer);
        }
    }
    // Check config device ip2
    Pointer = strstr((char *)recv_buffer,DEVICE_IP2);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.IP.cdata[1] = atoi(Pointer);
        }
    }
    // Check config device ip3
    Pointer = strstr((char *)recv_buffer,DEVICE_IP3);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.IP.cdata[2] = atoi(Pointer);
        }
    }
    // Check config device ip4
    Pointer = strstr((char *)recv_buffer,DEVICE_IP4);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.IP.cdata[3] = atoi(Pointer);
        }
    }
    // Check config device mask1
    Pointer = strstr((char *)recv_buffer,DEVICE_MASK1);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.Subnetmask.cdata[0] = atoi(Pointer);
        }
    }
    // Check config device mask2
    Pointer = strstr((char *)recv_buffer,DEVICE_MASK2);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.Subnetmask.cdata[1] = atoi(Pointer);
        }
    }
    // Check config device mask3
    Pointer = strstr((char *)recv_buffer,DEVICE_MASK3);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.Subnetmask.cdata[2] = atoi(Pointer);
        }
    }
    // Check config device mask4
    Pointer = strstr((char *)recv_buffer,DEVICE_MASK4);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.Subnetmask.cdata[3] = atoi(Pointer);
        }
    }
    // Check config device gateway1
    Pointer = strstr((char *)recv_buffer,DEVICE_GATEWAY1);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.gateway.cdata[0] = atoi(Pointer);
        }
    }
    // Check config device gateway2
    Pointer = strstr((char *)recv_buffer,DEVICE_GATEWAY2);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.gateway.cdata[1] = atoi(Pointer);
        }
    }
    // Check config device gateway3
    Pointer = strstr((char *)recv_buffer,DEVICE_GATEWAY3);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.gateway.cdata[2] = atoi(Pointer);
        }
    }
    // Check config device gateway4
    Pointer = strstr((char *)recv_buffer,DEVICE_GATEWAY4);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.gateway.cdata[3] = atoi(Pointer);
        }
    }
    // Check network config DHCP
    Pointer = strstr((char *)recv_buffer,DEVICE_DHCP);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            ip_lan.dhcp = atoi(Pointer);
        }
    }
    //Flash_Write_Information();
    ucWriteProgram = 1;
}

void Web_lan_config(char *recv_buffer)
{
    char *Pointer,*Ptend;
    char temp[30];
    // read flash
    Host_read_flash();
    Lan_read_flash();
    User_read_flash();
    Port_read_flash();
    
    Pointer = strstr((char *)recv_buffer,DEVICE_SET_IP);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        Pointer++;
        Ptend = strstr((char *)Pointer,"&");
        if((Pointer!=NULL)&&(Ptend !=NULL))
        {
            memset(temp,0,sizeof(temp));
            strncat(temp,Pointer,Ptend-Pointer);
            ip_lan.IP.idata = inet_addr(temp);
        }
    }
    
    Pointer = strstr((char *)recv_buffer,DEVICE_SET_MASK);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        Pointer++;
        Ptend = strstr((char *)Pointer,"&");
        if((Pointer!=NULL)&&(Ptend !=NULL))
        {
            memset(temp,0,sizeof(temp));
            strncat(temp,Pointer,Ptend-Pointer);
            ip_lan.Subnetmask.idata = inet_addr(temp);
        }
    }
    
    Pointer = strstr((char *)recv_buffer,DEVICE_SET_GATEWAY);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        Pointer++;
        Ptend = strstr((char *)Pointer,"&");
        if((Pointer!=NULL)&&(Ptend !=NULL))
        {
            memset(temp,0,sizeof(temp));
            strncat(temp,Pointer,Ptend-Pointer);
            ip_lan.gateway.idata = inet_addr(temp);
        }
    }
    // Check network config DHCP
    Pointer = strstr((char *)recv_buffer,DEVICE_DHCP);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        Pointer++;
        if(Pointer!=NULL)
        {
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(temp,0,sizeof(temp));
            if(Ptend !=NULL)
            {
                strncat(temp,Pointer,Ptend-Pointer); 
            }
            else
            {
                strncat(temp,Pointer,strlen(Pointer)); 
            }
            ip_lan.dhcp = atoi(temp);
        }
    }
    //Flash_Write_Information();
    ucWriteProgram = 1;
}
void Api_host_config(char *recv_buffer)
{
    char *Pointer;
    // read flash
    Host_read_flash();
    Lan_read_flash();
    User_read_flash();
    Port_read_flash();
    
    // Check config host ip1 
    Pointer = strstr((char *)recv_buffer,HOST_IP1);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            host.IP.cdata[0] = atoi(Pointer);
        }
    }
    // Check config host ip2
    Pointer = strstr((char *)recv_buffer,HOST_IP2);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            host.IP.cdata[1] = atoi(Pointer);
        }
    }
    // Check config host ip3 
    Pointer = strstr((char *)recv_buffer,HOST_IP3);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            host.IP.cdata[2] = atoi(Pointer);
        }
    }
    // Check config host ip4 
    Pointer = strstr((char *)recv_buffer,HOST_IP4);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            host.IP.cdata[3] = atoi(Pointer);
        }
    }
    
    // Check network config host name
    Pointer = strstr((char *)recv_buffer,HOST_NAME);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            memset(host.Host_Name,0,sizeof(host.Host_Name));
            strncat(&host.Host_Name[0].cdata[0],Pointer,strstr((char *)Pointer,"&")-Pointer);
        }
    }
    
    // Check device name
    Pointer = strstr((char *)recv_buffer,DEVICE_NAME);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            memset(host.Device_ID,0,sizeof(host.Device_ID));
            strncat(&host.Device_ID[0].cdata[0],Pointer,strstr((char *)Pointer,"&")-Pointer);
        }
    }
    // Write to flash for information
    //Flash_Write_Information();
    ucWriteProgram = 1;
}

void Web_host_config(char *recv_buffer)
{
    char *Pointer,*Ptend;
    char temp[30];
    // read flash
    
    Host_read_flash();
    Lan_read_flash();
    User_read_flash();
    Port_read_flash();
    
    Pointer = strstr((char *)recv_buffer,HOST_IP);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer != NULL)
        {
            Pointer++;
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(temp,0,sizeof(temp));
            if(Ptend !=NULL)
            {
                strncat(temp,Pointer,Ptend-Pointer);
            }
            else
            {
                strncat(temp,Pointer,strlen(Pointer));
            }
            host.IP.idata = inet_addr(temp);
        }
    }
    Pointer = strstr((char *)recv_buffer,HOST_NAME);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer != NULL)
        {
            Pointer++;
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(host.Host_Name,0,sizeof(host.Host_Name));
            if(Ptend !=NULL)
            {
                strncat(host.Host_Name[0].cdata,Pointer,Ptend-Pointer);
            }
            else
            {
                strncat(host.Host_Name[0].cdata,Pointer,strlen(Pointer));
            }
        }
    }
    Pointer = strstr((char *)recv_buffer,USER_NAME);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer != NULL)
        {
            Pointer++;
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(host.Host_Username,0,sizeof(host.Host_Username));
            if(Ptend !=NULL)
            {
                strncat(host.Host_Username[0].cdata,Pointer,Ptend-Pointer);
            }
            else
            {
                strncat(host.Host_Username[0].cdata,Pointer,strlen(Pointer));
            }
        }
    }
    Pointer = strstr((char *)recv_buffer,PASSWORD);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer != NULL)
        {
            Pointer++;
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(host.Host_Password,0,sizeof(host.Host_Password));
            if(Ptend !=NULL)
            {
                strncat(host.Host_Password[0].cdata,Pointer,Ptend-Pointer);
            }
            else
            {
                strncat(host.Host_Password[0].cdata,Pointer,strlen(Pointer));
            }
        }
    }
    // Write to flash for information
    //Flash_Write_Information();
    ucWriteProgram = 1;
}

void port_config(char *recv_buffer)
{
    char *Pointer,*Ptend;
    char Port[30];
    char deviceID[30];
    char i;
    // read flash
    Host_read_flash();
    Lan_read_flash();
    User_read_flash();
    Port_read_flash();
    
    // Check config port
    for(i=0;i<CTport.number;i++)
    {
        memset(Port,0,sizeof(Port));
        sprintf(Port,"port%d_type",i);
        Pointer = strstr((char *)recv_buffer,Port);
        if(Pointer!=NULL)
        {
            Pointer = strstr((char *)Pointer,"=");
            if(Pointer!=NULL)
            {
                Pointer++;
                Ptend = strpbrk((char *)Pointer,"& ");
                memset(Port,0,sizeof(Port));
                if(Ptend!=NULL)
                {
                    strncat(Port,Pointer,Ptend-Pointer);
                }
                else
                {
                    strncat(Port,Pointer,strlen(Pointer));
                }
                CTport.Port[i].type = atoi(Port);
                CTport.Port[i].event |= PORT_CHANGE_CONFIG;
                CTport.Port[i].timeout = PORT_TIMEOUT;
            }     
        }
        memset(Port,0,sizeof(Port));
        sprintf(Port,"port%d_active",i);
        Pointer = strstr((char *)recv_buffer,Port);
        if(Pointer!=NULL)
        {
            Pointer = strstr((char *)Pointer,"=");
            if(Pointer!=NULL)
            {
                Pointer++;
                Ptend = strpbrk((char *)Pointer,"& ");
                memset(Port,0,sizeof(Port));
                if(Ptend!=NULL)
                {
                    strncat(Port,Pointer,Ptend-Pointer);
                }
                else
                {
                    strncat(Port,Pointer,strlen(Pointer));
                }
                CTport.Port[i].active = atoi(Port);
                CTport.Port[i].event |= PORT_CHANGE_CONFIG;
            }
        }
    }
    memset(deviceID,0,sizeof(deviceID));
    sprintf(deviceID,"deviceID");
    Pointer = strstr((char *)recv_buffer,deviceID);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(host.Device_ID[0].cdata,0,sizeof(host.Device_ID[0].cdata));
            if(Ptend!=NULL)
            {
                strncat(host.Device_ID[0].cdata,Pointer,Ptend-Pointer);
            }
            else
            {
                strncat(host.Device_ID[0].cdata,Pointer,strlen(Pointer));
            }
        }
    }
    // Write to flash for information
    //Flash_Write_Information();
    ucWriteProgram = 1;
}

void username_password_config(char * recv_buffer)
{
	char *Pointer,*Ptend;
	
	// read flash
    Host_read_flash();
    Lan_read_flash();
    User_read_flash();
    Port_read_flash();
    
    Pointer = strstr((char *)recv_buffer,USER_NAME);
    if(Pointer!=NULL)
    {
        Pointer = strstr((char *)Pointer,"=");
        if(Pointer!=NULL)
        {
            Pointer++;		
            // Check username config Username
            Ptend = strpbrk((char *)Pointer,"& ");
            memset(&user.UserName[0].cdata[0],0,sizeof(user.UserName));
            if(Ptend!=NULL)
            {
                strncat(&user.UserName[0].cdata[0],Pointer,Ptend-Pointer);
            }
            else
            {
                strcat(&user.UserName[0].cdata[0],Pointer);
            }
            // Check network config Password
            Pointer = strstr((char *)recv_buffer,PASSWORD_NEW);
            if(Pointer == NULL)
            {
              Pointer = strstr((char *)recv_buffer,PASSWORD_CONFIRM);
            }
            if(Pointer!=NULL)
            {
                Pointer = strstr((char *)Pointer,"=");
                if(Pointer!=NULL)
                {
                    Pointer++;
                    Ptend = strpbrk((char *)Pointer,"& ");
                    memset(&user.PassWord[0].cdata[0],0,sizeof(user.PassWord));
                    if(Ptend!=NULL)
                    {
                        strncat(&user.PassWord[0].cdata[0],Pointer,Ptend-Pointer);
                    }
                    else
                    {
                        strcat(&user.PassWord[0].cdata[0],Pointer);
                    }
                }
            }
            //Flash_Write_Information();
            ucWriteProgram = 1;
            ucReadProgram = 1;
        }
    }
}

void set_time(char * recv_buffer)
{
		char *Pointer;
		ErrorStatus status;
		RTC_TimeTypeDef RTC_TimeStruct;
		RTC_DateTypeDef RTC_DateStruct;
		// Get time from RTC
		RTC_GetTime(RTC_Format_BIN,&RTC_TimeStruct);
		// Get date from RTC
		RTC_GetDate(RTC_Format_BIN,&RTC_DateStruct);
		// Check set 12H
		Pointer = strstr((char *)recv_buffer,SET_12H);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_TimeStruct.RTC_H12 = atoi(Pointer);
				if(RTC_TimeStruct.RTC_H12)
				{
					RTC_TimeStruct.RTC_H12 = RTC_H12_PM;
				}
        else
        {
          RTC_TimeStruct.RTC_H12 = RTC_H12_AM;
        }
			}
		}
		// Check set seconds
		Pointer = strstr((char *)recv_buffer,SET_SECONDS);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_TimeStruct.RTC_Seconds = atoi(Pointer);
			}
		}
		// Check set minutes
		Pointer = strstr((char *)recv_buffer,SET_MINUTES);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_TimeStruct.RTC_Minutes = atoi(Pointer);
			}
		}
		// Check set hours
		Pointer = strstr((char *)recv_buffer,SET_HOURS);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_TimeStruct.RTC_Hours = atoi(Pointer);
			}
		}
		// Check set weekday
		Pointer = strstr((char *)recv_buffer,SET_WEEKDAY);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_DateStruct.RTC_WeekDay = atoi(Pointer);
			}
		}
		// Check set date
		Pointer = strstr((char *)recv_buffer,SET_DATE);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_DateStruct.RTC_Date = atoi(Pointer);
			}
		}
		// Check set month
		Pointer = strstr((char *)recv_buffer,SET_MONTH);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_DateStruct.RTC_Month = atoi(Pointer);
			}
		}
		// Check set year
		Pointer = strstr((char *)recv_buffer,SET_YEAR);
		if(Pointer!=NULL)
		{
			Pointer = strstr((char *)Pointer,"=");
			if(Pointer!=NULL)
			{
				Pointer++;
				RTC_DateStruct.RTC_Year = atoi(Pointer);
			}
		}
		while(RTC_SetTime(RTC_Format_BIN,&RTC_TimeStruct)!=SUCCESS);
		while(RTC_SetDate(RTC_Format_BIN,&RTC_DateStruct)!=SUCCESS);
}

void ApiSystemReboot(void)
{
  ucSystemReboot = 1;
}

void ApiProcessComplete(void)
{
  if((ucWriteProgram == 1)&&(ucReadProgram == 1))
  {
    ucWriteProgram = 0;
    ucReadProgram = 0;
    ucFlashUpdate = 1;
  }
  if(ucSystemReboot)
  {
    while(ucFlashUpdate);
    vTaskDelay(10);
    NVIC_SystemReset();
  }
}

char Get_CTPort( void)
{
	char i;
	for(i=0;i<PORT_NUMBER;i++)
	{
			if(CTport.Port[i].event & PORT_TRANSMIT_DATA)
			{
					return i+1;
			}
	}
	return i;
}

void Port_driver(char rx_tx)
{
    char i;
    for(i=0;i<PORT_NUMBER;i++)
    {
        if(CTport.Port[i].event & PORT_TRANSMIT_DATA)
        {
           break;              
        }
    }
    if(i==1)
    {
        if(rx_tx)
        {
            GPIO_SetBits(GPIOC,GPIO_Pin_11);
        }
        else
        {
            GPIO_ResetBits(GPIOC,GPIO_Pin_11);
        }
    }
}
