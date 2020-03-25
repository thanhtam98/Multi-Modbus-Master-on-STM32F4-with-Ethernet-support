#include "stm32f4xx_uartstdio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

static const char * const g_pcHex = "0123456789abcdef";
static USART_TypeDef* g_ulBase = 0;

void UART_RCC_Configuration(USART_TypeDef* ulBase)
{
  /* --------------------------- System Clocks Configuration -----------------*/
		if(ulBase == USART1)
		{
			/* UARTx clock enable */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
			/* GPIOA clock enable */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		}
		else if(ulBase == USART2)
		{
			/* UARTx clock enable */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
			/* GPIOA clock enable */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		}
		else if(ulBase == USART3)
		{
			/* UARTx clock enable */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
			/* GPIOA clock enable */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		}
		else if(ulBase == UART4)
		{
			/* UARTx clock enable */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
			/* GPIOA clock enable */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		}
		else if(ulBase == UART5)
		{
			/* UARTx clock enable */
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
			/* GPIOA clock enable */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
		}
		else if(ulBase == USART6)
		{
			/* UARTx clock enable */
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
			/* GPIOA clock enable */
			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
		}
}
 
void UART_GPIO_Configuration(USART_TypeDef* ulBase)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_TypeDef* GPIOx0;
	GPIO_TypeDef* GPIOx1;
	uint16_t GPIO_Pin0, GPIO_Pin1;
	uint8_t GPIO_AF_USARTx;
	uint8_t GPIO_PinSource;
	
  /*-------------------------- GPIO Configuration ----------------------------*/
	if(ulBase == USART1)
	{
		GPIO_Pin0 = GPIO_Pin_9;
		GPIOx0 = GPIOA;
		GPIO_Pin1 = GPIO_Pin_10;
		GPIOx1 = GPIOA;
		GPIO_AF_USARTx = GPIO_AF_USART1;
	}
	else if(ulBase == USART2)
	{
		GPIO_Pin0 = GPIO_Pin_6;
		GPIOx0 = GPIOD;
		GPIO_Pin1 = GPIO_Pin_5;
		GPIOx1 = GPIOD;
		GPIO_AF_USARTx = GPIO_AF_USART2;
	}
	else if(ulBase == USART3)
	{
// 		GPIO_Pin0 = GPIO_Pin_10;
// 		GPIOx0 = GPIOB;
// 		GPIO_Pin1 = GPIO_Pin_11;
// 		GPIOx1 = GPIOB;
		GPIO_Pin0 = GPIO_Pin_8;
		GPIOx0 = GPIOD;
		GPIO_Pin1 = GPIO_Pin_9;
		GPIOx1 = GPIOD;
		GPIO_AF_USARTx = GPIO_AF_USART3;
	}
	else if(ulBase == UART4)
	{
		GPIO_Pin0 = GPIO_Pin_10;
		GPIOx0 = GPIOC;
		GPIO_Pin1 = GPIO_Pin_11;
		GPIOx1 = GPIOC;
		GPIO_AF_USARTx = GPIO_AF_UART4;
	}
	if(ulBase == UART5)
	{
		GPIO_Pin0 = GPIO_Pin_12;
		GPIOx0 = GPIOC;
		GPIO_Pin1 = GPIO_Pin_2;
		GPIOx1 = GPIOD;
		GPIO_AF_USARTx = GPIO_AF_UART5;
	}
	if(ulBase == USART6)
	{
		GPIO_Pin0 = GPIO_Pin_6;
		GPIOx0 = GPIOC;
		GPIO_Pin1 = GPIO_Pin_7;
		GPIOx1 = GPIOC;
		GPIO_AF_USARTx = GPIO_AF_USART6;
	}
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
  GPIO_Init(GPIOx0, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin1; 
	GPIO_Init(GPIOx1, &GPIO_InitStructure);
	/* Connect UART pins to AF */
	for(GPIO_PinSource=0;GPIO_PinSource <=15;GPIO_PinSource++)
	{
		GPIO_Pin0 >>=1;
		if(GPIO_Pin0==0)
			break;
	}
  GPIO_PinAFConfig(GPIOx0, GPIO_PinSource, GPIO_AF_USARTx);
	for(GPIO_PinSource=0;GPIO_PinSource <=15;GPIO_PinSource++)
	{
		GPIO_Pin1 >>=1;
		if(GPIO_Pin1==0)
			break;
	}
  GPIO_PinAFConfig(GPIOx1, GPIO_PinSource, GPIO_AF_USARTx);
}

