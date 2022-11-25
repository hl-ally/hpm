#include <stdio.h>
#include "pwm_beep.h"
#include "board.h"
#include "hpm_soc.h"
#include "hpm_trgm_drv.h"
#include "hpm_clock_drv.h"
#include "hpm_pwm_drv.h"


#define USE_PD16_TEST_BEEP_PWM              1           // 利用PD16来输出PWM
#if USE_PD16_TEST_BEEP_PWM
#define BOARD_BEEP_PWM_IRQ                  IRQn_PWM2
#define BOARD_BEEP_PWM                      HPM_PWM2
#define BOARD_BEEP_PWM_OUT                  8
#define BOARD_BEEP_PWM_CMP                  8
#define BOARD_BEEP_PWM_CMP_INITIAL_ZERO     true
#define BOARD_BEEP_PWM_CLOCK_NAME           clock_mot2
#else
#define BOARD_BEEP_PWM_IRQ                  IRQn_PWM3
#define BOARD_BEEP_PWM                      HPM_PWM3
#define BOARD_BEEP_PWM_OUT                  8
#define BOARD_BEEP_PWM_CMP                  8
#define BOARD_BEEP_PWM_CMP_INITIAL_ZERO     true
#define BOARD_BEEP_PWM_CLOCK_NAME           clock_mot3
#endif

#define PWM_PERIOD_IN_MS                    (1U)
#define PWM_DUTY_STEP_COUNT                 (1000U)

typedef struct {
    PWM_Type *pwm;
    uint32_t reload;
    uint32_t step;
    bool pwm_cmp_initial_zero;
    uint8_t pwm_irq;
    uint8_t pwm_cmp;
    uint8_t pwm_ch;
} beep_pwm_t;

beep_pwm_t st_beepPwm;
volatile bool g_bBeepPwmUpdate = false;


static void beep_pwm_isr(void)
{
    pwm_clear_status(st_beepPwm.pwm, PWM_IRQ_HALF_RELOAD);
    g_bBeepPwmUpdate = true;
}

SDK_DECLARE_EXT_ISR_M(BOARD_BEEP_PWM_IRQ, beep_pwm_isr);

static void config_beep_pwm(beep_pwm_t stPwm, uint8_t hw_event_cmp, bool off_level_high)
{
    pwm_cmp_config_t cmp_config = {0};
    pwm_config_t pwm_config = {0};

    pwm_stop_counter(stPwm.pwm);
    pwm_get_default_pwm_config(stPwm.pwm, &pwm_config);
    pwm_get_default_cmp_config(stPwm.pwm, &cmp_config);

    pwm_config.enable_output = false;
    pwm_config.dead_zone_in_half_cycle = 0;
    pwm_config.invert_output = !(stPwm.pwm_cmp_initial_zero && off_level_high);

    /*
     * reload and start counter
     */
    pwm_set_reload(stPwm.pwm, 0, stPwm.reload);
    pwm_set_start_count(stPwm.pwm, 0, 0);

    cmp_config.mode = pwm_cmp_mode_output_compare;

    cmp_config.cmp = stPwm.pwm_cmp_initial_zero ? 0 : stPwm.reload + 1;
    cmp_config.update_trigger = pwm_shadow_register_update_on_modify;

    /* config initial compare value which should take affect immediately */
    pwm_config_cmp(stPwm.pwm, stPwm.pwm_cmp, &cmp_config);

    /* update trigger type so that compare value will be updated on hardware event (RELOAD) */
    cmp_config.update_trigger = pwm_shadow_register_update_on_hw_event;
    /*
     * config pwm as output driven by cmp
     */
    if (status_success != pwm_setup_waveform(stPwm.pwm, stPwm.pwm_ch, &pwm_config, stPwm.pwm_cmp, &cmp_config, 1)) {
        printf("failed to setup waveform\n");
        while(1);
    }

    /*
     * config hw event
     */
    cmp_config.cmp = stPwm.reload - 1;
    cmp_config.update_trigger = pwm_shadow_register_update_on_hw_event;
    pwm_load_cmp_shadow_on_match(stPwm.pwm, hw_event_cmp, &cmp_config);
}


/*
 * PWM控制BEEP,IO口初始化
*/
static void init_beep_pwm_pin(void)
{
#if USE_PD16_TEST_BEEP_PWM
    HPM_IOC->PAD[IOC_PAD_PD16].FUNC_CTL = IOC_PD16_FUNC_CTL_TRGM2_P_06;
#else
    HPM_IOC->PAD[IOC_PAD_PC20].FUNC_CTL = IOC_PC20_FUNC_CTL_TRGM3_P_06;
#endif
}

