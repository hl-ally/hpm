#ifndef __UPGRADE_H__
#define __UPGRADE_H__

#include <stdint.h>
#include "Api_Flash.h"

typedef enum _WORK_MODE_
{
    NORMAL_MODE,                //wait for a few seconds for update
    AP_MODE,                    //Restart and then goto scan program
    IAP_MODE,                   //flash download fireware
    NORMAL_N_SECONDS_AP_MODE    //
} WorkMode;

#define MERGE_BOOT_APP_FLAG 0


typedef  void  (*pAppFunction) (void);

extern volatile uint32_t    nFootPrintFlag;
extern volatile uint32_t    nIAPEnableFlag;
extern volatile uint8_t     nIsInIap;

extern volatile uint32_t    g_nUpdWaitTime;
extern volatile uint32_t    g_nUpgradeTime;

extern volatile eAppUpgradeFlag_t g_eAppUpgradeFlag;
extern stStartBootPara_t g_stBootPara;

extern void SetBootWorkMode(void);
extern uint32_t CheckFroApplication(void);
extern void RunApplication(void);
extern uint32_t GetBackupAppFLashAddress(void);
//extern fmc_state_enum ClearBackupAppFLash(void);
//extern fmc_state_enum EraseAppData(void);
//extern fmc_state_enum SaveAppFLash(uint8_t* pBuf, int32_t nLen, int32_t nOffset);
//extern fmc_state_enum SaveBackupAppFLash(void);
extern int8_t CopyBackupApp2MainApp(void);
extern uint32_t CheckForBackupApplication(void);

#endif

