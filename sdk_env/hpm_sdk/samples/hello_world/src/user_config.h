#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#define FGPIO_TOGGLE_TEST           0       // 高速GPIO翻转测试
#if FGPIO_TOGGLE_TEST
#define ZLG_FAE_FGPIO_TEST          1       // ZLG 陈工验证GPIO翻转速率时的代码
#endif

#define LED_IO_TEST                 1       // IO口控制LED测试
#define RGB_LED_PWM_TEST            0       // PWM控制RGB LED测试
#define PWM_BEEP_TEST               0       // PWM控制蜂鸣器测试

#define CHERRYUSB_DEVICE_TEST       1       // cherryusb device测试程序
#if CHERRYUSB_DEVICE_TEST
#define USBD_BOOT_TEST              1
#define USBD_APP_TEST               0
#else
#define USBD_BOOT_TEST              0
#define USBD_APP_TEST               0
#endif


#define LED_FLASH_PERIOD_IN_MS      300




#if USBD_BOOT_TEST
extern void boot_hid_init(void);
extern void boot_hid_test(void);
#endif
#if USBD_APP_TEST
extern void app_hid_init(void);
extern void app_hid_test(void);
#endif


#endif
