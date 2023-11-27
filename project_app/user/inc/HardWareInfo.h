#ifndef _HARDWARE_INFO_H_
#define _HARDWARE_INFO_H_

#include "AlgCustomer.h"

#include "GlobalDataStructure.h"


#if (BOARD_TYPE == R30S86_C_C1_9CH)
#define OAWIDTH                         1924
#define OAHEIGHT                        1098

#define MAX_RATE_X                      (0.900f)//40     theta/2
#define MAX_RATE_Y                      (0.466f)//22     theta/2

#define XT_BOARD_1                      36
#define XT_BOARD_2                      30
#define XT_BOARD_3                      30
#define XT_BOARD_4                      16

#define XR_BOARD_1                      36
#define XR_BOARD_2                      30
#define XR_BOARD_3                      30
#define XR_BOARD_4                      16

#define YT_BOARD_1                      42
#define YT_BOARD_2                      43

#define YR_BOARD_1                      42
#define YR_BOARD_2                      43

#define TOTAL_X                         112
#define TOTAL_Y                         85

#define TOP_LED_POS                     {{   6.78, 1107.00},{  10.78, 1107.00},{  14.78, 1107.00},{  19.78, 1107.00},\
                                         {  25.78, 1107.00},{  32.78, 1107.00},{  40.78, 1107.00},{  49.78, 1107.00},\
                                         {  59.78, 1107.00},{  70.78, 1107.00},{  81.78, 1107.00},{  93.78, 1107.00},\
                                         { 106.98, 1107.00},{ 120.78, 1107.00},{ 135.78, 1107.00},{ 152.08, 1107.00},\
                                         { 168.68, 1107.00},{ 187.08, 1107.00},{ 204.98, 1107.00},{ 223.88, 1107.00},\
                                         { 242.68, 1107.00},{ 261.98, 1107.00},{ 281.68, 1107.00},{ 301.88, 1107.00},\
                                         { 322.08, 1107.00},{ 343.08, 1107.00},{ 364.18, 1107.00},{ 385.18, 1107.00},\
                                         { 407.48, 1107.00},{ 429.38, 1107.00},{ 451.18, 1107.00},{ 474.38, 1107.00},\
                                         { 497.48, 1107.00},{ 521.48, 1107.00},{ 545.48, 1107.00},{ 568.58, 1107.00},\
                                         { 593.42, 1107.00},{ 619.29, 1107.00},{ 646.14, 1107.00},{ 662.70, 1107.00},\
                                         { 678.22, 1107.00},{ 694.57, 1107.00},{ 711.44, 1107.00},{ 729.24, 1107.00},\
                                         { 746.52, 1107.00},{ 765.15, 1107.00},{ 783.78, 1107.00},{ 803.65, 1107.00},\
                                         { 823.31, 1107.00},{ 844.21, 1107.00},{ 864.91, 1107.00},{ 886.54, 1107.00},\
                                         { 899.58, 1107.00},{ 922.24, 1107.00},{ 945.21, 1107.00},{ 968.70, 1107.00},\
                                         { 993.54, 1107.00},{1017.72, 1107.00},{1040.83, 1107.00},{1062.35, 1107.00},\
                                         {1078.87, 1107.00},{1095.22, 1107.00},{1111.57, 1107.00},{1129.26, 1107.00},\
                                         {1146.95, 1107.00},{1165.58, 1107.00},{1190.42, 1107.00},{1216.29, 1107.00},\
                                         {1243.14, 1107.00},{1259.70, 1107.00},{1275.22, 1107.00},{1291.57, 1107.00},\
                                         {1308.44, 1107.00},{1326.24, 1107.00},{1343.52, 1107.00},{1362.15, 1107.00},\
                                         {1380.78, 1107.00},{1400.65, 1107.00},{1420.31, 1107.00},{1441.21, 1107.00},\
                                         {1461.91, 1107.00},{1483.54, 1107.00},{1496.58, 1107.00},{1519.24, 1107.00},\
                                         {1542.21, 1107.00},{1565.70, 1107.00},{1590.54, 1107.00},{1614.72, 1107.00},\
                                         {1637.83, 1107.00},{1659.35, 1107.00},{1675.87, 1107.00},{1692.22, 1107.00},\
                                         {1708.57, 1107.00},{1726.26, 1107.00},{1743.95, 1107.00},{1762.58, 1107.00},\
                                         {1779.03, 1107.00},{1792.03, 1107.00},{1805.03, 1107.00},{1817.03, 1107.00},\
                                         {1830.03, 1107.00},{1842.03, 1107.00},{1853.03, 1107.00},{1864.03, 1107.00},\
                                         {1874.03, 1107.00},{1883.03, 1107.00},{1891.03, 1107.00},{1898.03, 1107.00},\
                                         {1904.03, 1107.00},{1909.03, 1107.00},{1913.03, 1107.00},{1917.03, 1107.00}}

