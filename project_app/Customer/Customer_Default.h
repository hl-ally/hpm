#ifndef _CUSTOMER_DEFAULT_H_
#define _CUSTOMER_DEFAULT_H_

#define MODEL_E30A65N_D_A1_20P_4CH      1
#define MODEL_E30A75N_D_A1_20P_4CH      2
#define MODEL_E30A86N_D_A1_20P_4CH      3

#define PRODUCT_ID                  MODEL_E30A75N_D_A1_20P_4CH

//字符描述部分，对外发布时候需要修改SVN号
#define USB_DESC4_SVN_ID            "589"
#define USB_DESC5_COMPILE_TIME      "20230411-105606"

#define TOUCH_COORD_WIDTH_EN         1                       //USB数据传输使能(02开头)
#define TUIO_COORD_WIDTH_EN          0                       //TUIO数据传输使能(自定义的数据包，fb开头)
#define UART_COORD_WIDTH_EN          1                       //串口宽度数据使能
#define TOUCH_COORD_WIDTH_TYPE       0                       //TYPE为0时，坐标加1增加(0A区的大小(mm) * 10 / 32767)
#define TUIO_COORD_WIDTH_TYPE        1                       //TYPE为1时，坐标加1增加(5.13mm / 32)
#define UART_COORD_WIDTH_TYPE        1

#define UART_SAME_PACKET_NO_SEND_EN  1                       //是否传输相同包的数据

#define USB0_DEVICE_CONFIG_TYPE      eUsbCfgStandardWithDualHid         //选择USB的传输方式(中断、带虚拟键盘)
#define USB1_DEVICE_CONFIG_TYPE      eUsbCfgStandard         //选择USB的传输方式(中断、带虚拟键盘、批量)

#define PHYSICAL_SIZE_SCALE          (0.5f)
#define FOR_CLICK_MIN_AREA           (0)            //针对所有直径的触摸物做右键
#define ANTI_ALIASING_SMOOTH         (1)            //开启抗锯齿平滑
#define ANTI_GLASS_CURVE             (1)            //抗玻璃形变优化
#define FOR_BBOARD_REFLECTTION       (1)            //黑板遮挡触摸框写字优化
#define ALG_SINOGRAM_FUNC_ENABLE     (1)            //开启旋转角度识别代码
#define MULTI_PALM_PEN_ENABLE        (1)            //开启擦写同步功能
#define FORCE_FILL_BLIND             (1)            //盲区内强制补点
#define TOUCH_FRAME_MS               (3)            //定制帧率 0代表不定制
#define TOUCH_FRAME_SPLIT            (3)            //插值次数

#define RECOGNISE_OBJ_DEFAULT_GROUP (eObjGpSeewo5_8)//笔径识别组合定义(颜色识别)

#if (PRODUCT_ID == MODEL_E30A65N_D_A1_20P_4CH)
#define BOARD_TYPE                  E30A65N_D_A1_4CH
#define MANTIS_ID                   "1008"
#define USER_POINT                  40
#define MODEL_DESC                  "E30A65N-D-A1-40P"

#define REOX1                       0x227
#define REOX2                       0xdf6
#define REOY1                       0x255
#define REOY2                       0xdfd
#define ROTATION_DIRECTION          0x0

#define TOUCH_V2_REPORT_TYPE_EN     (0)      //开启Repotd Id 0x32, 包含板擦旋转和压感信息上报
#define ALG_SINOGRAM_FUNC_ENABLE    (1)      //开启旋转角度识别代码
#define TOUCH_PRESSURE_LEVELS       1024     //0代表不开启压感
#define ALG_PRESSURE_PARA_ID        ALG_PRESSURE_PARA_E30A86_A1_4CH_F

#define Firmware_Version            0x6200
#elif (PRODUCT_ID == MODEL_E30A75N_D_A1_20P_4CH)
#define BOARD_TYPE                  E30A75N_D_A1_4CH
#define MANTIS_ID                   "1009"
#define USER_POINT                  20
#define MODEL_DESC                  "E30A75N-D-A1-20P"

#define REOX1                       0x224
#define REOX2                       0xdf3
#define REOY1                       0x232
#define REOY2                       0xdd1
#define ROTATION_DIRECTION          0x0

#define TOUCH_V2_REPORT_TYPE_EN     (0)      //开启Repotd Id 0x32, 包含板擦旋转和压感信息上报
#define ALG_SINOGRAM_FUNC_ENABLE    (1)      //开启旋转角度识别代码
#define TOUCH_PRESSURE_LEVELS       1024     //0代表不开启压感
#define ALG_PRESSURE_PARA_ID        ALG_PRESSURE_PARA_E30A86_A1_4CH_F

#define Firmware_Version            0x6200
#elif (PRODUCT_ID == MODEL_E30A86N_D_A1_20P_4CH)
#define BOARD_TYPE                  E30A86N_D_A1_4CH
#define MANTIS_ID                   "1010"
#define USER_POINT                  20
#define MODEL_DESC                  "E30A86N-D-A1-20P"

#define REOX1                       0x224
#define REOX2                       0xdf3
#define REOY1                       0x232
#define REOY2                       0xdd1
#define ROTATION_DIRECTION          0x0

#define Firmware_Version            0x6200
#else
#error "no define model id"
#endif

#ifndef MANTIS_ID
#define MANTIS_ID                   "1008"
#endif

#ifndef SCAN_SOLUTION
#define SCAN_SOLUTION               SCAN_SOLUTION_4CH_E30A
#endif

#ifndef USE_LOW_HEIGHT_PT_CNT
#define USE_LOW_HEIGHT_PT_CNT       (1)      //低触默认支持点数
#endif

#endif //_CUSTOMER_DEFAULT_H_

