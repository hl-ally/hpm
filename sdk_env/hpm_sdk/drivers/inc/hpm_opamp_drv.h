/*
 * Copyright (c) 2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef HPM_OPAMP_DRV_H
#define HPM_OPAMP_DRV_H

#include "hpm_common.h"
#include "hpm_opamp_regs.h"

/**
 * @brief OPMAP driver APIs
 * @defgroup opamp_interface OPAMP driver APIs
 * @ingroup io_interfaces
 * @{
 *
 */
typedef enum {
    opaout_sel_vout_pad = 0,    /**< Connect vout pad only */
    opaout_sel_vout_adc = 1,    /**< Connect vout adc only */
    opaout_sel_vout_pad_and_adc = 2,    /**< Connect vout adc and pad */
    opaout_sel_vout_none = 3    /**< Disconnect vout adc and pad */
} hpm_opamp_opaout_sel_t;

typedef enum {
    vswitch_sel_vsupply_x_0_5 = 0,    /**< 0.5*vsupply */
    vswitch_sel_vsupply_x_0_55 = 1,   /**< 0.55*vsupply */
    vswitch_sel_vsupply_x_0_6 = 2,    /**< 0.4*vsupply */
    vswitch_sel_vsupply_x_0_65 = 3,   /**< 0.45*vsupply */
    vswitch_sel_vsupply_x_0_3 = 4,    /**< 0.3*vsupply */
    vswitch_sel_vsupply_x_0_35 = 5,   /**< 0.35*vsupply */
    vswitch_sel_vsupply_x_0_4 = 6,    /**< 0.4*vsupply */
    vswitch_sel_vsupply_x_0_45 = 7,   /**< 0.45*vsupply */
} hpm_opamp_vswitch_sel_t;

typedef enum {
    isel_iref_x_1 = 0,       /**< 1x iref */
    isel_iref_x_1_333 = 1,   /**< 1.333x iref */
    isel_iref_x_2 = 2,       /**< 2x iref */
    isel_iref_x_0_667 = 3,   /**< 0.667x iref */
} hpm_opamp_iref_sel_t;

typedef enum {
    vim_inm_sel_vim0 = 0,    /**< Connect pad vim0 */
    vim_inm_sel_vim1 = 1,    /**< Connect pad vim1 */
    vim_inm_sel_vim2 = 2,    /**< Connect pad vim2 */
    vim_inm_sel_vim_dac = 3,    /**< Connect pad vim dac */
    vim_inm_sel_floating = 4    /**< Connect inm floating */
} hpm_opamp_vim_sel_t;

typedef enum {
    gpa_sel_1r = 0,     /**< 1R */
    gpa_sel_3r = 1,
    gpa_sel_7r = 2,
    gpa_sel_15r = 3,
    gpa_sel_31r = 4,
    gpa_sel_63r = 5,
    gpa_sel_127r = 6,   /**< 6R */
} hpm_opamp_gpa_sel_t;

typedef enum {
    cap_sel_X7 = 0, /**< 7 * 0.36pF */
    cap_sel_x8 = 1,
    cap_sel_x10 = 2,
    cap_sel_x13 = 3,
    cap_sel_x15 = 4,    /**< 15 * 0.36pF */
    cap_sel_x18 = 5,
    cap_sel_x5 = 6,
    cap_sel_x6 = 7, /**< 6 * 0.36pF */
} hpm_opamp_cap_sel_t;

typedef enum {
    vip_inp_sel_vip0 = 0,    /**< Connect pad vip0 */
    vip_inp_sel_vip1 = 1,    /**< Connect pad vip1 */
    vip_inp_sel_vip2 = 2,    /**< Connect pad vip2 */
    vip_inp_sel_vip_dac = 3,    /**< Connect pad vip dac */
    vip_inp_sel_vsupply_x_0_25 = 4,    /**< Connect reference = 0.25 * vsupply */
    vip_inp_sel_vsupply_x_0_5 = 5,    /**< Connect reference = 0.5 * vsupply */
    vip_inp_sel_vsupply_x_0_75 = 6,    /**< Connect reference = 0.75 * vsupply */
    vip_inp_sel_floating = 7    /**< Connect inp floating */
} hpm_opamp_vip_sel_t;

/**
 * @brief opamp preset channel
 *
 */
typedef enum {
    opamp_preset_0 = OPAMP_CFG_PRESET0,
    opamp_preset_1 = OPAMP_CFG_PRESET1,
    opamp_preset_2 = OPAMP_CFG_PRESET2,
    opamp_preset_3 = OPAMP_CFG_PRESET3,
    opamp_preset_4 = OPAMP_CFG_PRESET4,
    opamp_preset_5 = OPAMP_CFG_PRESET5,
    opamp_preset_6 = OPAMP_CFG_PRESET6,
    opamp_preset_7 = OPAMP_CFG_PRESET7,
} hpm_opamp_preset_chn_t;

/**
 * @brief opamp ctrl0 configuration
 *
 */
