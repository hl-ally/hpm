/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include <stddef.h>
#include "board.h"
#include "user_config.h"
#include "hpm_debug_console.h"
#include "hpm_gpio_drv.h"
#include "hpm_usb_drv.h"
#include "hpm_l1c_drv.h"
#include "app_systick.h"
#include "Api_Flash.h"
#include "Api_UsbDesc.h"
#include "Api_UsbDevice.h"
#include "CommService.h"
#include "GlobalDefaultDefine.h"
#include "GlobalVariables.h"
#include "FlashPara.h"

void HwPlatformInit(void)
{
    board_init();
    FlashInit();
    UsbDevCallbackRegister(CMD_FC_REPORT_ID, AddCmdQueueBlock); //注册USB相关回调函数
    UsbDevCallbackRegister(CMD_FE_REPORT_ID, AddCmdQueueBlock); //注册USB相关回调函数

    // IO口控制LED
    #if HPM_5300EVK_BOARD
    board_init_led_pins();
    #else
    HPM_IOC->PAD[IOC_PAD_PA12].FUNC_CTL = IOC_PA12_FUNC_CTL_GPIO_A_12;
    gpio_set_pin_output_with_initial(HPM_GPIO0, GPIO_DI_GPIOA, 12, 0);
    #endif
    
    printf("use cherry usb lib.\n");
    board_init_usb_pins();
    /* As QFN32, QFN48 and LQFP64 has no vbus pin, so should be call usb_phy_using_internal_vbus() API to use internal vbus. */
    /* usb_phy_using_internal_vbus(BOARD_USB); */
    usb_phy_using_internal_vbus(BOARD_USB);

    InitFlashPara();
}

int main(void)
{
    uint64_t nLastTime = 0;
    uint64_t nLedToggleTime = 0;

    HwPlatformInit();

    if(1)
    {
        stUsbEnumInfo_t stUsbTouchDevice = {.nUSBVid = USB0_VID,
                            .nUSBPid = USB0_PID,
                            .nVersion = Firmware_Version,
                            .eUsbCfgType = USB0_DEVICE_CONFIG_TYPE};
        l1c_dc_disable();
        StartUsbDev(stUsbTouchDevice); //USB开始枚举
        l1c_dc_enable();
    }

    
#if 0
    // 开机延时等待
    nLastTime = GetCurrentTimeUs();
    while(GetCurrentTimeUs() - nLastTime < 1*1000*1000)
    {
        CmdService();
    }
#endif
    nLastTime = GetCurrentTimeUs();
    nLedToggleTime = GetCurrentTimeUs();
    while(1)
    {
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

        if(GetCurrentTimeUs() - nLastTime >= 5*1000*1000)
        {
            nLastTime = GetCurrentTimeUs();
            printf("heartbeat, %llu\n", nLastTime);
        }
    }
    return 0;
}
