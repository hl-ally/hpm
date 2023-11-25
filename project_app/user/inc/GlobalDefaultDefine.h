#ifndef _GLOBALDEFAULTDEFINE_H_
#define _GLOBALDEFAULTDEFINE_H_

#define OBQ_COUNT                           3

#ifndef USER_POINT
#define USER_POINT                          20
#endif

#ifndef LIGHT_WIDTH
#define LIGHT_WIDTH                          (1000)      //定义光线宽度，以um为单位
#endif

#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH                   100
#endif

#ifndef PHYSICAL_SIZE_X
#define PHYSICAL_SIZE_X                     0           // 单位0.1mm
#endif

#ifndef PHYSICAL_SIZE_Y
#define PHYSICAL_SIZE_Y                     0           // 单位0.1mm
#endif

#ifndef PHYSICAL_SIZE_SCALE
#define PHYSICAL_SIZE_SCALE                 (0.6f)  //默认缩放物理尺寸比例60%  解决win10 1809之前的非16:9分辨率的触控问题
#endif


#ifndef USB0_DEVICE_CONFIG_TYPE
#define USB0_DEVICE_CONFIG_TYPE             eUsbCfgStandardWithDualHid
#endif

#ifndef MAX_INK_PENS
#define MAX_INK_PENS                        (2)   //若开启INK协议 最大支持pen数量
#endif

#ifndef SYSTEM_SUPPORT_INK
#define SYSTEM_SUPPORT_INK                  (SYSTEM_SUPPORT_INK_V1) //默认系统支持的Ink是版本1
#endif

#ifndef MOUSE_INTERFACE_STANDALONE
#define MOUSE_INTERFACE_STANDALONE          (1)    //鼠标独立接口
#endif


#ifndef CUSTOMER_REPORT_ID
#define CUSTOMER_REPORT_ID                  (FW_IN_REPORT_ID)   //默认使用0xFC包头
#endif

#ifndef TOUCH_V2_REPORT_TYPE_EN
#define TOUCH_V2_REPORT_TYPE_EN             0   //默认关闭板擦旋转和压感信息上报
#endif

#ifndef TOUCH_PRESSURE_LEVELS
#define TOUCH_PRESSURE_LEVELS               0
#endif

#ifndef XT_BOARD_1
#define XT_BOARD_1                          0
#endif

#ifndef XT_BOARD_2
#define XT_BOARD_2                          0
#endif

#ifndef XT_BOARD_3
#define XT_BOARD_3                          0
#endif

#ifndef XT_BOARD_4
#define XT_BOARD_4                          0
#endif

#ifndef XT_BOARD_5
#define XT_BOARD_5                          0
#endif

#ifndef XT_BOARD_6
#define XT_BOARD_6                          0
#endif

#ifndef XT_BOARD_7
#define XT_BOARD_7                          0
#endif

#ifndef XT_BOARD_8
#define XT_BOARD_8                          0
#endif

#ifndef YT_BOARD_1
#define YT_BOARD_1                          0
#endif

#ifndef YT_BOARD_2
#define YT_BOARD_2                          0
#endif

#ifndef YT_BOARD_3
#define YT_BOARD_3                          0
#endif

#ifndef YT_BOARD_4
#define YT_BOARD_4                          0
#endif

#ifndef YT_BOARD_5
#define YT_BOARD_5                          0
#endif

#ifndef YT_BOARD_6
#define YT_BOARD_6                          0
#endif

#ifndef YT_BOARD_7
#define YT_BOARD_7                          0
#endif

#ifndef YT_BOARD_8
#define YT_BOARD_8                          0
#endif

#ifndef XR_BOARD_1
#define XR_BOARD_1                          0
#endif

#ifndef XR_BOARD_2
#define XR_BOARD_2                          0
#endif

#ifndef XR_BOARD_3
#define XR_BOARD_3                          0
#endif

#ifndef XR_BOARD_4
#define XR_BOARD_4                          0
#endif

#ifndef XR_BOARD_5
#define XR_BOARD_5                          0
#endif

#ifndef XR_BOARD_6
#define XR_BOARD_6                          0
#endif

#ifndef XR_BOARD_7
#define XR_BOARD_7                          0
#endif

#ifndef XR_BOARD_8
#define XR_BOARD_8                          0
#endif

#ifndef YR_BOARD_1
#define YR_BOARD_1                          0
#endif

#ifndef YR_BOARD_2
#define YR_BOARD_2                          0
#endif

#ifndef YR_BOARD_3
#define YR_BOARD_3                          0
#endif

#ifndef YR_BOARD_4
#define YR_BOARD_4                          0
#endif

#ifndef YR_BOARD_5
#define YR_BOARD_5                          0
#endif

#ifndef YR_BOARD_6
#define YR_BOARD_6                          0
#endif

#ifndef YR_BOARD_7
#define YR_BOARD_7                          0
#endif

#ifndef YR_BOARD_8
#define YR_BOARD_8                          0
#endif

#ifndef OAWIDTH
#define OAWIDTH                             0xFFF
#endif

#ifndef OAHEIGHT
#define OAHEIGHT                            0XFFF
#endif

#ifndef USB0_VID
#define USB0_VID                            0x1FF7
#endif

#ifndef USB0_PID
#define USB0_PID                            0x0F32
#endif

#ifndef USB1_VID
#define USB1_VID                            0x1FF7
#endif

#ifndef USB1_PID
#define USB1_PID                            0x0F33
#endif

#define USB_VID                             0x1ff7
#define USB_PID                             0x0f32

#define BOARD_DA                            0x01
#define BOARD_DB                            0x02
#define BOARD_DC                            0x03



#endif//_GLOBALDEFAULTDEFINE_H_