typedef union hpm_opamp_ctrl0_cfg {
    struct {
        uint32_t vip_sel: 3;
        uint32_t is_vssa_disconnect: 1;
        uint32_t cap_sel1: 3;
        uint32_t cap_sel2: 1;
        uint32_t function_mode: 5;
        uint32_t gap_sel: 3;
        uint32_t vim_sel: 3;
        uint32_t iref_sel: 2;
        uint32_t vswitch_sel: 3;
        uint32_t opaout_sel: 2;
        uint32_t en_lv: 1;
        uint32_t reserve: 6;
    };
    uint32_t val;
} hpm_opamp_ctrl0_cfg_t;

/**
 * @brief opamp configuration preset0
 *
 */
typedef union hpm_opamp_cfg_preset0 {
    struct {
        uint32_t vip_sel: 3;
        uint32_t vim_sel: 3;
        uint32_t vswitch_sel: 3;
        uint32_t cap_sel: 4;
        uint32_t reserve: 19;
    };
    uint32_t val;
} hpm_opamp_ctrl_cfg_preset0_t;

/**
 * @brief opamp configuration preset1
 *
 */
typedef union hpm_opamp_cfg_preset1 {
    struct {
        uint32_t function_mode: 5;
        uint32_t gap_sel: 3;
        uint32_t iref_sel: 2;
        uint32_t opaout_sel: 2;
        uint32_t is_vssa_disconnect: 1;
        uint32_t en_lv: 1;
        uint32_t hw_trig_en: 1;
        uint32_t reserve: 17;
    };
    uint32_t val;
} hpm_opamp_ctrl_cfg_preset1_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief enable opamp function
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_enable(OPAMP_Type *opamp)
{
    opamp->CTRL0 |= OPAMP_CTRL0_EN_LV_MASK;
}

/**
 * @brief disable opamp function
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_disable(OPAMP_Type *opamp)
{
    opamp->CTRL0 &= ~OPAMP_CTRL0_EN_LV_MASK;
}

/**
 * @brief opamp core vout connect
 *
 * @param opamp @ref OPAMP_Type
 * @param sel @ref hpm_opamp_opaout_sel_t
 */
static inline void hpm_opamp_ctrl0_opaout_sel(OPAMP_Type *opamp, hpm_opamp_opaout_sel_t sel)
{
    opamp->CTRL0 = (opamp->CTRL0 & (~OPAMP_CTRL0_OPAOUT_SEL_MASK)) | OPAMP_CTRL0_OPAOUT_SEL_SET(sel);
}

/**
 * @brief opamp vsupply selection
 *
 * @param opamp @ref OPAMP_Type
 * @param sel @ref hpm_opamp_vswitch_sel_t
 */
static inline void hpm_opamp_vswitch_sel(OPAMP_Type *opamp, hpm_opamp_vswitch_sel_t sel)
{
    opamp->CTRL0 = (opamp->CTRL0 & (~OPAMP_CTRL0_VSWITCH_SEL_MASK)) | OPAMP_CTRL0_VSWITCH_SEL_SET(sel);
}

/**
 * @brief opamp multiplier of iref
 *
 * @param opamp @ref OPAMP_Type
 * @param sel @ref hpm_opamp_iref_sel_t
 */
static inline void hpm_opamp_iref_sel(OPAMP_Type *opamp, hpm_opamp_iref_sel_t sel)
{
    opamp->CTRL0 = (opamp->CTRL0 & (~OPAMP_CTRL0_ISEL_MASK)) | OPAMP_CTRL0_ISEL_SET(sel);
}

/**
 * @brief opamp core inm connect pad
 *
 * @param opamp @ref OPAMP_Type
 * @param sel @ref hpm_opamp_vim_sel_t
 */
static inline void hpm_opamp_vim_sel(OPAMP_Type *opamp, hpm_opamp_vim_sel_t sel)
{
    opamp->CTRL0 = (opamp->CTRL0 & (~OPAMP_CTRL0_VIM_SEL_MASK)) | OPAMP_CTRL0_VIM_SEL_SET(sel);
}

/**
 * @brief main string resistor selection
 *
 * @param opamp @ref OPAMP_Type
 * @param sel @ref hpm_opamp_gpa_sel_t
 * @param func_mode function mode
 */
static inline void hpm_opamp_gpa_sel(OPAMP_Type *opamp, hpm_opamp_gpa_sel_t sel, uint8_t func_mode)
{
    opamp->CTRL0 = (opamp->CTRL0 & (~OPAMP_CTRL0_GPA_SEL_MASK)) | OPAMP_CTRL0_GPA_SEL_SET((sel << 5) | (func_mode));
}

/**
 * @brief disconnect the main series resistor and VSSA
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_disconnect_vssa(OPAMP_Type *opamp)
{
    opamp->CTRL0 |= OPAMP_CTRL0_VBYPASS_MASK;
}

/**
 * @brief connect the main series resistor and VSSA
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_connect_vssa(OPAMP_Type *opamp)
{
    opamp->CTRL0 &= ~OPAMP_CTRL0_VBYPASS_MASK;
}

/**
 * @brief opamp inp select
 *
 * @param opamp @ref OPAMP_Type
 * @param sel @ref hpm_opamp_vip_sel_t
 */
