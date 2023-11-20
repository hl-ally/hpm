#ifndef _USER_CONFIG_H
#define _USER_CONFIG_H

#define HPM_5300EVK_BOARD           1       // 当前是否在HPM 5300EVK开发板上执行程序
#define LED_FLASH_PERIOD_IN_MS      300



extern void boot_hid_init(void);
extern void boot_hid_test(void);

#endif
