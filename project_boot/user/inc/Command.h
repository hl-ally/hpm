#ifndef _COMMANDS_H_
#define _COMMANDS_H_


// Main Command
/*************查询Boot的版本信息**********************************/
#define CMD_VERSION                  0x01
#define CMD_VERSION_ACK              0x81
/*************查询Boot的版本信息，而且暂停到Boot中****************/
#define CMD_VERSION_PAUSE            0x02
#define CMD_VERSION_PAUSE_ACK        0x82
/*************使能升级，下发升级相关信息，包含固件的CRC,包数等****/
#define CMD_IAP_ENABLE               0x03
#define CMD_IAP_ENABLE_ACK           0x83
/*************下发新固件的CODE数据********************************/
#define CMD_DOWNLOAD                 0x04
#define CMD_DOWNLOAD_ACK             0x84
/*************固件Code数据下发完成，升级结束**********************/
#define CMD_FINISH                   0x05
#define CMD_FINISH_ACK               0x85
/*************读取FLASH指定地址，指定长度的数据长度**********************/
#define CMD_ReadFlash                0x06
#define CMD_ReadFlash_ACK            0x86
/*************读取FLASH全片数据**********************/
#define CMD_READFLASH_ALL            0x07
#define CMD_READFLASH_ALL_ACK        0x87
/*************进入管理员权限**********************/
#define CMD_ADMIN_ACCESS             0x08
#define CMD_ADMIN_ACCESS_ACK         0x88
/*************Boot跳转到APP的命令*********************************/
#define CMD_RUN                      0x24
#define CMD_RUN_ACK                  0xA4

//非主命令
#define CMD_ACK                      0xFF
#define CMD_NACK                     0xFE

#define CMD_OK_ACK                   0x00
#define CMD_ERROR_ACK                0x01


#define NACK_FLASH_ERASE             0x00
#define NACK_FLASH_WRITRE            0x01
#define NACK_PACKET_NO               0x02
#define NACK_CHECKSUM                0x03
#define NACK_ERROR_CMD               0x04
#define NACK_IAPMODE                 0x05

#define CMD_HEADER_LEN               5
#define DATA_LEN                     56

#endif

