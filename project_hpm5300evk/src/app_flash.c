/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */
#include <stdio.h>
#include "board.h"
#include "hpm_debug_console.h"
#include "hpm_l1c_drv.h"
#include "hpm_romapi.h"
#include "hpm_clock_drv.h"
#include "hpm_mchtmr_drv.h"
#include "app_flash.h"

#define NORFLASH_LOG(...)            printf(__VA_ARGS__)

static xpi_nor_config_t s_xpi_nor_config;
static struct flashstress_context flashstress_ctxs[CONFIG_CONTEXT_NUM];

static uint64_t interface_get_tick_us(void)
{
    static int is_inited = 0;
    static uint32_t div;
    uint32_t freq = 0;
    uint64_t tick_us;

    if (!is_inited) {
        is_inited = 1;
        board_ungate_mchtmr_at_lp_mode();
        freq = clock_get_frequency(clock_mchtmr0);
        div = freq / 1000000;
    }

    tick_us = mchtmr_get_count(HPM_MCHTMR) / div;
    return tick_us;
}
/****************************************************************/

static uint64_t tick_elapesd(uint64_t start, uint64_t current)
{
    uint64_t tick;
    if (start >= current) {
        tick = start - current;
    } else {
        tick = current + (0xffffffffffffffffull - start);
    }
    return tick;
}

static struct flashstress_context *flashstress_context_alloc(void)
{
    struct flashstress_context *ctx;

    for (int i = 0; i < CONFIG_CONTEXT_NUM; i++) {
        if (!flashstress_ctxs[i].is_used) {
            flashstress_ctxs[i].is_used = 1;
            ctx = &flashstress_ctxs[i];
            break;
        }
    }

    return ctx;
}

static void flashstress_context_free(struct flashstress_context *ctx)
{
    if (!ctx) {
        return ;
    }

    ctx->is_used = 0;
}

struct flashstress_context *flashstress_create(struct flashstress_driver *drv, const char *name)
{
    struct flashstress_context *ctx = NULL;

    if (!drv || !name || !drv->erase_block || !drv->read ||
        !drv->write || !drv->read ||
        !drv->get_flash_block_size_bytes ||
        !drv->get_flash_chip_size_bytes) {
        NORFLASH_LOG("FLASHSTRESS: ERR: drv is invalid!!!\n");
        return ctx;
    }

    if (!drv->erase_chip) {
        NORFLASH_LOG("[%s]: WARNNING: <erase chip> api invalid!\n", name);
    }
    
    ctx = flashstress_context_alloc();
    if (!ctx) {
        NORFLASH_LOG("FLASHSTRESS: ERROR: [%s] context alloc failed!!!\n", name);
    }

    ctx->drv = *drv;
    strncpy(ctx->name, name, CONFIG_NAME_LEN);

    return ctx;
}

void flashstress_destroy(struct flashstress_context *ctx)
{
    flashstress_context_free(ctx);
}

int flashstress_erase_chip(struct flashstress_context *ctx)
{
    struct flashstress_driver *drv = &ctx->drv;
    uint32_t chip_size_bytes = drv->get_flash_chip_size_bytes();
    uint32_t block_size_bytes = drv->get_flash_block_size_bytes();
    uint8_t erase_value = drv->get_flash_erase_value();
    uint32_t offset;
    uint32_t i;
    uint64_t erase_time_us;
    uint64_t start_time_us;
    float erase_time_s;

    if (drv->erase_chip) {
        start_time_us = interface_get_tick_us();
        if (drv->erase_chip()) {
            NORFLASH_LOG("[%s]: ERROR: <erase_chip> FAILED!!!\n");
            return -1;
        }
        erase_time_us = tick_elapesd(start_time_us, interface_get_tick_us());
    } else {
        NORFLASH_LOG("[%s]: WARNNING: <erase chip> api invalid, <erase block> api to be used!\n", ctx->name);
        offset = 0;
        start_time_us = interface_get_tick_us();
        while (offset < chip_size_bytes) {
            if (drv->erase_block(offset)) {
                NORFLASH_LOG("[%s]: ERROR: <erase_block> offset: 0x%X FAILED!!!\n", ctx->name, offset);
                return -1;
            }
            offset += block_size_bytes;
        }
        erase_time_us = tick_elapesd(start_time_us, interface_get_tick_us());
    }

    /*
     * verify erase
     */
    offset = 0;
    while (offset < chip_size_bytes) {
        memset(ctx->data_buf, !erase_value, CONFIG_DATA_BUF_SIZE);
        if (drv->read(offset, ctx->data_buf, CONFIG_DATA_BUF_SIZE)) {
            NORFLASH_LOG("[%s]: ERROR: <erase verify read api> offset: 0x%X FAILED!!!\n", ctx->name, offset);
            return -1;
        }

        for (i = 0; i < CONFIG_DATA_BUF_SIZE; i++) {
            if (ctx->data_buf[i] != erase_value) {
                NORFLASH_LOG("[%s]: ERROR: <ease verify data> 0x%X!= 0x%X FAILED!!!\n", ctx->name, ctx->data_buf[i], erase_value);
                return -1;
            }
        }
        offset += CONFIG_DATA_BUF_SIZE;
    }

    erase_time_s = erase_time_us / 1000000.0f;
    NORFLASH_LOG("[%s]: [erase_chip] PASSED!!!\n", ctx->name);
    NORFLASH_LOG("[%s]: [erase_chip] take %f seconds!!!\n", ctx->name, erase_time_s);

    return 0;
}