#define BOTTOM_LED_POS                  {{   6.78,   -9.50},{  10.78,   -9.50},{  14.78,   -9.50},{  19.78,   -9.50},\
                                         {  25.78,   -9.50},{  32.78,   -9.50},{  40.78,   -9.50},{  49.78,   -9.50},\
                                         {  59.78,   -9.50},{  70.78,   -9.50},{  81.78,   -9.50},{  93.78,   -9.50},\
                                         { 106.98,   -9.50},{ 120.78,   -9.50},{ 135.78,   -9.50},{ 152.08,   -9.50},\
                                         { 168.68,   -9.50},{ 187.08,   -9.50},{ 204.98,   -9.50},{ 223.88,   -9.50},\
                                         { 242.68,   -9.50},{ 261.98,   -9.50},{ 281.68,   -9.50},{ 301.88,   -9.50},\
                                         { 322.08,   -9.50},{ 343.08,   -9.50},{ 364.18,   -9.50},{ 385.18,   -9.50},\
                                         { 407.48,   -9.50},{ 429.38,   -9.50},{ 451.18,   -9.50},{ 474.38,   -9.50},\
                                         { 497.48,   -9.50},{ 521.48,   -9.50},{ 545.48,   -9.50},{ 568.58,   -9.50},\
                                         { 590.34,   -9.50},{ 612.47,   -9.50},{ 635.43,   -9.50},{ 656.09,   -9.50},\
                                         { 669.95,   -9.50},{ 684.92,   -9.50},{ 701.03,   -9.50},{ 714.43,   -9.50},\
                                         { 731.36,   -9.50},{ 752.54,   -9.50},{ 775.63,   -9.50},{ 795.70,   -9.50},\
                                         { 815.65,   -9.50},{ 833.40,   -9.50},{ 852.04,   -9.50},{ 875.34,   -9.50},\
                                         { 898.08,   -9.50},{ 919.68,   -9.50},{ 939.36,   -9.50},{ 959.68,   -9.50},\
                                         { 983.27,   -9.50},{1002.60,   -9.50},{1025.04,   -9.50},{1046.23,   -9.50},\
                                         {1068.54,   -9.50},{1091.91,   -9.50},{1113.00,   -9.50},{1129.29,   -9.50},\
                                         {1150.25,   -9.50},{1164.58,   -9.50},{1187.34,   -9.50},{1209.47,   -9.50},\
                                         {1232.43,   -9.50},{1253.09,   -9.50},{1266.95,   -9.50},{1281.92,   -9.50},\
                                         {1298.03,   -9.50},{1311.43,   -9.50},{1328.36,   -9.50},{1349.54,   -9.50},\
                                         {1372.63,   -9.50},{1392.70,   -9.50},{1412.65,   -9.50},{1430.40,   -9.50},\
                                         {1449.04,   -9.50},{1472.34,   -9.50},{1495.08,   -9.50},{1516.68,   -9.50},\
                                         {1536.36,   -9.50},{1556.68,   -9.50},{1580.27,   -9.50},{1599.60,   -9.50},\
                                         {1622.04,   -9.50},{1643.23,   -9.50},{1665.54,   -9.50},{1688.91,   -9.50},\
                                         {1710.00,   -9.50},{1726.29,   -9.50},{1747.25,   -9.50},{1761.58,   -9.50},\
                                         {1777.03,   -9.50},{1792.03,   -9.50},{1806.03,   -9.50},{1819.03,   -9.50},\
                                         {1831.03,   -9.50},{1843.03,   -9.50},{1854.03,   -9.50},{1864.03,   -9.50},\
                                         {1874.03,   -9.50},{1883.03,   -9.50},{1891.03,   -9.50},{1898.03,   -9.50},\
                                         {1904.03,   -9.50},{1909.03,   -9.50},{1913.03,   -9.50},{1917.03,   -9.50}}

