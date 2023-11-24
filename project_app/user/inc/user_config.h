#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#define HPM_5300EVK_BOARD           0       // 当前是否在HPM 5300EVK开发板上执行程序

#define FGPIO_TOGGLE_TEST           0       // 高速GPIO翻转测试
#if FGPIO_TOGGLE_TEST
#define ZLG_FAE_FGPIO_TEST          1       // ZLG 陈工验证GPIO翻转速率时的代码
#endif

#define LED_IO_TEST                 1       // IO口控制LED测试
#define RGB_LED_PWM_TEST            0       // PWM控制RGB LED测试
#define PWM_BEEP_TEST               0       // PWM控制蜂鸣器测试

#define CHERRYUSB_DEVICE_TEST       0       // cherryusb device测试程序
#if CHERRYUSB_DEVICE_TEST
#define USBD_BOOT_TEST              1
#define USBD_APP_TEST               0
#else
#define USBD_BOOT_TEST              0
#define USBD_APP_TEST               0
#endif

#define OTP_TEST                    0
#define NOR_FLASH_TEST              0
#define WDOG_TEST                   0
#define SOFTWARE_RESET_TEST         0
#define DAC_TEST                    0
#define PWM_TEST                    0
#define ADC_TEST                    0
#define TIM_TEST                    0
#define TSNS_TEST                   0
#define DMA_MGR_TEST                0

#define LED_FLASH_PERIOD_IN_MS     1000
#define USER_TEST_WDOG              HPM_WDG0




#if USBD_BOOT_TEST
extern void boot_hid_init(void);
extern void boot_hid_test(void);
#endif
#if USBD_APP_TEST
extern void app_hid_init(void);
extern void app_hid_test(void);
#endif

#if WDOG_TEST
extern void WatchDogInit(EWDG_Type *ptr, uint32_t nMs);
extern void FeedWatchDog(EWDG_Type *ptr);
#endif

#endif
