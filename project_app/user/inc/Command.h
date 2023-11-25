#ifndef _COMMANDS_H_
#define _COMMANDS_H_


#define CAL                             0x01//旧校准相关父命令，整机安卓校准使用
//旧校准相关子命令  
#define CAL_BEGIN                       0x00//启动校准命令
#define CAL_STEP1                       0x01//step1
#define CAL_STEP2                       0x02//step2
#define CAL_STEP3                       0x03//step3
#define CAL_STEP4                       0x04//step4
#define CAL_CHECK                       0x05//校准确认
#define CAL_RESET                       0x06//校准复位
#define CAL_GET                         0x08
#define CAL_GET_RE                      0x88
#define CAL_END                         0xFF

#define SET_TIME                        0x02//设置右击、双击时间
#define GET_TIME                        0x03//读取设置的右击、双击时间、右击使能
#define SET_RB_EN                       0x04//右击功能使能开关
#define GET_CAL_COEFF                   0x05//not use
#define SET_ALL_TOUCH_ENABLE            0x06//通道坐标使能开关
#define GET_ALL_TOUCH_ENABLE            0x07//获取通道坐标使能状态
#define GET_MAXPOINTNUM                 0x08//获取触摸框支持的最大点数

#define ERASE_KEY                       0x09//bootloader参数擦除父命令
//bootloader参数擦除子命令 
#define ERASE_KEY_OPTION                0x00//擦除bootloader所有参数
#define ERASE_KEY_OPTION_RE             0x80

#define ALL_RESET                       0x10//复位所有的设置参数

#define EASY_MEETING                    0x20
#define EM_SEEWO                        0x00
#define EM_SEEWO_RE                     0x80
#define EM_EK                           0x02
#define EM_EK_RE                        0x82
 
#define FORMAT                          0x30//坐标发送格式配置父命令
//坐标发送格式配置子命令 
#define FMT_SET                         0x00//坐标发送格式设置
#define FMT_RE                          0xff
#define FMT_CVT                         0x01//TUIO坐标数据使能开关
#define FMT_CAL                         0x02//校准使能配置
#define FMT_SMO                         0x03//平滑使能设置
#define FMT_GET                         0x04//获取当前坐标发送格式
#define FMT_GET_RE                      0x84
#define FMT_WIDTH                       0x05
#define FMT_WIDTH_RE                    0x85
#define FMT_WIDTH_EN                    0x06//触摸宽度使能设置
#define FMT_USB_COORD                   0x07//读取USB坐标数据使能状态
#define FMT_USB_COORD_RE                0x87
#define FMT_USB_COORD_EN                0x08//USB坐标数据使能开关
#define FMT_MAC_10_SET                  0x09//Mac10系统下设置使能开关
#define FMT_MAC_10_SET_RE               0x89
#define FMT_MAC_10_GET                  0x10//读取Mac10系统下设置使能开关状态
#define FMT_MAC_10_GET_RE               0x90
#define FMT_GET_ALL                     0x11//读取所有的设置状态
#define FMT_GET_ALL_RE                  0x91
#define FMT_UART_COORD                  0x12//获取串口坐标数据使能状态
#define FMT_UART_COORD_RE               0x92
#define FMT_UART_COORD_EN               0x0A//串口坐标数据使能开关
#define FMT_UART_POINT_NUM_SET          0x13//设置串口发送的最大点数
#define FMT_UART_POINT_NUM_SET_RE       0x93
#define FMT_UART_POINT_NUM_GET          0x14//读取串口发送的最大点数
#define FMT_UART_POINT_NUM_GET_RE       0x15
#define FMT_UART_COORD_WIDTH_SET        0x16//串口宽度数据发送使能开关
#define FMT_UART_COORD_WIDTH_SET_RE     0x96
#define FMT_UART_COORD_WIDTH_GET        0x17//读取串口宽度数据发送使能开关状态
#define FMT_UART_COORD_WIDTH_GET_RE     0x97
#define FMT_PHYSICAL_SIZE_GET           0x18//读取触摸框的物理尺寸
#define FMT_PHYSICAL_SIZE_GET_RE        0x98
#define FMT_CMD_0B_RES                  0x0B//保留命令
#define FMT_UART_COORD_PROTOCOL_RE      0x0C
#define FMT_CMD_32_RES                  0x32//保留命令
 //触摸数据传输主命令