int flashstress_erase_all_blocks(struct flashstress_context *ctx)
{
    struct flashstress_driver *drv = &ctx->drv;
    uint32_t chip_size_bytes = drv->get_flash_chip_size_bytes();
    uint32_t block_size_bytes = drv->get_flash_block_size_bytes();
    uint8_t erase_value = drv->get_flash_erase_value();
    uint32_t offset;
    uint32_t i;
    uint64_t erase_time_us;
    uint64_t start_time_us;
    float erase_time_s;

    offset = 0;
    start_time_us = interface_get_tick_us();
    while (offset < chip_size_bytes) {
        if (drv->erase_block(offset)) {
            NORFLASH_LOG("[%s]: ERROR: <erase_block> offset: 0x%X FAILED!!!\n", offset);
            return -1;
        }
        offset += block_size_bytes;
    }
    erase_time_us = tick_elapesd(start_time_us, interface_get_tick_us());
    erase_time_s = erase_time_us / 1000000.0f;

    /*
     * verify erase
     */
    offset = 0;
    while (offset < chip_size_bytes) {
        memset(ctx->data_buf, !erase_value, CONFIG_DATA_BUF_SIZE);
        if (drv->read(offset, ctx->data_buf, CONFIG_DATA_BUF_SIZE)) {
            NORFLASH_LOG("[%s]: ERROR: <erase verify read api> offset: 0x%X FAILED!!!\n", ctx->name, offset);
            return -1;
        }

        for (i = 0; i < CONFIG_DATA_BUF_SIZE; i++) {
            if (ctx->data_buf[i] != erase_value) {
                NORFLASH_LOG("[%s]: ERROR: <ease verify data> offset: 0x%X FAILED!!!\n", ctx->name, offset);
                return -1;
            }
        }

        offset += CONFIG_DATA_BUF_SIZE;
    }

    NORFLASH_LOG("[%s]: [erase all blocks] PASSED\n", ctx->name);
    NORFLASH_LOG("[%s]: [erase all blocks] take time: %f seconds\n", ctx->name, erase_time_s);

    return 0;
}

