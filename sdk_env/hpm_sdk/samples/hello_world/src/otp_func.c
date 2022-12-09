#include <stdio.h>
#include "otp_func.h"
#include "hpm_otp_drv.h"
#include "hpm_romapi.h"

//#define READ_OPT_VALUE            otp_read_from_shadow

// 使用rom API读取OPT数据
#define READ_OPT_VALUE              ROM_API_TABLE_ROOT->otp_driver_if->read_from_shadow  


#define OTP_CHIP_UID_IDX_START      (8U)
#define OTP_CHIP_UID_IDX_END        (11U)

#define OTP_CHIP_UUID_IDX_START     (88U)
#define OTP_CHIP_UUID_IDX_END       (91U)

void OtpValuePrint(void)
{
    printf("\r\n******************************************\r\n");
    printf("\t\tAll OTP value\r\n");
    printf("word[0], HARD_LOCK=0x%08X\r\n", READ_OPT_VALUE(0));
    
    printf("word[1], LIFECYCLE_B=%d\r\n", (READ_OPT_VALUE(1)>>28)&& 0xF);
    printf("word[1], TCU_DISABLE=%d\r\n", (READ_OPT_VALUE(1)>>19)&& 0x1);
    printf("word[1], DEBUG_DISABLE=%d\r\n", (READ_OPT_VALUE(1)>>17)&& 0x1);
    printf("word[1], JTAG_DISABLE=%d\r\n", (READ_OPT_VALUE(1)>>16)&& 0x1);
    printf("word[1], PUK_REVOKE=%d\r\n", (READ_OPT_VALUE(1)>>8)&& 0xFF);
    printf("word[1], LIFECYCLE_A=%d\r\n", READ_OPT_VALUE(1)&& 0xF);

    printf("word[2], MONO_EPOCH=%d\r\n", (READ_OPT_VALUE(2)>>16)&& 0xFFFF);
    printf("word[2], EXIP1_RESTRICT=%d\r\n", (READ_OPT_VALUE(2)>>1)&& 0x1);
    printf("word[2], EXIP0_RESTRICT=%d\r\n", READ_OPT_VALUE(2)&& 0x1);

    printf("word[3], SW_VER=0x%08X\r\n", READ_OPT_VALUE(3));

    printf("word[8], DIE_TRACE=[0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", READ_OPT_VALUE(8), READ_OPT_VALUE(9), READ_OPT_VALUE(10), READ_OPT_VALUE(11));
    printf("word[12], DEBUG_KEY=[0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", READ_OPT_VALUE(12), READ_OPT_VALUE(13), READ_OPT_VALUE(14), READ_OPT_VALUE(15));

    printf("word[21], TSNS_BASE=%d\r\n", READ_OPT_VALUE(21)&& 0xFFFF);
    printf("word[21], TSNS_SLOPE=%d\r\n", (READ_OPT_VALUE(21)>> 16)&& 0xFFFF);

    printf("word[64], CHIP_ID=0x%08X\r\n", READ_OPT_VALUE(64));

    printf("word[65], MAC0=0x%08X\r\n", READ_OPT_VALUE(65));
    printf("word[66], MAC1=0x%08X\r\n", READ_OPT_VALUE(66));

    printf("word[67], USB_VID=%d\r\n", READ_OPT_VALUE(67)&& 0xFFFF);
    printf("word[67], USB_PID=%d\r\n", (READ_OPT_VALUE(67)>> 16)&& 0xFFFF);

    printf("word[80], PUBLIC_KEY_HASH=[0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", 
            READ_OPT_VALUE(80), READ_OPT_VALUE(81), READ_OPT_VALUE(82), READ_OPT_VALUE(83), 
            READ_OPT_VALUE(84), READ_OPT_VALUE(85), READ_OPT_VALUE(86), READ_OPT_VALUE(87));

    printf("word[88], UUID=[0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", READ_OPT_VALUE(88), READ_OPT_VALUE(89), READ_OPT_VALUE(90), READ_OPT_VALUE(91));

    printf("word[96], EXIP0_KEY=[0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", READ_OPT_VALUE(96), READ_OPT_VALUE(97), READ_OPT_VALUE(98), READ_OPT_VALUE(99)); 
    printf("word[104], EXIP1_KEY=[0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", READ_OPT_VALUE(104), READ_OPT_VALUE(105), READ_OPT_VALUE(106), READ_OPT_VALUE(107)); 

    printf("word[112], MASTER_KEY=[0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X]\r\n", 
            READ_OPT_VALUE(112), READ_OPT_VALUE(113), READ_OPT_VALUE(114), READ_OPT_VALUE(115), 
            READ_OPT_VALUE(116), READ_OPT_VALUE(117), READ_OPT_VALUE(118), READ_OPT_VALUE(119));
    printf("\r\n******************************************\r\n");
}

void ShowUuid(void)
{
    uint32_t uuid_words[4];

    uint32_t word_idx = 0;
    for (uint32_t i = OTP_CHIP_UUID_IDX_START; i <= OTP_CHIP_UUID_IDX_END; i++) {
        uuid_words[word_idx++] = ROM_API_TABLE_ROOT->otp_driver_if->read_from_shadow(i);
    }

    printf("UUID: ");
    uint8_t *uuid_bytes = (uint8_t *)uuid_words;
    for (uint32_t i = 0; i < sizeof(uuid_words); i++) {
        printf("%02x ", uuid_bytes[i]);
    }
    printf("\n");
}

void ShowUid(void)
{
    uint32_t uid_words[4];

    uint32_t word_idx = 0;
    for (uint32_t i = OTP_CHIP_UID_IDX_START; i <= OTP_CHIP_UID_IDX_END; i++) {
        uid_words[word_idx++] = ROM_API_TABLE_ROOT->otp_driver_if->read_from_shadow(i);
    }

    printf("UID:  ");
    uint8_t *uid_bytes = (uint8_t *)uid_words;
    for (uint32_t i = 0; i < sizeof(uid_words); i++) {
        printf("%02x ", uid_bytes[i]);
    }
    printf("\n");
}