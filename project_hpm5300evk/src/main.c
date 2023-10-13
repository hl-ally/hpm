/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "board.h"
#include "user_config.h"
#include "hpm_debug_console.h"
#include "pwm_rgb_led.h"
#include "pwm_beep.h"
#include "otp_func.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "hpm_pllctlv2_drv.h"
#include "nor_flash.h"
#include "hpm_romapi.h"
#include "hpm_ppor_drv.h"
#include "systick.h"
#include "app_dac.h"


int main(void)
{
    int u = 0;
    uint64_t nLastTime = 0;
    board_init();
    // 修改主频为480MHz
    if(status_success != pllctlv2_init_pll_with_freq(HPM_PLLCTLV2, 0, 960000000))
    {
        printf("change pll freq fail\n");
    }
    else
    {
        printf("change pll freq success\n");
        printf("==============================\n");
        printf(" %s clock summary\n", BOARD_NAME);
        printf("==============================\n");
        printf("cpu0:\t\t %luHz\n", clock_get_frequency(clock_cpu0));
        printf("ahb:\t\t %luHz\n", clock_get_frequency(clock_ahb));
        printf("mchtmr0:\t %luHz\n", clock_get_frequency(clock_mchtmr0));
        printf("xpi0:\t\t %luHz\n", clock_get_frequency(clock_xpi0));
        printf("==============================\n");
    }

#if 1
    // 开机延时等待
    nLastTime = GetCurrentTimeUs();
    while(GetCurrentTimeUs() - nLastTime < 3*1000*1000)
    {
        ;
    }
#endif

#if LED_IO_TEST
    // IO口控制LED
    board_init_led_pins();

    board_timer_create(LED_FLASH_PERIOD_IN_MS, board_led_toggle);
#endif

#if FGPIO_TOGGLE_TEST
    board_init_fgpio_pin();
#endif

#if RGB_LED_PWM_TEST
    pwm_rgb_led_init();
#endif

#if PWM_BEEP_TEST
    pwm_beep_init();
#endif

#if CHERRYUSB_DEVICE_TEST
    printf("cherry usb device test.\n");
    board_init_usb_pins();
    #if USBD_BOOT_TEST
    uint64_t nUsbBootTime = GetCurrentTimeUs();
    boot_hid_init();
    #endif
    #if USBD_APP_TEST
    app_hid_init();
    #endif
#endif

#if OTP_TEST
    OtpValuePrint();
    ShowUuid();
    ShowUid();
    ShowXpiFlashInfo();
#endif

#if NOR_FLASH_TEST
    #if defined(RUN_IN_RAM) && RUN_IN_RAM
    printf("run in ram\n");
    norflash_init();
    #endif
#endif

#if SOFTWARE_RESET_TEST
    uint64_t nSortwareResetTime = GetCurrentTimeUs();
#endif

#if WDOG_TEST
    WatchDogInit();
#endif

#if DAC_TEST
    dac_test_init();
#endif

#if defined(__GNUC__)
    printf("gcc version %d\n", __GNUC__);
#endif
    printf("hello world\n");
    printf("now into while loop, enjoy......\n");
    nLastTime = GetCurrentTimeUs();
    while(1)
    {
    #if 0
        u = getchar();
        if (u == '\r') {
            u = '\n';
        }
        printf("%c", u);
    #endif

    #if NOR_FLASH_TEST
        #if defined(RUN_IN_RAM) && RUN_IN_RAM
        static uint8_t norflash_count = 1;
        if(norflash_count > 0)
        {
            norflash_count--;
            norflash_test();
        }
        #endif
    #endif

    #if FGPIO_TOGGLE_TEST
        // 测试IO翻转速率
        //clock_cpu_delay_us(1);
        //board_delay_ms(300);
        board_fgpio_toggle_process();
    #endif

    #if RGB_LED_PWM_TEST
        pwm_rgb_led_process();
    #endif

    #if PWM_BEEP_TEST
        board_delay_ms(10);
        pwm_beep_process();
    #endif

    #if CHERRYUSB_DEVICE_TEST
        #if USBD_BOOT_TEST
        if(GetCurrentTimeUs()- nUsbBootTime >= 1*1000*1000)
        {
            nUsbBootTime = GetCurrentTimeUs();
            boot_hid_test();
        }
        #endif
        
        #if USBD_APP_TEST
        app_hid_test();
//        board_delay_ms(1000);
        #endif
    #endif

    #if 0
        // 运行rom api, run_bootloader测试
        if (GetCurrentTimeUs() - nLastTime > 20*1000*1000)
        {
            api_boot_arg_t boot_arg = { .index = 0, 
                                        .peripheral = API_BOOT_PERIPH_AUTO,
                                        .src = API_BOOT_SRC_ISP,
                                        .tag = API_BOOT_TAG};
            ROM_API_TABLE_ROOT->run_bootloader(&boot_arg);
        }
    #endif

    #if SOFTWARE_RESET_TEST
        // 复位操作
        if (GetCurrentTimeUs() - nSortwareResetTime >= 20*1000*1000)
        {
            uint32_t resetTime = 5*24*1000*1000; 
            printf("softerware reset after %ds\r\n", resetTime/(24*1000*1000));
            ppor_sw_reset(HPM_PPOR, resetTime);
            while(1);
        }
    #endif

    #if WDOG_TEST
        FeedWatchDog();
        #endif

    #if DAC_TEST
        dac_test_process();
    #endif

        if(GetCurrentTimeUs() - nLastTime >= 5*1000*1000)
        {
            nLastTime = GetCurrentTimeUs();
            printf("heartbeat, %llu\n", nLastTime);
        }
    }
    return 0;
}