#define LEFT_LED_POS                    {{  -9.50,    9.81},{  -9.50,   13.81},{  -9.50,   17.81},{  -9.50,   23.81},\
                                         {  -9.50,   31.31},{  -9.50,   39.81},{  -9.50,   49.31},{  -9.50,   59.81},\
                                         {  -9.50,   70.81},{  -9.50,   82.81},{  -9.50,   95.50},{  -9.50,  107.49},\
                                         {  -9.50,  119.00},{  -9.50,  132.88},{  -9.50,  146.61},{  -9.50,  159.18},\
                                         {  -9.50,  172.79},{  -9.50,  186.43},{  -9.50,  200.87},{  -9.50,  214.71},\
                                         {  -9.50,  228.62},{  -9.50,  244.25},{  -9.50,  258.73},{  -9.50,  273.23},\
                                         {  -9.50,  287.87},{  -9.50,  302.19},{  -9.50,  317.83},{  -9.50,  333.05},\
                                         {  -9.50,  348.27},{  -9.50,  363.74},{  -9.50,  378.96},{  -9.50,  394.46},\
                                         {  -9.50,  410.38},{  -9.50,  426.33},{  -9.50,  442.34},{  -9.50,  457.17},\
                                         {  -9.50,  471.67},{  -9.50,  486.95},{  -9.50,  502.11},{  -9.50,  514.91},\
                                         {  -9.50,  526.81},{  -9.50,  539.61},{  -9.50,  555.58},{  -9.50,  571.59},\
                                         {  -9.50,  587.54},{  -9.50,  603.55},{  -9.50,  616.56},{  -9.50,  630.35},\
                                         {  -9.50,  642.79},{  -9.50,  656.79},{  -9.50,  672.07},{  -9.50,  687.52},\
                                         {  -9.50,  703.53},{  -9.50,  717.97},{  -9.50,  732.52},{  -9.50,  747.55},\
                                         {  -9.50,  761.87},{  -9.50,  775.93},{  -9.50,  788.29},{  -9.50,  800.04},\
                                         {  -9.50,  812.85},{  -9.50,  827.51},{  -9.50,  842.29},{  -9.50,  857.13},\
                                         {  -9.50,  871.20},{  -9.50,  885.43},{  -9.50,  899.04},{  -9.50,  912.70},\
                                         {  -9.50,  927.39},{  -9.50,  941.39},{  -9.50,  954.41},{  -9.50,  967.80},\
                                         {  -9.50,  980.50},{  -9.50,  993.50},{  -9.50, 1005.80},{  -9.50, 1017.50},\
                                         {  -9.50, 1029.00},{  -9.50, 1040.20},{  -9.50, 1050.70},{  -9.50, 1059.70},\
                                         {  -9.50, 1067.70},{  -9.50, 1074.70},{  -9.50, 1080.70},{  -9.50, 1085.70},\
                                         {  -9.50, 1089.70}}

