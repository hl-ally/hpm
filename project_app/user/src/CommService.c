#include <stdio.h>
#include <string.h>
#include <stdio.h>
#include <string.h>
//#include "SystemDefine.h"
#include "Api_CommandQueue.h"
#include "Api_UsbDevice.h"
#include "Api_Flash.h"
//#include "Api_Gpio.h"
#include "Command.h"
#include "CommService.h"
#include "Communication.h"
#include "ProtocolAnalysis.h"
#include "Evaluation.h"


/*************************************************************************************************

 *************************************************************************************************/

ATTR_WEAK void UsartSendPacketForComm(uint8_t *pBuf, uint16_t nLen)
{
//    uint8_t pPack[PROTOCOL_RX_SIZE + 3] = {FN_BYTE(UART_HEAD, 1), FN_BYTE(UART_HEAD, 0), 0x00};
//
//    nLen = FN_MIN(PROTOCOL_RX_SIZE, nLen);
//    MemCpy(&pPack[2], pBuf, nLen);
//    pPack[2 + nLen] = GetSum(pPack, 2 + nLen);
//
//    UartSendPacket(pPack, nLen + 3);
}

eCmdType_t GetCmdType(uint8_t *pBuf, uint8_t nDataLen)
{
    eCmdType_t eCmdType = eCmdTypeUnknown;

    if (pBuf[0] == CMD_FC_REPORT_ID ||
            pBuf[0] == CMD_FE_REPORT_ID ||
            pBuf[0] == CMD_F0_REPORT_ID)
    {
        eCmdType = eCmdTypeCD;
    }
    else if (pBuf[0] == 0x55)
    {
        eCmdType = eCmdTypeFCT;
    }
    return eCmdType;
}


ATTR_WEAK CmdAnswerType CustomerCommunication(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{
    return CMD_ANSWER_NONE;
}


void AnswerCommand(uint8_t pAnsCmdBuf[], uint32_t nlength, eCmdSource_t eCmdSource)
{
    if ((nlength == 0) || (nlength > CMD_QUEUE_DATA_MAX_SIZE) )
    {
        printf("\r\n boot Send Max Length !nlength = %d,MAX:%d\r\n",nlength,CMD_QUEUE_DATA_MAX_SIZE);
        return;
    }

    switch(eCmdSource)
    {
        case eUsb0Ep1Mode:
        case eUsb0Ep2Mode:
        case eUsb0Ep3Mode:
        case eUsb0Ep4Mode:
        case eUsb0Ep5Mode:
        case eUsb0Ep6Mode:
        case eUsb0Ep7Mode:
        {
            USBEPSendPacket(eUsbDev0, (uint8_t)(eCmdSource), pAnsCmdBuf, CMD_QUEUE_DATA_MAX_SIZE);
//            printf("AnswerCommand, source:%d,len:%d,send:%x,%x,%x\n",eCmdSource,nlength,pAnsCmdBuf[0],pAnsCmdBuf[1],pAnsCmdBuf[2]);
        }
        break;

        case eUsb1Ep1Mode:
        case eUsb1Ep2Mode:
        case eUsb1Ep3Mode:
        case eUsb1Ep4Mode:
        case eUsb1Ep5Mode:
        case eUsb1Ep6Mode:
        case eUsb1Ep7Mode:
        {
            USBEPSendPacket(eUsbDev1, (uint8_t)(eCmdSource-eUsb1Ep0Mode), pAnsCmdBuf, CMD_QUEUE_DATA_MAX_SIZE);
        }
        break;

        default:
            printf("AnswerCommand, source:%d,len:%d,send:%x,%x,%x\n",eCmdSource,nlength,pAnsCmdBuf[0],pAnsCmdBuf[1],pAnsCmdBuf[2]);
        break;
    }
}

void CmdService(void)
{
    CMD_QUEUE_BLOCK  stCmdBlock;
    CMD_QUEUE_BLOCK  stReCmdBlock;
    CmdAnswerType    eResult;

//    UsartProtocolDecoder();
    if (GetCmdQueueBlock(&stCmdBlock))
    {
        MemSet(&stReCmdBlock, 0, sizeof(CMD_QUEUE_BLOCK));
        stReCmdBlock.eCmdSource = stCmdBlock.eCmdSource;
        switch (GetCmdType(stCmdBlock.DataPacket, stCmdBlock.nDataLen))
        {
            case eCmdTypeCD:
            {
                eResult = Communication(&stCmdBlock, &stReCmdBlock);
            }
            break;
            default:
            {
                eResult = CustomerCommunication(&stCmdBlock, &stReCmdBlock);
            }
            break;
        }
        if (eResult == CMD_ANSWER_UNKNOWN)
        {
            stReCmdBlock.DataPacket[0] = CMD_FC_REPORT_ID;
            stReCmdBlock.DataPacket[1] = CMD_ANSWER;
            stReCmdBlock.DataPacket[2] = CMD_ANSWER_UNKNOWN;
            stReCmdBlock.DataPacket[3] = 2;
            stReCmdBlock.DataPacket[4] = stCmdBlock.DataPacket[4];
            stReCmdBlock.DataPacket[5] = stCmdBlock.DataPacket[1];
            stReCmdBlock.DataPacket[6] = stCmdBlock.DataPacket[2];
            stReCmdBlock.nDataLen = 7;
            AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
        }
        else if (eResult == CMD_ANSWER_FAIL)
        {
            stReCmdBlock.DataPacket[0] = CMD_FC_REPORT_ID;
            stReCmdBlock.DataPacket[1] = CMD_ANSWER;
            stReCmdBlock.DataPacket[2] = CMD_ANSWER_FAIL;
            stReCmdBlock.DataPacket[3] = 2;
            stReCmdBlock.DataPacket[4] = stCmdBlock.DataPacket[4];
            stReCmdBlock.DataPacket[5] = stCmdBlock.DataPacket[1];
            stReCmdBlock.DataPacket[6] = stCmdBlock.DataPacket[2];
            stReCmdBlock.nDataLen = 7;
            AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
        }
        else if (eResult == CMD_ANSWER_OK)
        {
            stReCmdBlock.DataPacket[0] = CMD_FC_REPORT_ID;
            stReCmdBlock.DataPacket[1] = CMD_ANSWER;
            stReCmdBlock.DataPacket[2] = CMD_ANSWER_OK;
            stReCmdBlock.DataPacket[3] = 2;
            stReCmdBlock.DataPacket[4] = stCmdBlock.DataPacket[4];
            stReCmdBlock.DataPacket[5] = stCmdBlock.DataPacket[1];
            stReCmdBlock.DataPacket[6] = stCmdBlock.DataPacket[2];
            stReCmdBlock.nDataLen = 7;
            AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
        }
        else if (eResult == CMD_ANSWER_DATA)
        {
            AnswerCommand(stReCmdBlock.DataPacket, stReCmdBlock.nDataLen, stReCmdBlock.eCmdSource);
        }
    }
}