#define TOUCH_DATA_USB_TRAN             0x01    //USB传输
#define TOUCH_DATA_UART_TRAN            0x02    //UART传输

#define NEW_CAL                         0xA1//新校准协议父命令
//新校准协议子命令 
#define NEW_CAL_BEGIN                   0x01//启动校准
#define NEW_CAL_BEGIN_RE                0x81
#define NEW_CAL_END                     0x02//校准数据
#define NEW_CAL_END_RE                  0x82
#define NEW_CAL_SAVE                    0x03//保存校准数据
#define NEW_CAL_SAVE_RE                 0x83
#define NEW_CAL_GET                     0x04//读取校准数据
#define NEW_CAL_GET_RE                  0x84
#define NEW_CAL_RESET                   0x10//校准复位
#define NEW_CAL_SAVE_DEFAULT            0x05//将校准数据保存为默认校准数据
#define NEW_CAL_RESET_DEFAULT           0x06//将校准数据重置为出厂值
#define NEW_CAL_APP_SET                 0x07//预留接口，通过App调整校准参数
#define NEW_CAL_APP_SET_RE              0x87//预留接口，通过App调整校准参数



#define NEW_SET                         0xA2//设置父命令
//设置子命令  
#define NEW_SET_SET                     0x00//触摸相关参数设置（点击、右击、滚轮开关）
#define NEW_SET_GET                     0x01//读取触摸相关参数设置
#define NEW_SET_GET_RE                  0x81
#define NEW_SET_SETRESET                0x02//复位手势相关设置
#define NEW_SET_ALLRESET                0x03//所有参数复位
#define NEW_SET_ALLRESET_RE             0x83
#define NEW_SET_ADCSET                  0x04
#define NEW_SET_ADCGET                  0x05
#define NEW_SET_ADCGET_RE               0x85
#define NEW_SET_LIGHTSET                0x06
#define NEW_SET_LIGHTGET                0x07
#define NEW_SET_LIGHTGET_RE             0x87
#define NEW_SET_LIGHTREMOVE             0x08
#define NEW_SET_AGC_INIT                0x09//AGC初始化
#define NEW_SET_AGC_INIT_RE             0x89
#define NEW_SET_DEC_SET                 0x12//设置去污功能，转发到应用执行
#define NEW_SET_DEC_GET                 0x13//获取去污功能状态
#define NEW_SET_DEC_GET_RE              0x93//获取去污功能状态应答命令，接收应用数据再转发给工具
#define NEW_SET_AREA_SET                0x18
#define NEW_SET_AREA_SET_RE             0x98
#define NEW_SET_AREA_GET                0x19
#define NEW_SET_AREA_GET_RE             0x99
#define NEW_SET_DRAW_TEST_EN            0x20// 开启/关闭画线测试功能     1：开启，0：关闭
#define NEW_SET_DRAW_TEST_EN_RE         0xA0
#define NEW_SET_DRAW_TEST               0x21//not use
#define NEW_SET_DRAW_TEST_RE            0xA1
  
