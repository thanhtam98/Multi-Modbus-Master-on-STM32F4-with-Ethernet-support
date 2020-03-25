#ifndef __API_SERVER_H
#define __API_SERVER_H

#include "user_rs485_app.h"

#define GET_REBOOT 				"GET /protect/cgi/reboot.cgi"
#define GET_SUSPEND_CARD 	"GET /protect/cgi/suspendCard.cgi"
#define GET_DATA_TIME 		"GET /protect/cgi/getTime.cgi"
#define GET_EVENT_COUNT 	"GET /protect/cgi/eventsCount.cgi"
#define GET_EVENT_ID 			"GET /protect/cgi/eventID.cgi"
#define GET_EVENT 				"GET /protect/cgi/events.cgi"
#define GET_CLEAR_EVENT		"GET /protect/cgi/clearEvents.cgi?event=0"
#define GET_UNLOCK_DOOR		"GET /protect/cgi/parser.cgi?cmd=UNLOCK"
#define GET_DOOR_STATUS		"GET /protect/cgi/doorStatus.cgi"
#define GET_SET_ARMING		"GET /protect/cgi/setArming.cgi"
#define GET_PORT_CONFIG   "GET /protect/cgi/portconfig.cgi"



/*Define api for POST Methods*/
#define POST_DEVICE_CONFIG							"POST /protect/cgi/deviceconfig.cgi"
#define POST_NETWORK_CONFIG							"POST /protect/cgi/networkconfig.cgi"
#define POST_WEB_NETWORK_CONFIG         "POST /lanconfig"
#define POST_HOST_CONFIG                "POST /protect/cgi/hostconfig.cgi"
#define POST_WEB_HOST_CONFIG            "POST /hostconfig"
#define POST_WEB_PORT_CONFIG            "POST /portconfig"
#define POST_PORT_CONFIG                "POST /protect/cgi/portconfig.cgi"
#define POST_CHANGE_USERNAME_PASSWORD 	"POST /protect/cgi/password.cgi"
#define POST_USERNAME_PASSWORD          "POST /maintain"
#define POST_ADD_OR_MODIFY_CARD					"POST /protect/cgi/cards.cgi"
#define POST_SET_TIME 									"POST /protect/cgi/setTime.cgi"
#define POST_OPEN_DOOR									"POST /protect/cgi/opendoor.cgi"
// Sefine api for SCRIPTS
#define POST_SCRIPTS										"POST /protect/cgi/parser.cgi"
// Define api for XML
#define POST_READER_CONFIG							"POST /protect/xml/systemconfig.xml"
#define POST_CARD_LIST									"POST /protect/xml/cardslist.xml"


// Define parameter for Network config
#define HOST_IP1 						"hostip1"
#define HOST_IP2 						"hostip2"
#define HOST_IP3 						"hostip3"
#define HOST_IP4 						"hostip4"
#define HOST_IP        			"hostip"
#define HOST_NAME 					"hostname"
#define DEVICE_NAME					"devicename"
#define DEVICE_DHCP					"Dhcp"
#define DEVICE_IP1					"deviceip1"
#define DEVICE_IP2					"deviceip2"
#define DEVICE_IP3					"deviceip3"
#define DEVICE_IP4					"deviceip4"
#define DEVICE_SET_IP   		"Set_ip"
#define DEVICE_MASK1				"devicemask1"
#define DEVICE_MASK2				"devicemask2"
#define DEVICE_MASK3				"devicemask3"
#define DEVICE_MASK4				"devicemask4"
#define DEVICE_SET_MASK     "Subnet"
#define DEVICE_GATEWAY1	    "devicegate1"
#define DEVICE_GATEWAY2	    "devicegate2"
#define DEVICE_GATEWAY3	    "devicegate3"
#define DEVICE_GATEWAY4	    "devicegate4"
#define DEVICE_SET_GATEWAY  "Gateway"
#define	USER_NAME 					"username"
#define PASSWORD            "password"
#define PASSWORD_PREV				"prevpass"
#define PASSWORD_NEW				"newpass"
#define PASSWORD_CONFIRM    "confirmpass"

