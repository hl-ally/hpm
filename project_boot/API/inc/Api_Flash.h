#ifndef API_FLASH_H
#define API_FLASH_H
#include <stdint.h>


//boot跳转行为定义
typedef enum
{
    eAppTimeoutRunMode = 0,     //powerup->timeout->app
    eAppRunMode = 0xcc,         //powerup->app
    eAppIAPMode = 0xff,         //powerup->boot

    eReverse = 0xFFFFFF,        // 确保枚举占4字节
} eAppUpgradeFlag_t;

typedef struct
{
    uint32_t nFirewareLen;             
    uint32_t nFirewareCrc;
    uint8_t  nKey[32];                   
} BootFlashPara;

typedef struct
{
    uint16_t nVid;             //boot Vid
    uint16_t nPid;             //boot Pid
    uint16_t nBootVersion;     //boot版本号
    uint16_t nBoardType;       //硬件单板号
    uint16_t nDefultWaitTime;  //boot默认跳转时间
    uint16_t nbake;
    BootFlashPara stBootFlashPara;
    eAppUpgradeFlag_t eUpgradeFlag;//升级标志位
    char     cModeDesc[32];
    uint32_t nReserved[2];
    uint32_t nCheckSum;
} stStartBootPara_t;



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


#endif //API_FLASH_H