#define VERSION                         0xA3//固件信息父命令
//固件信息子命令  
#define VER_GET                         0x00//固件校验和、USB枚举信息（PID/VID）
#define VER_GET_RE                      0x80//固件校验和、USB枚举信息（PID/VID）应答命令
#define GET_OTA_KEY                     0x02//SR获取触摸框唯一 KEY 值
#define GET_OTA_KEY_RE                  0x82//SR获取触摸框唯一 KEY 值应答命令
#define VER_BOARD_TYPE_GET              0x01//获取触摸框型号
#define VER_BOARD_TYPE_GET_RE           0x81//获取触摸框型号应答命令
#define VER_DRIVE_GET                   0x03
#define VER_DRIVE_GET_RE                0x83
#define VER20_GET                       0x05//固件校验和、USB枚举信息（PID/VID），与VER_GET格式有差异
#define VER20_GET_RE                    0x85//固件校验和、USB枚举信息（PID/VID）应答命令
#define VER_EEPROM_CHECK_GET            0x06//EEPROM校验结果检测
#define VER_EEPROM_CHECK_GET_RE         0x86//

#define ANDROIDTEST                     0xA8//android touch test interface
//免驱算法位置子命令
#define TOUCH_BOX_CONTRO_PENETRATION    0x00//Touch penetration is controlled by a touch box
#define MCU_TOUCH_CONTRO_PENETRATION    0x01//Touch penetration is controlled by Mcu Touch
#define ANDROIDTEST_GETINFO             0x06

#define CMD_DATA                        0xD1//数据相关父命令
#define DATANEW                         0xA5
//数据相关子命令  
#define DATA_SEEWO                      0xA5
#define DATA_BACK                       0x04
#define DATA_BACK_RE                    0x84
#define DATA_GET_TOTAL                  0x00//获取测试各方向的灯管数
#define DATA_GET_TOTAL_RE               0x80
#define DATA_GET_BUF                    0x01//PC端请求QT信号测试扫描结果数据
#define DATA_GET_BUF_RE                 0x81//PC端请求QT信号测试扫描结果数据应答命令
#define RESUME_GET_TOTAL                0x02
#define RESUME_GET_TOTAL_RE             0x82
#define RESUME_GET_BUF                  0x03
#define RESUME_GET_BUF_RE               0x83
#define DATA_BACK                       0x04//通过USB1_EP1重发USB接收数据
#define DATA_BACK_RE                    0x84
#define DATA_TO_UART                    0x05//串口重发接收到的值
#define DATA_TO_UART_RE                 0x85
#define USB_READY                       0x06
#define USB_READY_RE                    0x86
#define DATA_GET_OBLIQUE_PARA           0x07
#define DATA_GET_OBLIQUE_PARA_RE        0x87
#define SCAN_INTERVAL                   0x08//获取各方向上测试扫描范围
#define SCAN_INTERVAL_RE                0x88
  
#define RETEST                          0xA6
#define RETEST_TOUCH                    0x01
  
#define ROTATION                        0xD9 //旋转设置父命令
//旋转设置子命令  
#define ROTATION_SET                    0x00//旋转设置
#define ROTATION_DEF_GET                0x01
#define ROTATION_DEF_GET_RE             0x81
#define ROTATION_GET                    0x02//读取旋转设置
#define ROTATION_GET_RE                 0x82
#define ROTATION_OS_SET                 0x03
#define ROTATION_REVERSE_SET            0x04
#define ROTATION_0                      0x00
#define ROTATION_90                     0x01
#define ROTATION_180                    0x02
#define ROTATION_270                    0x03
  
