#include <stdio.h>
#include "Api_Math.h"
#include "Api_Flash.h"
//#include "Api_UsbDevice.h"
#include "upgrade.h"
#include "GlobalVariables.h"
//#include "SystemDefine.h"
#include "app_systick.h"
//#include "Api_Gpio.h"
#include "hpm_common.h"


typedef void (*pFunction)(void);
volatile uint32_t nFootPrintFlag = 0;
volatile uint32_t nIAPEnableFlag = 0;
//extern DEVICE_PROP * pProperty;
volatile uint8_t nIsInIap = 0;

volatile eAppUpgradeFlag_t g_eAppUpgradeFlag = eAppIAPMode;
static volatile uint32_t sg_nBackupAppFlashOffsetErase = 0; //当前擦除偏移

stStartBootPara_t g_stBootPara = {.nVid = 0x1FF7,
                                  .nPid = 0x0F0A,
                                  .nBootVersion = Firmware_Version,
                                  .nBoardType = PRODUCT_BOARD_TYPE,
                                  .nDefultWaitTime = 2000};

uint32_t CheckFroApplication(void)
{
    uint32_t flag = 0;
    uint32_t ret = 0;
    uint32_t nFwChecksum = GetFwCheckSum();
    stStartBootPara_t stPara;
    GetBootPara(&stPara);

    flag = ((*(volatile uint32_t*)APPLICATION_ADDRESS) & 0x2FF00000);
    if ((flag == 0x20000000) &&
            (((*(volatile uint32_t*)(APPLICATION_ADDRESS+4)) & 0xFF000000) == 0x08000000))
    {
        ret = 1;
    }

    printf("App checksum save %X, get %X\r\n", stPara.stBootFlashPara.nFirewareCrc, nFwChecksum);
    if((nFwChecksum == 0 || stPara.stBootFlashPara.nFirewareCrc ==0) || 
        (nFwChecksum != stPara.stBootFlashPara.nFirewareCrc))
    {
        ret = 0;
    }

    if(0 == ret)
    {
        printf("\r\nCheckFroApplication:\r\n ");
        uint32_t* pAppData = (uint32_t*)APPLICATION_ADDRESS;
        for(int i=0; i<32; i++)
            printf("%08X,",pAppData[i]);
        printf("End...\r\n");
    	
    }
    return ret;
}

uint32_t CheckForBackupApplication(void)
{
    uint32_t ret = 1;
    //stStartBootPara_t stPara;
    //stStartBootPara_t stBackupPara;
    //uint32_t nBackupFwChecksum = GetBackupFwCheckSum();
    //int32_t nInfoLen = FN_MIN(sizeof(stStartBootPara_t), INFO_LEN);
    //FLASH_ReadLenByte(FLASH_BACKUP_ADDRESS + INFO_OFFSET, (uint8_t *)(&stBackupPara), nInfoLen);
    
    //GetBootPara(&stPara);
    
    //if((nBackupFwChecksum != stBackupPara.stBootFlashPara.nFirewareCrc)||
    //    (nBackupFwChecksum == 0 || stBackupPara.stBootFlashPara.nFirewareCrc == 0)||
    //    (stPara.stBootFlashPara.nFirewareCrc != stBackupPara.stBootFlashPara.nFirewareCrc))
    //{
    //    printf("CheckForBackupApplication, main app crc %X, backup app crc save %X, get %X\n", 
    //                        stPara.stBootFlashPara.nFirewareCrc, 
    //                        stBackupPara.stBootFlashPara.nFirewareCrc, 
    //                        nBackupFwChecksum);
    //    ret = 0;
    //}

    return ret;
}


