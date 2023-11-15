#include <stdio.h>
#include "board.h"
#include "user_config.h"
#include "hpm_gpio_drv.h"
#include "hpm_usb_drv.h"
#include "app_systick.h"
#include "Api_Flash.h"


int main(void)
{
    int u = 0;
    uint64_t nLastTime = 0;
    board_init();
    norflash_init();

#if 0
    // 开机延时等待
    nLastTime = GetCurrentTimeUs();
    while(GetCurrentTimeUs() - nLastTime < 3*1000*1000)
    {
        ;
    }
#endif

    // IO口控制LED
    #if HPM_5300EVK_BOARD
    board_init_led_pins();
    #else
    HPM_IOC->PAD[IOC_PAD_PA12].FUNC_CTL = IOC_PA12_FUNC_CTL_GPIO_A_12;
    gpio_set_pin_output_with_initial(HPM_GPIO0, GPIO_DI_GPIOA, 12, 0);
    #endif
    uint64_t nLedToggleTime = GetCurrentTimeUs();



    printf("cherry usb device init.\n");
    board_init_usb_pins();
    /* As QFN32, QFN48 and LQFP64 has no vbus pin, so should be call usb_phy_using_internal_vbus() API to use internal vbus. */
    /* usb_phy_using_internal_vbus(BOARD_USB); */
    usb_phy_using_internal_vbus(BOARD_USB);

    uint64_t nUsbBootTime = GetCurrentTimeUs();
    boot_hid_init();

    //norflash_test();

#if defined(__GNUC__)
    printf("gcc version %d\n", __GNUC__);
#endif

    printf("now into while loop, enjoy......\n");
    nLastTime = GetCurrentTimeUs();
    while(1)
    {

        if(GetCurrentTimeUs()- nLedToggleTime >= LED_FLASH_PERIOD_IN_MS*1000)
        {
            nLedToggleTime = GetCurrentTimeUs();
            #if (1 == HPM_5300EVK_BOARD)
            board_led_toggle();
            #else
            gpio_toggle_pin(HPM_GPIO0, GPIO_DI_GPIOA, 12);
            #endif
        }

        if(GetCurrentTimeUs()- nUsbBootTime >= 1*1000*1000)
        {
            nUsbBootTime = GetCurrentTimeUs();
            boot_hid_test();
        }

        if(GetCurrentTimeUs() - nLastTime >= 5*1000*1000)
        {
            nLastTime = GetCurrentTimeUs();
            printf("heartbeat, %llu\n", nLastTime);
        }
    }
    return 0;
}