#define HW                              0xD0//硬件相关父命令
//硬件相关子命令  
#define HW_RESET                        0x00//硬件复位命令
#define HW_SCAN                         0x01//not use
#define HW_SCAN_RE                      0x81
#define HW_AGEING                       0x02//老化开关命令
#define HW_AGEING_RE                    0x82//老化应答命令
#define HW_LED                          0x03//主板指示灯开关命令
#define HW_AGC_SET                      0x04//not use
#define HW_EDGE_GET                     0x05//获取发射管发射状态 not use
#define HW_EDGE_GET_RE                  0x85//获取发射管发射状态应答命令
#define HW_T_GET                        0x06//获取测试信息（门限值、LED灯管数）
#define HW_T_GET_RE                     0x86//获取测试信息（门限值、LED灯管数）应答命令
#define HW_USB_CTR                      0x07//关闭USB命令
#define HW_TON_SET                      0x08//发射灯开关命令
#define HW_TON_SET_RE                   0x88//发射灯开关命令应答命令
#define HW_TON_GET                      0x09//获取发射管发射状态
#define HW_TON_GET_RE                   0x89//获取发射管发射状态应答命令
#define HW_F_MODE                       0x0C//not use
#define HW_F_MODE_RE                    0x8C//not use
#define HW_SET_THRESHOLE                0x0D//not use
#define HW_LOCKAGC_SET                  0x0E//设置RAGC的挡位
#define HW_LOCKAGC_SET_RE               0x8E//设置RAGC的挡位应答命令
#define HW_LOCKAGC_GET                  0x0F//获取MAX RAGC的挡位
#define HW_LOCKAGC_GET_RE               0x8F//获取MAX RAGC的挡位应答命令
#define HW_LOCKAGC_RESET                0x10//复位 RAGC
#define HW_LOCKAGC_RESET_RE             0x90//复位 RAGC应答命令
#define HW_TESTING                      0x11//not use
#define HW_TESTING_RE                   0x91//not use
#define HW_CHECKSIGNAL                  0x12//not use//信号自检命令    data[5]  自检类型（暂定 00 表示工厂 ，01 表示客户， 02表示终极用户）
#define HW_CHECKSIGNAL_RE               0x92//not use//信号自检应答命令

#define HW_ADC_INVERSE                  0x15//获取反向值
#define HW_ADC_INVERSE_RE               0x95//获取反向值应答命令
#define HW_TEST_CHANNEL                 0x16 //测试通道差异
#define HW_TEST_CHANNEL_RE              0x96 //测试通道差异ACK
#define HW_GET_RAGC_MAX_GEAR            0x17 //获取最大RAGC挡位
#define HW_GET_RAGC_MAX_GEAR_RE         0x97 //获取最大RAGC挡位ACK
#define HW_GET_RAGC_MAPPING             0x18 //获取RAGC挡位
#define HW_GET_RAGC_MAPPING_RE          0x98 //获取RAGC挡位ACK
#define HW_ADC_INVERSE_V2               0x19//获取反向值V2版本,底层只传数据,结果由上位机判断
#define HW_ADC_INVERSE_V2_RE            0x99//获取反向值应答命令

#define USERKEY                         0xDD //0xA9 Modified for Debug//老化相关父命令
#define USERKEYNEW                      0xA9//new commmand
//免驱数据获取与设置父命令
#define ANDROIDT_DRIVE_FREE             0xA9//免驱数据获取与设置父命令
//老化相关子命令  
#define USERKEY_GET_ID                  0x00//获取MCU ID
#define USERKEY_GET_ID_RE               0x80//获取MCU ID命令应答
#define USERKEY_SET_KEY                 0x01//设置老化时间
#define USERKEY_RELOAD_KEY              0x02//将老化时间从flash读出
#define USERKEY_SAVE_KEY                0x03//将老化时间存到flash
#define USERKEY_GET_KEY                 0x04//获取老化时间
#define USERKEY_GET_KEY_RE              0x84//获取老化时间应答命令

#define BL                              0xDA//bootloader升级相关父命令
#define BLNEW                           0xAA
//bootloader升级相关子命令  
#define BL_UPD_FLAG_GET                 0x00//获取bootloader升级状态
#define BL_UPD_FLAG_GET_RE              0x80//获取bootloader升级状态应答命令
#define BL_UPD_FLAG_SET                 0x01//设置bootloader升级状态
#define BL_UPD_FLAG_SET_RE              0x81//设置bootloader升级状态应答命令（BL_UPD_FLAG_SET_RE下）
#define BL_UPD_FLAG_SET_OK              0x01//给PC回复bootloader升级标志位设置成功命令
#define BL_UPD_FLAG_SET_NO_NEED         0x02//给PC回复bootloader升级标志位不需要设置命令（BL_UPD_FLAG_SET_RE下）
#define BL_RUN_BL                       0x02//bootloader运行命令（复位系统）
#define BL_ERASE_CONF                   0x03//暂无
#define BL_ERASE_CONF_RE                0x83//暂无
#define BL_UPD_START                    0x04//bootloader升级命令
#define BL_UPD_START_RE                 0x84//bootloader升级应答命令
  
