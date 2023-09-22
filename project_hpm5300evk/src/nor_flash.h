#ifndef _NOR_FLASH_H
#define _NOR_FLASH_H

#include <stdint.h>

struct flashstress_driver {
    uint32_t (*get_flash_chip_size_bytes)(void);
    uint32_t (*get_flash_block_size_bytes)(void);
    uint8_t (*get_flash_erase_value)(void);
    int (*erase_chip)(void);
    int (*erase_block)(uint32_t offset);
    int (*read)(uint32_t offset, void *buf, uint32_t size_bytes);
    int (*write)(uint32_t offset, const void *buf, uint32_t size_bytes);
};

#define CONFIG_CONTEXT_NUM              1
#define CONFIG_DATA_BUF_SIZE            4096
#define CONFIG_NAME_LEN                 64
struct flashstress_context {
    int is_used;
    char name[CONFIG_NAME_LEN + 1];
    struct flashstress_driver drv;
    uint8_t data_buf[CONFIG_DATA_BUF_SIZE];
};


int norflash_init(void);
int norflash_test(void);

#endif