#include "Api_Crc.h"

volatile static int32_t sg_bHwCrcBusy = 0;

//硬件CRC
uint32_t GetHwCRC32(uint8_t *pBuf, uint32_t nLen)
{
    sg_bHwCrcBusy = 1;
    //crc_data_register_reset();
    //uint32_t nCalCRC = crc_block_data_calculate((uint32_t *)pBuf, nLen/4);
    uint32_t nCalCRC = 0;
    sg_bHwCrcBusy = 0;
    return nCalCRC;
}

//硬件/软件CRC
uint32_t GetCRC32(uint8_t *pBuf, uint32_t nLen)
{
    if (!sg_bHwCrcBusy)
    {
        return GetHwCRC32(pBuf, nLen);
    }
    unsigned int i, j; // byte counter, bit counter
    unsigned int byte;
    unsigned int poly = 0x04C11DB7;
    unsigned int crc = 0xFFFFFFFF;

    unsigned int *nData = (unsigned int *)pBuf;
    unsigned int nDataLen = nLen / 4;

    for (i = 0; i < nDataLen; i++)
    {
        byte = nData[i];
        for (j = 0; j < 32; j++)
        {
            if ((int)(crc ^ byte) < 0)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc = crc << 1;
            }
            byte = byte << 1;    // Ready next msg bit.
        }
    }
    return crc;
}

uint32_t CalCRC32(uint8_t *pBuf, uint32_t nLen, uint32_t crc)
{
    uint32_t poly = 0x04C11DB7;
    uint32_t i, j; // byte counter, bit counter
    uint32_t byte;

    uint32_t *nData = (uint32_t *)pBuf;
    uint32_t nDataLen = nLen/4;

    for (i = 0; i < nDataLen; i++)
    {
        byte = nData[i];
        for (j = 0; j < 32; j++)
        {
            if ((int)(crc ^ byte) < 0)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc = crc << 1;
            }
            byte = byte << 1;     
        }
    }
    
    return crc;
}


uint32_t GetDataCRC32(uint8_t *pBuf, uint32_t Length, uint32_t nCrcData, uint32_t nBoardType)
{
    unsigned int i = 0, j = 0; // byte counter, bit counter
    unsigned int byte = 0;
    unsigned int poly = 0x04C11DB7;
    unsigned int crc = 0xFFFFFFFF;
    unsigned int *message = (unsigned int *)pBuf;
    unsigned int msgsize = Length / 4;

    byte = nCrcData | nBoardType;
    for (j = 0; j < 32; j++)
    {
        if ((int)(crc ^ byte) < 0)
        {
            crc = (crc << 1) ^ poly;
        }
        else
        {
            crc = crc << 1;
        }
        byte = byte << 1;    // Ready next msg bit.
    }

    for (i = 0; i < msgsize; i++)
    {
        byte = message[i];
        for (j = 0; j < 32; j++)
        {
            if ((int)(crc ^ byte) < 0)
            {
                crc = (crc << 1) ^ poly;
            }
            else
            {
                crc = crc << 1;
            }
            byte = byte << 1;    // Ready next msg bit.
        }
    }

    byte = 0x67e67e00 | nBoardType;
    for (j = 0; j < 32; j++)
    {
        if ((int)(crc ^ byte) < 0)
        {
            crc = (crc << 1) ^ poly;
        }
        else
        {
            crc = crc << 1;
        }
        byte = byte << 1;    // Ready next msg bit.
    }
    return crc;
}

uint32_t GetCheckSum(uint8_t *pBuf, uint32_t nLen)
{
    uint32_t nSum = 0;
    uint32_t i = 0;
    for (i = 0; i < nLen; i++)
    {
        nSum += pBuf[i];
    }
    return nSum;
}

static uint32_t Crc32_SW(uint32_t *message, uint32_t nmsgsize, uint32_t nCrc)
{
    uint32_t i, j;
    uint32_t nByte;
    uint32_t poly = 0x04C11DB7;

    for (i = 0; i < nmsgsize; i++)
    {
        nByte = message[i];
        for (j = 0; j < 32; j++)
        {
            if ((int)(nCrc ^ nByte) < 0)
            {
                nCrc = (nCrc << 1) ^ poly;
            }
            else
            {
                nCrc = nCrc << 1;
            }
            nByte = nByte << 1;
        }
    }

    return nCrc;
}