void pwm_beep_init(void)
{
    uint32_t freq;
    uint32_t hw_event_cmp;
    trgm_output_t config = {0};
    
    printf("beep pwm init\n");

    init_beep_pwm_pin();

    config.type = trgm_output_same_as_input;
    config.invert = false;  // TRGM输出是否取反

#if USE_PD16_TEST_BEEP_PWM
    /* PD16: TRGM2 P6 */
    config.input = HPM_TRGM2_INPUT_SRC_PWM2_CH8REF;
    trgm_output_config(HPM_TRGM2, TRGM_TRGOCFG_TRGM_OUT6, &config);
#else
    /* BEEP - PC20: TRGM3 P6 */
    config.input = HPM_TRGM3_INPUT_SRC_PWM3_CH8REF;
    trgm_output_config(HPM_TRGM3, TRGM_TRGOCFG_TRGM_OUT6, &config);
#endif
    
    freq = clock_get_frequency(BOARD_BEEP_PWM_CLOCK_NAME);
    printf("beep pwm freq: %luHz\n", freq);
    st_beepPwm.reload = freq / 1000 * PWM_PERIOD_IN_MS - 1;
    st_beepPwm.step = st_beepPwm.reload / PWM_DUTY_STEP_COUNT;
    st_beepPwm.pwm = BOARD_BEEP_PWM;
    st_beepPwm.pwm_ch = BOARD_BEEP_PWM_OUT;
    st_beepPwm.pwm_cmp = BOARD_BEEP_PWM_CMP;
    st_beepPwm.pwm_cmp_initial_zero = BOARD_BEEP_PWM_CMP_INITIAL_ZERO;
    st_beepPwm.pwm_irq = BOARD_BEEP_PWM_IRQ;

    hw_event_cmp = PWM_SOC_CMP_MAX_COUNT;
    for (uint8_t i = 0; i < PWM_SOC_CMP_MAX_COUNT; i++) 
    {
        if (i != BOARD_BEEP_PWM_CMP)
        {
            continue;
        }
        hw_event_cmp = i;
        break;
    }
    if (hw_event_cmp == PWM_SOC_CMP_MAX_COUNT) {
        printf("Failed to find a comparator for hardware event\n");
        while (1) {
        };
    }

    config_beep_pwm(st_beepPwm, hw_event_cmp, BOARD_LED_OFF_LEVEL);
    pwm_start_counter(st_beepPwm.pwm);
    intc_m_enable_irq_with_priority(st_beepPwm.pwm_irq, 1);
    
}

void pwm_beep_process(void)
{
    uint32_t duty;
    bool increase_step = true;
    PWM_Type *pwm;
    uint32_t reload, pwm_cmp, step;
    uint32_t initial;

    step = st_beepPwm.step;
    reload = st_beepPwm.reload;
    pwm = st_beepPwm.pwm;
    pwm_cmp = st_beepPwm.pwm_cmp;
    duty = step;

    pwm_enable_irq(pwm, PWM_IRQ_HALF_RELOAD);
      

    /* make sure cmp value is set to initial, before enabling output */
    initial = st_beepPwm.pwm_cmp_initial_zero ? 0 : (st_beepPwm.reload + 1);
    pwm_update_raw_cmp_edge_aligned(pwm, pwm_cmp, initial);
#if USE_PD16_TEST_BEEP_PWM
    trgm_enable_io_output(HPM_TRGM2, 1 << 6);
#else
    trgm_enable_io_output(HPM_TRGM3, 1 << 6);
#endif
    
    while(!(!increase_step && (duty < 2 * step))) {
        if (increase_step && (duty + step > reload)) {
            increase_step = false;
        } 

        if (increase_step) {
            duty += step;
        } else {
            duty -= step;
        }

        while (!g_bBeepPwmUpdate) {
            ;
        }
        pwm_update_raw_cmp_edge_aligned(pwm, pwm_cmp, duty);
        g_bBeepPwmUpdate = false;
    }
#if USE_PD16_TEST_BEEP_PWM
    trgm_disable_io_output(HPM_TRGM2, 1 << 6);
#else
    trgm_disable_io_output(HPM_TRGM3, 1 << 6);
#endif
    pwm_disable_irq(pwm, PWM_IRQ_HALF_RELOAD);
}

