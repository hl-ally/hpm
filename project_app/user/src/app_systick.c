#include <stdint.h>
#include "app_systick.h"
#include "board.h"
#include "hpm_mchtmr_drv.h"


void Delay_ms(uint16_t ms)
{
    board_delay_ms(ms);
}


uint64_t GetCurrentTimeUs(void)
{
#if 1
    // 利用机器定时器MCHTMR获取当前的时间
    static int is_inited = 0;
    static uint32_t div;
    uint32_t freq = 0;
    uint64_t tick_us;

    if (!is_inited) {
        is_inited = 1;
        //board_ungate_mchtmr_at_lp_mode();
        freq = clock_get_frequency(clock_mchtmr0);
        div = freq / 1000000;
    }

    tick_us = mchtmr_get_count(HPM_MCHTMR) / div;
    return tick_us;
#else
    // 利用cycle获取当前时间
    static int is_inited = 0;
    static uint32_t ticks_per_us;
    if (!is_inited)
    {
        is_inited = 1;
        ticks_per_us = (hpm_core_clock + 1000000U - 1U) / 1000000U;
    }
    uint64_t result;
    uint32_t resultl_first = read_csr(CSR_CYCLE);
    uint32_t resulth = read_csr(CSR_CYCLEH);
    uint32_t resultl_second = read_csr(CSR_CYCLE);
    if (resultl_first < resultl_second) {
        result = ((uint64_t)resulth << 32) | resultl_first; /* if MCYCLE didn't roll over, return the value directly */
    } else {
        resulth = read_csr(CSR_CYCLEH);
        result = ((uint64_t)resulth << 32) | resultl_second; /* if MCYCLE rolled over, need to get the MCYCLEH again */
    }
    return result/ticks_per_us;
#endif
 }


uint64_t GetCurrentTime(void)
{
    return GetCurrentTimeUs()/1000;
}