uint32_t GetCrc32_SW(uint8_t *Buffer, uint32_t nLength)
{
    return Crc32_SW((uint32_t *)Buffer, nLength/4, 0xFFFFFFFF);
}

uint32_t GetDataCrc32_SW(uint8_t *Buffer, uint32_t nLength, uint32_t nCrcData, uint32_t nBoardType)
{
    uint32_t *nStart   = (uint32_t *)(Buffer);
    uint32_t  nData    = nCrcData| nBoardType;
    uint32_t  nCrcDataResult = 0;
    
     nCrcDataResult = Crc32_SW((uint32_t *)&nData, 1, 0xFFFFFFFF);
     nCrcDataResult = Crc32_SW((uint32_t *)nStart, nLength/4, nCrcDataResult);

     nData    = 0x12ABAB12| nBoardType;
     nCrcData = Crc32_SW((uint32_t *)&nData, 1, nCrcDataResult);
    
    return  nCrcData;
}


/*********************************************************************************************************
 * @Function : 对数据进行大小端模式转换
 * @Parameter: [srcArray] 待转换数据
             : [desLeData] 数据转换结果
 * @return   : 返回CRC的计算结果
*********************************************************************************************************/
int ConversBTS(uint32_t *srcArray,uint32_t *desLeData)
{
    uint8_t * pSrc = (uint8_t *)srcArray ; 
    *desLeData = (uint32_t)(pSrc[3]<<24) + (uint32_t)(pSrc[2]<<16) + 
                    (uint32_t)(pSrc[1]<<8) + (uint32_t)pSrc[0];
    return 1;
}

/*********************************************************************************************************
 * @Function : 对数据进行大小端模式转换
 * @Parameter: [srcArray] 待转换数据
             : [desLeData] 数据转换结果
 * @return   : 返回CRC的计算结果
*********************************************************************************************************/
int ConverseSTB(uint32_t *srcArray,uint32_t *desBeData)
{
    uint8_t * pSrc = (uint8_t *)srcArray ; 
       
    *desBeData = (uint32_t)(pSrc[0]<<24) + (uint32_t)(pSrc[1]<<16) + 
                    (uint32_t)(pSrc[2]<<8) + (uint32_t)pSrc[3];
    return 1;
}

/*******************************************************************************************************
 * @Function :通过软件进行大端模式CRC计算
 * @Parameter: [message] CRC计算的数据
             : [nmsgsize] CRC计算数据的长度
             : [nCrc] CRC计算的初始值
 * @return   : 返回CRC的计算结果
*********************************************************************************************************/
uint32_t Crc32_SW_BigEndian(uint32_t *message, uint32_t nmsgsize, uint32_t nCrc)
{
    uint32_t i, j; // byte counter, bit counter
    uint32_t _nByte;
    uint32_t nByte;
    uint32_t poly = 0x04C11DB7;

    for (i = 0; i < nmsgsize; i++)
    {
        _nByte = message[i];
        ConverseSTB(&_nByte,&nByte);
        for (j = 0; j < 32; j++)
        {
            if ((int)(nCrc ^ nByte) < 0)
            {
                nCrc = (nCrc << 1) ^ poly;
            }
            else
            {
                nCrc = nCrc << 1;
            }
            nByte = nByte << 1;
        }
    }
    
    return nCrc;
}

/*********************************************************************************************************
 * @Function :通过软件进行大端模式CRC计算
 * @Parameter: [message] CRC计算的数据
             : [nmsgsize] CRC计算数据的长度
             : [nCrc] CRC计算的初始值
 * @return   : 返回CRC的计算结果
*********************************************************************************************************/
uint32_t GetCrc32_SW_BigEndian(uint8_t *Buffer, uint32_t nLength)
{
    return Crc32_SW_BigEndian((uint32_t *)Buffer, nLength/4, 0xFFFFFFFF);
}