#define RIGHT_LED_POS                   {{1933.40,    9.81},{1933.40,   13.81},{1933.40,   17.81},{1933.40,   23.81},\
                                         {1933.40,   31.31},{1933.40,   39.81},{1933.40,   49.31},{1933.40,   59.81},\
                                         {1933.40,   70.81},{1933.40,   82.81},{1933.40,   95.50},{1933.40,  107.49},\
                                         {1933.40,  119.00},{1933.40,  132.88},{1933.40,  146.61},{1933.40,  159.18},\
                                         {1933.40,  172.79},{1933.40,  186.43},{1933.40,  200.87},{1933.40,  214.71},\
                                         {1933.40,  228.62},{1933.40,  244.25},{1933.40,  258.73},{1933.40,  273.23},\
                                         {1933.40,  287.87},{1933.40,  302.19},{1933.40,  317.83},{1933.40,  333.05},\
                                         {1933.40,  348.27},{1933.40,  363.74},{1933.40,  378.96},{1933.40,  394.46},\
                                         {1933.40,  410.38},{1933.40,  426.33},{1933.40,  442.34},{1933.40,  457.17},\
                                         {1933.40,  471.67},{1933.40,  486.95},{1933.40,  502.11},{1933.40,  514.91},\
                                         {1933.40,  526.81},{1933.40,  539.61},{1933.40,  555.58},{1933.40,  571.59},\
                                         {1933.40,  587.54},{1933.40,  603.55},{1933.40,  616.56},{1933.40,  630.35},\
                                         {1933.40,  643.79},{1933.40,  657.79},{1933.40,  673.07},{1933.40,  688.52},\
                                         {1933.40,  704.53},{1933.40,  718.97},{1933.40,  733.52},{1933.40,  748.55},\
                                         {1933.40,  762.87},{1933.40,  776.93},{1933.40,  789.29},{1933.40,  801.74},\
                                         {1933.40,  814.55},{1933.40,  829.21},{1933.40,  843.99},{1933.40,  858.83},\
                                         {1933.40,  872.90},{1933.40,  887.13},{1933.40,  900.74},{1933.40,  914.40},\
                                         {1933.40,  929.09},{1933.40,  943.09},{1933.40,  956.11},{1933.40,  969.50},\
                                         {1933.40,  982.20},{1933.40,  994.30},{1933.40, 1006.30},{1933.40, 1018.00},\
                                         {1933.40, 1029.50},{1933.40, 1040.70},{1933.40, 1050.70},{1933.40, 1059.70},\
                                         {1933.40, 1067.70},{1933.40, 1074.70},{1933.40, 1080.70},{1933.40, 1085.70},\
                                         {1933.40, 1089.70}}

#else
#error Msg:"灯管数量定义失败:未找到匹配的尺寸型号"
#endif

#if (TOTAL_X > 0)
#define X_TOTAL                         TOTAL_X
#else
#error Msg:"X轴灯管数量定义失败"
#endif

#if (TOTAL_Y > 0)
#define Y_TOTAL                         TOTAL_Y
#else
#error Msg:"Y轴灯管数量定义失败"
#endif

#ifndef PHYSICAL_SIZE_X
#define PHYSICAL_SIZE_X                 (OAWIDTH*10)
#endif        
        
#ifndef PHYSICAL_SIZE_Y        
#define PHYSICAL_SIZE_Y                 (OAHEIGHT*10)
#endif        
        
#ifndef PHYSICAL_TOTAL_X        
#define PHYSICAL_TOTAL_X                (TOTAL_X)
#endif        
        
#ifndef PHYSICAL_TOTAL_Y        
#define PHYSICAL_TOTAL_Y                (TOTAL_Y)
#endif

#if (SCAN_SOLUTION == SCAN_SOLUTION_6CH_R30S)

/*******接收一组通道数*******/
#define R_ADC_GROUP                     (uint8_t)(6)
        
/*******接收一组通道数*******/        
#define R164_GROUP                      (uint8_t)(6)
        
/*******发射一组通道数*******/        
#define T164_GROUP                      (uint8_t)(5)

/*******ADC1通道数*********/
#define ADC1_CH_NUM                     (2)
/*******ADC2通道数*********/
#define ADC2_CH_NUM                     (2)
/*******ADC3通道数*********/
#define ADC3_CH_NUM                     (2)
/*******ADC4通道数*********/
#define ADC4_CH_NUM                     (0)

#elif (SCAN_SOLUTION == SCAN_SOLUTION_9CH_R30S)

/*******接收一组通道数*******/
#define R_ADC_GROUP                     (uint8_t)(9)
        
/*******接收一组通道数*******/        
#define R164_GROUP                      (uint8_t)(9)
        
