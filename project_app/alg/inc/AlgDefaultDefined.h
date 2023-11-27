#ifndef _ALG_DEFAULT_DEFINED_
#define _ALG_DEFAULT_DEFINED_

#include "GlobalDefaultDefine.h"  //此处优先使用外部用户自定义的宏参数

#ifndef NULL
#define NULL                                0
#endif

#ifndef OBQ_COUNT
#define OBQ_COUNT                           3
#endif

#ifndef MAX_STRING_LENGTH
#define MAX_STRING_LENGTH                   100
#endif

#ifndef ALG_VER_SFX
#define ALG_VER_SFX                         0x0001
#endif

#ifndef DRV_VER_SFX
#define DRV_VER_SFX                         "0x0001"
#endif

#ifndef FW_VER_SFX
#define FW_VER_SFX                          0x2021
#endif

#ifndef MAX_POINT_SFX
#define MAX_POINT_SFX                       (MAX_POINT)
#endif

#ifndef MAX_LOGICAL_VALUE_SFX
#define MAX_LOGICAL_VALUE_SFX               0x7FFF
#endif

#ifndef LOGICAL_VALUE_PER_MM_SFX
#define LOGICAL_VALUE_PER_MM_SFX()          (g_stCfgData.fLogicalValuePerMM)
#endif

#ifndef SMOOTH_EN_SFX
#define SMOOTH_EN_SFX                       (1)
#endif

#ifndef MAX_KEEP_TOUCH_FOR_DECON
#define MAX_KEEP_TOUCH_FOR_DECON            (15 * 1000)
#endif

#ifndef MAX_KEEP_UNTOUCH_FOR_DECON
#define MAX_KEEP_UNTOUCH_FOR_DECON          (3 * 1000)
#endif

#ifndef MAX_KEEP_UNTOUCH_MASKING_FOR_DECON
#define MAX_KEEP_UNTOUCH_MASKING_FOR_DECON (2000)
#endif

#ifndef TOUCH_MASKING_LINES
#define TOUCH_MASKING_LINES                (128)
#endif

#ifndef CHECK_TOUCH_HEIGHT_EN
#define CHECK_TOUCH_HEIGHT_EN               (1)    //触摸高度优化使能
#endif

#ifndef USE_CUSTOMER_CHECKHEIGHT
#define USE_CUSTOMER_CHECKHEIGHT            (0)    //是否使用客户特定的触摸高度优化功能
#endif

#ifndef USE_CUSTOMER_RECOGNISE
#define USE_CUSTOMER_RECOGNISE              (0)
#endif

#ifndef BRUSH_DRAW_OBJ_CONST
#define BRUSH_DRAW_OBJ_CONST                (0)    //排笔不进行颜色识别 固定粗笔输出
#endif

#ifndef FINGER_DRAW_OBJ_CONST
#define FINGER_DRAW_OBJ_CONST               (0)    //手指不进行颜色识别 固定粗笔输出
#endif

#ifndef SINGLE_POINT_GLASS_CURVE_OPT
#define SINGLE_POINT_GLASS_CURVE_OPT        (0)    //单点在玻璃较软的情况下画线断线优化
#endif

#ifndef EDGE_MASK_CNT_DIR_HEAD_X
#define EDGE_MASK_CNT_DIR_HEAD_X            (0)    //X 轴首灯去污 LED 数量
#endif

#ifndef EDGE_MASK_CNT_DIR_TAIL_X
#define EDGE_MASK_CNT_DIR_TAIL_X            (0)    //X 轴尾灯去污 LED 数量
#endif

#ifndef EDGE_MASK_CNT_DIR_HEAD_Y
#define EDGE_MASK_CNT_DIR_HEAD_Y            (0)    //Y 轴首灯去污 LED 数量
#endif

#ifndef EDGE_MASK_CNT_DIR_TAIL_Y
#define EDGE_MASK_CNT_DIR_TAIL_Y            (0)    //Y 轴尾灯去污 LED 数量
#endif

