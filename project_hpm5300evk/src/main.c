/*
 * Copyright (c) 2021 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdio.h>
#include "board.h"
#include "user_config.h"
#include "hpm_debug_console.h"
#include "pwm_rgb_led.h"
#include "app_pwm.h"
#include "app_otp.h"
#include "hpm_gpio_drv.h"
#include "hpm_gpiom_drv.h"
#include "hpm_pllctlv2_drv.h"
#include "app_flash.h"
#include "hpm_romapi.h"
#include "hpm_ppor_drv.h"
#include "app_systick.h"
#include "app_dac.h"
#include "app_adc.h"
#include "app_tim.h"
#include "app_tsns.h"
#include "app_dma_mgr.h"
#include "hpm_l1c_drv.h"


#define TEST_ADDR_START 0x40000

#define XPI_MEM_BASE 0x80000000UL


static uint8_t program_buf[4096];

xpi_nor_config_t nor_config;

/**
  * @brief Mini Sector Erase command
  * @Note  This function must be placed in RAM
  */
ATTR_RAMFUNC
hpm_stat_t sip_flash_minisector_erase(XPI_Type *base,
                                                    xpi_xfer_channel_t port,
                                                    const xpi_nor_config_t *nor_config,
                                                    uint32_t start)
{
    hpm_stat_t status;
    xpi_instr_seq_t sector1k_erase = 
    {
        .entry = {
            XPI_INSTR_SEQ(XPI_PHASE_CMD_SDR, XPI_1PAD, 0x82, XPI_PHASE_RADDR_SDR, XPI_1PAD, 24),
            0,
            0,
            0
        }
    };

    /* Issue Write Enable command */
    ROM_API_TABLE_ROOT->xpi_nor_driver_if->enable_write(base, port, nor_config, start);

    /* Prepare the Mini Sector Erase command */
    ROM_API_TABLE_ROOT->xpi_driver_if->update_instr_table(base, &sector1k_erase.entry[0], 1, 1);

    /* Issue Mini Sector Erase Command */
    xpi_xfer_ctx_t xfer_ctx;
    (void) memset(&xfer_ctx, 0, sizeof(xfer_ctx));
    xfer_ctx.addr = start;
    xfer_ctx.channel = port;
    xfer_ctx.cmd_type = xpi_apb_xfer_type_cmd;
    xfer_ctx.seq_idx = 1;
    xfer_ctx.seq_num = 1;
    status = ROM_API_TABLE_ROOT->xpi_driver_if->transfer_blocking(base, &xfer_ctx);
    if (status != status_success) {
        return status;
    }

    /* Wait until the Mini Sector Erase command completed */
    status = ROM_API_TABLE_ROOT->xpi_nor_driver_if->wait_busy(base, port, nor_config, start); 

    return status;
}

void show_memory_content(uint32_t addr, size_t size_in_bytes)
{
    for (uint32_t offset = 0; offset < size_in_bytes; offset += 256) {

        const uint8_t *pbuf_8 = (const uint8_t *)(addr + offset);
        uint32_t remaining_size = size_in_bytes - offset;
        uint32_t print_size = MIN(remaining_size, 256);
        for (uint32_t j = 0; j < print_size; j++) {
            printf("%02x ", pbuf_8[j]);
        }
        printf("\n");
    }
}

bool sip_flash_init(xpi_nor_config_t *config)
{
    xpi_nor_config_option_t option;
    option.header.U = 0xfcf90002;
    option.option0.U = 0x7;
    option.option0.U = 0x0006;
    option.option1.U = 0x1000;

    hpm_stat_t status = rom_xpi_nor_auto_config(HPM_XPI0, config, &option);

    return (status == status_success) ? true : false;
}