uint32_t IsBackupAppNoData(void)
{
    uint32_t ret = 0;
    //stStartBootPara_t stPara;
    //stStartBootPara_t stBackupPara;
    //uint32_t nBackupFwChecksum = GetBackupFwCheckSum();
    //int32_t nInfoLen = FN_MIN(sizeof(stStartBootPara_t), INFO_LEN);
    //FLASH_ReadLenByte(FLASH_BACKUP_ADDRESS + INFO_OFFSET, (uint8_t *)(&stBackupPara), nInfoLen);
    
    //GetBootPara(&stPara);
    
    //if((nBackupFwChecksum != stBackupPara.stBootFlashPara.nFirewareCrc)&&
    //    (stPara.stBootFlashPara.nFirewareCrc != stBackupPara.stBootFlashPara.nFirewareCrc)&&
    //    (stBackupPara.stBootFlashPara.nFirewareCrc == 0xFFFFFFFF || stBackupPara.stBootFlashPara.nFirewareLen == 0xFFFFFFFF))
    //{
    //    printf("backup app no data, [%X %X]\n", stBackupPara.stBootFlashPara.nFirewareCrc, stBackupPara.stBootFlashPara.nFirewareLen);
    //    ret = 1;
    //}

    return ret;
}


/*
static void BeforeRunApplication(void)
{
    stStartBootPara_t stPara;
    stStartBootPara_t stBackupPara;
    uint32_t nBackupFwChecksum = GetBackupFwCheckSum();
    int32_t nInfoLen = FN_MIN(sizeof(stStartBootPara_t), INFO_LEN);
    FLASH_ReadLenByte(FLASH_BACKUP_ADDRESS + INFO_OFFSET, (uint8_t *)(&stBackupPara), nInfoLen);
    
    GetBootPara(&stPara);
    
    if((nBackupFwChecksum != stBackupPara.stBootFlashPara.nFirewareCrc)||
        (nBackupFwChecksum == 0 || stBackupPara.stBootFlashPara.nFirewareCrc == 0)||
        (stPara.stBootFlashPara.nFirewareCrc != stBackupPara.stBootFlashPara.nFirewareCrc))
    {
        printf("Main app crc %X, backup app crc save %X, get %X\n", 
                            stPara.stBootFlashPara.nFirewareCrc, 
                            stBackupPara.stBootFlashPara.nFirewareCrc, 
                            nBackupFwChecksum);
        SaveBackupAppFLash();

    }
}
*/

void RunApplication(void)
{
    //uint32_t  JumpAddress = *(__IO uint32_t*)(APPLICATION_ADDRESS + 4);
    //pFunction Jump = (pFunction)JumpAddress;

    //// 只有在升级状态，或者备份区无数据时备份一次
    //if(nIsInIap || IsBackupAppNoData())
    //{
    //    uint8_t nRetry = 3;
    //    while((CheckForBackupApplication() == 0) && (nRetry > 0))
    //    {
    //        printf("after IAP sava backup flash\n");
    //        SaveBackupAppFLash();
    //        nRetry--;
    //    }
    //}
    
    
    //if (GetUsbFsAppInit())
    //{
    //    uint32_t nTick = GetSystickTime();
    //    while (GetUsbQueueBusy(USB_DEVICES0) && (GetSystickTime() - nTick) < 2000);
    //    StopUsbFsDev();
    //    Delay_ms(10);
    //    ResetGpioBits(USBFS_PORT, USBFS_DP_PIN);
    //    ResetGpioBits(USBFS_PORT, USBFS_DM_PIN);
    //    Delay_ms(10);       
    //}

    //if (GetUsbHsAppInit())
    //{
    //    uint32_t nTickHs = GetSystickTime();
    //    while (GetUsbQueueBusy(USB_DEVICES1) && (GetSystickTime() - nTickHs) < 2000);
    //    StopUsbHsDev();
    //    Delay_ms(10);
    //    ResetGpioBits(USBHS_PORT, USBHS_DP_PIN);
    //    ResetGpioBits(USBHS_PORT, USBHS_DM_PIN);
    //    Delay_ms(10);        
    //}
    
    //usart_disable(USART0);

    //__disable_irq();
   
    //__set_MSP(*(__IO uint32_t*)APPLICATION_ADDRESS);
    //Jump();
    //while(1);
}

uint32_t GetBackupAppFLashAddress(void)
{
    //return FLASH_BACKUP_ADDRESS;
    return 0;
}

int ClearBackupAppFLash(void)
{
    sg_nBackupAppFlashOffsetErase = 0;
    return 0;
}