int flashstress_write_read(struct flashstress_context *ctx, uint8_t verify_data)
{
    struct flashstress_driver *drv = &ctx->drv;
    uint32_t chip_size_bytes = drv->get_flash_chip_size_bytes();
    uint32_t block_size_bytes = drv->get_flash_block_size_bytes();
    uint32_t offset;
    uint32_t i;
    uint64_t read_time_us;
    uint64_t write_time_us;
    uint64_t start_time_us;
    float read_time_s;
    float read_speed;
    float write_time_s;
    float write_speed;
    
    uint32_t write_len = block_size_bytes;

    while (1) {
        if (write_len <= CONFIG_DATA_BUF_SIZE) {
            break;
        } else {
            write_len /= 2;
        }
    }

    if (write_len == 0) {
        NORFLASH_LOG("[%s]: ERROR: write len = %u!!!\n", ctx->name, write_len);
        return -1;
    }

    /*
     * write flash
     */
    memset(ctx->data_buf, verify_data, write_len);
    offset = 0;
    start_time_us = interface_get_tick_us();
    while (offset < chip_size_bytes) {
        if (drv->write(offset, ctx->data_buf, write_len)) {
            NORFLASH_LOG("[%s]: ERROR: <write> offset: 0x%X FAILED!!!\n", ctx->name, offset);
            return -1;
        }
        offset += write_len;
    }
    write_time_us = tick_elapesd(start_time_us, interface_get_tick_us());
    write_time_s = write_time_us / 1000000.0f;
    write_speed = (write_time_us == 0) ? -1.0 : (chip_size_bytes / write_time_s / 1024.0 / 1024.0);

    /*
     * verify flash data
     */
    offset = 0;
    while (offset < chip_size_bytes) {
        memset(ctx->data_buf, 0xff, CONFIG_DATA_BUF_SIZE);
        if (drv->read(offset, ctx->data_buf, CONFIG_DATA_BUF_SIZE)) {
            NORFLASH_LOG("[%s]: ERROR: <write verify read api> offset: 0x%X FAILED!!!\n", ctx->name, offset);
            return -1;
        }

        for (i = 0; i < CONFIG_DATA_BUF_SIZE; i++) {
            if (ctx->data_buf[i] != verify_data) {
                NORFLASH_LOG("[%s]: ERROR: <write verify data> offset: 0x%X FAILED!!!\n", ctx->name, offset);
                return -1;
            }
        }

        offset += CONFIG_DATA_BUF_SIZE;
    }

    NORFLASH_LOG("[%s]: [write] PASSED!!!\n", ctx->name);
    NORFLASH_LOG("[%s]: [write] take time: %f, speed: %f MiB/s\n", ctx->name, write_time_s, write_speed);

    /*
     * only read speed test, don't care data
     */
    offset = 0;
    start_time_us = interface_get_tick_us();
    while (offset < chip_size_bytes) {
        if (drv->read(offset, ctx->data_buf, CONFIG_DATA_BUF_SIZE)) {
            NORFLASH_LOG("[%s]: ERROR: <read speed read api> offset: 0x%X FAILED!!!\n", ctx->name, offset);
            return -1;
        }
        offset += CONFIG_DATA_BUF_SIZE;
    }
    read_time_us = tick_elapesd(start_time_us, interface_get_tick_us());
    read_time_s = read_time_us / 1000000.0f;
    read_speed = (read_time_us == 0) ? -1.0 : (chip_size_bytes / read_time_s / 1024.0 / 1024.0);

    /*
     * verify data
     */
    offset = 0;
    while (offset < chip_size_bytes) {
        memset(ctx->data_buf, 0xff, CONFIG_DATA_BUF_SIZE);
        if (drv->read(offset, ctx->data_buf, CONFIG_DATA_BUF_SIZE)) {
            NORFLASH_LOG("[%s]: ERROR: <read verify read api> offset: 0x%X FAILED!!!\n", ctx->name, offset);
            return -1;
        }

        for (i = 0; i < CONFIG_DATA_BUF_SIZE; i++) {
            if (ctx->data_buf[i] != verify_data) {
                NORFLASH_LOG("[%s]: ERROR: <read verify data> offset: 0x%X FAILED!!!\n", ctx->name, offset);
                return -1;
            }
        }

        offset += CONFIG_DATA_BUF_SIZE;
    }
    NORFLASH_LOG("[%s]: [read] PASSED!!!\n", ctx->name);
    NORFLASH_LOG("[%s]: [read] take time: %f, speed: %f MiB/s\n", ctx->name, read_time_s, read_speed);

    return 0;
}


int flashstress_run(struct flashstress_context *ctx)
{
    uint32_t chip_size_bytes = ctx->drv.get_flash_chip_size_bytes();
    uint32_t block_size_bytes = ctx->drv.get_flash_block_size_bytes();

    NORFLASH_LOG("\n=============================================\n");
    NORFLASH_LOG("[%s]: chip size: %u(Byte)=%d(KB)\n", ctx->name, chip_size_bytes, chip_size_bytes/1024);
    NORFLASH_LOG("[%s]: block size: %u(Byte)=%d(KB)\n", ctx->name, block_size_bytes, block_size_bytes/1024);
    NORFLASH_LOG("=============================================\n");

    if (flashstress_erase_chip(ctx) < 0) {
        goto ERROR;
    }

    if (flashstress_write_read(ctx, 0x55) < 0) {
        goto ERROR;
    }

    if (flashstress_erase_all_blocks(ctx) <0) {
        goto ERROR;
    }

    if (flashstress_write_read(ctx, 0xAA) < 0) {
        goto ERROR;
    }

    return 0;

ERROR:
    NORFLASH_LOG("[%s]: FAILED\n\n", ctx->name);
    return -1;
}
int norflash_init(void)
{
    xpi_nor_config_option_t option; // XPI NOR配置选项, 在用户手册的19.3.3.1有介绍
    const char *freq_str_list[] = {"Reserved", "30MHz", "50MHz", "66MHz", "80MHz", "100MHz", "120MHz", "133MHz", "166MHz"}; // XPI的频率配置, 在手册额19.8.2.1有介绍
    
    option.header.U = BOARD_APP_XPI_NOR_CFG_OPT_HDR;
    option.option0.U = BOARD_APP_XPI_NOR_CFG_OPT_OPT0;
    option.option1.U = BOARD_APP_XPI_NOR_CFG_OPT_OPT1;

    hpm_stat_t status = rom_xpi_nor_auto_config(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config, &option);
    if (status != status_success) {
        printf("Error: rom_xpi_nor_auto_config\n");
        while (1);
    }
    NORFLASH_LOG("Flash Size:%dMBytes\n"
               "Flash Sector Size:%dKBytes\n"
               "Flash Block Size:%dKBytes\n"
               "Flash Page Size:%dBytes\n",
               s_xpi_nor_config.device_info.size_in_kbytes / 1024U, 
               s_xpi_nor_config.device_info.sector_size_kbytes, 
               s_xpi_nor_config.device_info.block_size_kbytes, 
               s_xpi_nor_config.device_info.page_size);

    NORFLASH_LOG("XPI Frequency:%s\n", freq_str_list[option.option0.freq_opt]);
    return 0;
}

