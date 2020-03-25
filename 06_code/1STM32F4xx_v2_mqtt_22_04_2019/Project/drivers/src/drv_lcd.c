/**
  ******************************************************************************
  * @file    lcd_lib.c
  * @author  MCD Application Team
  * @version V4.1.0
  * @date    03/01/2010
  * @brief   LCD driver for AM-240320L8TNQW00H (LCD_ILI9320), 
	*													AM-240320LDTNQW00H (LCD_SPFD5408B)
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "drv_lcd.h"
#include "drv_spi.h"
#include "hw_config.h"
#include "string.h"
#include "stm32f4xx_spi.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "miscellaneous.h"

/** @addtogroup Drivers
  * @{
  */

    
/** @defgroup LCD 
  * @brief This file includes the LCD driver for 	AM-240320L8TNQW00H (LCD_ILI9320), 
	*																								AM-240320LDTNQW00H (LCD_SPFD5408B)
  * @{
  */ 

/** @defgroup LCD_private_Defines
  * @{
  */ 
	/* LCD IDs */
	#define LCD_ILI9320        0x9320
	#define LCD_SPFD5408       0x5408

	#define START_BYTE         0x70
	#define SET_INDEX          0x00
	#define READ_STATUS        0x01
	#define LCD_WRITE_REG      0x02
	#define LCD_READ_REG       0x03
	
	#define LCD_NCS_LOW() 	GPIO_ResetBits(LCD_NCS_GPIO_PORT, LCD_NCS_PIN)
	#define LCD_NCS_HIGH()  GPIO_SetBits  (LCD_NCS_GPIO_PORT, LCD_NCS_PIN)
/**
  * @}
  */ 


/** @defgroup LCD_Private_Variables
  * @{
  */ 

/* Global variable to set the Current font */
static sFONT *LCD_Currentfonts;
/* Global variables to set the written text color */
static uint16_t TextColor = 0x0000, BackColor = 0xFFFF; 


/** @defgroup LCD_Functions
  * @{
  */ 


/**
 * @brief  Setups ILI9320 controller.
 * @param  None
 * @retval None
 */
void LCD_ILI9320_Setup(void)
{
  delay_ms(50); /* Delay 50 ms */
  /* Start Initial Sequence ------------------------------------------------*/
  LCD_WriteReg(LCD_REG_229, 0x8000); /* Set the internal vcore voltage */
  LCD_WriteReg(LCD_REG_0,  0x0001); /* Start internal OSC. */
  LCD_WriteReg(LCD_REG_1,  0x0100); /* set SS and SM bit */
  LCD_WriteReg(LCD_REG_2,  0x0700); /* set 1 line inversion */
  LCD_WriteReg(LCD_REG_3,  0x1030); /* set GRAM write direction and BGR=1. */
  LCD_WriteReg(LCD_REG_4,  0x0000); /* Resize register */
  LCD_WriteReg(LCD_REG_8,  0x0202); /* set the back porch and front porch */
  LCD_WriteReg(LCD_REG_9,  0x0000); /* set non-display area refresh cycle ISC[3:0] */
  LCD_WriteReg(LCD_REG_10, 0x0000); /* FMARK function */
  LCD_WriteReg(LCD_REG_12, 0x0000); /* RGB interface setting */
  LCD_WriteReg(LCD_REG_13, 0x0000); /* Frame marker Position */
  LCD_WriteReg(LCD_REG_15, 0x0000); /* RGB interface polarity */
  /* Power On sequence -----------------------------------------------------*/
  LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */
  delay_ms(200);               /* Dis-charge capacitor power voltage (200ms) */
  LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
  delay_ms(50);                /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
  delay_ms(50);                /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
  delay_ms(50);                /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_32, 0x0000); /* GRAM horizontal Address */
  LCD_WriteReg(LCD_REG_33, 0x0000); /* GRAM Vertical Address */
  /* Adjust the Gamma Curve ------------------------------------------------*/
  LCD_WriteReg(LCD_REG_48, 0x0006);
  LCD_WriteReg(LCD_REG_49, 0x0101);
  LCD_WriteReg(LCD_REG_50, 0x0003);
  LCD_WriteReg(LCD_REG_53, 0x0106);
  LCD_WriteReg(LCD_REG_54, 0x0b02);
  LCD_WriteReg(LCD_REG_55, 0x0302);
  LCD_WriteReg(LCD_REG_56, 0x0707);
  LCD_WriteReg(LCD_REG_57, 0x0007);
  LCD_WriteReg(LCD_REG_60, 0x0600);
  LCD_WriteReg(LCD_REG_61, 0x020b);

  /* Set GRAM area ---------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_80, 0x0000); /* Horizontal GRAM Start Address */
  LCD_WriteReg(LCD_REG_81, 0x00EF); /* Horizontal GRAM End Address */
  LCD_WriteReg(LCD_REG_82, 0x0000); /* Vertical GRAM Start Address */
  LCD_WriteReg(LCD_REG_83, 0x013F); /* Vertical GRAM End Address */
  LCD_WriteReg(LCD_REG_96,  0x2700); /* Gate Scan Line */
  LCD_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
  LCD_WriteReg(LCD_REG_106, 0x0000); /* set scrolling line */
  /* Partial Display Control -----------------------------------------------*/
  LCD_WriteReg(LCD_REG_128, 0x0000);
  LCD_WriteReg(LCD_REG_129, 0x0000);
  LCD_WriteReg(LCD_REG_130, 0x0000);
  LCD_WriteReg(LCD_REG_131, 0x0000);
  LCD_WriteReg(LCD_REG_132, 0x0000);
  LCD_WriteReg(LCD_REG_133, 0x0000);
  /* Panel Control ---------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_144, 0x0010);
  LCD_WriteReg(LCD_REG_146, 0x0000);
  LCD_WriteReg(LCD_REG_147, 0x0003);
  LCD_WriteReg(LCD_REG_149, 0x0110);
  LCD_WriteReg(LCD_REG_151, 0x0000);
  LCD_WriteReg(LCD_REG_152, 0x0000);
  /* Set GRAM write direction and BGR = 1 */
  /* I/D=01 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  LCD_WriteReg(LCD_REG_3, 0x1018);
  LCD_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */  
}

