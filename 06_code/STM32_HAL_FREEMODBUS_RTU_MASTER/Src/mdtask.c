#include "stm32f4xx_hal.h"
#include "cmsis_os.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

extern TIM_HandleTypeDef htim7;

#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 8

static USHORT usRegInputStart = REG_INPUT_START;
static USHORT usRegInputBuf[REG_INPUT_NREGS];

void ModbusRTUTask(void const *argument)
{
    /* ABCDEF */
    usRegInputBuf[0] = 11;
    usRegInputBuf[1] = 22;
    usRegInputBuf[2] = 33;
    usRegInputBuf[3] = 44;
    usRegInputBuf[4] = 55;
    usRegInputBuf[5] = 66;
    usRegInputBuf[6] = 77;
    usRegInputBuf[7] = 88;
//eMBMasterInit(eMBMode eMode, UCHAR ucPort, ULONG ulBaudRate, eMBParity eParity)
    eMBErrorCode eStatus = eMBMasterInit(MB_RTU, 1,  9600, MB_PAR_NONE);
    eStatus = eMBMasterEnable(PORT1);
    eStatus = eMBMasterEnable(PORT2);
    eStatus = eMBMasterEnable(PORT3);
    eStatus = eMBMasterEnable(PORT4);

		 // HAL_TIM_Base_Start_IT(&htim7);
    while (1)
    {
        eMBMasterPoll();
				vTaskDelay(10);
    }
}

void ModbusTestTask(void const *argument)
{
	vTaskDelay(500);
	printf("ModbusTestTask: Starting");
	eMBErrorCode   eStatus = MB_ENOERR;
		
	while (1)
	{
			//eMBMasterReqReadHoldingRegister(0x00,0x01,0x00,2,1);
			eMBMasterReqWriteHoldingRegister(0x00, 0x01,0x01,0xFA,0x01);
			vTaskDelay(1000);
			printf("\r\n ModbusTestTask: Sent data to dest");
	}
}

eMBErrorCode
eMBRegInputCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs)
{
    eMBErrorCode eStatus = MB_ENOERR;
    int iRegIndex;

    if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
    {
        iRegIndex = (int)(usAddress - usRegInputStart);
        while (usNRegs > 0)
        {
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] >> 8);
            *pucRegBuffer++ =
                (unsigned char)(usRegInputBuf[iRegIndex] & 0xFF);
            iRegIndex++;
            usNRegs--;
        }

        //	HAL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
    }
    else
    {
        // HAL_GPIO_TogglePin(LD5_GPIO_Port, LD5_Pin);
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNRegs,
                eMBRegisterMode eMode)
{
    return MB_ENOERR;
}

eMBErrorCode
eMBRegCoilsCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNCoils,
              eMBRegisterMode eMode)
{
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB(UCHAR *pucRegBuffer, USHORT usAddress, USHORT usNDiscrete)
{
    return MB_ENOREG;
}