int norflash_read(uint32_t offset, void *buf, uint32_t size_bytes)
{
    hpm_stat_t status = rom_xpi_nor_read(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto,
                        &s_xpi_nor_config, (uint32_t *)buf, offset, size_bytes);
    if (status != status_success) {
        return -1;
    }

    return 0;
}

int norflash_read_mem(uint32_t offset, void *buf, uint32_t size_bytes)
{
    uint32_t flash_addr = 0x80000000 + offset;
    uint32_t aligned_start = HPM_L1C_CACHELINE_ALIGN_DOWN(flash_addr);
    uint32_t aligned_end = HPM_L1C_CACHELINE_ALIGN_UP(flash_addr + size_bytes);
    uint32_t aligned_size = aligned_end - aligned_start;

    l1c_dc_invalidate(aligned_start, aligned_size);

    memcpy(buf, (void *)flash_addr, size_bytes);
    return 0;
}

int norflash_write(uint32_t offset, const void *buf, uint32_t size_bytes)
{
    hpm_stat_t status = rom_xpi_nor_program(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto,
                                 &s_xpi_nor_config, (const uint32_t *)buf, offset, size_bytes);
    if (status != status_success) {
        return -1;
    }
    return 0;
}

int norflash_erase_chip(void)
{
    hpm_stat_t status = rom_xpi_nor_erase_chip(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config);
    if (status != status_success) {
        return -1;
    }
    return 0;
}

int norflash_erase_block(uint32_t offset)
{
    hpm_stat_t status = rom_xpi_nor_erase_block(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config, offset);
    if (status != status_success) {
        return -1;
    }
    return 0;
}

uint32_t norflash_get_chip_size(void)
{
    uint32_t flash_size;
    rom_xpi_nor_get_property(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config, 
            xpi_nor_property_total_size, &flash_size);
    return flash_size;
}

uint32_t norflash_get_block_size(void)
{
    uint32_t block_size;
    rom_xpi_nor_get_property(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config, 
            xpi_nor_property_block_size, &block_size);
    return block_size;
}

uint8_t norflash_get_erase_value(void)
{
    return 0xffu;
}

int norflash_test(void)
{
    struct flashstress_driver drv;
    struct flashstress_context *nor_ctx;

    disable_global_irq(CSR_MSTATUS_MIE_MASK);

    if (!l1c_dc_is_enabled()) {
        l1c_dc_enable();
        l1c_dc_invalidate_all();
    }
    __asm volatile("fence rw, rw");

    drv.get_flash_chip_size_bytes = norflash_get_chip_size;
    drv.get_flash_block_size_bytes = norflash_get_block_size;
    drv.get_flash_erase_value = norflash_get_erase_value;
    drv.erase_chip = norflash_erase_chip;
    drv.erase_block = norflash_erase_block;
    drv.read = norflash_read;
    drv.write = norflash_write;
    nor_ctx = flashstress_create(&drv, "read api(dcache enable)");
    if (flashstress_run(nor_ctx) < 0) {
        while (1);
    }
    flashstress_destroy(nor_ctx);


    drv.read = norflash_read_mem;
    nor_ctx = flashstress_create(&drv, "read mem(dcache enable)");
    if (flashstress_run(nor_ctx) < 0) {
        while (1);
    }
    flashstress_destroy(nor_ctx);

    if (l1c_dc_is_enabled()) {
        l1c_dc_writeback_all();
        __asm volatile("fence rw, rw");
        l1c_dc_disable();
    }
    __asm volatile("fence rw, rw");

    drv.read = norflash_read;
    nor_ctx = flashstress_create(&drv, "read api(dcache disable)");
    if (flashstress_run(nor_ctx) < 0) {
        while (1);
    }
    flashstress_destroy(nor_ctx);


    drv.read = norflash_read_mem;
    nor_ctx = flashstress_create(&drv, "read mem(dcache disable)");
    if (flashstress_run(nor_ctx) < 0) {
        while (1);
    }
    flashstress_destroy(nor_ctx);
    
    enable_global_irq(CSR_MSTATUS_MIE_MASK);

    printf("\n=============================================\n");
    printf("All cases are PASSED\n");
    printf("=============================================\n");
    return 0;
}

