/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "board.h"
#include "hpm_debug_console.h"
#include "pwm_rgb_led.h"
#include "pwm_beep.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"


#define FGPIO_TOGGLE_TEST           0       // 高速GPIO翻转测试
#if FGPIO_TOGGLE_TEST
#define ZLG_FAE_FGPIO_TEST          1       // ZLG 陈工验证GPIO翻转速率时的代码
#endif

#define LED_IO_TEST                 1       // IO口控制LED测试
#define RGB_LED_PWM_TEST            0       // PWM控制RGB LED测试
#define PWM_BEEP_TEST               0       // PWM控制蜂鸣器测试
#define CHERRYUSB_DEVICE_TEST       1       // cherryusb device测试程序


#define LED_FLASH_PERIOD_IN_MS      300

extern void hid_custom_init(void);

int main(void)
{
    int u = 0;
    board_init();

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

    hid_custom_init();
   #if 0
    while (u < 2) 
    {
        hid_mouse_test();
        if (dtr_enable) {
            u++;
            cdc_acm_data_send_with_dtr_test();
        }
    }
    #endif
#endif

#if defined(__GNUC__)
    printf("gcc version %d\n", __GNUC__);
#endif
    printf("hello world\n");
    while(1)
    {
#if 0
        u = getchar();
        if (u == '\r') {
            u = '\n';
        }
        printf("%c", u);
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
        ;
#endif
    }
    return 0;
}