/*******发射一组通道数*******/        
#define T164_GROUP                      (uint8_t)(6)

/*******ADC1通道数*********/
#define ADC1_CH_NUM                     (3)
/*******ADC2通道数*********/
#define ADC2_CH_NUM                     (3)
/*******ADC3通道数*********/
#define ADC3_CH_NUM                     (3)
/*******ADC4通道数*********/
#define ADC4_CH_NUM                     (0)
#else
#error "Miss Scansolution"
#endif

#ifndef THRESHOLD_RATE_LV
#define THRESHOLD_RATE_LV               69     //默认遮挡率
#endif        

#if (THRESHOLD_RATE_LV == 25)
#define THRESHOLD_RATE                  (0.25f)
#elif (THRESHOLD_RATE_LV == 38)
#define THRESHOLD_RATE                  (0.375f)
#elif (THRESHOLD_RATE_LV == 44)
#define THRESHOLD_RATE                  (0.44f)
#elif (THRESHOLD_RATE_LV == 50)
#define THRESHOLD_RATE                  (0.50f)
#elif (THRESHOLD_RATE_LV == 57)
#define THRESHOLD_RATE                  (0.57f)
#elif (THRESHOLD_RATE_LV == 60)
#define THRESHOLD_RATE                  (0.60f)
#elif (THRESHOLD_RATE_LV == 69)
#define THRESHOLD_RATE                  (0.69f)
#elif (THRESHOLD_RATE_LV == 75)
#define THRESHOLD_RATE                  (0.75f)
#elif (THRESHOLD_RATE_LV == 81)
#define THRESHOLD_RATE                  (0.8125f)
#elif (THRESHOLD_RATE_LV == 88)
#define THRESHOLD_RATE                  (0.875f)
#else
#error "missing threshold rate!"
#endif

//选择传输压感背景参数
#ifndef ALG_PRESSURE_PARA_ID
#define ALG_PRESSURE_PARA_ID            (ALG_PRESSURE_PARA_DEFAULT)
#endif

#define ADC_CH_TOTAL                    (ADC1_CH_NUM + ADC2_CH_NUM + ADC3_CH_NUM + ADC4_CH_NUM)

#define MIN_ADC_REQ                     70                   //55(MA Value)
#define NOR_ADC_UP_BOUND                (MIN_ADC_REQ * 5 / 4)    //commonly, ADC won't bigger than NOR_ADC_UP_BOUND
#define MAX_ADC_REQ                     (MIN_ADC_REQ * 2)    //
#define ADC_CHANGE_EACH_AGC             (MIN_ADC_REQ * 5 / 7)
#define ADC_SATURATED                   (0xFF * 7 / 8)       //if adc > 7/8 * maxADC

#define ADC_FLUCTUATION_VAL             (uint8_t)(12)//波动值
//#define ADC_CHANGE_EACH_AGC             30
#define ADC_UPPER_BOUND                 200

#define ERR_ADC                         13
#define FAIL_LED_ADC                    20        //Threshold in Failed LED check
#define ADC_DEEP_RATE                   (0.3f)    //deep blocked lights' rate
#define ADC_DEEP_BLOCKED                ((uint8_t)(MIN_ADC_REQ * ADC_DEEP_RATE))
#define ADC_UNSTABLE_VALUE              20        //used for determine adc vibrate
#define MAX_ADC_VARIATION_CNT           (4)
#define UNSTABLE_ADC_TOTAL_VALUE        (ADC_UNSTABLE_VALUE*MAX_ADC_VARIATION_CNT)
#define ADC_BLOCKED                     30        //遮挡的光线，ADC值不会小于120
#define ADC_WEAK_RATE                   (0.5)
#define ADC_WEAK_BLOCK                  ((uint8_t)(MIN_ADC_REQ * ADC_WEAK_RATE))