void UARTstdio_Configuration(USART_TypeDef* Uartx,uint32_t BaudRate)
{
  USART_InitTypeDef USART_InitStructure;
	g_ulBase = Uartx;
	/*****************************************************************************/
	/* Configured CLOCK for UARTSTDIO*/
	UART_RCC_Configuration(Uartx);
	/*****************************************************************************/
	/* Configured GPIO for UARTSTDIO*/
	UART_GPIO_Configuration(Uartx);
  /* USARTx configuration ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = BaudRate baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = BaudRate;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
  USART_Init(g_ulBase, &USART_InitStructure);
 
  USART_Cmd(g_ulBase, ENABLE);
}
 
//*****************************************************************************
//
//! Read a single character from the UART, blocking if necessary.
//!
//! This function will receive a single character from the UART and store it at
//! the supplied address.
//!
//! In both buffered and unbuffered modes, this function will block until a
//! character is received.  If non-blocking operation is required in buffered
//! mode, a call to UARTRxAvail() may be made to determine whether any
//! characters are currently available for reading.
//!
//! \return Returns the character read.
//
//*****************************************************************************
unsigned char
UARTgetc(void)
{
    //
    // Block until a character is received by the UART then return it to
    // the caller.
    //
		while(USART_GetFlagStatus(g_ulBase, USART_FLAG_RXNE) == RESET);
    return(USART_ReceiveData(g_ulBase));
}
//*****************************************************************************
//
//! A simple UART based get string function, with some line processing.
//!
//! \param pcBuf points to a buffer for the incoming string from the UART.
//! \param ulLen is the length of the buffer for storage of the string,
//! including the trailing 0.
//!
//! This function will receive a string from the UART input and store the
//! characters in the buffer pointed to by \e pcBuf.  The characters will
//! continue to be stored until a termination character is received.  The
//! termination characters are CR, LF, or ESC.  A CRLF pair is treated as a
//! single termination character.  The termination characters are not stored in
//! the string.  The string will be terminated with a 0 and the function will
//! return.
//!
//! In both buffered and unbuffered modes, this function will block until
//! a termination character is received.  If non-blocking operation is required
//! in buffered mode, a call to UARTPeek() may be made to determine whether
//! a termination character already exists in the receive buffer prior to
//! calling UARTgets().
//!
//! Since the string will be null terminated, the user must ensure that the
//! buffer is sized to allow for the additional null character.
//!
//! \return Returns the count of characters that were stored, not including
//! the trailing 0.
//
//*****************************************************************************
unsigned int
UARTgets(char *pcBuf, unsigned long ulLen)
{
    unsigned long ulCount = 0;
    char cChar;
    static char bLastWasCR = 0;
    //
    // Adjust the length back by 1 to leave space for the trailing
    // null terminator.
    //
    ulLen--;

    //
    // Process characters until a newline is received.
    //
    while(1)
    {
        //
        // Read the next character from the console.
        //
				while(USART_GetFlagStatus(g_ulBase, USART_FLAG_RXNE) == RESET);
        cChar = USART_ReceiveData(g_ulBase);

        //
        // See if the backspace key was pressed.
        //
        if(cChar == '\b')
        {
            //
            // If there are any characters already in the buffer, then delete
            // the last.
            //
            if(ulCount)
            {
                //
                // Rub out the previous character.
                //
                UARTwrite("\b \b", 3);

                //
                // Decrement the number of characters in the buffer.
                //
                ulCount--;
            }

            //
            // Skip ahead to read the next character.
            //
            continue;
        }

        //
        // If this character is LF and last was CR, then just gobble up the
        // character because the EOL processing was taken care of with the CR.
        //
        if((cChar == '\n') && bLastWasCR)
        {
            bLastWasCR = 0;
            continue;
        }

        //
        // See if a newline or escape character was received.
        //
        if((cChar == '\r') || (cChar == '\n') || (cChar == 0x1b))
        {
            //
            // If the character is a CR, then it may be followed by a LF which
            // should be paired with the CR.  So remember that a CR was
            // received.
            //
            if(cChar == '\r')
            {
                bLastWasCR = 1;
            }

            //
            // Stop processing the input and end the line.
            //
            break;
        }

        //
        // Process the received character as long as we are not at the end of
        // the buffer.  If the end of the buffer has been reached then all
        // additional characters are ignored until a newline is received.
        //
        if(ulCount < ulLen)
        {
            //
            // Store the character in the caller supplied buffer.
            //
            pcBuf[ulCount] = cChar;

            //
            // Increment the count of characters received.
            //
            ulCount++;

            //
            // Reflect the character back to the user.
            //
// 						while(USART_GetFlagStatus(g_ulBase, USART_FLAG_TXE) == RESET);
//             USART_SendData(g_ulBase, cChar);
						if(ulCount == ulLen)
							break;
        }
    }

    //
    // Add a null termination to the string.
    //
		if(bLastWasCR)
			pcBuf[ulCount] = '\n';
		else
			pcBuf[ulCount] = 0;

    //
    // Send a CRLF pair to the terminal to end the line.
    //
    //UARTwrite("\r\n", 2);

    //
    // Return the count of chars in the buffer, not counting the trailing 0.
    //
    return(ulCount);
}

//*****************************************************************************
//
//! Write a single character from the UART, blocking if necessary.
//!
//! This function will send a single character to the UART
//!
//! In both buffered and unbuffered modes, this function will block until a
//! character is sent.  If non-blocking operation is required in buffered
//! mode, a call to UARTRxAvail() may be made to determine whether any
//! characters are currently available for reading.
//!
//! \return None
//
//*****************************************************************************
int UARTputc(char ch)
{
  USART_SendData(g_ulBase,ch);
	while(USART_GetFlagStatus(g_ulBase, USART_FLAG_TXE) == RESET);
	return 1;
}
//*****************************************************************************
//
//! Writes a string of characters to the UART output.
//!
//! \param pcBuf points to a buffer containing the string to transmit.
//! \param ulLen is the length of the string to transmit.
//!
//! This function will transmit the string to the UART output.  The number of
//! characters transmitted is determined by the \e ulLen parameter.  This
//! function does no interpretation or translation of any characters.  Since
//! the output is sent to a UART, any LF (/n) characters encountered will be
//! replaced with a CRLF pair.
//!
//! Besides using the \e ulLen parameter to stop transmitting the string, if a
//! null character (0) is encountered, then no more characters will be
//! transmitted and the function will return.
//!
//! In non-buffered mode, this function is blocking and will not return until
//! all the characters have been written to the output FIFO.  In buffered mode,
//! the characters are written to the UART transmit buffer and the call returns
//! immediately.  If insufficient space remains in the transmit buffer,
//! additional characters are discarded.
//!
//! \return Returns the count of characters written.
//
//*****************************************************************************
int
UARTwrite(const char *pcBuf, unsigned long ulLen)
{
	unsigned int uIdx;
	for(uIdx = 0; uIdx < ulLen; uIdx++)
  {
		if(*pcBuf == '\n')
			USART_SendData(g_ulBase,'\n');
		else
			USART_SendData(g_ulBase,*(pcBuf++));
		while(USART_GetFlagStatus(g_ulBase, USART_FLAG_TXE) == RESET);
	}
	return(uIdx);
}

struct __FILE
{
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};
/* FILE is typedef’d in stdio.h. */
FILE __stdout;
int fputc(int ch, FILE *f)
{
	USART_SendData(g_ulBase,ch);
	while(USART_GetFlagStatus(g_ulBase, USART_FLAG_TXE) == RESET);
	return 1;
}
int ferror(FILE *f)
{
  /* Your implementation of ferror(). */
  return 0;
}

//*****************************************************************************
//
//! A simple UART based printf function supporting \%c, \%d, \%p, \%s, \%u,
//! \%x, and \%X.
//!
//! \param pcString is the format string.
//! \param ... are the optional arguments, which depend on the contents of the
//! format string.
//!
//! This function is very similar to the C library <tt>fprintf()</tt> function.
//! All of its output will be sent to the UART.  Only the following formatting
//! characters are supported:
//!
//! - \%c to print a character
//! - \%d to print a decimal value
//! - \%s to print a string
//! - \%u to print an unsigned decimal value
//! - \%x to print a hexadecimal value using lower case letters
//! - \%X to print a hexadecimal value using lower case letters (not upper case
//! letters as would typically be used)
//! - \%f to print a float
//! - \%p to print a pointer as a hexadecimal value
//! - \%\% to print out a \% character
//!
//! For \%s, \%d, \%u, \%p, \%x, and \%X, an optional number may reside
//! between the \% and the format character, which specifies the minimum number
//! of characters to use for that value; if preceded by a 0 then the extra
//! characters will be filled with zeros instead of spaces.  For example,
//! ``\%8d'' will use eight characters to print the decimal value with spaces
//! added to reach eight; ``\%08d'' will use eight characters as well but will
//! add zeroes instead of spaces.
//!
//! The type of the arguments after \e pcString must match the requirements of
//! the format string.  For example, if an integer was passed where a string
//! was expected, an error of some kind will most likely occur.
//!
//! \return None.
//
//*****************************************************************************
 void
 UARTprintf(const char *pcString, ...)
{
    unsigned long ulIdx, ulValue, ulPos, ulCount, ulBase, ulNeg;
    char *pcStr, pcBuf[16], cFill;
		__IO float fValue;
		uint8_t ucStart=0;
		uint8_t i=0;
    va_list vaArgP;

    //
    // Check the arguments.
    //
   // ASSERT(pcString != 0);

    //
    // Start the varargs processing.
    //
    va_start(vaArgP, pcString);

    //
    // Loop while there are more characters in the string.
    //
    while(*pcString)
    {
        //
        // Find the first non-% character, or the end of the string.
        //
        for(ulIdx = 0; (pcString[ulIdx] != '%') && (pcString[ulIdx] != '\0');
            ulIdx++)
        {
        }

        //
        // Write this portion of the string.
        //
        UARTwrite(pcString, ulIdx);

        //
        // Skip the portion of the string that was written.
        //
        pcString += ulIdx;

        //
        // See if the next character is a %.
        //
        if(*pcString == '%')
        {
            //
            // Skip the %.
            //
            pcString++;

            //
            // Set the digit count to zero, and the fill character to space
            // (i.e. to the defaults).
            //
            ulCount = 0;
            cFill = ' ';

            //
            // It may be necessary to get back here to process more characters.
            // Goto's aren't pretty, but effective.  I feel extremely dirty for
            // using not one but two of the beasts.
            //
again:

            //
            // Determine how to handle the next character.
            //
            switch(*pcString++)
            {
                //
                // Handle the digit characters.
                //
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                {
                    //
                    // If this is a zero, and it is the first digit, then the
                    // fill character is a zero instead of a space.
                    //
                    if((pcString[-1] == '0') && (ulCount == 0))
                    {
                        cFill = '0';
                    }

                    //
                    // Update the digit count.
                    //
                    ulCount *= 10;
                    ulCount += pcString[-1] - '0';

                    //
                    // Get the next character.
                    //
                    goto again;
                }

                //
                // Handle the %c command.
                //
                case 'c':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ulValue = va_arg(vaArgP, unsigned long);

                    //
                    // Print out the character.
                    //
                    UARTwrite((char *)&ulValue, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %d command.
                //
                case 'd':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ulValue = va_arg(vaArgP, unsigned long);

                    //
                    // Reset the buffer position.
                    //
 convertfloat:       ulPos = 0;

                    //
                    // If the value is negative, make it positive and indicate
                    // that a minus sign is needed.
                    //
                    if((long)ulValue < 0)
                    {
                        //
                        // Make the value positive.
                        //
                        ulValue = -(long)ulValue;

                        //
                        // Indicate that the value is negative.
                        //
                        ulNeg = 1;
                    }
                    else
                    {
                        //
                        // Indicate that the value is positive so that a minus
                        // sign isn't inserted.
                        //
                        ulNeg = 0;
                    }

                    //
                    // Set the base to 10.
                    //
                    ulBase = 10;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }
								case 'f':
								{
//									fValue = va_arg(vaArgP, float);
									ulValue = (long) fValue;
									ucStart =1;
									goto convertfloat;
nextfloat:				ucStart =0;
									if(fValue<0)
										fValue = -fValue;
									fValue = (fValue-ulValue);
									for(i=0;i<7;i++)
									{
										fValue = fValue*10;
										ulValue = (long) fValue;
										if(ulValue !=0)
											break;
									}
									UARTwrite(".",1);
									for(ulValue=0;ulValue<i;ulValue++)
										UARTwrite("0",1);
									for(i=7-i;i>0;i--)
										fValue = fValue*10;
									ulValue = (long) fValue;
									if(fValue==0)
										break;
									ulValue = (unsigned long)fValue;
									goto convertfloat;
								}

                //
                // Handle the %s command.
                //
                case 's':
                {
                    //
                    // Get the string pointer from the varargs.
                    //
                    pcStr = va_arg(vaArgP, char *);

                    //
                    // Determine the length of the string.
                    //
                    for(ulIdx = 0; pcStr[ulIdx] != '\0'; ulIdx++)
                    {
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcStr, ulIdx);

                    //
                    // Write any required padding spaces
                    //
                    if(ulCount > ulIdx)
                    {
                        ulCount -= ulIdx;
                        while(ulCount--)
                        {
                            UARTwrite(" ", 1);
                        }
                    }
                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle the %u command.
                //
                case 'u':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ulValue = va_arg(vaArgP, unsigned long);

                    //
                    // Reset the buffer position.
                    //
                    ulPos = 0;

                    //
                    // Set the base to 10.
                    //
                    ulBase = 10;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ulNeg = 0;

                    //
                    // Convert the value to ASCII.
                    //
                    goto convert;
                }

                //
                // Handle the %x and %X commands.  Note that they are treated
                // identically; i.e. %X will use lower case letters for a-f
                // instead of the upper case letters is should use.  We also
                // alias %p to %x.
                //
                case 'x':
                case 'X':
                case 'p':
                {
                    //
                    // Get the value from the varargs.
                    //
                    ulValue = va_arg(vaArgP, unsigned long);
										UARTwrite("0x",2);
                    //
                    // Reset the buffer position.
                    //
                    ulPos = 0;

                    //
                    // Set the base to 16.
                    //
                    ulBase = 16;

                    //
                    // Indicate that the value is positive so that a minus sign
                    // isn't inserted.
                    //
                    ulNeg = 0;

                    //
                    // Determine the number of digits in the string version of
                    // the value.
                    //
convert:
                    for(ulIdx = 1;
                        (((ulIdx * ulBase) <= ulValue) &&
                         (((ulIdx * ulBase) / ulBase) == ulIdx));
                        ulIdx *= ulBase, ulCount--)
                    {
                    }

                    //
                    // If the value is negative, reduce the count of padding
                    // characters needed.
                    //
                    if(ulNeg)
                    {
                        ulCount--;
                    }

                    //
                    // If the value is negative and the value is padded with
                    // zeros, then place the minus sign before the padding.
                    //
                    if(ulNeg && (cFill == '0'))
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ulPos++] = '-';

                        //
                        // The minus sign has been placed, so turn off the
                        // negative flag.
                        //
                        ulNeg = 0;
                    }

                    //
                    // Provide additional padding at the beginning of the
                    // string conversion if needed.
                    //
                    if((ulCount > 1) && (ulCount < 16))
                    {
                        for(ulCount--; ulCount; ulCount--)
                        {
                            pcBuf[ulPos++] = cFill;
                        }
                    }

                    //
                    // If the value is negative, then place the minus sign
                    // before the number.
                    //
                    if(ulNeg)
                    {
                        //
                        // Place the minus sign in the output buffer.
                        //
                        pcBuf[ulPos++] = '-';
                    }

                    //
                    // Convert the value into a string.
                    //
                    for(; ulIdx; ulIdx /= ulBase)
                    {
                        pcBuf[ulPos++] = g_pcHex[(ulValue / ulIdx) % ulBase];
                    }

                    //
                    // Write the string.
                    //
                    UARTwrite(pcBuf, ulPos);

                    //
                    // This command has been handled.
                    //
										if(ucStart)
											goto nextfloat;
										else
											break;
                }

                //
                // Handle the %% command.
                //
                case '%':
                {
                    //
                    // Simply write a single %.
                    //
                    UARTwrite(pcString - 1, 1);

                    //
                    // This command has been handled.
                    //
                    break;
                }

                //
                // Handle all other commands.
                //
                default:
                {
                    //
                    // Indicate an error.
                    //
                    UARTwrite("ERROR", 5);

                    //
                    // This command has been handled.
                    //
                    break;
                }
            }
        }
    }

    //
    // End the varargs processing.
    //
    va_end(vaArgP);
}