/**  
 * @brief  Setups SPFD5408 controller.
 * @param  None
 * @retval None
 */
void LCD_SPFD5408_Setup(void)
{
	/* Start Initial Sequence --------------------------------------------------*/
	LCD_WriteReg(LCD_REG_227, 0x3008); /* Set internal timing */
	LCD_WriteReg(LCD_REG_231, 0x0012); /* Set internal timing */
	LCD_WriteReg(LCD_REG_239, 0x1231); /* Set internal timing */
	LCD_WriteReg(LCD_REG_1, 0x0100);   /* Set SS and SM bit */
	LCD_WriteReg(LCD_REG_2, 0x0700);   /* Set 1 line inversion */
	LCD_WriteReg(LCD_REG_3, 0x1030);   /* Set GRAM write direction and BGR=1. */
	LCD_WriteReg(LCD_REG_4, 0x0000);   /* Resize register */
	LCD_WriteReg(LCD_REG_8, 0x0202);   /* Set the back porch and front porch */
	LCD_WriteReg(LCD_REG_9, 0x0000);   /* Set non-display area refresh cycle ISC[3:0] */
	LCD_WriteReg(LCD_REG_10, 0x0000);  /* FMARK function */
	LCD_WriteReg(LCD_REG_12, 0x0000);  /* RGB interface setting */
	LCD_WriteReg(LCD_REG_13, 0x0000);  /* Frame marker Position */
	LCD_WriteReg(LCD_REG_15, 0x0000);  /* RGB interface polarity */
	/* Power On sequence -------------------------------------------------------*/
	LCD_WriteReg(LCD_REG_16, 0x0000);  /* SAP, BT[3:0], AP, DSTB, SLP, STB */
	LCD_WriteReg(LCD_REG_17, 0x0000);  /* DC1[2:0], DC0[2:0], VC[2:0] */
	LCD_WriteReg(LCD_REG_18, 0x0000);  /* VREG1OUT voltage */
	LCD_WriteReg(LCD_REG_19, 0x0000);  /* VDV[4:0] for VCOM amplitude */
	delay_ms(20);                /* Dis-charge capacitor power voltage (200ms) */  
	LCD_WriteReg(LCD_REG_17, 0x0007);  /* DC1[2:0], DC0[2:0], VC[2:0] */
	delay_ms(5);                 /* Delay 50 ms */
	LCD_WriteReg(LCD_REG_16, 0x12B0);  /* SAP, BT[3:0], AP, DSTB, SLP, STB */
	delay_ms(5);                  /* Delay 50 ms */
	LCD_WriteReg(LCD_REG_18, 0x01BD);  /* External reference voltage= Vci */
	delay_ms(5);                 /* Delay 50 ms */ 
	LCD_WriteReg(LCD_REG_19, 0x1400);       /* VDV[4:0] for VCOM amplitude */
	LCD_WriteReg(LCD_REG_41, 0x000E);  /* VCM[4:0] for VCOMH */
	delay_ms(5);                 /* Delay 50 ms */
	LCD_WriteReg(LCD_REG_32, 0x0000);  /* GRAM horizontal Address */
	LCD_WriteReg(LCD_REG_33, 0x013F);  /* GRAM Vertical Address */
	/* Adjust the Gamma Curve --------------------------------------------------*/
	LCD_WriteReg(LCD_REG_48, 0x0007);
	LCD_WriteReg(LCD_REG_49, 0x0302);
	LCD_WriteReg(LCD_REG_50, 0x0105);
	LCD_WriteReg(LCD_REG_53, 0x0206);
	LCD_WriteReg(LCD_REG_54, 0x0808);
	LCD_WriteReg(LCD_REG_55, 0x0206);
	LCD_WriteReg(LCD_REG_56, 0x0504);
	LCD_WriteReg(LCD_REG_57, 0x0007);
	LCD_WriteReg(LCD_REG_60, 0x0105);
	LCD_WriteReg(LCD_REG_61, 0x0808);
	/* Set GRAM area -----------------------------------------------------------*/
	LCD_WriteReg(LCD_REG_80, 0x0000);  /* Horizontal GRAM Start Address */
	LCD_WriteReg(LCD_REG_81, 0x00EF);  /* Horizontal GRAM End Address */
	LCD_WriteReg(LCD_REG_82, 0x0000);  /* Vertical GRAM Start Address */
	LCD_WriteReg(LCD_REG_83, 0x013F);  /* Vertical GRAM End Address */
	LCD_WriteReg(LCD_REG_96,  0xA700); /* Gate Scan Line */
	LCD_WriteReg(LCD_REG_97,  0x0001); /* NDL,VLE, REV */
	LCD_WriteReg(LCD_REG_106, 0x0000); /* Set scrolling line */
	/* Partial Display Control -------------------------------------------------*/
	LCD_WriteReg(LCD_REG_128, 0x0000);
	LCD_WriteReg(LCD_REG_129, 0x0000);
	LCD_WriteReg(LCD_REG_130, 0x0000);
	LCD_WriteReg(LCD_REG_131, 0x0000);
	LCD_WriteReg(LCD_REG_132, 0x0000);
	LCD_WriteReg(LCD_REG_133, 0x0000);
	/* Panel Control -----------------------------------------------------------*/
	LCD_WriteReg(LCD_REG_144, 0x0010);
	LCD_WriteReg(LCD_REG_146, 0x0000);
	LCD_WriteReg(LCD_REG_147, 0x0003);
	LCD_WriteReg(LCD_REG_149, 0x0110);
	LCD_WriteReg(LCD_REG_151, 0x0000);
	LCD_WriteReg(LCD_REG_152, 0x0000);
	/* Set GRAM write direction and BGR = 1
	   I/D=01 (Horizontal : increment, Vertical : decrement)
	   AM=1 (address is updated in vertical writing direction) */
	LCD_WriteReg(LCD_REG_3, 0x1018);
	LCD_WriteReg(LCD_REG_7, 0x0112);   /* 262K color and display ON */
}