static inline void hpm_opamp_vip_sel(OPAMP_Type *opamp, hpm_opamp_vip_sel_t sel)
{
    opamp->CTRL0 = (opamp->CTRL0 & (~OPAMP_CTRL0_VIP_SEL_MASK)) | OPAMP_CTRL0_VIP_SEL_SET(sel);
}

/**
 * @brief opamp set ctrl0 value
 *
 * @param opamp @ref OPAMP_Type
 * @param ctrl0 @ref hpm_opamp_ctrl0_cfg_t
 */
static inline void hpm_opamp_ctrl0_set(OPAMP_Type *opamp, hpm_opamp_ctrl0_cfg_t *ctrl0)
{
    opamp->CTRL0 = ctrl0->val;
}

/**
 * @brief opamp get ctrl0 value
 *
 * @param opamp @ref OPAMP_Type
 * @param ctrl0 @ref hpm_opamp_ctrl0_cfg_t
 */
static inline void hpm_opamp_ctrl0_get(OPAMP_Type *opamp, hpm_opamp_ctrl0_cfg_t *ctrl0)
{
    ctrl0->val = opamp->CTRL0;
}

/**
 * @brief opamp get current preset
 *
 * @param opamp @ref OPAMP_Type
 * @return value
 */
static inline uint8_t hpm_opamp_get_cur_preset(OPAMP_Type *opamp)
{
    return OPAMP_STATUS_CUR_PRESET_GET(opamp->STATUS);
}

/**
 * @brief get the current preset value
 *
 * @param opamp @ref OPAMP_Type
 * @return true one of cur_preset is selected for opamp
 * @return false opamp use cfg0 parameters
 */
static inline bool hpm_opamp_get_is_preset(OPAMP_Type *opamp)
{
    return OPAMP_STATUS_PRESET_ACT_GET(opamp->STATUS);
}

/**
 * @brief Get the trigger conflict status
 *
 * @param opamp @ref OPAMP_Type
 * @return  if more than one hardware trigger is set, will put all trigger input there.
 */
static inline uint8_t hpm_opamp_get_trig_conflict_status(OPAMP_Type *opamp)
{
    return OPAMP_STATUS_TRIG_CONFLICT_GET(opamp->STATUS);
}

/**
 * @brief Clear the trigger conflict status
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_clear_conflict_status(OPAMP_Type *opamp)
{
    opamp->STATUS = OPAMP_STATUS_TRIG_CONFLICT_MASK;
}

/**
 * @brief Set opamp preset value
 *
 * @param opamp @ref OPAMP_Type
 * @param val @ref hpm_opamp_preset_chn_t
 */
static inline void hpm_opamp_set_sw_preset_val(OPAMP_Type *opamp, hpm_opamp_preset_chn_t val)
{
    opamp->CTRL1 = (opamp->CTRL1 & (~OPAMP_CTRL1_SW_SEL_MASK)) | OPAMP_CTRL1_SW_SEL_SET(val);
}

/**
 * @brief Enable software preset
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_enable_sw_preset(OPAMP_Type *opamp)
{
    opamp->CTRL1 |= OPAMP_CTRL1_SW_PRESET_MASK;
}

/**
 * @brief Disable software preset
 *
 * @param opamp @ref OPAMP_Type
 */
static inline void hpm_opamp_disable_sw_preset(OPAMP_Type *opamp)
{
    opamp->CTRL1 &= ~OPAMP_CTRL1_SW_PRESET_MASK;
}

/**
 * @brief Set preset x configuration 0 value
 *
 * @param opamp @ref OPAMP_Type
 * @param preset_chn @ref hpm_opamp_preset_chn_t
 * @param cfg @ref hpm_opamp_ctrl_cfg_preset0_t
 */
static inline void hpm_opamp_set_preset_x_cfg0(OPAMP_Type *opamp, hpm_opamp_preset_chn_t preset_chn, hpm_opamp_ctrl_cfg_preset0_t cfg)
{
    opamp->CFG[preset_chn].CFG0 = cfg.val;
}

/**
 * @brief Set preset x configuration 1 value
 *
 * @param opamp @ref OPAMP_Type
 * @param preset_chn @ref hpm_opamp_preset_chn_t
 * @param cfg @ref hpm_opamp_ctrl_cfg_preset0_t
 */
static inline void hpm_opamp_set_preset_x_cfg1(OPAMP_Type *opamp, hpm_opamp_preset_chn_t preset_chn, hpm_opamp_ctrl_cfg_preset1_t cfg)
{
    opamp->CFG[preset_chn].CFG1 = cfg.val;
}

/**
 * @brief Set preset x channel value
 *
 * @param opamp @ref OPAMP_Type
 * @param preset_chn @ref hpm_opamp_preset_chn_t
 * @param chn channel
 */
static inline void hpm_opamp_set_preset_x_chn(OPAMP_Type *opamp, hpm_opamp_preset_chn_t preset_chn, uint8_t chn)
{
    opamp->CFG[preset_chn].CFG2 = OPAMP_CFG_CFG2_CHANNEL_SET(chn);
}

/**
 * @}
 *
 */

#ifdef __cplusplus
}
#endif

#endif /* HPM_ACMP_DRV_H */
