#ifndef _API_CRC_
#define _API_CRC_
#include <stdint.h>

extern uint32_t GetCRC32(uint8_t *Buffer, uint32_t Length);
extern uint32_t CalCRC32(uint8_t *pBuf, uint32_t nLen, uint32_t crc);
extern uint32_t GetDataCRC32(uint8_t *Buffer, uint32_t Length, uint32_t nCrcData, uint32_t nBoardType);
extern uint32_t GetCheckSum(uint8_t *pBuf, uint32_t nLen);
extern uint32_t GetCrc32_SW(uint8_t *Buffer, uint32_t nLength);
extern uint32_t GetDataCrc32_SW(uint8_t *Buffer, uint32_t nLength, uint32_t nCrcData, uint32_t nBoardType);
extern void Crc32ResetDT(void);
extern int32_t Crc32CalcBlkCRC(uint32_t pBuffer[], uint32_t BufferLength);
extern uint32_t GetCrc32_SW_BigEndian(uint8_t *Buffer, uint32_t nLength);
extern uint32_t Crc32_SW_BigEndian(uint32_t *message, uint32_t nmsgsize, uint32_t nCrc);

#endif//_API_CRC_