/**  
 * @brief  Setup the LCD accoding to the LCD controller
 * @param  None
 * @retval None
 */
void LCD_Setup(void)
{
	uint16_t type;

	/* Configures the LCD_SPI interface */
	LCD_Init( );

	type = LCD_ReadReg(LCD_REG_0);
	if(type == LCD_ILI9320)
	{
	  /* Setups the LCD */
	  LCD_ILI9320_Setup();	
	  LCD_SetFont(&LCD_DEFAULT_FONT);
	}
	else if(type == LCD_SPFD5408)
	{
	  /* Setups the LCD */
	  LCD_SPFD5408_Setup();	
	  LCD_SetFont(&LCD_DEFAULT_FONT);
	}
	else
	{
		LCD_DeInit();
	}
	
	LCD_Clear(LCD_COLOR_WHITE);
	LCD_SetTextColor(LCD_COLOR_BLUE2);

}

/** 
 * @brief  Send Start-Byte
 * @param  Start_Byte : the Start-Byte to be sent
 * @retval None
 */
void LCD_nCS_StartByte(uint8_t Start_Byte)
{
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);
  /* SPI in 8-bit mode */
  SPI_DataSizeConfig(LCD_SPI, SPI_DataSize_8b);

	/* Write Start byte */
	SPI_SendByte(LCD_SPI, Start_Byte);
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);

  /* SPI in 16-bit mode */
  SPI_DataSizeConfig(LCD_SPI, SPI_DataSize_16b);
}

/** 
 * @brief  Writes index to select the LCD register.
 * @param  LCD_Reg : address of the selected register.
 * @retval None
 */
void LCD_WriteRegIndex(uint8_t LCD_Reg)
{
	/* Reset LCD control line(/CS) */
	LCD_NCS_LOW();

  /* Send Start-Byte */
  LCD_nCS_StartByte(START_BYTE | SET_INDEX);

  /* Write 16-bit Reg Index (High Byte is 0) */
	LCD_WriteRAM((uint16_t)(LCD_Reg & 0x00FF));
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);

	/* Set LCD control line(/CS) */
	LCD_NCS_HIGH();
}

