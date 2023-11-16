#include <string.h>
#include <stdio.h>
#include "board.h"
#include "hpm_debug_console.h"
#include "hpm_l1c_drv.h"
#include "hpm_romapi.h"
#include "hpm_clock_drv.h"
#include "hpm_mchtmr_drv.h"
#include "Api_Flash.h"
#include "Api_Math.h"
#include "Api_Crc.h"


#define FLASH_LOG(...)            printf(__VA_ARGS__)

static xpi_nor_config_t s_xpi_nor_config;

hpm_stat_t FlashRead(uint32_t addr, void *buf, uint32_t size_bytes)
{
    hpm_stat_t status = rom_xpi_nor_read(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto,
                        &s_xpi_nor_config, (uint32_t *)buf, addr, size_bytes);
    return status;
}

hpm_stat_t FlashWrite(uint32_t addr, const void *buf, uint32_t size_bytes)
{
    hpm_stat_t status = rom_xpi_nor_program(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto,
                                 &s_xpi_nor_config, (const uint32_t *)buf, addr, size_bytes);
    return status;
}

hpm_stat_t FlashErase(uint32_t addr, uint32_t size_bytes)
{
    hpm_stat_t status = rom_xpi_nor_erase(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config, addr, size_bytes);
    return status;
}


hpm_stat_t FlashEraseChip(void)
{
    hpm_stat_t status = rom_xpi_nor_erase_chip(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config);
    return status;
}

hpm_stat_t FlashEraseChipNonBlocking(void)
{
    hpm_stat_t status = rom_xpi_nor_erase_chip_nonblocking(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config);
    return status;
}


hpm_stat_t FlashEraseSector(uint32_t addr)
{
    hpm_stat_t status = rom_xpi_nor_erase_sector(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config, addr);
    return status;
}

hpm_stat_t FlashEraseSectorNonBlocking(uint32_t addr)
{
    hpm_stat_t status = rom_xpi_nor_erase_sector_nonblocking(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config, addr);
    return status;
}

hpm_stat_t FlashEraseBlock(uint32_t addr)
{
    hpm_stat_t status = rom_xpi_nor_erase_block(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config, addr);
    return status;
}

hpm_stat_t FlashEraseBlockNonBlocking(uint32_t addr)
{
    hpm_stat_t status = rom_xpi_nor_erase_block_nonblocking(BOARD_APP_XPI_NOR_XPI_BASE, xpi_xfer_channel_auto, &s_xpi_nor_config, addr);
    return status;
}


uint32_t FlashGetChipSize(void)
{
    uint32_t flash_size;
    rom_xpi_nor_get_property(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config, 
            xpi_nor_property_total_size, &flash_size);
    return flash_size;
}

uint32_t FlashGetPageSize(void)
{
    uint32_t page_size;
    rom_xpi_nor_get_property(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config,
            xpi_nor_property_page_size, &page_size);
    return page_size;
}

uint32_t FlashGetSectorSize(void)
{
    uint32_t sector_size;
    rom_xpi_nor_get_property(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config,
            xpi_nor_property_sector_size, &sector_size);
    return sector_size;
}


uint32_t FlashGetBlockSize(void)
{
    uint32_t block_size;
    rom_xpi_nor_get_property(BOARD_APP_XPI_NOR_XPI_BASE, &s_xpi_nor_config, 
            xpi_nor_property_block_size, &block_size);
    return block_size;
}


int FlashInit(void)
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
    FLASH_LOG("Flash Size:%dMBytes-%dKBytes\n"
               "Flash Sector Size:%dKBytes-%dKBytes\n"
               "Flash Block Size:%dKBytes-%dKBytes\n"
               "Flash Page Size:%dBytes-%dBytes\n",
               s_xpi_nor_config.device_info.size_in_kbytes / 1024U, FlashGetChipSize()/1024, 
               s_xpi_nor_config.device_info.sector_size_kbytes, FlashGetSectorSize()/1024, 
               s_xpi_nor_config.device_info.block_size_kbytes, FlashGetBlockSize()/1024, 
               s_xpi_nor_config.device_info.page_size, FlashGetPageSize());

    FLASH_LOG("XPI Frequency:%s\n", freq_str_list[option.option0.freq_opt]);
    return 0;
}


