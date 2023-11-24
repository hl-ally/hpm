#include <stdio.h>
#include "board.h"
#include "user_config.h"
#include "hpm_gpio_drv.h"
#include "hpm_usb_drv.h"
#include "app_systick.h"
#include "Api_Flash.h"
#include "Upgrade.h"
#include "Api_Crc.h"
#include "Api_UsbDesc.h"
#include "Api_UsbDevice.h"
#include "hpm_l1c_drv.h"
#include "GlobalDefaultDefine.h"
#include "CommService.h"

/*
 * 输出app启动信息
 */
void BuildInfoOutput(uint32_t nStep)
{
    switch (nStep)
    {
        case 0:
        {
            printf("\r\nBootloader loading...\r\n");
        }
        break;

        case 1:
        {
            printf("Bootloader version %x.%x%x%x\r\n"
               "BootCrc: 0x%04X\r\n"
               "BoardType: 0x%04X\r\n"
               "Boot flag: 0x%02x\r\n"
               "Default jumping: %dms\r\n"
               "Build date:%s-%s-%s\r\n",
               (g_stBootPara.nBootVersion & 0xF000) >> 3*4,
               (g_stBootPara.nBootVersion & 0x0F00) >> 2*4,
               (g_stBootPara.nBootVersion & 0x00F0) >> 1*4,
               (g_stBootPara.nBootVersion & 0x000F) >> 0*4,
               GetCrc32_SW((uint8_t *)(BOOTLOADER_ADDRESS), BOOTLOADER_MAX_SIZE),
               g_stBootPara.nBoardType,
               g_eAppUpgradeFlag,
               g_stBootPara.nDefultWaitTime,
               "2023","11","23");
        }
        break;

        default:
        break;
    }
}

void HwPlatformInit(void)
{
    board_init();
    FlashInit();
    UsbDevCallbackRegister(CMD_FE_REPORT_ID, AddCmdQueueBlock); //注册USB相关回调函数

    // IO口控制LED
    #if HPM_5300EVK_BOARD
    board_init_led_pins();
    #else
    HPM_IOC->PAD[IOC_PAD_PA12].FUNC_CTL = IOC_PA12_FUNC_CTL_GPIO_A_12;
    gpio_set_pin_output_with_initial(HPM_GPIO0, GPIO_DI_GPIOA, 12, 0);
    #endif
    BuildInfoOutput(0);
    
    printf("use cherry usb lib.\n");
    board_init_usb_pins();
    /* As QFN32, QFN48 and LQFP64 has no vbus pin, so should be call usb_phy_using_internal_vbus() API to use internal vbus. */
    /* usb_phy_using_internal_vbus(BOARD_USB); */
    usb_phy_using_internal_vbus(BOARD_USB);
}

int main(void)
{
    uint64_t nLastTime = 0;
    uint64_t nUpgradeTime = 0;
    uint64_t nLedToggleTime = 0;

    g_eAppUpgradeFlag = GetUpgradeFlag();
    HwPlatformInit();
    BootParaInit(&g_stBootPara);
    if(1)
    {
        stUsbEnumInfo_t stUsbTouchDevice = {.nUSBVid = g_stBootPara.nVid,
                            .nUSBPid = g_stBootPara.nPid,
                            .nVersion = g_stBootPara.nBootVersion,
                            .eUsbCfgType = eUsbCfgBootDefault};
        l1c_dc_disable();
        StartUsbDev(stUsbTouchDevice); //USB开始枚举
        l1c_dc_enable();
    }

    BuildInfoOutput(1);
    
#if 1
    // 开机延时等待
    nLastTime = GetCurrentTimeUs();
    while(GetCurrentTimeUs() - nLastTime < 1*1000*1000)
    {
        CmdService();
    }
#endif
    nLastTime = GetCurrentTimeUs();
    nUpgradeTime = GetCurrentTime();
    nLedToggleTime = GetCurrentTimeUs();
    while(1)
    {
        switch(g_eAppUpgradeFlag)
        {
        case eAppIAPMode:
            break;
        case eAppRunMode:
            if(GetCurrentTimeUs() - nLastTime >= 5*1000*1000)
            {
                nLastTime = GetCurrentTimeUs();
                printf("Goto Application...\r\n\r\n");
                if (CheckFroApplication())
                {
                    RunApplication();
                }
                else
                {
                    g_eAppUpgradeFlag = eAppIAPMode;
                }
            }
            
            break;
        case eAppTimeoutRunMode:
        default:
            if((GetCurrentTime() - nUpgradeTime) > g_stBootPara.nDefultWaitTime)
            {
                nUpgradeTime = GetCurrentTime();
                printf("Falg:[%x] Timeout %dms...\n", g_eAppUpgradeFlag,g_stBootPara.nDefultWaitTime);
                g_eAppUpgradeFlag = eAppRunMode;
            }
            break;
        }

        CmdService();
        if(GetCurrentTimeUs()- nLedToggleTime >= LED_FLASH_PERIOD_IN_MS*1000)
        {
            nLedToggleTime = GetCurrentTimeUs();
            #if (1 == HPM_5300EVK_BOARD)
            board_led_toggle();
            #else
            gpio_toggle_pin(HPM_GPIO0, GPIO_DI_GPIOA, 12);
            #endif
        }

//        if(GetCurrentTimeUs() - nLastTime >= 5*1000*1000)
//        {
//            nLastTime = GetCurrentTimeUs();
////            printf("heartbeat, %llu\n", nLastTime);
//        }
    }
    return 0;
}