/**  
 * @brief  Writes to the selected register
 * @param  LCD_RegValue : value to write to the selected register.
 * @retval None
 */
void LCD_WriteRegData(uint16_t LCD_RegValue)
{
	/* Reset LCD control line(/CS) */
	LCD_NCS_LOW();

  /* Send Start-Byte */
  LCD_nCS_StartByte(START_BYTE | LCD_WRITE_REG);

	/* Write 16-bit Reg */
	LCD_WriteRAM(LCD_RegValue);
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET); 

	/* Set LCD control line(/CS) */
	LCD_NCS_HIGH();
}

/**
 * @brief  Writes to the selected LCD ILI9320 register.
 * @param  LCD_Reg : address of the selected register.
 * @param  LCD_RegValue : value to write to the selected register.
 * @retval None
 */
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
  /* Write 16-bit Index (then Write Reg) */
  LCD_WriteRegIndex(LCD_Reg);

	/* Write 16-bit Reg */
	LCD_WriteRegData(LCD_RegValue);
}

/** 
 * @brief  Prepare to write to the LCD RAM.
 * @param  None
 * @retval None
 */
void LCD_WriteRAM_Prepare(void)
{
	/* Select GRAM Reg */
  LCD_WriteRegIndex(LCD_REG_34);
}

/** 
 * @brief  Writes to the LCD RAM.
 * @param  RGB_Code : the pixel color in RGB mode (5-6-5).
 * @retval None
 */
void LCD_WriteRAM(uint16_t RGB_Code)
{
	SPI_SendWord(LCD_SPI, RGB_Code);
}

/**
 * @brief  Reads the selected LCD Register.
 * @param  LCD_Reg : address of the selected register.
 * @retval LCD Register Value.
 */

uint16_t LCD_ReadReg(uint8_t LCD_Reg)
{
  uint16_t tmp = 0;
  uint8_t i = 0;
  
  /* LCD_SPI prescaler: 4 */
  LCD_SPI->CR1 &= 0xFFC7;
  LCD_SPI->CR1 |= 0x0008;

  /* Write 16-bit Index (then Read Reg) */
  LCD_WriteRegIndex(LCD_Reg);

	/* Reset LCD control line(/CS) */
	LCD_NCS_LOW();

  /* Read 16-bit Reg */
  /* Reset LCD control line(/CS) and Send Start-Byte */
  LCD_nCS_StartByte(START_BYTE | LCD_READ_REG);

	/* SPI in 8-bit mode */
  SPI_DataSizeConfig(LCD_SPI, SPI_DataSize_8b);
  
	/* Send dummy bytes */
  for(i = 0; i < 6; i++)
  {
		SPI_SendReceiveByte(LCD_SPI, 0xFF);
  }

	/* Read upper byte */
  tmp = SPI_SendReceiveByte(LCD_SPI, 0xFF)<<8; 
	/* Read lower byte */
	tmp |= SPI_SendReceiveByte(LCD_SPI, 0xFF);
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET); 

	/* Set LCD control line(/CS) */
	LCD_NCS_HIGH();

  /* LCD_SPI prescaler: 2 */
  LCD_SPI->CR1 &= 0xFFC7;

  return tmp;
}

/**
 * @brief  Sets the LCD Text and Background colors.
 * @param  _TextColor : specifies the Text Color.
 * @param  _BackColor : specifies the Background Color.
 * @retval None
 */
void LCD_SetColors(__IO uint16_t _TextColor, __IO uint16_t _BackColor)
{
  TextColor = _TextColor; 
  BackColor = _BackColor;
}

/** 
 * @brief  Gets the LCD Text and Background colors.
 * @param  _TextColor : pointer to the variable that will contain the Text color.
 * @param  _BackColor : pointer to the variable that will contain the Background color.
 * @retval None
 */
void LCD_GetColors(__IO uint16_t *_TextColor, __IO uint16_t *_BackColor)
{
  *_TextColor = TextColor; *_BackColor = BackColor;
}

/**  
 * @brief  Sets the Text color.
 * @param  Color : specifies the Text color code RGB(5-6-5).
 * @retval None
 */
void LCD_SetTextColor(__IO uint16_t Color)
{
  TextColor = Color;
}


/** 
 * @brief  Sets the Background color.
 * @param  Color : specifies the Background color code RGB(5-6-5).
 * @retval None
 */
void LCD_SetBackColor(__IO uint16_t Color)
{
  BackColor = Color;
}