/*
  * 写入数据 - 为了加速写入，只能顺序地写入，不能乱序写入
 */
hpm_stat_t SaveAppFLash(uint8_t* pBuf, int32_t nLen, int32_t nOffset)
{
    if (nLen <= 0)
    {
        return status_invalid_argument;
    }
    return FlashWrite(APPLICATION_ADDRESS+nOffset, pBuf, FN_MAX(0, (int32_t)FN_MIN((int32_t)(APPLICATION_MAX_LEN-nOffset), nLen)));
}


hpm_stat_t EraseAppData(void)
{
    return FlashErase(APPLICATION_ADDRESS, APPLICATION_MAX_LEN);
}


int EraseBackupAppData(void)
{
	//return FLASH_EraseLenByte(FLASH_BACKUP_ADDRESS, FLASH_BACKUP_SIZE);
    return 0 ;
}


/*
  * 写入数据 - 为了加速写入，只能顺序地写入，不能乱序写入
 */
int SaveBackupAppFLash(void)
{
    uint8_t flashData[1024];
    uint16_t nCopyLen = 1024;
    uint32_t nOffset = 0;
    //uint32_t nMaxBackupLen = FN_MIN(APPLICATION_MAX_LEN, FLASH_BACKUP_SIZE);
    uint32_t nMaxBackupLen = 0x3000;

    printf("SaveBackupAppFLash\n");
    
    if(EraseBackupAppData() != 1)
    {
        printf("Erase backup flash fail\n");
    }

    while(nOffset < nMaxBackupLen)
    {
        //FLASH_ReadLenByte(APPLICATION_ADDRESS+nOffset, flashData, nCopyLen);
        //if(FLASH_WriteLenByte(FLASH_BACKUP_ADDRESS+nOffset, flashData, FN_MAX(0, (int32_t)FN_MIN((int32_t)(nMaxBackupLen-nOffset), nCopyLen))) != FMC_READY)
        //{
        //    printf("%s, write app data fail. [%X]\n", __func__, nOffset);
        //}
        //nOffset += nCopyLen;
    }

    printf("SaveBackupAppFLash success\n");
    
    return 1;
}



int8_t CopyBackupApp2MainApp(void)
{
    //uint8_t flashData[1024];
    //uint16_t nCopyLen = 1024;
    //uint32_t nOffset = 0;
    //uint32_t nBackupFwChecksum = GetBackupFwCheckSum();
    //uint32_t nMaxBackupLen = FN_MIN(APPLICATION_MAX_LEN, FLASH_BACKUP_SIZE);
    //stStartBootPara_t stPara;
    
    //stStartBootPara_t stBackupPara;
    //int32_t nInfoLen = FN_MIN(sizeof(stStartBootPara_t), INFO_LEN);
    //FLASH_ReadLenByte(FLASH_BACKUP_ADDRESS + INFO_OFFSET, (uint8_t *)(&stBackupPara), nInfoLen);
    
    //GetBootPara(&stPara);

    //printf("Backup App checksum save %X, get %X\r\n", stBackupPara.stBootFlashPara.nFirewareCrc, nBackupFwChecksum);
    //if((nBackupFwChecksum == 0 || stBackupPara.stBootFlashPara.nFirewareCrc == 0) || 
    //    (nBackupFwChecksum != stBackupPara.stBootFlashPara.nFirewareCrc))
    //{
        
    //    return 0;
    //}

    //printf("CopyBackupApp2MainApp\n");

    //if(EraseAppData() != FMC_READY)
    //{
    //    printf("%s, erase app data fail\n", __func__);
    //}

    //while(nOffset < nMaxBackupLen)
    //{
    //    FLASH_ReadLenByte(FLASH_BACKUP_ADDRESS+nOffset, flashData, nCopyLen);
    //    FLASH_WriteLenByte(APPLICATION_ADDRESS+nOffset, flashData, FN_MAX(0, (int32_t)FN_MIN((int32_t)(nMaxBackupLen-nOffset), nCopyLen)));
    //    nOffset += nCopyLen;
    //}
    
    return 1;

}



