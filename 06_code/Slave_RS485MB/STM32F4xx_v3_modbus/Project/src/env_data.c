#include "stdint.h"
#include "env_data.h"




void env_data_reset(unsigned char *pData, uint8_t length)
{
	uint8_t i ;
	
		//memset(pData,0,length);
		for(i=0; i < length; i ++)
	{
		*(pData+i) = 0;
	}
	
}

void env_data_set(unsigned char *pData, uint8_t offset, uint16_t value)
{
	*(pData + offset) =  (unsigned char)((0xFF00&value)>>8);
	*(pData + offset + 1) =   (unsigned char)(0x00FF&value );
	
}
uint16_t env_data_get(unsigned char *pData, uint8_t offset)
{
	
		return (uint16_t) (*(pData + offset)<<8|*(pData + offset+1));
}
	