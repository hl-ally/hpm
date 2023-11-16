#include "usbd_core.h"
#include "usbd_hid.h"
#include "user_config.h"
#include "Api_UsbDesc.h"


/* USB0 的配置描述符总Buffer */
__attribute__((aligned(64))) uint8_t USB0_ConfigDesc[USB_HID_CONFIG_DESC_LEN] =
{
    0x09,                           /* bLength: Configuration Descriptor size */
    USB_DESCRIPTOR_TYPE_CONFIGURATION,            /* bDescriptorType: Configuration */
    USB_HID_CONFIG_DESC_LEN,        /* wTotalLength: Bytes returned */
    0x00,
    0x00,                           /*bNumInterfaces: 1 interface*/
    0x01,                           /*bConfigurationValue: Configuration value*/
    0x00,                           /*iConfiguration: Index of string descriptor describing the configuration*/
    0xA0,                           /*bmAttributes: bus powered */
    0x32                            /*MaxPower 100 mA: this current is used for detecting Vbus*/
};