#include "CommService.h"
//#include "SystemDefine.h"
#include "Command.h"
#include <string.h>
#include <stdio.h>
#include "API_Math.h"
#include "Api_Flash.h"
#include "Api_Crc.h"
//#include "Api_Gpio.h"
#include "upgrade.h"
#include "GlobalVariables.h"
//#include "Api_UsbDevice.h"
#include "hpm_common.h"
#include "hpm_gpio_drv.h"


static volatile int32_t sg_bAdminAccess = 0;  //是否进入管理员模式

CmdAnswerType Communication(CMD_QUEUE_BLOCK* pCmdBlock, CMD_QUEUE_BLOCK* pReCmdBlock)
{

    CmdAnswerType eRetVal = CMD_ANSWER_DATA;

    static uint32_t snTotalPacketNum = 0;
    static uint32_t sn_CurPacketNum = 0;
    static uint8_t s_nCmdDownAck = CMD_DOWNLOAD;

    uint32_t nCurPacketNum = 0;
    uint32_t nDataCrc32  = 0;
    uint32_t i = 0;

    pReCmdBlock->DataPacket[0] = CMD_FD_REPORT_ID;
    switch(pCmdBlock->DataPacket[1])
    {
        case CMD_VERSION:
        {
            nDataCrc32 = GetBootDataCrc32();
            pReCmdBlock->DataPacket[1] = CMD_VERSION_ACK;
            pReCmdBlock->DataPacket[2] = 0x00;
            pReCmdBlock->DataPacket[3] = 0x08;
            pReCmdBlock->DataPacket[4] = CMD_OK_ACK;

            pReCmdBlock->DataPacket[5] = g_stBootPara.nBoardType;
            pReCmdBlock->DataPacket[6] = FN_LOBYTE(g_stBootPara.nBootVersion);
            pReCmdBlock->DataPacket[7] = FN_HIBYTE(g_stBootPara.nBootVersion);
            memcpy(&(pReCmdBlock->DataPacket[8]), &nDataCrc32, sizeof(nDataCrc32));
            pReCmdBlock->nDataLen = 12;

        }
        break;

        case CMD_VERSION_PAUSE:
        {
            nDataCrc32 = GetBootDataCrc32();

            pReCmdBlock->DataPacket[1] = CMD_VERSION_PAUSE_ACK;
            pReCmdBlock->DataPacket[2] = 0x00;
            pReCmdBlock->DataPacket[3] = 0x08;
            pReCmdBlock->DataPacket[4] = CMD_OK_ACK;

            pReCmdBlock->DataPacket[5] = g_stBootPara.nBoardType;
            pReCmdBlock->DataPacket[6] = FN_LOBYTE(g_stBootPara.nBootVersion);
            pReCmdBlock->DataPacket[7] = FN_HIBYTE(g_stBootPara.nBootVersion);
            memcpy(&(pReCmdBlock->DataPacket[8]), &nDataCrc32, sizeof(nDataCrc32));
            pReCmdBlock->nDataLen = 12;
            g_eAppUpgradeFlag = eAppIAPMode;

            s_nCmdDownAck = CMD_DOWNLOAD_ACK;
        }
        break;

        case CMD_IAP_ENABLE:
        {
            g_eAppUpgradeFlag = eAppIAPMode;
            nIAPEnableFlag = 0;
            pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
            nCurPacketNum = 0;
            nDataCrc32 = GetDataCrc32_SW(&pCmdBlock->DataPacket[8], 0x10,CRC32_DATA, g_stBootPara.nBoardType);

            if((uint8_t)g_stBootPara.nBoardType != pCmdBlock->DataPacket[7])
            {
                eRetVal = CMD_ANSWER_NONE;
                break;
            }

            sn_CurPacketNum = 0;
            if ((nDataCrc32 == *(uint32_t *)(&pCmdBlock->DataPacket[CMD_HEADER_LEN + 3 + 0x10])))
            {
                //when CRC Check passed, USB Transfered data are valid
                nFootPrintFlag = 1;

                snTotalPacketNum = (pCmdBlock->DataPacket[6] << 8) | pCmdBlock->DataPacket[5];

                pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
                pReCmdBlock->DataPacket[2] = 0x00;
                pReCmdBlock->DataPacket[3] = 0x02;
                pReCmdBlock->DataPacket[4] = CMD_OK_ACK;

                pReCmdBlock->DataPacket[5] = (nCurPacketNum) & 0xff;
                pReCmdBlock->DataPacket[6] = ((nCurPacketNum) >> 8) & 0xff;
                pReCmdBlock->nDataLen = 7;

                //擦除backup Flash
                if(status_success == EraseAppData())
                {
                    nIAPEnableFlag = 1;
                    printf("\r\nFlashErase Success\r\n");
                }
                else
                {
                    pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
                    pReCmdBlock->DataPacket[2] = 0x00;
                    pReCmdBlock->DataPacket[3] = 0x01;
                    pReCmdBlock->DataPacket[4] = CMD_ERROR_ACK;

                    pReCmdBlock->DataPacket[5] = NACK_FLASH_ERASE;
                    pReCmdBlock->nDataLen = 6;
                }
            }
            else
            {
                nFootPrintFlag = 0;

                pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
                pReCmdBlock->DataPacket[2] = 0x00;
                pReCmdBlock->DataPacket[3] = 0x01;
                pReCmdBlock->DataPacket[4] = CMD_ERROR_ACK;
                pReCmdBlock->DataPacket[5] = NACK_FLASH_ERASE;
                pReCmdBlock->nDataLen = 6;
            }

        }
        break;

        case CMD_DOWNLOAD:
        {
            pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
            printf(".");
            if(nIAPEnableFlag)
            {
                nCurPacketNum = (pCmdBlock->DataPacket[63] << 8) | pCmdBlock->DataPacket[62];

                if ((nCurPacketNum <= snTotalPacketNum) && ((sn_CurPacketNum == (nCurPacketNum -1))||(nCurPacketNum == sn_CurPacketNum)))
                {
                    nDataCrc32 = GetDataCrc32_SW(&(pCmdBlock->DataPacket[CMD_HEADER_LEN]), DATA_LEN,CRC32_DATA,g_stBootPara.nBoardType);

                    for (i = CMD_HEADER_LEN; i < DATA_LEN+CMD_HEADER_LEN; i++)
                    {
                        pReCmdBlock->DataPacket[i] = ~pCmdBlock->DataPacket[i];
                    }

                    if ((nDataCrc32 & 0xffff) == (pCmdBlock->DataPacket[61]|(pCmdBlock->DataPacket[4] << 8)))
                    {
                        //升级过程指示灯闪烁
                        if (nCurPacketNum % 20 == 0)
                        {
                            #if (1 == HPM_5300EVK_BOARD)
                            board_led_toggle();
                            #else
                            gpio_toggle_pin(HPM_GPIO0, GPIO_DI_GPIOA, 12);
                            #endif
                        }
                        
                        sn_CurPacketNum = nCurPacketNum;

                        if (status_success !=
                                SaveAppFLash(&pReCmdBlock->DataPacket[CMD_HEADER_LEN], DATA_LEN, nCurPacketNum * DATA_LEN))
                        {
                            pReCmdBlock->DataPacket[4] = CMD_ERROR_ACK;
                            printf("SaveAppFLash error!\n");
                        }
                        if (nCurPacketNum == snTotalPacketNum)
                        {
                        	printf("Upgrade data finish!\r\n");
                            pReCmdBlock->DataPacket[1] = CMD_FINISH;
                            pReCmdBlock->DataPacket[2] = 0x00;
                            pReCmdBlock->DataPacket[3] = 0x00;
                            pReCmdBlock->DataPacket[4] = CMD_OK_ACK;
                            pReCmdBlock->nDataLen = 5;
                            
                            //USBDisableEp0(&(g_stUsbFsDriver.regs));
                            nIAPEnableFlag = 0;
                            //SaveAppDataLen((snTotalPacketNum + 1) * DATA_LEN);
                            //SaveAppCheckSum();
                            //SetUpgradeFlag(eAppTimeoutRunMode);
                            g_eAppUpgradeFlag = eAppRunMode;
                        }
                        else
                        {
                            pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
                            pReCmdBlock->DataPacket[2] = 0x00;
                            pReCmdBlock->DataPacket[3] = 0x02;
                            pReCmdBlock->DataPacket[4] = CMD_OK_ACK;
                            
                            pReCmdBlock->DataPacket[5] = (nCurPacketNum + 1) & 0xff;
                            pReCmdBlock->DataPacket[6] = ((nCurPacketNum + 1) >> 8) & 0xff;
                            
                            pReCmdBlock->nDataLen = 7;
                        }
                    }
                    else
                    {
                        pReCmdBlock->DataPacket[1] = s_nCmdDownAck;
                        pReCmdBlock->DataPacket[2] = 0x00;
                        pReCmdBlock->DataPacket[3] = 0x09;
                        pReCmdBlock->DataPacket[4] = CMD_ERROR_ACK;

                        pReCmdBlock->DataPacket[5] = NACK_CHECKSUM;
                        memcpy(&(pReCmdBlock->DataPacket[6]), &nDataCrc32, sizeof(uint32_t));
                        nDataCrc32 = (pCmdBlock->DataPacket[61]|(pCmdBlock->DataPacket[4] << 8)) | pCmdBlock->DataPacket[5];
                        memcpy(&(pReCmdBlock->DataPacket[10]), &nDataCrc32, sizeof(uint32_t));
                        pReCmdBlock->nDataLen = 14;
                        printf("iap data checksum error[%04X %02X %02X]...\r\n", nDataCrc32, pCmdBlock->DataPacket[4], pCmdBlock->DataPacket[61]);
                    }
                }
                else
                {
                    //包号错误
                    pReCmdBlock->DataPacket[2] = 0x00;
                    pReCmdBlock->DataPacket[3] = 9;
                    pReCmdBlock->DataPacket[4] = CMD_ERROR_ACK;

                    pReCmdBlock->DataPacket[5] = NACK_PACKET_NO;
                    memcpy(&(pReCmdBlock->DataPacket[6]), &nCurPacketNum, sizeof(uint32_t));
                    memcpy(&(pReCmdBlock->DataPacket[10]), &snTotalPacketNum, sizeof(uint32_t));
                    pReCmdBlock->nDataLen = 14;
                    printf("iap data pack no error[%d %d %d]...\r\n", nCurPacketNum, sn_CurPacketNum, snTotalPacketNum);
                }
            }
            else
            {
                pReCmdBlock->DataPacket[2] = 0x00;
                pReCmdBlock->DataPacket[3] = 0x01;
                pReCmdBlock->DataPacket[4] = CMD_ERROR_ACK;
                pReCmdBlock->DataPacket[5] = NACK_IAPMODE;
                pReCmdBlock->nDataLen = 6;
                printf("not in IAP mode...\r\n");
            }
        }
        break;

        case CMD_ReadFlash:
        {
            static uint32_t  nReadAddress = 0;
            static uint32_t  nReadLen = 0;
            uint32_t  i = 0,j;
            uint8_t* pReadAddress = (uint8_t*)APPLICATION_ADDRESS;

            eRetVal = CMD_ANSWER_FAIL;
            if (sg_bAdminAccess) //管理员模式下才允许读取flash
            {
                nReadAddress = (((uint32_t)pCmdBlock->DataPacket[5] << 24) + ((uint32_t)pCmdBlock->DataPacket[6] << 16) +((uint32_t)pCmdBlock->DataPacket[7] << 8) | (uint32_t)pCmdBlock->DataPacket[8]);
                nReadLen     = ((uint32_t)(pCmdBlock->DataPacket[9] << 8) + (uint32_t)pCmdBlock->DataPacket[10]);
                pReadAddress = (uint8_t*)nReadAddress;

                //printf("nReadAddress:0x%x,nReadLen:%d\r\n",nReadAddress,nReadLen);
                for(i = 0;i < nReadLen;i += 64)
                {
                    for(j = 0;j < 64;j += 1)
                    {
                        pReCmdBlock->DataPacket[j] = pReadAddress[j];
                    }
                    nReadAddress += 64;
                    AnswerCommand(pReCmdBlock->DataPacket,64,pCmdBlock->eCmdSource);
                }
                eRetVal = CMD_ANSWER_NONE;
            }
        }
        break;

        case CMD_READFLASH_ALL:
        {
            eRetVal = CMD_ANSWER_FAIL;
            if (sg_bAdminAccess) //管理员模式下才允许读取flash
            {
                uint32_t  nReadAddress = FLASH_START_ADDRESS;
                uint8_t *pReadAddress = (uint8_t*)nReadAddress;
                uint32_t  nReadLen = FLASH_CHIP_SIZE;
                uint32_t  i = 0, j = 0;

                for(i = 0;i < nReadLen;i += 64)
                {
                    pReadAddress = (uint8_t*)nReadAddress;
                    for(j = 0;j < 64;j += 1)
                    {
                        pReCmdBlock->DataPacket[j] = pReadAddress[j];
                    }
                    nReadAddress += 64;
                    AnswerCommand(pReCmdBlock->DataPacket, 64, pCmdBlock->eCmdSource);
                }
                eRetVal = CMD_ANSWER_NONE;
            }
        }
        break;

        case CMD_ADMIN_ACCESS:
        {
            char *pAdminKey = "ZYVERYNB!";

            eRetVal = CMD_ANSWER_FAIL;
            sg_bAdminAccess = 0;
            if (memcmp(&pCmdBlock->DataPacket[5], pAdminKey, strlen(pAdminKey)) == 0)
            {
                sg_bAdminAccess = 1;
                eRetVal = CMD_ANSWER_OK;
            }
        }
        break;

        case CMD_RUN:
        {
            g_eAppUpgradeFlag = eAppRunMode;

            pReCmdBlock->DataPacket[1] = CMD_RUN_ACK;
            pReCmdBlock->DataPacket[2] = 0x00;
            pReCmdBlock->DataPacket[3] = 0x00;
            pReCmdBlock->DataPacket[4] = CMD_OK_ACK;
            pReCmdBlock->nDataLen = 5;

        }
        break;

        case CMD_ACK:    // ACK和NACK都采用不应答的处理
        case CMD_NACK:
        {
            eRetVal = CMD_ANSWER_NONE;
            pReCmdBlock->nDataLen = 0;
        }
        break;

        default:
        {
            pReCmdBlock->DataPacket[1] = CMD_NACK;
            pReCmdBlock->DataPacket[2] = NACK_ERROR_CMD;
            pReCmdBlock->nDataLen = 3;
        }
        break;
    }
    
    return eRetVal;;
}
