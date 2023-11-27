#ifndef _BOARDID_H_
#define _BOARDID_H_
/*
 * 硬件机型定义
 */

#define R30S55_C_C1_6CH             1   //Seewo
#define R30S65_C_C1_9CH             2
#define R30S75_C_C1_9CH             3
#define R30S86_C_C1_9CH             4

#define R30S65_DH_C1_9CH            5   //大华
#define R30S86_DH_C1_9CH            6
/*
 * 扫描方案
 */
#define SCAN_SOLUTION_9CH_R30S     1    //65~86寸扫描方案
#define SCAN_SOLUTION_6CH_R30S     2    //55寸扫描方案

//被动压感参数方案
#define ALG_PRESSURE_PARA_DEFAULT               0
#define ALG_PRESSURE_PARA_R30S86_C1_9CH         1
#define ALG_PRESSURE_PARA_R30S75_C1_9CH         2
#define ALG_PRESSURE_PARA_R30S65_C1_9CH         3
#define ALG_PRESSURE_PARA_R30S55_C1_6CH         4

#endif //_BOARDID_H_