/**
 * @brief  Sets the Text Font.
 * @param  fonts : specifies the font to be used.
 * @retval None
 */
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}

/** 
 * @brief  Gets the Text Font.
 * @param  None.
 * @retval the used font.
 */
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/**  
 * @brief  Clears the selected line.
 * @param  Line : the Line to be cleared.
 *   This parameter can be @e LineX: where X can be 0..9
 * @retval None
 */
void LCD_ClearLine(uint8_t Line)
{
  uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;
  /* Send the string character by character on lCD */
  while (((refcolumn + 1) & 0xFFFF) >= LCD_Currentfonts->Width)
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, ' ');
    /* Decrement the column position by 16 */
    refcolumn -= LCD_Currentfonts->Width;
  }
}


/**  
 * @brief  Clears the hole LCD.
 * @param  Color : the color of the background.
 * @retval None
 */
void LCD_Clear(uint16_t Color)
{
	uint32_t length = MAX_PIXEL_NUM;

  LCD_SetCursor(0x00, 0x013F); 

	/* Prepare to write GRAM */	
  LCD_WriteRAM_Prepare(); 

	/* Reset LCD control line(/CS) */
	LCD_NCS_LOW();

  /* Send Start-Byte */
  LCD_nCS_StartByte(START_BYTE | LCD_WRITE_REG);

	for( ; length>0; length--)
	{
		/* Write 16-bit Reg */
		LCD_WriteRAM(Color);		
	}
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET); 

	/* Set LCD control line(/CS) */
	LCD_NCS_HIGH();
}

/** 
 * @brief  Clears the active window below the frame(200*320).
 * @param  void
 * @retval void
 */
void LCD_Clear_ActiveWindow( void )
{
	uint16_t length = 64000;

  LCD_SetCursor(0x28, 0x013F); 

	/* Prepare to write GRAM */	
  LCD_WriteRAM_Prepare(); 

	/* Reset LCD control line(/CS) */
	LCD_NCS_LOW();

  /* Send Start-Byte */
  LCD_nCS_StartByte(START_BYTE | LCD_WRITE_REG);

	for( ; length>0; length--)
	{
		/* Write 16-bit Reg */
		LCD_WriteRAM(DUMMY_WORD);		
	}
	while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET); 

	/* Set LCD control line(/CS) */
	LCD_NCS_HIGH();
}


/** 
 * @brief  Sets the cursor position.
 * @param  Xpos : specifies the X position.
 * @param  Ypos : specifies the Y position. 
 * @retval None
 */
void LCD_SetCursor(uint8_t Xpos, uint16_t Ypos)
{
  LCD_WriteReg(LCD_REG_32, Xpos);
  LCD_WriteReg(LCD_REG_33, Ypos);
}


/** 
 * @brief  Draws a character on LCD.
 * @param  Xpos : the Line where to display the character shape.
 * @param  Ypos : start column address.
 * @param  c : pointer to the character data.
 * @retval None																																							
 */
void LCD_DrawChar(uint8_t Xpos, uint16_t Ypos, const uint16_t *c)
{
  uint32_t index = 0, i = 0;
  uint8_t Xaddress = 0;
   
  Xaddress = Xpos;
  
  LCD_SetCursor(Xaddress, Ypos);
	  
  for(index = 0; index < LCD_Currentfonts->Height; index++)
  {
		/* Prepare to write GRAM */
  	LCD_WriteRAM_Prepare();

		/* Reset LCD control line(/CS) */
		LCD_NCS_LOW();	
	
	  /* Reset LCD control line(/CS) and Send Start-Byte */
	  LCD_nCS_StartByte(START_BYTE | LCD_WRITE_REG);
						   
    for(i = 0; i < LCD_Currentfonts->Width; i++)
    {
      if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&
				 (LCD_Currentfonts->Width <= 12))																									||
         (((c[index] & (0x1 << i)) == 0x00)																								&&
				 (LCD_Currentfonts->Width > 12 )))
      {
        LCD_WriteRAM(BackColor);
      }
      else
      {
        LCD_WriteRAM(TextColor);
      }
    }
		while(SPI_I2S_GetFlagStatus(LCD_SPI, SPI_I2S_FLAG_BSY) != RESET);

		/* Reset LCD control line(/CS) */
		LCD_NCS_HIGH();
				     
    Xaddress++;
    LCD_SetCursor(Xaddress, Ypos);
  }
}


/** 
 * @brief  Displays one character (16dots width, 24dots height).
 * @param  Line : the Line where to display the character shape .
 *   This parameter can be @e LineX: where X can be 0..9
 * @param  Column : start column address.
 * @param  Ascii : character ascii code, must be between 0x20 and 0x7E.
 * @retval None
 */
