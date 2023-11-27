#ifndef _CUSTOMER_SEEWO_H_
#define _CUSTOMER_SEEWO_H_

#define MODEL_R30S55N_C_C1_50P_6CH_UR   1
#define MODEL_R30S65N_C_C1_50P_9CH_UR   2
#define MODEL_R30S75N_C_C1_50P_9CH_UR   3
#define MODEL_R30S86N_C_C1_50P_9CH_UR   4



#define PRODUCT_ID    MODEL_R30S86N_C_C1_50P_9CH_UR

//字符描述部分，对外发布时候需要修改SVN号
#define USB_DESC4_SVN_ID            "1430"
#define USB_DESC5_COMPILE_TIME      "20231109-213808"

#define TOUCH_COORD_WIDTH_EN         1                       //USB数据传输使能(02开头)
#define TUIO_COORD_WIDTH_EN          0                       //TUIO数据传输使能(自定义的数据包，fb开头)
#define UART_COORD_WIDTH_EN          1                       //串口宽度数据使能
#define TUIO_COORD_WIDTH_TYPE        1                       //TYPE为1时，坐标加1增加(5.13mm / 32)
#define UART_SAME_PACKET_NO_SEND_EN  1                       //是否传输相同包的数据

/*******重要*******
*TOUCH_REPORT_VERSION 与 USB0_DEVICE_CONFIG_TYPE是有关联的, 更改其中任意一个需要注意另外一个是否匹配
*TOUCH_REPORT_VERSION: V0:仅有0x02报告, V1:支持0x02/0x32报告, V2:同时支持0x02和0x32报告
*USB0_DEVICE_CONFIG_TYPE选择了单Hid, 则TOUCH_REPORT_VERSION必须选择V0
*USB0_DEVICE_CONFIG_TYPE选择了DualHid, 则TOUCH_REPORT_VERSION必须选择V1
*USB0_DEVICE_CONFIG_TYPE选择了ThreeHid,则TOUCH_REPORT_VERSION必须选择V2
*/
#define TOUCH_REPORT_VERSION         1                      //, 选择V2需要设置USB Type为Three Hid
#define USB0_DEVICE_CONFIG_TYPE      eUsbCfgStandardInkWithBulk  //选择USB的传输方式(ThreelHid Mouse Ink 带虚拟键盘)
#define USB1_DEVICE_CONFIG_TYPE      eUsbCfgStandard              //选择USB的传输方式(中断、带虚拟键盘、批量)

#define DRAW_EDGE_PULL_OVER          (1)                     //快速往边沿画线拉边处理
#define USB0_DEFAULT_TOUCH_FMT       (eFormatMultiTouch)     //seewo 客户默认发多点
#define PHYSICAL_SIZE_SCALE          (0.5f)
#define FOR_CLICK_MIN_AREA           (0)                     //针对所有直径的触摸物做右键
#define USE_CUSTOMER_CHECKHEIGHT     (1)                     //使用自定义的触摸高度优化参数
#define USE_CUSTOMER_CLICK_EVENT     (1)                     //使用自定义的点击事件
#define ANTI_ALIASING_SMOOTH         (1)                     //开启抗锯齿平滑
#define ANTI_GLASS_CURVE             (1)                     //抗玻璃形变优化
#define MAX_OBJ_POOL                 (20)                    //最大支持20点颜色识别
#define FOR_BBOARD_REFLECTTION       (1)                     //黑板遮挡触摸框写字优化
#define HUGE_OBJ_ONLY                (0)                     //仅限单个大物体出点
#define PALM_REJECTION_ENABLE        (1)                     //防掌触功能: Seewo默认关闭防掌触
#define PALM_REJECTION_VER           (4)                     //防掌触功能版本: Seewo使用V4
#define PALM_ERASER_FUNC_TYPE        (1)                     //屏蔽防掌触手掌报点(若开启防掌触,需要设置为1)
#define MULTI_PALM_PEN_ENABLE        (1)                     //开启擦写同步功能
#define FORCE_FILL_BLIND             (1)                     //盲区内强制补点
#define MAX_UPDATE_SMOOTH_TICK       (16)                    //开启抗锯齿平滑的基础上强制更新平滑的最低时间要求
#define USE_HOLTW_SMOOTHNESS         (1)                    //默认开启Holt-Winter平滑
#define MAX_ACC_DIST                 (6)                    //加速超前距离倍数(实际使用结果为MAX_ACC_DIST/10，最小设置为10)
#define DRAW_IN_INCLINE              (1)                     //细笔倾斜快速划线画线
#define SMO_INCLINE_DRAWING          (0)                     //倾斜画线加强平滑
#define USB_EUM_SCAN_STOP_EN         (1)                     //USB枚举过程中,停止扫描
//TODO 暂时改为2Pen,改为5Pen为导致在E14电脑上硬插拔USB枚举失败，系统卡顿，待解决
#define MAX_INK_PENS                 (2)                     //若开启INK协议 最大支持pen数量


#if (PRODUCT_ID == MODEL_R30S86N_C_C1_50P_9CH_UR)
#define BOARD_TYPE                  R30S86_C_C1_9CH
#define MANTIS_ID                   "1316"
#define USER_POINT                  50
#define MODEL_DESC                  "R30S86N-C-C1-UR-50P"
#define USB_PRODUCT_STRING          "Touch Device"

