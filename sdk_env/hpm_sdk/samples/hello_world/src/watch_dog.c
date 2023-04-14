
#include "board.h"
#include "hpm_wdg_drv.h"
#include "hpm_ppor_drv.h"

#define USER_WDG_RESET_INTERVAL_US          reset_interval_clock_period_mult_16k
#define USER_WDG_INTERRUPT_INTERVAL_US      interrupt_interval_clock_period_multi_128k

#define USER_WDG_EXTCLK_FREQ                (32768UL)
#define USER_WDG_PCLK_FREQ                  (clock_get_frequency(clock_ahb))

/*
                                                    EXT_CLK
reset_interval_clock_period_mult_128                3.9ms
reset_interval_clock_period_mult_256                7.8125ms
reset_interval_clock_period_mult_512                15.625ms
reset_interval_clock_period_mult_1k                 31.25ms
reset_interval_clock_period_mult_2k                 62.5ms
reset_interval_clock_period_mult_4k                 125ms
reset_interval_clock_period_mult_8k                 250ms
reset_interval_clock_period_mult_16k                500ms

interrupt_interval_clock_period_multi_64            1.953ms
interrupt_interval_clock_period_multi_256           7.8125ms
interrupt_interval_clock_period_multi_1k            31.25ms
interrupt_interval_clock_period_multi_2k            62.5ms
interrupt_interval_clock_period_multi_4k            125ms
interrupt_interval_clock_period_multi_8k            250ms
interrupt_interval_clock_period_multi_16k           500ms
interrupt_interval_clock_period_multi_32k           1s
interrupt_interval_clock_period_multi_128k          4s
interrupt_interval_clock_period_multi_256k          8s
interrupt_interval_clock_period_multi_2m            32s
interrupt_interval_clock_period_multi_8m            128s
interrupt_interval_clock_period_multi_32m           512s
interrupt_interval_clock_period_multi_128m          2048s
interrupt_interval_clock_period_multi_512m          8192s
interrupt_interval_clock_period_multi_2g            32768s


    };
*/
void WatchDogInit(void)
{
    if ((ppor_reset_get_flags(HPM_PPOR) & ppor_reset_wdog0) != 0U) 
    {
        ppor_reset_clear_flags(HPM_PPOR, ppor_reset_wdog0);
        printf("The last reset was triggered by WDG0 reset\n");
    }
    
    wdg_control_t wdg_ctrl = {
        .reset_enable = true,
        .interrupt_enable = false,
        .wdg_enable = true,
        .clksrc = wdg_clksrc_extclk,
        .reset_interval = reset_interval_clock_period_mult_128,
        .interrupt_interval = interrupt_interval_clock_period_multi_64
    };
    wdg_ctrl.reset_interval = USER_WDG_RESET_INTERVAL_US;
    wdg_ctrl.interrupt_interval = USER_WDG_INTERRUPT_INTERVAL_US;

    /* Initialize the WDG */
    if (status_success != wdg_init(HPM_WDG0, &wdg_ctrl))
    {
        printf("wdg init fail. reset interval=%d, interrupt interval=%d\r\n", wdg_ctrl.reset_interval, wdg_ctrl.interrupt_interval);
    }
    else
    {
        uint32_t reset_interval_in_us = wdg_get_total_reset_interval_in_us(HPM_WDG0, USER_WDG_EXTCLK_FREQ);
        printf("Actual WDG reset interval:%dms\n", reset_interval_in_us / 1000U);
    }
}

void FeedWatchDog(void)
{
    wdg_restart(HPM_WDG0);
}