void LCD_DisplayChar(uint8_t Line, uint16_t Column, uint8_t Ascii)
{
  Ascii -= 32;
  LCD_DrawChar(Line, Column, &LCD_Currentfonts->table[Ascii * LCD_Currentfonts->Height]);
}


/** 
 * @brief  Displays a maximum of 20 char on the LCD.
 * @param  Line : the Line where to display the character shape .
 *   This parameter can be @e LineX: where X can be 0..9
 * @param  *ptr : pointer to string to display on LCD.
 * @retval None																																							
 */
void LCD_DisplayStringLine(uint8_t Line, uint8_t *ptr)
{
  uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (((refcolumn + 1) & 0xFFFF) >= LCD_Currentfonts->Width))
  {
    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by 16 */
    refcolumn -= LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;
  }
}

/**
 * @brief  Sets a display window
 * @param  Xpos : specifies the X buttom left position.
 * @param  Ypos : specifies the Y buttom left position.
 * @param  Height : display window height.
 * @param  Width : display window width.
 * @retval None
 */
void LCD_SetDisplayWindow(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  /* Horizontal GRAM Start Address */
  if(Xpos >= Height)
  {
    LCD_WriteReg(LCD_REG_80, (Xpos - Height + 1));
  }
  else
  {
    LCD_WriteReg(LCD_REG_80, 0);
  }

  /* Horizontal GRAM End Address */
  LCD_WriteReg(LCD_REG_81, Xpos);

  /* Vertical GRAM Start Address */
  if(Ypos >= Width)
  {
    LCD_WriteReg(LCD_REG_82, (Ypos - Width + 1));
  }  
  else
  {
    LCD_WriteReg(LCD_REG_82, 0);
  }
  /* Vertical GRAM End Address */
  LCD_WriteReg(LCD_REG_83, Ypos);
  
  LCD_SetCursor(Xpos, Ypos);
}


/**  
 * @brief  Disables LCD Window mode.
 * @param  None
 * @retval None
 */
void LCD_WindowModeDisable(void)
{
  LCD_SetDisplayWindow(239, 0x13F, 240, 320);
  LCD_WriteReg(LCD_REG_3, 0x1018);    
}


/** 
 * @brief  Power on the LCD.
 * @param  None
 * @retval None																																							 
 */
