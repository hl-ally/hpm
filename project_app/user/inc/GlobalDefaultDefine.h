#ifndef _GLOBALDEFAULTDEFINE_H_
#define _GLOBALDEFAULTDEFINE_H_

#ifndef USER_POINT
#define USER_POINT              20
#endif

#ifndef USB0_DEVICE_CONFIG_TYPE
#define USB0_DEVICE_CONFIG_TYPE  eUsbCfgStandardWithDualHid
#endif

#ifndef MAX_INK_PENS
#define MAX_INK_PENS            (2)   //若开启INK协议 最大支持pen数量
#endif

#ifndef SYSTEM_SUPPORT_INK
#define SYSTEM_SUPPORT_INK      (SYSTEM_SUPPORT_INK_V1) //默认系统支持的Ink是版本1
#endif

#ifndef MOUSE_INTERFACE_STANDALONE
#define MOUSE_INTERFACE_STANDALONE   (1)    //鼠标独立接口
#endif


#ifndef CUSTOMER_REPORT_ID
#define CUSTOMER_REPORT_ID      (FW_IN_REPORT_ID)   //默认使用0xFC包头
#endif

#ifndef TOUCH_V2_REPORT_TYPE_EN
#define TOUCH_V2_REPORT_TYPE_EN         0   //默认关闭板擦旋转和压感信息上报
#endif

#ifndef TOUCH_PRESSURE_LEVELS
#define TOUCH_PRESSURE_LEVELS           0
#endif

#ifndef USB0_VID
#define USB0_VID                0X1FF7
#endif

#ifndef USB0_PID
#define USB0_PID                0x0F32
#endif


#endif//_GLOBALDEFAULTDEFINE_H_


