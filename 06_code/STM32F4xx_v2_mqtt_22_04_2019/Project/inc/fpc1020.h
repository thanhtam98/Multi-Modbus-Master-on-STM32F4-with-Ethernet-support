#ifndef _FPC1020_H_
#define _FPC1020_H_

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
	 
/* Define SPI used in the FPC102x---------------------------------------------*/
#define FPC102x_SPI SPI2
   
#define GPIO_Pin_SPI_FPC102x_SCK  GPIO_Pin_10
#define GPIO_Pin_SPI_FPC102x_MISO GPIO_Pin_2
#define GPIO_Pin_SPI_FPC102x_MOSI GPIO_Pin_3
#define GPIO_Pin_SPI_FPC102x_CS   GPIO_Pin_0
#define GPIO_SPI_SCK              GPIOB
#define GPIO_SPI_MISO             GPIOC
#define GPIO_SPI_MOSI             GPIOC
#define GPIO_SPI_CS               GPIOC
   
#define GPIO_FPC102x_RESET        GPIOB
#define GPIO_Pin_PFC102x_RESET    GPIO_Pin_1
   
#define GPIO_FPC102x_IRQ          GPIOB
#define GPIO_Pin_PFC102x_IRQ      GPIO_Pin_0
/* Define Buffer used for the protocol*/
#define BUF_N 8

#define PIXELS 192

/* Define Command-------------------------------------------------------------*/
#define CMD_CAPTURE_IMAGE            0xC0  // Capture new image
#define CMD_READ_IMAGE_DATA          0xC4  // Read image data
#define CMD_READ_INT_NO_CLEAR        0x18  // Read interrupt with no clear
#define CMD_READ_INT_CLEAR           0x1C  // Read interrupt with clear
#define CMD_FINGER_PRESENT_QUERY     0x20  // Check if a finger is present
#define CMD_WAIT_FINGER_PRESENT      0x24  // Continue to check for a finger until a finger is present
#define CMD_ACTIVATE_SLEEP_MODE      0x28  // Go to sleep mode
#define CMD_ACTIVATE_DEEP_SLEEP_MODE 0x2C  // Go to Deep Sleep mode
#define CMD_ACTIVATE_IDLE_MODE       0x34  // Go to Idle mode
#define CMD_SOFT_RESET               0xF8  // Soft reset
#define CMD_CLK_BITS                 0x3C  // To measure the two internal oscillator's frequency

/*Define Register------------------------------------------------------------*/
#define REG_FPCERROR          0x38  // 
#define REG_IMGCAPSIZE        0x54  //
#define REG_TSTCOLPATTERN     0x78  //
#define REG_CLKBISTRESULT     0x7C  //
#define REG_FINGERDRIVECONF   0x8C  //
#define REG_OSCTRIM           0x94  //
#define REG_ADCSHIFTGAIN      0xA0  //
#define REG_FNGRPRESENTSTATUS 0xD4  // 
#define REG_FNGRDETTHRESH     0xD8  // Finger Detection Threshold
#define REG_FNGRDETCNTR       0xDC  // Finger Detection Queries
#define REG_HWID              0xFC  // Finger deteced the Hardware ID

	 
void Fpc102x_spi_int(void);
void Fpc102x_int(void);
void Fpc102x_reset(void);
char Fpc102x_send_byte(unsigned char data);
char Fpc102x_read_irq(void);
char Fpc102x_read_irq_clear(void);
void Fpc102x_ImageCapture(char *Image);
uint16_t Fpc102x_Prensentquery(void);
char Fpc102x_HardwareID_check(void);
void Fpc102x_Calibration(void);
void Fpc102x_Deep_sleep_mode(void);
void Fpc102x_Sleep_mode(void);
void Fpc102x_Idle_mode(void);
#endif