//边缘 LED 去污处理
#define IS_EDGE_LED_LINE_MASK_PRO()         (EDGE_MASK_CNT_DIR_HEAD_X > 0 || EDGE_MASK_CNT_DIR_TAIL_X > 0 ||\
                                               EDGE_MASK_CNT_DIR_HEAD_Y > 0 || EDGE_MASK_CNT_DIR_TAIL_Y > 0)
#ifndef DRAW_WITH_MASKING_VER
#define DRAW_WITH_MASKING_VER               (1)    //单点算法补点版本
#endif

#ifndef SINGLE_DIR_PRE_FIX
#define SINGLE_DIR_PRE_FIX                  (1)    //只挡到单个灯处理
#endif

#ifndef  PEN_TYPE_RECOGNIZATION_SVM
#define  PEN_TYPE_RECOGNIZATION_SVM         (0)     // 默认颜色识别不使用SVM算法
#endif

#ifndef FINGERS_10P_EDGE_OPT
#define FINGERS_10P_EDGE_OPT                (0)     //十指边沿画线优化
#endif

#ifndef ONLY_LOCK_PEN_DOWN_PT
#define ONLY_LOCK_PEN_DOWN_PT               (0)     //颜色识别只锁定 down 点
#endif

#ifndef NO_ONLY_LOCK_PEN_HW
#define NO_ONLY_LOCK_PEN_HW                 (1)     //锁定颜色识别的宽高
#endif

#ifndef EDGE_H_W_AREA_ZOOM
#define EDGE_H_W_AREA_ZOOM                  (0)     //边缘宽高面积缩放处理
#endif

#ifndef INCLINE_H_W_AREA_ZOOM
#define INCLINE_H_W_AREA_ZOOM               (0)     //倾斜宽高面积缩放处理
#endif

#ifndef PEN_SLOPE_WRITE_OPTIMIZE
#define PEN_SLOPE_WRITE_OPTIMIZE            (0)     //3mm倾斜丢笔写字优化
#endif

#ifndef BIG_OBJ_BLACK_ERASE
#define BIG_OBJ_BLACK_ERASE                 (0)     //大物体黑边板擦优化
#endif

#ifndef TOUCH_PRESSURE_LEVELS
#define TOUCH_PRESSURE_LEVELS               (0)
#endif

#ifndef TOUCH_PRESSURE_RWOFFSET
#define TOUCH_PRESSURE_RWOFFSET             (0)
#endif

#ifndef SMOOTH_ACC_NUM
#define SMOOTH_ACC_NUM                      (1)     //加速点数
#endif

#ifndef MAX_ACC_DIST
#define MAX_ACC_DIST                        (50)    //加速超前距离倍数(实际使用结果为MAX_ACC_DIST/10，最小设置为10)
#endif

#ifndef MIN_PRESSURE_VALUE
#define MIN_PRESSURE_VALUE                  (0)
#endif

#ifndef PRESSURE_VALUE_MUL
#define PRESSURE_VALUE_MUL                  (1.0f)  //压感信号变化量倍数
#endif

#ifndef ALL_LINE_FILTER_ENABLE
#define ALL_LINE_FILTER_ENABLE              (0)     //默认关闭所有光线使能过滤
#endif

#ifndef PALM_WRITE_REGULAR_OBJ_TYPE
#define PALM_WRITE_REGULAR_OBJ_TYPE         (0)     //掌触书写固定笔径(1:细笔 2:大笔)
#endif

#ifndef DRAW_IN_BLACK_NONE_FILL_CNT
#define DRAW_IN_BLACK_NONE_FILL_CNT         (20)
#endif

#ifndef USE_HOLTW_SMOOTHNESS
#define USE_HOLTW_SMOOTHNESS                (0)     //HOLTW平滑算法
#endif

#endif //_ALG_DEFAULT_DEFINED_