#define REOX1                       0x21D
#define REOX2                       0xDED
#define REOY1                       0x22F
#define REOY2                       0xDD8
#define ROTATION_DIRECTION          0x0

#define SCAN_SOLUTION               SCAN_SOLUTION_9CH_R30S

//#define USB0_PID                    0x0F27 //SR-UR方案，MAC下支持多点需要用此PID
#define Firmware_Version            0x6700

#define UART_COORD_EN               (1)      //串口数据传输使能
#define SYSTEM_SUPPORT_INK          (SYSTEM_SUPPORT_INK_V2)
#define GLASS_SENSITIVE_V1_ENABLE   (1)      //针对玻璃超敏感形变的优化
#define PEN_TYPE_RECOGNIZATION_SVM  (1)
#define EDGE_H_W_AREA_ZOOM          (1)
#define TOUCH_COORD_WIDTH_TYPE      3        //以0.1mm为单位上报物理值
#define UART_COORD_WIDTH_TYPE       3        //以0.1mm为单位上报物理值
#define RECOGNISE_OBJ_DEFAULT_GROUP (eObjGpSeewo3_7_11)        //笔径识别组合定义(颜色识别)
#define DEFAULT_PENSIZE_TYP         (eObjPenType_SW13F)        //大小笔参数=RECOGNISE_OBJ_DEFAULT_GROUP一一匹配
#define ALG_PRESSURE_PARA_ID        ALG_PRESSURE_PARA_R30S86_C1_9CH
#define MIN_LINE_NUM_PROCESS_REQ    (0)      //单轴的遮挡光线小于此数量的不处理
#define CHECK_TOUCH_HEIGHT_MM       (2502)   //笔径
#define FORCE_FILL_PEN_WRITE_SMALL  (1)      //细笔写字面积偏小强制补点
#define USE_LOW_HEIGHT_PT_CNT       (1)      //低触默认支持点数
#define INCLINE_H_W_AREA_ZOOM       (1)      //倾斜宽高面积缩放处理
#define ALL_LINE_FILTER_ENABLE      (1)      //开启所有光线使能过滤
#if (TOUCH_REPORT_VERSION == 2)
#define TOUCH_V2_REPORT_TYPE_EN     (1)     //开启Report ID 0x32
#else
#define TOUCH_V2_REPORT_TYPE_EN     (0)     //开启Report ID 0x32
#endif
#define UART_TOUCH_V2_REPORT_TYPE_EN (0)    //串口使用新协议报点


#else
#error "no define ptoduct id"
#endif

#ifndef SCAN_SOLUTION
#define SCAN_SOLUTION               SCAN_SOLUTION_9CH_R30S
#endif

#ifndef MANTIS_ID
#define MANTIS_ID                   "949"
#endif

#ifndef CHECK_TOUCH_HEIGHT_MM
#define CHECK_TOUCH_HEIGHT_MM         (30)    //笔径
#endif

#ifndef DEFAULT_SMOOTH_EXP_FAC
#define DEFAULT_SMOOTH_EXP_FAC        (0.80f) ///默认指数平滑参数
#endif

#ifndef DEFAULT_SMOOTH_EXP_WRITE_FAC
#define DEFAULT_SMOOTH_EXP_WRITE_FAC  (0.65f) //默认写字指数平滑参数
#endif

#ifndef TOUCH_COORD_WIDTH_TYPE
#define TOUCH_COORD_WIDTH_TYPE        (0)      //TYPE为0时，坐标加1增加(0A区的大小(mm) * 10 / 32767)
#endif

#ifndef RECOGNISE_OBJ_DEFAULT_GROUP
#define RECOGNISE_OBJ_DEFAULT_GROUP  (eObjGpSeewo3_7_11)        //笔径识别组合定义(颜色识别)
#endif

#ifndef DEFAULT_PENSIZE_TYP
#define DEFAULT_PENSIZE_TYP          (eObjPenType_SW13F)     //大小笔参数=RECOGNISE_OBJ_DEFAULT_GROUP一一匹配
#endif

#ifndef UART_COORD_EN
#define UART_COORD_EN                (0)                       //串口数据传输使能
#endif

#ifndef TOUCH_V2_REPORT_TYPE_EN
#define TOUCH_V2_REPORT_TYPE_EN      (0)   //默认关闭板擦旋转和压感信息上报，report id 0x02
#endif

#if (TOUCH_V2_REPORT_TYPE_EN)
#ifndef ALG_SINOGRAM_FUNC_ENABLE
#define ALG_SINOGRAM_FUNC_ENABLE     (0)   //默认关闭旋转角度识别代码
#endif

#ifndef TOUCH_PRESSURE_LEVELS
#define TOUCH_PRESSURE_LEVELS        (0)
#endif
#else
#ifdef ALG_SINOGRAM_FUNC_ENABLE
#undef ALG_SINOGRAM_FUNC_ENABLE
#endif
#define ALG_SINOGRAM_FUNC_ENABLE     (0)   //关闭旋转角度识别代码

#ifdef TOUCH_PRESSURE_LEVELS
#undef TOUCH_PRESSURE_LEVELS
#endif
#define TOUCH_PRESSURE_LEVELS        (0)   //0代表不开启压感
#endif

#endif//_CUSTOMER_SEEWO_H_


