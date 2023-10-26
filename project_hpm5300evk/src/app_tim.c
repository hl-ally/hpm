#include "app_tim.h"
#include "board.h"
#include "hpm_gptmr_drv.h"

typedef enum tim_test_type
{
    TIM_TYPE_BASIC_TIMER = 0,
    TIM_TYPE_GENERATE_PWM,
}tim_test_type_t;

#define APP_BOARD_GPTMR                 BOARD_GPTMR
#define APP_BOARD_GPTMR_CH              BOARD_GPTMR_CHANNEL
#define APP_BOARD_GPTMR_IRQ             BOARD_GPTMR_IRQ
#define APP_BOARD_GPTMR_CLOCK           BOARD_GPTMR_CLK_NAME

#define APP_BOARD_PWM                   BOARD_GPTMR_PWM
#define APP_BOARD_PWM_CH                BOARD_GPTMR_PWM_CHANNEL
#define APP_BOARD_PWM_GPTMR_CLOCK       BOARD_GPTMR_PWM_CLK_NAME

#define APP_TICK_MS                     (500)


tim_test_type_t nTimTestType = TIM_TYPE_GENERATE_PWM;

void tick_ms_isr(void)
{
    if(gptmr_check_status(APP_BOARD_GPTMR, GPTMR_CH_RLD_STAT_MASK(APP_BOARD_GPTMR_CH)))
    {
        gptmr_clear_status(APP_BOARD_GPTMR, GPTMR_CH_RLD_STAT_MASK(APP_BOARD_GPTMR_CH));
        board_led_toggle();
    }
}
SDK_DECLARE_EXT_ISR_M(APP_BOARD_GPTMR_IRQ, tick_ms_isr);


static void set_pwm_frequency_duty(uint32_t nPeriodUs, uint8_t duty)
{
    gptmr_channel_config_t config;
    uint32_t current_reload;
    uint32_t gptmr_freq;
    uint32_t cmp;

    gptmr_channel_get_default_config(APP_BOARD_PWM, &config);
    gptmr_freq = clock_get_frequency(APP_BOARD_GPTMR_CLOCK);
    current_reload = gptmr_freq / nPeriodUs;
    config.reload = current_reload;
    config.cmp_initial_polarity_high = false;
    gptmr_stop_counter(APP_BOARD_PWM, APP_BOARD_PWM_CH);
    gptmr_channel_config(APP_BOARD_PWM, APP_BOARD_PWM_CH, &config, false);
    gptmr_channel_reset_count(APP_BOARD_PWM, APP_BOARD_PWM_CH);

    if(duty > 100)
    {
        duty = 100;
    }
    cmp = ((current_reload * duty) / 100) + 1;
    gptmr_update_cmp(APP_BOARD_PWM, APP_BOARD_PWM_CH, 0, cmp);

    gptmr_start_counter(APP_BOARD_PWM, APP_BOARD_PWM_CH);
}


void tim_test_init(void)
{
    init_gptmr_pins(APP_BOARD_GPTMR);

    switch(nTimTestType)
    {
    case TIM_TYPE_BASIC_TIMER:
        board_init_led_pins();
        
        uint32_t gptmr_freq;
        gptmr_channel_config_t config;

        gptmr_channel_get_default_config(APP_BOARD_GPTMR, &config);

        gptmr_freq = clock_get_frequency(APP_BOARD_GPTMR_CLOCK);
        printf("%s, gptmr freq = %d\n", __func__, gptmr_freq);
        config.reload = gptmr_freq / 1000 * APP_TICK_MS;
        gptmr_channel_config(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH, &config, false);
        gptmr_start_counter(APP_BOARD_GPTMR, APP_BOARD_GPTMR_CH);

        gptmr_enable_irq(APP_BOARD_GPTMR, GPTMR_CH_RLD_IRQ_MASK(APP_BOARD_GPTMR_CH));
        intc_m_enable_irq_with_priority(APP_BOARD_GPTMR_IRQ, 1);
        break;
    case TIM_TYPE_GENERATE_PWM:
        // PB07输出对应的PWM波形
        set_pwm_frequency_duty(1000, 35);
        break;
    default:
        break;
    }
}

void tim_test_process(void)
{

}