void test_minisector_erase(xpi_nor_config_t *nor_config)
{
    for (uint32_t i = 0; i < sizeof(program_buf); i++) {
        program_buf[i] = (uint8_t)( i & 0xFF);
    }

    /* Pre-program test data to test memory region */
    rom_xpi_nor_erase(HPM_XPI0, xpi_xfer_channel_a1, nor_config, TEST_ADDR_START, 4096);
    rom_xpi_nor_program(HPM_XPI0, xpi_xfer_channel_a1, nor_config, (uint32_t*)program_buf, TEST_ADDR_START, sizeof(program_buf));

    uint32_t ahb_mem_addr = XPI_MEM_BASE + TEST_ADDR_START;
    l1c_dc_invalidate(ahb_mem_addr, sizeof(program_buf));
    printf("Before MINI sector erase test, data content:\n");
    show_memory_content(ahb_mem_addr, 4096);

    uint32_t erase_offset = TEST_ADDR_START + SIZE_1KB;
    sip_flash_minisector_erase(HPM_XPI0, xpi_xfer_channel_a1, nor_config, erase_offset);
    l1c_dc_invalidate(ahb_mem_addr, sizeof(program_buf));
    printf("After MINI sector erase test, data content:\n");
    show_memory_content(ahb_mem_addr, 4096);

    printf("Please check the data after mini sector erase, the 2nd 1KB data should be all 0xFFs\n");

}