#define QUERY_STATE                     0xAF
#define ENUMERATION_STATE               0x01
#define ENUMERATION_STATE_RE            0x81
  
#define SLAVE                           0xB2//主机设置父命令
//主机设置子命令  
#define SLAVE_UPGRADE                   0x01//not use
#define SLAVE_UPGRADE_RE                0x81//not use
#define SLAVE_RUN_USER_APP              0x02//not use
#define SLAVE_RUN_USER_APP_RE           0x82//not use
#define SLAVE_DEBUG_PARA                0x03//not use
#define SLAVE_DEBUG_PARA_RE             0x83//not use
#define SLAVE_AGC_OFF                   0x04//not use
#define SLAVE_AGC_OFF_RE                0x84//not use
#define SLAVE_AGC_ON                    0x05//not use
#define SLAVE_AGC_ON_RE                 0x85//not use
#define SLAVE_AGC_DATA                  0x06//not use
#define SLAVE_AGC_DATA_RE               0x86//not use
#define SLAVE_PARA_DATA                 0x07//not use
#define SLAVE_PARA_DATA_RE              0x87//not use
#define SLAVE_SET_SEND_DATA             0x08//强制发送ADC数据（算法接口）
#define SLAVE_SET_SEND_DATA_RE          0x88//强制发送ADC数据应答命令
#define SLAVE_SET_STOP_DATA             0x09//not use
#define SLAVE_SET_STOP_DATA_RE          0x89//not use
#define SLAVE_SEND_CHECKHARDWARE        0x10//not use
#define SLAVE_SEND_CHECKHARDWARE_RE     0x90//not use
#define SLAVE_QRIY_STATE_RESET          0x0A//not use
#define SLAVE_ADC_TEST                  0x11//临时修改门限值
#define SLAVE_ADC_TEST_RE               0x91//临时修改门限值应答命令
  
#define CONFIGPARA                      0xC0//获取参数父命令
//获取参数子命令  
#define CONFIGPARA_INFO_READ            0x00
#define CONFIGPARA_INFO_READ_RE         0x80
#define CONFIGPARA_READ                 0x01
#define CONFIGPARA_READ_RE              0x81
#define CONFIGPARA_WRITE                0x02
#define CONFIGPARA_WRITE_RE             0x82
#define CONFIGPARA_SLAVECRC32_GET       0x03
#define CONFIGPARA_SLAVECRC32_GET_RE    0x83
#define CONFIGPARA_RESET                0x04
#define CONFIGPARA_XY_LED_GET           0x05//获取发射灯PCB板数与灯数
#define CONFIGPARA_XY_LED_GET_RE        0x85
#define CONFIGPARA_XY_RLED_GET          0x06//获取接收灯PCB板数与灯数
#define CONFIGPARA_XY_RLED_GET_RE       0x86
  
// 固件配置命令  
#define CMD_CONFIG                      0xAC//固件配置父命令
//固件配置子命令  
#define CONFIG_OK                       0x00
#define CONFIG_FINISH                   0x01
#define CONFIG_PACKETNO_ERROR           0x02
#define CONFIG_CHECK_ERROR              0x03
#define CONFIG_OTHER_ERROR              0x04
#define CONFIG_SET_DATA                 0x00//配置触摸框参数（存储在flash中）
#define CONFIG_SET_DATA_RE              0x80
#define CONFIG_GET_DATA                 0x01//读取触摸框参数
#define CONFIG_GET_DATA_RE              0x81
#define CONFIG_SET_CHECKSUM             0x02
#define CONFIG_SET_CHECKSUM_RE          0x82
#define CONFIG_GET_CHECKSUM             0x03
#define CONFIG_GET_CHECKSUM_RE          0x83
#define CONFIG_TEST                     0x0A
#define CONFIG_SET_TESTMODE             0x06
#define CONFIG_SET_TESTMODE_RE          0x86
#define CONFIG_SET_QRCODE               0x20
#define CONFIG_SET_QRCODE_RE            0xA0
#define CONFIG_GET_QRCODE               0x21
#define CONFIG_GET_QRCODE_RE            0xA1
  
