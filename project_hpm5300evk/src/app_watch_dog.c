
#include "board.h"
#include "user_config.h"
#include "hpm_ewdg_drv.h"
#include "hpm_ppor_drv.h"

#define EWDG_CNT_CLK_FREQ           32768UL
#define EWDG_INT_TIMEOUT_MASK       (1UL << 4)

volatile static bool has_interrupt;

static void ewdg0_isr(void)
{
    uint32_t ewdg_stat = ewdg_get_status_flags(HPM_WDG0);
    has_interrupt = true;
    if((ewdg_stat & EWDG_INT_TIMEOUT_MASK) != 0)
    {
        ewdg_refresh(HPM_WDG0);
        printf("wdg0 timeout interrupt\n");
    }
    ewdg_clear_status_flags(HPM_WDG0, ewdg_stat);
}

static void ewdg1_isr(void)
{
    uint32_t ewdg_stat = ewdg_get_status_flags(HPM_WDG1);
    has_interrupt = true;
    if((ewdg_stat & EWDG_INT_TIMEOUT_MASK) != 0)
    {
        ewdg_refresh(HPM_WDG1);
        printf("wdg1 timeout interrupt\n");
    }
    ewdg_clear_status_flags(HPM_WDG1, ewdg_stat);
}

SDK_DECLARE_EXT_ISR_M(IRQn_WDG0, ewdg0_isr);
SDK_DECLARE_EXT_ISR_M(IRQn_WDG1, ewdg1_isr);


static void ewdg_interrupt_int(EWDG_Type *ptr, uint32_t nMs)
{
    ewdg_config_t config;
    hpm_stat_t status;
    ewdg_get_default_config(ptr, &config);
    /* Enable EWDG */
    config.enable_watchdog = true;
    config.ctrl_config.use_lowlevel_timeout = false;
    /* Enable EWDG Timeout interrupt */
    config.int_rst_config.enable_timeout_interrupt = true;
    /* Enable EWDG Timeout Reset */
    config.int_rst_config.enable_timeout_reset = true;
    /* Set EWDG Count clock source to OSC32 */
    config.ctrl_config.cnt_clk_sel = ewdg_cnt_clk_src_ext_osc_clk;

    /* Set the EWDG timeout interrupt interval
     * Set the EWDG timeout reset interval
     */
    config.cnt_src_freq = EWDG_CNT_CLK_FREQ;
    config.ctrl_config.timeout_reset_us = nMs * 1000UL;
    config.ctrl_config.timeout_interrupt_us = (nMs - 100) * 1000UL;

    /* Initialize the EWDG */
    status = ewdg_init(ptr, &config);
    if(status != status_success)
    {
        printf(" EWDG initialization failed, error_code=%d\n", status);
        return;
    }

#if defined(EWDG_TIMEOUT_INTERRUPT_REQUIRE_EDGE_TRIGGER) && (EWDG_TIMEOUT_INTERRUPT_REQUIRE_EDGE_TRIGGER == 1)
    __plic_enable_irq_edge_trigger(HPM_PLIC_BASE, IRQn_WDG0);
#endif

    if(ptr == HPM_WDG0)
    {
        intc_m_enable_irq_with_priority(IRQn_WDG0, 1);
    }
    else if(ptr == HPM_WDG1)
    {
        intc_m_enable_irq_with_priority(IRQn_WDG1, 1);
    }
    else
    {

    }

    printf("EWDG timeout interrupt init\n");
}

static void ewdg_reset_init(EWDG_Type *ptr, uint32_t nMs)
{
    ewdg_config_t config;
    ewdg_get_default_config(ptr, &config);

    config.enable_watchdog = true;
    config.int_rst_config.enable_timeout_reset = true;
    config.ctrl_config.use_lowlevel_timeout = false;
    config.ctrl_config.cnt_clk_sel = ewdg_cnt_clk_src_ext_osc_clk;

    /* Set the EWDG reset timeout*/
    config.cnt_src_freq = EWDG_CNT_CLK_FREQ;
    config.ctrl_config.timeout_reset_us = 1000UL * nMs;

    /* Initialize the WDG */
    hpm_stat_t status = ewdg_init(ptr, &config);
    if (status != status_success) 
    {
        printf(" EWDG initialization failed, error_code=%d\n", status);
    }

    printf("Waiting for WDG reset...\n");
}

void WatchDogInit(EWDG_Type *ptr, uint32_t nMs)
{
    if((ppor_reset_get_flags(HPM_PPOR) & ppor_reset_wdog0) != 0U)
    {
        ppor_reset_clear_flags(HPM_PPOR, ppor_reset_wdog0);
        printf("The last reset was triggered by EWDG0 reset\n");
    }

    if((ppor_reset_get_flags(HPM_PPOR) & ppor_reset_wdog1) != 0U)
    {
        ppor_reset_clear_flags(HPM_PPOR, ppor_reset_wdog1);
        printf("The last reset was triggered by EWDG1 reset\n");
    }

//    ewdg_reset_init(ptr, nMs);
    ewdg_interrupt_int(ptr, nMs);
}

void FeedWatchDog(EWDG_Type *ptr)
{
    //ewdg_refresh(ptr);
}

