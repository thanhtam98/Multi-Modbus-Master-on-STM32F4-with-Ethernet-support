#ifndef    USER_APP
#define USER_APP
/* ----------------------- RS485bus includes ----------------------------------*/
#include "rs485.h"
#include "rs485config.h"
#include "rs485frame.h"
#include "rs485utils.h"
#include "rs485proto.h"
/* -----------------------Slave Defines -------------------------------------*/
#define PORT_NUMBER		      6
#define PORT_TIMEOUT	      50 // 5s timeout
#define PORT_BUFFER_NUMBER  12
#define RS485_PORT_OFFSET	2
typedef struct
{
    unsigned char type;
    unsigned char active;
    unsigned char status;
    unsigned char event;
		unsigned int timeout;
		unsigned char Buffer[PORT_BUFFER_NUMBER];
}PORT_TypeDef;

typedef enum
{
    PORT_NONE,
    PORT_EXIT,
    PORT_ENTRY_READER,
    PORT_EXIT_READER,
    PORT_ENTRY_KEYPAD,
    PORT_EXIT_KEYPAD,
    PORT_DOOR_DETECT
}PORT_TYPE;

typedef enum
{
    PORT_DISABLE = 0,
    PORT_ENABLE,
}PORT_ACTIVE;

typedef enum
{
		PORT_ERROR,
		PORT_OK,
}PORT_STATUS;

typedef enum
{
    PORT_EVENT_NONE = 0x00,
    PORT_AUTH_PASS = 0x01,
    PORT_AUTH_FAIL = 0x02,
    PORT_AUTH_ERROR = 0x03,
    PORT_DOOR_OPEN = 0x04,
    PORT_RECEIVE_DATA = 0x10,
		PORT_TRANSMIT_DATA = 0x20,
    PORT_CHANGE_CONFIG = 0x40
}PORT_EVENT;

typedef struct
{
    unsigned char number;
    PORT_TypeDef Port[PORT_NUMBER];
}CTPORT_TypeDef;
/* -----------------------Master Defines -------------------------------------*/

#endif
