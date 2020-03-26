#ifndef __ENV_DATA_H__
#define __ENV_DATA_H__
#include "stdint.h"

#define MAX_DATA_SIZE 	20 

#define DEV_ID_OFF				0x00
#define TEMP_VALUE_OFF 		0x02
#define LIGHT_VALUE_OFF	  0x04
#define HUMID_VALUE_OFF   0x06
#define RAIN_VALUE_OFF		0x05


//unsigned char envData[MAX_DATA_SIZE];



void env_data_reset(unsigned char *pData, uint8_t length);

void env_data_set(unsigned char *pData, uint8_t offset, uint16_t value);

uint16_t env_data_get(unsigned char *pData, uint8_t offset);





#endif