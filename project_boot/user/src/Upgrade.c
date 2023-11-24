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
#include "board.h"
#include "hpm_ppor_drv.h"
#include "hpm_l1c_drv.h"


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

//    flag = ((*(volatile uint32_t*)APPLICATION_ADDRESS) & 0x2FF00000);
//    if ((flag == 0x20000000) &&
//            (((*(volatile uint32_t*)(APPLICATION_ADDRESS+4)) & 0xFF000000) == 0x08000000))
//    {
//        ret = 1;
//    }

    printf("App checksum save %X, get %X\r\n", stPara.stBootFlashPara.nFirewareCrc, nFwChecksum);
    if((nFwChecksum == 0 || stPara.stBootFlashPara.nFirewareCrc ==0) || 
        (nFwChecksum != stPara.stBootFlashPara.nFirewareCrc))
    {
        ret = 0;
    }
    else
    {
        ret = 1;
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


void RunApplication(void)
{
    printf("Jump to application @0x%x(0x%x)\r\n", (APPLICATION_ADDRESS), *(volatile uint32_t*)(APPLICATION_ADDRESS));
    l1c_dc_writeback_all();
    l1c_dc_disable();
    l1c_ic_disable();
    fencei();
    __asm("la a0, %0" ::"i"(APPLICATION_ADDRESS));
    __asm("jr a0");
    while (1);
    
    // 复位操作
//    uint32_t resetTime = 5*24*1000*1000; 
//    printf("softerware reset after %ds\r\n", resetTime/(24*1000*1000));
//    ppor_sw_reset(HPM_PPOR, resetTime);
//    while(1);
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


