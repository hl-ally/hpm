#ifndef API_FLASH_H
#define API_FLASH_H
#include <stdint.h>
#include "hpm_common.h"


#define SIZE_256B                       ((uint32_t)0x000000FF)
#define SIZE_4KB                        ((uint32_t)0x00001000)
#define SIZE_64KB                       ((uint32_t)0x00010000)

#define FLASH_PAGE_SIZE                 SIZE_256B
#define FLASH_SECTOR_SIZE               SIZE_4KB
#define FLASH_BLOCK_SIZE                SIZE_64KB

#define FLASH_START_ADDRESS             ((uint32_t)0x80000000)
#define FLASH_CHIP_SIZE                 ((uint32_t)0x00100000)

#define BOOTLOADER_ADDRESS              (FLASH_START_ADDRESS)            //Boot的启动地址
#define BOOTLOADER_MAX_SIZE             (64 * 1024)                      // 32K

#define APPLICATION_ADDRESS             (FLASH_START_ADDRESS + SIZE_64KB)
#define APPLICATION_MAX_LEN             (256 * 1024)
//#define APPLICATION_MAX_LEN             (FLASH_CHIP_SIZE - BOOTLOADER_MAX_SIZE)

/*************************固件保存数据索引的表数据的相关地址和大小信息 ******************************/
#define FLASH_FW_TABLE_ADDRESS          (APPLICATION_ADDRESS + APPLICATION_MAX_LEN)
#define FLASH_FW_TABLE_LENGTH           (2 * 1024)

/*************************保存零等待区的剩余空间信息 占2k******************************/
#define FLASH_ZW_PARA_ADDRESS           (FLASH_FW_TABLE_ADDRESS + FLASH_FW_TABLE_LENGTH)
#define FLASH_ZW_PARA_LENGTH            (2 * 1024)

/*************************持久化数据 占2k******************************/
#define FLASH_PERSISTENT_DATA_ADDRESS   (FLASH_ZW_PARA_ADDRESS + FLASH_ZW_PARA_LENGTH)
#define FLASH_PERSISTENT_DATA_LENGTH    (2 * 1024)

/*************************USB相关描述符 占6k******************************/
#define FLASH_USB_DESC_ADDRESS          (FLASH_PERSISTENT_DATA_ADDRESS + FLASH_PERSISTENT_DATA_LENGTH)
#define FLASH_USB_DESC_LENGTH           (6 * 1024)

/*************************Customer私有信息 占2k******************************/
#define FLASH_CUSTOMR_PRIVATE_ADDRESS   (FLASH_USB_DESC_ADDRESS + FLASH_USB_DESC_LENGTH)
#define FLASH_CUSTOMR_PRIVATE_LENGTH    (2 * 1024)

/*************************后置扩展代码信息 占2k******************************/
#define FLASH_EXT_CODE_PARA_ADDRESS     (FLASH_CUSTOMR_PRIVATE_ADDRESS + FLASH_CUSTOMR_PRIVATE_LENGTH)
#define FLASH_EXT_CODE_PARA_LENGTH      (2 * 1024)


#define FLASH_BOOT_PARA_ADDRESS         (FLASH_EXT_CODE_PARA_ADDRESS + FLASH_EXT_CODE_PARA_LENGTH)
#define FLASH_BOOT_PARA_LENGTH          (0x100)




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

typedef enum
{
    ePersistentData = 0,
    eUsbDescData,
    eZwParaData,
    eFwTableData,
    eCustomerPrivateData,
    eDataListTotal
} eDataList_t;




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

int FlashInit(void);
extern hpm_stat_t FlashErase(uint32_t addr, uint32_t size_bytes);
extern hpm_stat_t FlashWrite(uint32_t addr, const void *buf, uint32_t size_bytes);

int32_t GetBootPara(stStartBootPara_t *pPara);
int32_t BootParaInit(stStartBootPara_t *pDefault);
eAppUpgradeFlag_t GetUpgradeFlag(void);
uint32_t GetFwCheckSum(void);
uint32_t GetBootDataCrc32(void);

extern uint32_t SaveDataList(eDataList_t eType, uint8_t *pBuf, uint32_t nLen);


extern void FlashTest(void);



#endif //API_FLASH_H