#define CMD_MULTPOITS                   0xEA
#define CMD_MULTPOITS_EN                0x01
#define CMD_MULTPOITS_EN_RE             0x81
#define CMD_MULTPOITS_DATA              0x02//转发点数据
#define CMD_MULTPOITS_DATA_RE           0x82
#define CMD_MULTPOITS_OTHER_RE          0xEE
#define CMD_MULTPOITS_ERROR_RE          0xEF
  
#define CMD_SENDCOORD_CONFIG            0xB6//发送坐标配置父命令
#define CMD_SENDCOORD_EN                0x01//坐标数据发送开关设置
#define CMD_CMD_SENDCOORD_EN_RE         0x81//坐标数据发送开关设置应答
  
#define CMD_SCAN_PARAMETER              0xB7 
#define CMD_GET_SCAN_NUM                0x01
#define CMD_GET_SCAN_NUM_RE             0x81 
  
#define INITIATIVE_REPORT               0xFC
#define DRAW_REPORT                     0x01
#define DRAW_TYPE                       0x00
  
/*erase command*/  
#define FLASH_COMMAND                   0xDB//flash擦除父命令
//flash擦除子命令  
#define ERASE_BOOT                      0x01//擦除boot
#define ERASE_BOOT_RE                   0x81//擦除boot应答命令
#define ERASE_APP                       0x02//擦除固件
#define ERASE_APP_RE                    0x82//擦除固件应答命令

#define DEBUG_COMMAND                   0xDC//调试命令
#define DEBUG_GET_USB_CONFIG_TYPE       0x00
#define DEBUG_SET_USB_CONFIG_TYPE       0x01
#define DEBUG_REG_USB_PRINTF_REPORT_ID  0x02
#define DEBUG_PRINTF_INFO               0x04//打印关键的信息
#define DEBUG_AUTO_AGC_EN               0x05//自动调节AGC使能或失能

#define CMD_NON_PENETRAING_REGION       0xB4    //区域设置主命令
#define CMD_ADD_REGION                  0x01    //添加区域
#define CMD_ADD_REGION_RE               0x81    //添加区域应答
#define CMD_DELETE_REGION               0x02    //删除区域
#define CMD_DELETE_REGION_RE            0x82    //删除区域应答
#define CMD_QUERY_REGION                0x03    //查询区域
#define CMD_QUERY_REGION_RE             0x83    //查询区域应答
#define CMD_QUERY_REGION_ID             0x04    //查询区域
#define CMD_QUERY_REGION_ID_RE          0x84    //查询区域应答

#define KEYBOARD                        0xAD    //模拟键盘
#define KEYBOARD_SENDKEY                0x01    //模拟发送键盘命令
#define KEYBOARD_SENDKEY_RE             0x81    //返回值
#define KEYBOARD_KEY_PRESS              0x02    // 按键按下
#define KEYBOARD_KEY_PRESS_RE           0x82    // 按键按下回复
#define KEYBOARD_KEY_RELEASE            0x03    // 按键抬起
#define KEYBOARD_KEY_RELEASE_RE         0x83    // 按键抬起回复

/*
 * 保留命令
 */
#define CMD_C3_RES                      0xC3

#define CMD_6D_RES                      0x6D    //屏幕缩放命令
#define SCREEN_ZOOM_SETTING             0x08

#define CMD_B0_RES                      0xB0


#endif

