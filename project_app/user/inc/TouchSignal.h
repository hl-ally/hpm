#ifndef _TOUCH_SIGNAL_H_
#define _TOUCH_SIGNAL_H_
#include <stdint.h>
#include "GlobalVariables.h"

#define TOUCH_DATA_MASK                     0x80
/****Modified by Roc*****
TOUCH_NULL_REPORT_ID            ------>>>>>>    Send ADC 8bit-SA Value Directly
TOUCH_LINE_STATU_REPORT_ID      ------>>>>>>    Send all lights status info
TOUCH_LINE_VALUE_REPORT_ID  
TOUCH_LINE_VALUE_COMPRESS_ID
TOUCH_LINE_STATU_COMPRESS_ID    ------>>>>>>    Send block percentage and unblock light with compressed data
*********/
#define TOUCH_NULL_REPORT_ID                0x00
#define TOUCH_LINE_STATU_REPORT_ID          0x01
#define TOUCH_LINE_VALUE_REPORT_ID          0x02
#define TOUCH_LINE_VALUE_COMPRESS_ID        0x03
#define TOUCH_LINE_STATU_COMPRESS_ID        0x04

#define TOUCH_NULL_REPORT_MASK_ID           (TOUCH_NULL_REPORT_ID | TOUCH_DATA_MASK)
#define TOUCH_LINE_STATU_REPORT_MASK_ID     (TOUCH_LINE_STATU_REPORT_ID | TOUCH_DATA_MASK)
#define TOUCH_LINE_VALUE_REPORT_MASK_ID     (TOUCH_LINE_VALUE_REPORT_ID | TOUCH_DATA_MASK)
#define TOUCH_LINE_VALUE_COMPRESS_MASK_ID   (TOUCH_LINE_VALUE_COMPRESS_ID | TOUCH_DATA_MASK)
#define TOUCH_LINE_STATU_COMPRESS_MASK_ID   (TOUCH_LINE_STATU_COMPRESS_ID | TOUCH_DATA_MASK)

#define MAX_LINE_COUNT                      20000   //Max lines

//每帧数据的信息
typedef struct
{
    uint8_t *pData;      //数据
    int32_t  nLen;       //长度
    uint8_t  eDataType;  //数据类型
    int32_t  nTimestamp; //时间戳
} stETouchData_t;

extern stTONIdxInfo_t* g_arrIndexSum;
extern uint16_t* g_nAllAdcIndex;
extern uint16_t* g_nAllAdcIndexIndex;
extern uint16_t  g_nAllAdcIndexIndexCount;
extern uint16_t* g_nLineLedRelationArray ;
extern uint16_t* g_arrAdcIndexOfBlock;
extern uint16_t g_nAllAdcIndexCount;
extern uint16_t g_nBlockCount;
extern uint16_t g_nBlockCountAxis[2];
extern uint16_t g_nLedPositioCount;
extern uint16_t g_nLineLedRelationCount;

extern uint8_t g_bSendTouchData;
extern uint8_t g_eTouchData;
extern volatile int32_t g_bDebuging;
extern volatile int32_t g_bRecvQTData;
extern volatile int32_t g_bRunAlg;

int32_t SendNullData(uint8_t nFrame,uint8_t nReportID,adc_type_t * pAdc);
int32_t SendLineValuesArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc);
int32_t SendLineStatusArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc);
int32_t SendCompressLineStatusArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc);
int32_t SendCompressLineValuesArray(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc);
int16_t USB_SendCprsQueue(void);
int32_t CompressLineStatus(uint8_t nFrame, uint8_t nReportID, adc_type_t* pAdc);

extern int32_t SendTouchData(uint8_t eType, adc_type_t* pAdc);
extern int32_t GetTouchData(stETouchData_t *pETouchData);

#endif//_TOUCH_SIGNAL_H_

