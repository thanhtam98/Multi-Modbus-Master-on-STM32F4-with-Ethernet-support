/* Includes */
#include "fpc1020.h"

#ifdef __cplusplus
 extern "C" {
#endif
   
/** @defgroup FPC102x_Library
* @{
*/
  
void Fpc102x_spi_int(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC, ENABLE);
  
  GPIO_PinAFConfig(GPIO_SPI_SCK, GPIO_PinSource10, GPIO_AF_SPI2);	
	GPIO_PinAFConfig(GPIO_SPI_MISO, GPIO_PinSource2, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIO_SPI_MOSI, GPIO_PinSource3, GPIO_AF_SPI2);
	/* Configure SPI pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_FPC102x_SCK; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_SPI_SCK, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_FPC102x_MISO;
  GPIO_Init(GPIO_SPI_MISO, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_FPC102x_MOSI;
  GPIO_Init(GPIO_SPI_MOSI, &GPIO_InitStructure);

	/* Configure CS pin */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_SPI_FPC102x_CS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_SPI_CS, &GPIO_InitStructure);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);

  
  	/* SPI3 Config */
	SPI_I2S_DeInit(FPC102x_SPI);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(FPC102x_SPI, &SPI_InitStructure);

	SPI_CalculateCRC(FPC102x_SPI, DISABLE);

	/* SPI enable */
	SPI_Cmd(FPC102x_SPI, ENABLE);

}
void Fpc102x_int(void)
{
  int32_t i;
//  char status;
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PFC102x_RESET; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIO_FPC102x_RESET, &GPIO_InitStructure);
  GPIO_ResetBits(GPIO_FPC102x_RESET,GPIO_Pin_PFC102x_RESET);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_PFC102x_IRQ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIO_FPC102x_IRQ, &GPIO_InitStructure);
  
  for(i=0;i<10000;i++);
  GPIO_SetBits(GPIO_FPC102x_RESET,GPIO_Pin_PFC102x_RESET);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==RESET);
  Fpc102x_Calibration();
//   Fpc102x_reset();
//   while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ));
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==SET)
  {
    Fpc102x_read_irq_clear();
    for(i=100;i>0;i--);
  }
//  status = Fpc102x_HardwareID_check();
}
void Fpc102x_reset(void)
{
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_SOFT_RESET);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
}
char Fpc102x_send_byte(unsigned char data)
{
  char Data = 0;
  /* Wait until the transmit buffer is empty */
  while(SPI_I2S_GetFlagStatus(FPC102x_SPI, SPI_I2S_FLAG_TXE) == RESET);
  /* Send the byte */
  SPI_I2S_SendData(FPC102x_SPI, data);
  /* Wait until a data is received */
	while(SPI_I2S_GetFlagStatus(FPC102x_SPI, SPI_I2S_FLAG_RXNE) == RESET);
  /* Get the received data */
	  Data = SPI_I2S_ReceiveData(FPC102x_SPI);
  return Data;
}
char Fpc102x_read_irq_clear(void)
{
  char status;
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_READ_INT_CLEAR);
  status = Fpc102x_send_byte(0x00);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  return status;
}
char Fpc102x_read_irq(void)
{
  char status;
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_READ_INT_NO_CLEAR);
  status = Fpc102x_send_byte(0x00);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  return status;
}
void Fpc102x_ImageCapture(char *Image)
{
  int i;
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_CAPTURE_IMAGE);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==RESET);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==SET)
  {
    Fpc102x_read_irq_clear();
    for(i=100;i>0;i--);
  }
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_READ_IMAGE_DATA);
  Fpc102x_send_byte(0x00);
  for(i=0;i<(PIXELS*PIXELS);i++)
  {
    Image[i] = Fpc102x_send_byte(0x00);
  }
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
}
uint16_t Fpc102x_Prensentquery(void)
{
  int i;
  
  char status[2];
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_FINGER_PRESENT_QUERY);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==RESET);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==SET)
  {
    Fpc102x_read_irq_clear();
    for(i=100;i>0;i--);
  }
  Fpc102x_send_byte(REG_FNGRPRESENTSTATUS);
  status[0] = Fpc102x_send_byte(0x00);
  status[1] = Fpc102x_send_byte(0x00);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  return (uint16_t)((uint16_t) (status[0]<<8)|status[1]);
}
char Fpc102x_HardwareID_check(void)
{
  char HwID[2];
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(REG_HWID);
  HwID[0] = Fpc102x_send_byte(0x00);
  HwID[1] = Fpc102x_send_byte(0x00);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  if(HwID[0]!= 0x02)
  {
    return 1;
  }
  else if(HwID[1]!=0x1B)
  {
    return 1;
  }
  return 0;
}

void Fpc102x_Calibration(void)
{
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_CLK_BITS);
  Fpc102x_send_byte(0x00);
  Fpc102x_send_byte(0x00);
  Fpc102x_send_byte(0x00);
  Fpc102x_send_byte(0x00);
  Fpc102x_send_byte(0x00);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
}
void Fpc102x_Deep_sleep_mode(void)
{
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_ACTIVATE_DEEP_SLEEP_MODE);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
}
void Fpc102x_Sleep_mode(void)
{
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_ACTIVATE_SLEEP_MODE);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
}
void Fpc102x_Idle_mode(void)
{
  int i;
  GPIO_ResetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  Fpc102x_send_byte(CMD_ACTIVATE_IDLE_MODE);
  GPIO_SetBits(GPIO_SPI_CS,GPIO_Pin_SPI_FPC102x_CS);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==RESET);
  while(GPIO_ReadInputDataBit(GPIO_FPC102x_IRQ,GPIO_Pin_PFC102x_IRQ)==SET)
  {
    Fpc102x_read_irq_clear();
    for(i=100;i>0;i--);
  }
}