static void PrintBootPara(stStartBootPara_t *pPara)
{
    uint32_t i;
    printf("*********boot para***************\n");
    printf("nVid:%04X\n", pPara->nVid);
    printf("nPid:%04X\n", pPara->nPid);
    printf("nBootVersion:%04X\n", pPara->nBootVersion);
    printf("nDefultWaitTime:%d\n", pPara->nDefultWaitTime);
    printf("stBootFlashPara.nFirewareLen:%08X\n", pPara->stBootFlashPara.nFirewareLen);
    printf("stBootFlashPara.nFirewareCrc:%08X\n", pPara->stBootFlashPara.nFirewareCrc);

    printf("stBootFlashPara.nKey:[");
    for(i = 0;i<32;i++)
    {
        printf("%02X ", pPara->stBootFlashPara.nKey[i]);
    }
    printf("]\n");

    printf("eUpgradeFlag:%02X\n", pPara->eUpgradeFlag);
    printf("ModeDesc %s\n", pPara->cModeDesc);
    printf("nCheckSum:%08X\n", pPara->nCheckSum);

    printf("*********************************\n");
}



int32_t GetBootPara(stStartBootPara_t *pPara)
{
    int32_t nLen = FN_MIN(sizeof(stStartBootPara_t), INFO_LEN);
    FlashRead(APPLICATION_ADDRESS + INFO_OFFSET, (uint32_t *)pPara, nLen);
    return nLen;
}

/*
 * 初始化boot启动参数
 */
int32_t BootParaInit(stStartBootPara_t *pDefault)
{
    int32_t nRet = -1;
    stStartBootPara_t stPara;
    GetBootPara(&stPara);

    //若校验不对，则恢复默认值
    if (stPara.nCheckSum !=
            (uint32_t)GetCrc32_SW((uint8_t *)&stPara, sizeof(stStartBootPara_t)-4))
    {
        printf("Reseting boot parameter...\r\n");
        PrintBootPara(pDefault);
    }
    else
    {
        *pDefault = stPara;
        nRet = 0;
    }
    return nRet;
}


eAppUpgradeFlag_t GetUpgradeFlag(void)
{
	stStartBootPara_t stPara;
    GetBootPara(&stPara);
    return stPara.eUpgradeFlag;
}


uint32_t GetFwCheckSum(void)
{
	uint32_t nLen = APPLICATION_MAX_LEN;
    uint8_t* pApp = (uint8_t*)APPLICATION_ADDRESS;
    int32_t nChecksum32 = 0;
    int32_t i = 0;
    stStartBootPara_t stPara;
    
    GetBootPara(&stPara);
    nLen = FN_MIN(stPara.stBootFlashPara.nFirewareLen, nLen);
    
    for(i = 0; i < nLen; i++)
    {
        if((i >= INFO_OFFSET)&&(i < (INFO_OFFSET + INFO_LEN)))
        {
            pApp++;
            continue;
        }
        nChecksum32 += (*(pApp++));
    }

    return (~nChecksum32 + 1) & 0xFFFF;
}


uint32_t SaveDataList(eDataList_t eType, uint8_t *pBuf, uint32_t nLen)
{
    hpm_stat_t nStatus = status_success;
    uint32_t nEraseLen = 0;
    uint32_t nEraseAddr = 0;

    if (eType == ePersistentData)
    {
        nEraseAddr = FLASH_PERSISTENT_DATA_ADDRESS;
        nEraseLen = FLASH_PERSISTENT_DATA_LENGTH;
    }
    else if (eType == eUsbDescData)
    {
        nEraseAddr = FLASH_USB_DESC_ADDRESS;
        nEraseLen = FLASH_USB_DESC_LENGTH;
    }
    else if (eType == eZwParaData)
    {
        nEraseAddr = FLASH_ZW_PARA_ADDRESS;
        nEraseLen = FLASH_ZW_PARA_LENGTH;
    }
    else if (eType == eFwTableData)
    {
        nEraseAddr = FLASH_FW_TABLE_ADDRESS;
        nEraseLen = FLASH_FW_TABLE_LENGTH;
    }
    else if (eType == eCustomerPrivateData)
    {
        nEraseAddr = FLASH_CUSTOMR_PRIVATE_ADDRESS;
        nEraseLen = FLASH_CUSTOMR_PRIVATE_LENGTH;
    }
    if (nEraseLen > 0)
    {
        if(!(status_success == FlashErase(nEraseAddr, nEraseLen) &&
                status_success == FlashWrite(nEraseAddr, pBuf, nLen)))
        {
            nStatus = status_fail;
        }
    }
    return nStatus;
}


