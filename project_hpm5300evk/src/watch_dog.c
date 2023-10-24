
#include "board.h"
#include "hpm_ewdg_drv.h"
#include "hpm_ppor_drv.h"

#define EWDG_CNT_CLK_FREQ 32768UL


void WatchDogInit(uint32_t nMs)
{
    if ((ppor_reset_get_flags(HPM_PPOR) & ppor_reset_wdog0) != 0U) 
    {
        ppor_reset_clear_flags(HPM_PPOR, ppor_reset_wdog0);
        printf("The last reset was triggered by EWDG0 reset\n");
    }

    if((ppor_reset_get_flags(HPM_PPOR) & ppor_reset_wdog1) != 0U)
    {
        ppor_reset_clear_flags(HPM_PPOR, ppor_reset_wdog1);
        printf("The last reset was triggered by EWDG1 reset\n");
    }
    
    ewdg_config_t config;
    ewdg_get_default_config(HPM_WDG0, &config);

    config.enable_watchdog = true;
    config.int_rst_config.enable_timeout_reset = true;
    config.ctrl_config.use_lowlevel_timeout = false;
    uint32_t ewdg_src_clk_freq = EWDG_CNT_CLK_FREQ;
    config.ctrl_config.cnt_clk_sel = ewdg_cnt_clk_src_ext_osc_clk;

    /* Set the EWDG reset timeout to 10 second */
    config.cnt_src_freq = ewdg_src_clk_freq;
    config.ctrl_config.timeout_reset_us = 1000UL * nMs;

    /* Initialize the WDG */
    hpm_stat_t status = ewdg_init(HPM_WDG0, &config);
    if (status != status_success) 
    {
        printf(" EWDG initialization failed, error_code=%d\n", status);
    }

    printf("Waiting for WDG reset...\n");
}

void FeedWatchDog(void)
{
    ewdg_write_refresh_reg(HPM_WDG0);
}