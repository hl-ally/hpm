/*
 * Evaluation.h
 *
 *  Created on: 2018年12月11日
 *      Author: cvter
 */

#ifndef _EVALUATION_H_
#define _EVALUATION_H_

#include "stdio.h"
#include "GlobalDataStructure.h"
//#include "API_Timer.h"
#include "Api_UsbDevice.h"
#include "Api_Crc.h"
#include "TouchSignal.h"
#include "GlobalVariables.h"
#include "AlgApiMath.h"


#define CMD_F0_REPORT_ID                0xF0 //PC与下位机间的调试通道ID
#define CMD_F0_STOP_SLAVE_SCAN          0x00 //停止从机扫描
#define CMD_F0_START_EVALUATION         0x01 //开始数据仿真
#define CMD_F0_SET_FRAME_NO             0x02 //设置帧号
#define CMD_F0_SET_RUN_ALG              0x03 //设置是否下位机同步跑算法
#define CMD_F0_SET_USB_DEBUG_OUT        0x04 //设置由USB输出调试信息
#define CMD_F0_SET_USB_COORD_EN         0x05 //设置是否使能USB坐标发送
#define CMD_F0_CONFIG_DATA              0x06 //传输配置数据
#define CMD_F0_SYNC_SIGNAL              0x07 //同步信号
#define CMD_F0_PLAYBACK_DATA_EN         0x08 //使能上传回放数据
#define CMD_F0_SYNC_PLAYBACK_DATA       0x09 //同步回放数据
#define CMD_F0_RESET_ALG_DATA           0x0A //清除算法数据
#define CMD_F0_START_UPLOAD             0x0B //开始数据传输
#define CMD_F0_GET_MASK_DATA            0x0C //获取背景数据

#define DEFAULT_FRAME_TIMESTAMP         8000
#define TRACE(format,...)               do {\
                                            printf(format, ##__VA_ARGS__);\
                                        }while(0)
#define CAT(format,...)                 do {\
                                            printf(format, ##__VA_ARGS__);\
                                        }while(0)
#ifndef USE_ALG_EMULATOR
#define USE_ALG_EMULATOR                (0)
#endif

#ifndef UPLOAD_QT_FASETR
#define UPLOAD_QT_FASETR                (0)  //加速上传数据，将上传数据代码放置快速区
#endif

#ifndef USE_USB_DEBUG_INFO
#define USE_USB_DEBUG_INFO              (0)  //使用usb bulk接口发送调试信息 类似printf
#endif

#if USE_USB_DEBUG_INFO
#define uprintf(format,...) \
{\
    extern volatile uint8_t g_bUsbDebug;\
    extern volatile uint8_t g_nUsbPrintReportId;\
    extern eUsbDevice_t g_eUsbDevice;\
    if (g_bUsbDebug != 0)\
    {\
        char arrTmp[64] = {g_nUsbPrintReportId};\
        sprintf(&arrTmp[1], ""format"", ##__VA_ARGS__);\
        extern int16_t USBSendPacket(eUsbDevice_t eDev, eUsbCfgBitType_t eBitType, uint8_t *pBuffer, int16_t nLen);\
        USBSendPacket(g_eUsbDevice, eUsbCfgTouch0CommBit, (uint8_t *)arrTmp, 64);\
    }\
}
#else
#define uprintf(format,...)
#endif

extern volatile uint8_t g_nUsbPrintReportId;
extern volatile int32_t g_nCurFrame;
extern volatile int32_t g_bGetMaskData;
extern eUsbDevice_t g_eUsbDevice;
extern eUsbCfgBitType_t g_eUsbSendCfgType;
extern uint8_t g_nEvalReportId;
extern int32_t EmulatorInit(void);
extern int32_t Evaluation(uint8_t* pBuf, int16_t nLen, eCmdSource_t eMode);

#endif /* _EVALUATION_H_ */