void LCD_PowerOn(void)
{
  /* Power On sequence ---------------------------------------------------------*/
  LCD_WriteReg(LCD_REG_16, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  LCD_WriteReg(LCD_REG_18, 0x0000); /* VREG1OUT voltage */
  LCD_WriteReg(LCD_REG_19, 0x0000); /* VDV[4:0] for VCOM amplitude */
  delay_ms(200);               /* Dis-charge capacitor power voltage (200ms) */
  LCD_WriteReg(LCD_REG_16, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  LCD_WriteReg(LCD_REG_17, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
  delay_ms(50);                /* Delay 50 ms */
  LCD_WriteReg(LCD_REG_18, 0x0139); /* VREG1OUT voltage */
  delay_ms(50);                /* delay 50 ms */
  LCD_WriteReg(LCD_REG_19, 0x1d00); /* VDV[4:0] for VCOM amplitude */
  LCD_WriteReg(LCD_REG_41, 0x0013); /* VCM[4:0] for VCOMH */
  delay_ms(50);                /* delay 50 ms */
  LCD_WriteReg(LCD_REG_7, 0x0173);  /* 262K color and display ON */
}


/** 
 * @brief  Enables the Display.
 * @param  None
 * @retval None
 */
void LCD_DisplayOn(void)
{
  /* Display On */
  LCD_WriteReg(LCD_REG_7, 0x0173); /* 262K color and display ON */ 
}


/** 
 * 	@brief  Disables the Display.
 * 	@param  None
 * 	@retval None
 */
void LCD_DisplayOff(void)
{
  /* Display Off */
  LCD_WriteReg(LCD_REG_7, 0x0);
}

/** 
 * @brief  Displays a maximum of 60 char on the LCD in the active window
 * @param  *ptr : pointer to string to display on LCD.
 * @retval None
 */
void LCD_DisplayString(char *ptr)
{
  uint16_t refcolumn = LCD_PIXEL_WIDTH - 1;
  uint8_t Line, cnt = 1, LineNumber = 6;
  
  LCD_SetCursor(0,0);
  Line = LINE(LineNumber);

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (((refcolumn + 1) & 0xFFFF) >= LCD_Currentfonts->Width))
  {

    /* Display one character on LCD */
    LCD_DisplayChar(Line, refcolumn, *ptr);
    /* Decrement the column position by 16 */
    refcolumn -= LCD_Currentfonts->Width;
    /* Point on the next character */
    ptr++;

  	if(*ptr == 0x0A)
		{
			ptr++;
			LineNumber++;
			Line = LINE(LineNumber);
			refcolumn = LCD_PIXEL_WIDTH - 1;
			cnt = 1;
		}
		else if(cnt == 20)
		{
			LineNumber++;
			Line = LINE(LineNumber);
			refcolumn = LCD_PIXEL_WIDTH - 1;
			cnt = 1;
		}
		else
		{
			cnt++;
		}
  }
}

/** 
 * @brief  Displays a line.
 * @param  Xpos : specifies the X position.
 * @param  Ypos : specifies the Y position.
 * @param  Length : line length.
 * @param  Direction : line direction.
 *   This parameter can be one of the following values: @e Vertical or @e Horizontal.
 * @retval None
 */
void LCD_DrawLine(uint8_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
  uint32_t i = 0;
  
  LCD_SetCursor(Xpos, Ypos);
  if(Direction == LCD_DIR_HORIZONTAL)
  { 
		/* Prepare to write GRAM */
    LCD_WriteRAM_Prepare();
		
		LCD_NCS_LOW();
		/* Send Start-Byte */
  	LCD_nCS_StartByte(START_BYTE | LCD_WRITE_REG);
		 
	  for(i = 0; i < Length; i++)
	  {
			LCD_WriteRAM(TextColor);
		}
			    
		/* Reset LCD control line(/CS) */
		LCD_NCS_HIGH();  
  }
  else
  {
   for(i = 0; i < Length; i++)
    {		 
			/* Prepare to write GRAM */
	    LCD_WriteRAM_Prepare(); 
    
			LCD_WriteRegData(TextColor);

      Xpos++;
      LCD_SetCursor(Xpos, Ypos);
    }
  }
}

/** 
 * @brief  Displays a rectangle.
 * @param  Xpos : specifies the X position.
 * @param  Ypos : specifies the Y position.
 * @param  Height : display rectangle height.
 * @param  Width : display rectangle width.
 * @retval None
 */
void LCD_DrawRect(uint8_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine((Xpos + Height), Ypos, Width, LCD_DIR_HORIZONTAL);
  
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine(Xpos, (Ypos - Width + 1), Height, LCD_DIR_VERTICAL);
}
 
/** 
 * @brief  Displays a full rectangle.
 * @param  Xpos : specifies the X position.
 * @param  Ypos : specifies the Y position.
 * @param  Height : rectangle height.
 * @param  Width : rectangle width.
 * @retval None
 */
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  LCD_SetTextColor(TextColor);

  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine((Xpos + Height), Ypos, Width, LCD_DIR_HORIZONTAL);
  
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine(Xpos, (Ypos - Width + 1), Height, LCD_DIR_VERTICAL);

  Width -= 2;
  Height--;
  Ypos--;

  LCD_SetTextColor(BackColor);

  while(Height--)
  {
    LCD_DrawLine(++Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);    
  }

  LCD_SetTextColor(TextColor);
}

/** 
 * @brief  Initializes the SPI in order to communicate with the LCD
 * @param  None
 * @return None 
 */
void LCD_Init(void)
{
	SPI_I2S_DeInit(LCD_SPI);
	LCD_RCC_Config( );
	LCD_GPIO_Config( );	
	LCD_Structure_Config( );
}

/**
 * @brief  Initializes clock
 * @param  None
 * @return None 
 */
void LCD_RCC_Config(void)
{	
	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(	LCD_SPI_SCK_GPIO_CLK	|
							LCD_SPI_MISO_GPIO_CLK	|
							LCD_SPI_MOSI_GPIO_CLK,
							ENABLE);
	
	/* Enable SPI clock  */
	RCC_APB1PeriphClockCmd(LCD_SPI_CLK, ENABLE);
	
	/* Enable DMA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
}

/** 
 * @brief  Initializes GPIO
 * @param  None
 * @return None 
 */
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* Configure SPI pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_MISO_PIN;
	GPIO_Init(LCD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN;
	GPIO_Init(LCD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configure NCS,NWR,RS in Output Push-Pull mode */
	GPIO_InitStructure.GPIO_Pin = LCD_NCS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(LCD_NCS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_NWR_PIN;
	GPIO_Init(LCD_NWR_GPIO_PORT, &GPIO_InitStructure);
	
	/* NCS/RS/NWR High Initially */ 
	GPIO_SetBits(LCD_NCS_GPIO_PORT, LCD_NCS_PIN);
	GPIO_SetBits(LCD_RS_GPIO_PORT	, LCD_RS_PIN); 
	GPIO_SetBits(LCD_NWR_GPIO_PORT, LCD_NWR_PIN);   
}


/**
 * @brief  Initializes & enable the SPI for LCD
 * @param  None
 * @retval None 
 */ 
void LCD_Structure_Config( void )
{
	SPI_InitTypeDef  SPI_InitStructure;

	/* Initialize the SPI with default values */
	SPI_StructInit(&SPI_InitStructure);

	/* SPI Config */
	SPI_InitStructure.SPI_Direction 		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode 				= SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize 			= SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL 				= SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA 				= SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS 				= SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitStructure.SPI_FirstBit 			= SPI_FirstBit_MSB;
	SPI_Init(LCD_SPI, &SPI_InitStructure);

	/* SPI enable */
	SPI_Cmd(LCD_SPI, ENABLE);
}

/**
 * @brief  DeInitializes the LCD.
 * @param  None
 * @retval None
 */ 
void LCD_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* LCD_SPI disable */
	SPI_Cmd(LCD_SPI, DISABLE);
	
	/* LCD_SPI DeInit */
	SPI_I2S_DeInit(LCD_SPI);
	 
	/* Disable SPI clock  */
	RCC_APB1PeriphClockCmd(LCD_SPI_CLK, DISABLE);
	  
	/* Configure NCS in Output Push-Pull mode */
	GPIO_InitStructure.GPIO_Pin = LCD_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(LCD_NCS_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configure NWR(RNW), RS in Output Push-Pull mode */
	GPIO_InitStructure.GPIO_Pin = LCD_RS_PIN;
	GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_NWR_PIN;
	GPIO_Init(LCD_NWR_GPIO_PORT, &GPIO_InitStructure);
	
	/* Configure SPI pins: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_SCK_PIN;
	GPIO_Init(LCD_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_MISO_PIN;
	GPIO_Init(LCD_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LCD_SPI_MOSI_PIN;
	GPIO_Init(LCD_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);
}


/**
* @brief  
* @param  None
* @retval None
*/ 
int8_t InitializeDisplayConfigStructure (LCD_DisplayConfigStruct *LCD_DisplayConfig)
{
sFONT 	*CurrentFonts; 
	
	CurrentFonts = LCD_GetFont ();
	LCD_DisplayConfig->CurrentFonts.Width = CurrentFonts->Width;
	LCD_DisplayConfig->CurrentFonts.Height = CurrentFonts->Height;
	LCD_DisplayConfig->LCDWidth = LCD_PIXEL_WIDTH;
	LCD_DisplayConfig->LCDHeight = LCD_PIXEL_HEIGHT;
	LCD_DisplayConfig->NbLineMax = LCD_DisplayConfig->LCDHeight / LCD_DisplayConfig->CurrentFonts.Height;
	LCD_DisplayConfig->NbCarMax = LCD_DisplayConfig->LCDWidth / LCD_DisplayConfig->CurrentFonts.Width;
	return RESULTOK;
}
/**
* @brief 
* @param  None
* @retval None
*/ 
int8_t UpdateDisplayConfigStructure (LCD_DisplayConfigStruct *LCD_DisplayConfig)
{
	LCD_DisplayConfig->NbLineMax = LCD_DisplayConfig->LCDHeight / LCD_DisplayConfig->CurrentFonts.Height;
	LCD_DisplayConfig->NbCarMax = LCD_DisplayConfig->LCDWidth / LCD_DisplayConfig->CurrentFonts.Width;
	return RESULTOK;
}

/**
* @brief  
* @param  	LineToDisplay 	: 	string to display
			color		  	:	font color
			LeftOrRight		: 	align left or right 
* @retval None
*/ 
int8_t LCD_DisplayStringLineAdv (LCD_DisplayConfigStruct *LCD_DisplayConfig,
								char *LineToDisplay,
								uint8_t NbCarDisplay,
								uint16_t color,
								uint8_t LeftOrRight)
{
uint8_t StringToDisplay[MAX_LCDBUFFER_SIZE];

	LCD_SetTextColor(color);	

	LCD_ClearLine(LCD_DisplayConfig->CurrentLine);
	
	if (LeftOrRight == LCD_ALIGN_LEFT)
		LCD_DisplayStringLine(LCD_DisplayConfig->CurrentLine,(uint8_t *)LineToDisplay);
	// align the string to the right
	else
	{
		memset(StringToDisplay,' ',LCD_DisplayConfig->NbCarMax);
		if (LCD_DisplayConfig->NbCarMax<NbCarDisplay)
			memcpy(StringToDisplay,LineToDisplay,LCD_DisplayConfig->NbCarMax);
		else 
		 	memcpy(&StringToDisplay[LCD_DisplayConfig->NbCarMax-NbCarDisplay],LineToDisplay,NbCarDisplay);
		
		LCD_DisplayStringLine(LCD_DisplayConfig->CurrentLine ,StringToDisplay);
	}
	
	return RESULTOK;

}




/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