//Define control
#define CONTROL_DOOR		"control"
// Define set date time
#define SET_12H				"set12H"
#define SET_SECONDS			"setseconds"
#define SET_MINUTES			"setminutes"
#define SET_HOURS			"sethours"
#define SET_WEEKDAY			"setweekday"
#define SET_DATE			"setdate"
#define SET_MONTH			"setmonth"
#define SET_YEAR			"setyear"



/** @defgroup RTC_WeekDay_Definitions 
  * @{
  */ 

#define RTC_Weekday_Monday             ((uint8_t)0x01)
#define RTC_Weekday_Tuesday            ((uint8_t)0x02)
#define RTC_Weekday_Wednesday          ((uint8_t)0x03)
#define RTC_Weekday_Thursday           ((uint8_t)0x04)
#define RTC_Weekday_Friday             ((uint8_t)0x05)
#define RTC_Weekday_Saturday           ((uint8_t)0x06)
#define RTC_Weekday_Sunday             ((uint8_t)0x07)

#define IS_RTC_WEEKDAY(WEEKDAY) (((WEEKDAY) == RTC_Weekday_Monday) || \
                                 ((WEEKDAY) == RTC_Weekday_Tuesday) || \
                                 ((WEEKDAY) == RTC_Weekday_Wednesday) || \
                                 ((WEEKDAY) == RTC_Weekday_Thursday) || \
                                 ((WEEKDAY) == RTC_Weekday_Friday) || \
                                 ((WEEKDAY) == RTC_Weekday_Saturday) || \
                                 ((WEEKDAY) == RTC_Weekday_Sunday))

// Return for POST/GET method
typedef enum
{
	COMMAND_SUCCESS = 0x01,
	COMMAND_ERRO,
}RTCommand;

typedef struct
{
	unsigned char type;
	unsigned char active;
	unsigned char status;
	unsigned char event;
}FLASH_PORT_Type;
typedef struct
{
	unsigned char number;
  FLASH_PORT_Type Port[PORT_NUMBER];
}FLASH_PORT_TypeDef;

// Define flash address
#define FLASH_SECTOR_INFORMATION FLASH_Sector_6
#define ADD_STATIC_IP     0x08040000
#define ADD_SUBNETMASK    0x08040004
#define ADD_GATEWAY       0x08040008
#define ADD_DHCP          0x0804000C
#define ADD_USER_NAME     0x08040010
#define ADD_PASS_WORD     0x08040020
#define ADD_HOST_IP       0x08040030
#define ADD_HOST_NAME     0x08040040
#define ADD_DEVICE_NAME	  0x08040090
#define ADD_HOST_USERNAME 0x080400A0
#define ADD_HOST_PASSWORD 0x080400B0
#define ADD_PORT_BASE     0x080400BF

#define CONTROL_PORT ((FLASH_PORT_TypeDef *) ADD_PORT_BASE)


typedef union
{
	unsigned int idata;
	char cdata[4];
}int_to_byte;

typedef struct 
{
	int_to_byte IP;
	int_to_byte Host_Name[16];
	int_to_byte Device_ID[3];
	int_to_byte	Host_Username[3];
	int_to_byte Host_Password[3];
}Host_t;

typedef struct
{
	int_to_byte IP;
	int_to_byte Subnetmask;
	int_to_byte gateway;
	char dhcp;
}Device_Lan_t;

typedef struct
{
	int_to_byte UserName[3];
	int_to_byte PassWord[3];
}User_t;

typedef struct
{
	unsigned char Hours;
    unsigned char Minutes; 
    unsigned char Seconds;
    unsigned char H12;
}Time_t;

typedef struct
{
	unsigned char WeekDay;
	unsigned char Date;
    unsigned char Month;
    unsigned char Year;
}Date_t;
/*Define apifor GET Methods*/
//
char Flash_Write_Information(void);
void Host_read_flash(void);
void User_read_flash (void);
void Lan_read_flash(void);
void Port_read_flash(void);
char Get_CTPort( void);
void Port_driver(char rx_tx);
void Api_lan_config(char *recv_buffer);
void Web_lan_config(char *recv_buffer);
void Api_host_config(char *recv_buffer);
void Web_host_config(char *recv_buffer);
void port_config(char *recv_buffer);
void username_password_config(char * recv_buffer);
void set_time(char * recv_buffer);
void ApiSystemReboot(void);
void ApiProcessComplete(void);
#endif