#define TEST_MAX_REVERSE_ADC            7       //反向值测试标准
#define TEST_MAX_UNT_ADC                15      //波动值标准
/************客户测试标准*******************/
#define TEST_MAX_ADC                    (255)
#define TEST_MIN_ADC                    (TEST_MAX_UNT_ADC * 2)
#define TEST_OBQ_MAX_ADC                (255)
#define TEST_OBQ_MIN_ADC                (TEST_MAX_UNT_ADC * 2)
#define TEST_MAX_AGC                    (1023)
#define TEST_MIN_AGC                    (0)
#define TEST_MAX_UNT_ADC_CLT            (31)    //UNT standard for Client

/************工厂测试标准*******************/
#define TEST_MAX_ADC_FAC                (255)
#define TEST_MIN_ADC_FAC                (TEST_MAX_UNT_ADC * 2 + 15)
#define TEST_OBQ_MAX_ADC_FAC            (255)
#define TEST_OBQ_MIN_ADC_FAC            (TEST_MAX_UNT_ADC * 2 + 15)
#define TEST_MAX_AGC_FAC                (1023)
#define TEST_MIN_AGC_FAC                0

/*---------五条档位线（内部-硬件提供）---------*/
#define RAGC_GEAR_IN_1             (42)
#define RAGC_GEAR_IN_2             (58)
#define RAGC_GEAR_IN_3             (73)
#define RAGC_GEAR_IN_4             (85)
#define RAGC_GEAR_IN_5             (93)

//55寸单独档位线
#define RAGC_GEAR_IN_1_55          (63)
#define RAGC_GEAR_IN_2_55          (79)
#define RAGC_GEAR_IN_3_55          (87)
#define RAGC_GEAR_IN_4_55          (98)
#define RAGC_GEAR_IN_5_55          (106)

/*--------五条档位线（外部-硬件提供）---------*/
#define RAGC_GEAR_EX_1             (50)
#define RAGC_GEAR_EX_2             (65)
#define RAGC_GEAR_EX_3             (82)
#define RAGC_GEAR_EX_4             (89)
#define RAGC_GEAR_EX_5             (97)

#define RAGC_GEAR_EX_1_55          (63)
#define RAGC_GEAR_EX_2_55          (79)
#define RAGC_GEAR_EX_3_55          (92)
#define RAGC_GEAR_EX_4_55          (103)
#define RAGC_GEAR_EX_5_55          (111)

//record the ADC channel in first ValidLine for each time T-On & total num ValidLine for each time T-On
typedef struct
{
    uint16_t nCH;
    uint16_t nIndex;
}stTONIdxInfo_t;

extern int32_t InitLedMaxInterval(void);
extern int32_t InitLedPosition(stPosF_t* stLedPosArray, int32_t nMaxCount, eEdge_t eEdge);
extern int32_t InitTestAdcIndex(uint16_t* nTestAdcIndex, int32_t nMaxCount, eAxis_t eAxis, int32_t eDir);
extern int32_t InitTestAgcIndex(uint16_t* nTestAgcIndex, int32_t nMaxCount, eAxis_t eAxis, int32_t eDir);
extern int32_t InitTestRIdIndex(uint16_t* nTestRIdIndex, int32_t nMaxCount, eAxis_t eAxis, int32_t eDir);
extern int32_t InitAllAdcIndex(uint16_t* nAdcIndexArray, int32_t nMaxCount, eEdge_t eEdge, int32_t nLedIndex);
extern int32_t InitTLedRLedRelation(uint16_t* nTRLedRelationIndex, int32_t nMaxCount, eEdge_t eEdge, int32_t nLedIndex);
extern int32_t InitRBlockAdcStartIndex(uint16_t* nRBlockStartIndexArray, int32_t nMaxCount, eEdge_t eEdge, int32_t nLedIndex);
extern int32_t InitTLedRLedLableCountSum(stTONIdxInfo_t* nIndexIndexArray, int32_t nMaxCount, eEdge_t eEdge, int32_t nLedIndex, int32_t nIndex);
extern int32_t InitScanBlockRange(stSgmH_t* stScanSegment, int32_t nMaxCount, eAxis_t eAxis);
extern int32_t InitBlockStartTLedTimes(uint16_t* nStartTimesArray, int32_t nMaxCount, eAxis_t eAxis);

#endif //_HARDWARE_INFO_H_