int main(void)
{
    int u = 0;
    uint64_t nLastTime = 0;
    bool result;
    board_init();

    printf("HPMicro SiP Package FLASH test demo\n");

    result = sip_flash_init(&nor_config);
    printf("SIP FLASH Initialization %s\n", result ? "PASSED" : "FAILED");
    if (!result) return result;
    test_minisector_erase(&nor_config);

    #if 0
    // 修改主频为480MHz
    if(status_success != pllctlv2_init_pll_with_freq(HPM_PLLCTLV2, 0, 960000000))
    {
        printf("change pll freq fail\n");
    }
    else
    {
        printf("change pll freq success\n");
        printf("==============================\n");
        printf(" %s clock summary\n", BOARD_NAME);
        printf("==============================\n");
        printf("cpu0:\t\t %luHz\n", clock_get_frequency(clock_cpu0));
        printf("hpm_core_clock:\t %luHz\n", hpm_core_clock);
        printf("ahb:\t\t %luHz\n", clock_get_frequency(clock_ahb));
        printf("mchtmr0:\t %luHz\n", clock_get_frequency(clock_mchtmr0));
        printf("xpi0:\t\t %luHz\n", clock_get_frequency(clock_xpi0));
        printf("==============================\n");
    }
    #endif

#if 1
    // 开机延时等待
    nLastTime = GetCurrentTimeUs();
    while(GetCurrentTimeUs() - nLastTime < 3*1000*1000)
    {
        ;
    }
#endif

#if (1 == LED_IO_TEST)
    // IO口控制LED
    #if HPM_5300EVK_BOARD
    board_init_led_pins();
    #else
    HPM_IOC->PAD[IOC_PAD_PA12].FUNC_CTL = IOC_PA12_FUNC_CTL_GPIO_A_12;
    gpio_set_pin_output_with_initial(HPM_GPIO0, GPIO_DI_GPIOA, 12, 0);
    #endif
    uint64_t nLedToggleTime = GetCurrentTimeUs();
#endif

#if (1 == FGPIO_TOGGLE_TEST)
    board_init_fgpio_pin();
#endif

#if (1 == RGB_LED_PWM_TEST)
    pwm_rgb_led_init();
#endif

#if (1 == PWM_BEEP_TEST)
    pwm_beep_init();
#endif

#if (1 == CHERRYUSB_DEVICE_TEST)
    printf("cherry usb device test.\n");
    board_init_usb_pins();
    #if (1 == USBD_BOOT_TEST)
    uint64_t nUsbBootTime = GetCurrentTimeUs();
    boot_hid_init();
    #endif
    #if (1 == USBD_APP_TEST)
    app_hid_init();
    #endif
#endif

#if (1 == OTP_TEST)
    OtpValuePrint();
    ShowUuid();
    ShowUid();
    ShowXpiFlashInfo();
#endif

#if (1 == NOR_FLASH_TEST)
    #if defined(RUN_IN_RAM) && RUN_IN_RAM
    printf("run in ram\n");
    norflash_init();
    #endif
#endif

#if (1 == SOFTWARE_RESET_TEST)
    uint64_t nSortwareResetTime = GetCurrentTimeUs();
#endif

#if (1 == WDOG_TEST)
    WatchDogInit(USER_TEST_WDOG, 10*1000);
#endif

#if (1 == DAC_TEST)
    dac_test_init();
#endif

#if (1 == PWM_TEST)
    pwm_test_init();
    generate_edge_aligned_waveform();
#endif

#if (1 == ADC_TEST)
    adc_test_init();
    uint64_t nAdcTime = GetCurrentTimeUs();
#endif

#if (1 == TIM_TEST)
    tim_test_init();
#endif

#if (1 == TSNS_TEST)
    tsns_test_init();
#endif

#if (1 == DMA_MGR_TEST)
    dma_mgr_test_init();
    uint64_t nDmaTime = GetCurrentTimeUs();
#endif

#if defined(__GNUC__)
    printf("gcc version %d\n", __GNUC__);
#endif
    printf("hello world\n");
    printf("now into while loop, enjoy......\n");
    nLastTime = GetCurrentTimeUs();
    while(1)
    {
    #if 0
        u = getchar();
        if (u == '\r') {
            u = '\n';
        }
        printf("%c", u);
    #endif

    #if (1 == NOR_FLASH_TEST)
        #if defined(RUN_IN_RAM) && RUN_IN_RAM
        static uint8_t norflash_count = 1;
        if(norflash_count > 0)
        {
            norflash_count--;
            norflash_test();
        }
        #endif
    #endif

    #if (1 == LED_IO_TEST)
        if(GetCurrentTimeUs()- nLedToggleTime >= LED_FLASH_PERIOD_IN_MS*1000)
        {
            nLedToggleTime = GetCurrentTimeUs();
            #if (1 == HPM_5300EVK_BOARD)
            board_led_toggle();
            #else
            gpio_toggle_pin(HPM_GPIO0, GPIO_DI_GPIOA, 12);
            #endif
        }
    #endif

    #if (1 == FGPIO_TOGGLE_TEST)
        // 测试IO翻转速率
        //clock_cpu_delay_us(1);
        //board_delay_ms(300);
        board_fgpio_toggle_process();
    #endif

    #if (1 == RGB_LED_PWM_TEST)
        pwm_rgb_led_process();
    #endif

    #if (1 == PWM_BEEP_TEST)
        board_delay_ms(10);
        pwm_beep_process();
    #endif

    #if (1 == CHERRYUSB_DEVICE_TEST)
        #if (1 == USBD_BOOT_TEST)
        if(GetCurrentTimeUs()- nUsbBootTime >= 1*1000*1000)
        {
            nUsbBootTime = GetCurrentTimeUs();
            boot_hid_test();
        }
        #endif
        
        #if (1 == USBD_APP_TEST)
        app_hid_test();
//        board_delay_ms(1000);
        #endif
    #endif

    #if 0
        // 运行rom api, run_bootloader测试
        if (GetCurrentTimeUs() - nLastTime > 20*1000*1000)
        {
            api_boot_arg_t boot_arg = { .index = 0, 
                                        .peripheral = API_BOOT_PERIPH_AUTO,
                                        .src = API_BOOT_SRC_ISP,
                                        .tag = API_BOOT_TAG};
            ROM_API_TABLE_ROOT->run_bootloader(&boot_arg);
        }
    #endif

    #if (1 == SOFTWARE_RESET_TEST)
        // 复位操作
        if (GetCurrentTimeUs() - nSortwareResetTime >= 20*1000*1000)
        {
            uint32_t resetTime = 5*24*1000*1000; 
            printf("softerware reset after %ds\r\n", resetTime/(24*1000*1000));
            ppor_sw_reset(HPM_PPOR, resetTime);
            while(1);
        }
    #endif

    #if (1 == WDOG_TEST)
        FeedWatchDog(USER_TEST_WDOG);
        #endif

    #if (1 == DAC_TEST)
        dac_test_process();
    #endif

    #if (1 == ADC_TEST)
        if(GetCurrentTimeUs() - nAdcTime >= 2*1000*1000)
        {
            nAdcTime = GetCurrentTimeUs();
            adc_test_process();
        }
    #endif

    #if (1 == TIM_TEST)
        tim_test_process();
    #endif

    #if (1 == DMA_MGR_TEST)
        if(GetCurrentTimeUs() - nDmaTime >= 3*1000*1000)
        {
            nDmaTime = GetCurrentTimeUs();
            dma_mgr_test_process();
        }
    #endif

        if(GetCurrentTimeUs() - nLastTime >= 5*1000*1000)
        {
            nLastTime = GetCurrentTimeUs();
            printf("heartbeat, %llu\n", nLastTime);
            //show_temperature();
        }
    }
    return 0;
}
