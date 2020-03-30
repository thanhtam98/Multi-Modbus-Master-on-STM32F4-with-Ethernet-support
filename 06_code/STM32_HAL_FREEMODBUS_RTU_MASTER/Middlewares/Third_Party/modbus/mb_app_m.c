/*
    Brief: Optional functions callback when Modbus sequences are called in Master mode 
    Author: NTT
    Date: 30-3-2020
    Version: 1.0
*/
/*Built-in C lib includes-----------------*/

/*Platform includes-----------------------*/

/*FreeRTOS includes-----------------------*/
/*Modbus platform includes----------------*/
#include "mb_app_m.h"
#include "mb_m.h"
#include "mbconfig.h"
#include "mb.h"
/*Private define -------------------------*/

#define M_REG_HOLDING_START           0
#define M_REG_HOLDING_NREGS           40

/*Private variable------------------------*/


USHORT   usMRegHoldStart                            = M_REG_HOLDING_START;
USHORT   usMRegHoldBuf[MB_RS485_MAX_PORT][MB_MASTER_TOTAL_SLAVE_NUM][M_REG_HOLDING_NREGS];




/*Start inplementation--------------------------*/


eMBErrorCode eMBMasterRegHoldingCB(UCHAR ucPort, UCHAR * pucRegBuffer, USHORT usAddress,
		USHORT usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    USHORT          iRegIndex;
    USHORT *        pusRegHoldingBuf;
    USHORT          REG_HOLDING_START;
    USHORT          REG_HOLDING_NREGS;
    USHORT          usRegHoldStart;

	pusRegHoldingBuf = usMRegHoldBuf[ucPort][ucMBMasterGetDestAddress(ucPort) - 1];
    REG_HOLDING_START = M_REG_HOLDING_START;
    REG_HOLDING_NREGS = M_REG_HOLDING_NREGS;
    usRegHoldStart = usMRegHoldStart;
    /* if mode is read, the master will write the received date to buffer. */
    eMode = MB_REG_READ;

    /* it already plus one in modbus function method. Tao dell biet no inc lam gi de roi dec xuong */
    usAddress--;

    if ((usAddress >= REG_HOLDING_START)
            && (usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS))
    {
        iRegIndex = usAddress - usRegHoldStart;
        switch (eMode)
        {
        /* read current register values from the protocol stack. */
        //Swap this case 
        case MB_REG_WRITE:
            while (usNRegs > 0)
            {
                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] >> 8);
                *pucRegBuffer++ = (UCHAR) (pusRegHoldingBuf[iRegIndex] & 0xFF);
                iRegIndex++;
                usNRegs--;
            }
            break;
        /* write current register values with new values from the protocol stack. */
        case MB_REG_READ:
            while (usNRegs > 0)
            {
                pusRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                pusRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
               usNRegs--;
            }
            break;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}