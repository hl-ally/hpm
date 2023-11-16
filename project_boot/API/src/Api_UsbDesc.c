#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include "usbd_core.h"
#include "usbd_hid.h"
#include "user_config.h"
#include "Api_UsbDesc.h"
#include "Api_Flash.h"
#include "Api_Math.h"
#include "Api_Crc.h"
#include "GlobalDefaultDefine.h"
#include "GlobalDataStructure.h"
#include "Api_UsbDevice.h"


#define BOOT_USBD_VID                   0x1FF7
#define BOOT_USBD_PID                   0x0F0A
#define BOOT_USBD_MAX_POWER             100
#define USB_HID_CONFIG_DESC_SIZ         (9 + 9 + 9 + 7 + 7)


uint8_t g_nUserPoints = 0;
static stUsbDescFlash_t *sg_pUsbDescFlash = (stUsbDescFlash_t *)FLASH_USB_DESC_ADDRESS;
eReportSectionID_t g_arrHidReportMap[eReportSectionTotal];


const uint8_t CustomHID_DeviceDescriptor[CUSTOMHID_SIZ_DEVICE_DESC] =
{
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, BOOT_USBD_VID, BOOT_USBD_PID, 0x0002, 0x01),
};

const uint8_t CustomHID_ConfigDescriptor[CUSTOMHID_SIZ_CONFIG_DESC] =
{
    USB_CONFIG_DESCRIPTOR_INIT(CUSTOMHID_SIZ_CONFIG_DESC, 0x01, 0x01, 0xA0, BOOT_USBD_MAX_POWER),
};


const uint8_t CustomHID_ConfigDescriptor_Touch[32] =
{
    /************** Descriptor of HID Touch interface ****************/
    0x09,                                   /*bLength: Interface Descriptor size*/
    USB_DESCRIPTOR_TYPE_INTERFACE,                /*bDescriptorType: Interface descriptor type*/
    0x00,                                   /*bInterfaceNumber: Number of Interface*/
    0x00,                                   /*bAlternateSetting: Alternate setting*/
    0x02,                                   /*bNumEndpoints*/
    0x03,                                   /*bInterfaceClass: CUSTOM_HID wanglonghui*/
    0x00,                                   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,                                   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                                      /*iInterface: Index of string descriptor*/

    /******************** Descriptor of HID *************************/
    0x09,                                   /*bLength: CUSTOM_HID Descriptor size*/
    HID_DESCRIPTOR_TYPE_HID,             /*bDescriptorType: CUSTOM_HID*/
    0x00,                                   /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
    0x02,
    0x00,                                   /*bCountryCode: Hardware target country*/
    0x01,                                   /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
    0x22,                                   /*bDescriptorType*/
    KEYREPORT_DESC_SIZE,       /*wItemLength: Total length of Report descriptor*/
    0x00,

    /******************** Descriptor of endpoints ********************/
    0x07,                                   /*bLength: Endpoint Descriptor size*/
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /*bDescriptorType:*/
    CUSTOM_TOUCH0_EP_IN_ADDR,                     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                                         /*bmAttributes: Interrupt endpoint*/
    CUSTOM_TOUCH0_EP_IN_SIZE,                     /*wMaxPacketSize: 2 Byte max */
    0x00,
    0x01,                                   /*bInterval: Polling Interval (1 ms)*/

    0x07,                                   /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /* bDescriptorType: */
    CUSTOM_TOUCH0_EP_OUT_ADDR,                    /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,                                   /* bmAttributes: Interrupt endpoint */
    CUSTOM_TOUCH0_EP_OUT_SIZE,                    /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x01                                    /* bInterval: Polling Interval (1 ms) */

};


const uint8_t CustomHID_ConfigDescriptor_Touch1[32] =
{
    /************** Descriptor of HID Touch1 interface ****************/
    0x09,                                   /*bLength: Interface Descriptor size*/
    USB_DESCRIPTOR_TYPE_INTERFACE,                /*bDescriptorType: Interface descriptor type*/
    0x00,                                   /*bInterfaceNumber: Number of Interface*/
    0x00,                                   /*bAlternateSetting: Alternate setting*/
    0x02,                                   /*bNumEndpoints*/
    0x03,                                   /*bInterfaceClass: CUSTOM_HID wanglonghui*/
    0x00,                                   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,                                   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                                      /*iInterface: Index of string descriptor*/

    /******************** Descriptor of HID *************************/
    0x09,                                   /*bLength: CUSTOM_HID Descriptor size*/
    HID_DESCRIPTOR_TYPE_HID,             /*bDescriptorType: CUSTOM_HID*/
    0x00,                                   /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
    0x02,
    0x00,                                   /*bCountryCode: Hardware target country*/
    0x01,                                   /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
    0x22,                                   /*bDescriptorType*/
    KEYREPORT_DESC_SIZE,       /*wItemLength: Total length of Report descriptor*/
    0x00,

    /******************** Descriptor of endpoints ********************/
    0x07,                                   /*bLength: Endpoint Descriptor size*/
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /*bDescriptorType:*/
    CUSTOM_TOUCH1_EP_IN_ADDR,                     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                                         /*bmAttributes: Interrupt endpoint*/
    CUSTOM_TOUCH1_EP_IN_SIZE,                     /*wMaxPacketSize: 2 Byte max */
    0x00,
    0x01,                                   /*bInterval: Polling Interval (1 ms)*/

    0x07,                                   /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /* bDescriptorType: */
    CUSTOM_TOUCH1_EP_OUT_ADDR,                    /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,                                   /* bmAttributes: Interrupt endpoint */
    CUSTOM_TOUCH1_EP_OUT_SIZE,                    /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x01                                    /* bInterval: Polling Interval (1 ms) */

};

const uint8_t CustomHID_ConfigDescriptor_Ink[32] =
{
    /************** Descriptor of HID INK interface ****************/
    0x09,                                   /*bLength: Interface Descriptor size*/
    USB_DESCRIPTOR_TYPE_INTERFACE,                /*bDescriptorType: Interface descriptor type*/
    0x00,                                   /*bInterfaceNumber: Number of Interface*/
    0x00,                                   /*bAlternateSetting: Alternate setting*/
    0x02,                                   /*bNumEndpoints*/
    0x03,                                   /*bInterfaceClass: CUSTOM_HID */
    0x00,                                   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,                                   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                                      /*iInterface: Index of string descriptor*/

    /******************** Descriptor of HID *************************/
    0x09,                                   /*bLength: CUSTOM_HID Descriptor size*/
    HID_DESCRIPTOR_TYPE_HID,             /*bDescriptorType: CUSTOM_HID*/
    0x00,                                   /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
    0x02,
    0x00,                                   /*bCountryCode: Hardware target country*/
    0x01,                                   /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
    0x22,                                   /*bDescriptorType*/
    KEYREPORT_DESC_SIZE,       /*wItemLength: Total length of Report descriptor*/
    0x00,

    /******************** Descriptor of endpoints ********************/
    0x07,                                   /*bLength: Endpoint Descriptor size*/
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /*bDescriptorType:*/
    CUSTOM_INK_EP_IN_ADDR,                     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                                         /*bmAttributes: Interrupt endpoint*/
    CUSTOM_INK_EP_IN_SIZE,                     /*wMaxPacketSize: 2 Byte max */
    0x00,
    0x01,                                   /*bInterval: Polling Interval (1 ms)*/

    0x07,                                   /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /* bDescriptorType: */
    CUSTOM_INK_EP_OUT_ADDR,                    /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,                                   /* bmAttributes: Interrupt endpoint */
    CUSTOM_INK_EP_OUT_SIZE,                    /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x01                                    /* bInterval: Polling Interval (1 ms) */

};

const uint8_t CustomHID_ConfigDescriptor_KeyBoard[32] =
{
    /************** Descriptor of KeyBoard interface ****************/
    0x09,                                   /*bLength: Interface Descriptor size*/
    USB_DESCRIPTOR_TYPE_INTERFACE,                /*bDescriptorType: Interface descriptor type*/
    0x00,                                   /*bInterfaceNumber: Number of Interface*/
    0x00,                                   /*bAlternateSetting: Alternate setting*/
    0x02,                                   /*bNumEndpoints*/
    0x03,                                   /*bInterfaceClass: CUSTOM_HID wanglonghui*/
    0x00,                                   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,                                   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                                      /*iInterface: Index of string descriptor*/

    /******************** Descriptor of HID *************************/
    0x09,                                   /*bLength: CUSTOM_HID Descriptor size*/
    HID_DESCRIPTOR_TYPE_HID,             /*bDescriptorType: CUSTOM_HID*/
    0x00,                                   /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
    0x01,
    0x00,                                   /*bCountryCode: Hardware target country*/
    0x01,                                   /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
    0x22,                                   /*bDescriptorType*/
    KEYREPORT_DESC_SIZE,       /*wItemLength: Total length of Report descriptor*/
    0x00,

    /******************** Descriptor of endpoints ********************/
    0x07,                                   /*bLength: Endpoint Descriptor size*/
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /*bDescriptorType:*/
    CUSTOM_KEYBOARD_EP_IN_ADDR,                     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                                   /*bmAttributes: Interrupt endpoint*/
    CUSTOM_KEYBOARD_EP_IN_SIZE,                     /*wMaxPacketSize: 2 Byte max */
    0x00,
    0x01,                                   /*bInterval: Polling Interval (1 ms)*/

    0x07,                                   /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /* bDescriptorType: */
    CUSTOM_KEYBOARD_EP_OUT_ADDR,                    /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,                                   /* bmAttributes: Interrupt endpoint */
    CUSTOM_KEYBOARD_EP_OUT_SIZE,                    /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x01                                    /* bInterval: Polling Interval (1 ms) */
};

const uint8_t CustomHID_ConfigDescriptor_Mouse[32] =
{
    /************** Descriptor of KeyBoard interface ****************/
    0x09,                                   /*bLength: Interface Descriptor size*/
    USB_DESCRIPTOR_TYPE_INTERFACE,                /*bDescriptorType: Interface descriptor type*/
    0x00,                                   /*bInterfaceNumber: Number of Interface*/
    0x00,                                   /*bAlternateSetting: Alternate setting*/
    0x02,                                   /*bNumEndpoints*/
    0x03,                                   /*bInterfaceClass: CUSTOM_HID wanglonghui*/
    0x00,                                   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,                                   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                                      /*iInterface: Index of string descriptor*/

    /******************** Descriptor of HID *************************/
    0x09,                                   /*bLength: CUSTOM_HID Descriptor size*/
    HID_DESCRIPTOR_TYPE_HID,             /*bDescriptorType: CUSTOM_HID*/
    0x00,                                   /*bCUSTOM_HIDUSTOM_HID: CUSTOM_HID Class Spec release number*/
    0x02,
    0x00,                                   /*bCountryCode: Hardware target country*/
    0x01,                                   /*bNumDescriptors: Number of CUSTOM_HID class descriptors to follow*/
    0x22,                                   /*bDescriptorType*/
    KEYREPORT_DESC_SIZE,       /*wItemLength: Total length of Report descriptor*/
    0x00,

    /******************** Descriptor of endpoints ********************/
    0x07,                                   /*bLength: Endpoint Descriptor size*/
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /*bDescriptorType:*/
    CUSTOM_MOUSE_EP_IN_ADDR,                     /*bEndpointAddress: Endpoint Address (IN)*/
    0x03,                                   /*bmAttributes: Interrupt endpoint*/
    CUSTOM_MOUSE_EP_IN_SIZE,                     /*wMaxPacketSize: 2 Byte max */
    0x00,
    0x01,                                   /*bInterval: Polling Interval (1 ms)*/

    0x07,                                   /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /* bDescriptorType: */
    CUSTOM_MOUSE_EP_OUT_ADDR,                    /*bEndpointAddress: Endpoint Address (OUT)*/
    0x03,                                   /* bmAttributes: Interrupt endpoint */
    CUSTOM_MOUSE_EP_OUT_SIZE,                    /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x01                                    /* bInterval: Polling Interval (1 ms) */
};

const uint8_t CustomHID_ConfigDescriptor_Bulk[23]  =
{
    /************** Descriptor of CUSTOM HID Touch interface ****************/
    0x09,                                   /*bLength: Interface Descriptor size*/
    USB_DESCRIPTOR_TYPE_INTERFACE,                /*bDescriptorType: Interface descriptor type*/
    0x00,                                   /*bInterfaceNumber: Number of Interface*/
    0x00,                                   /*bAlternateSetting: Alternate setting*/
    0x02,                                   /*bNumEndpoints*/
    0xff,                                   /*bInterfaceClass: CUSTOM_HID wanglonghui*/
    0x00,                                   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
    0x00,                                   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
    0,                                      /*iInterface: Index of string descriptor*/

    /******************** Descriptor of Custom HID endpoints ********************/
    0x07,                                   /*bLength: Endpoint Descriptor size*/
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /*bDescriptorType:*/
    CUSTOM_BULK_EP_IN_ADDR,        /*bEndpointAddress: Endpoint Address (IN)*/
    0x02,                                   /*bmAttributes: Interrupt endpoint*/
    CUSTOM_BULK_EP_IN_SIZE,                     /*wMaxPacketSize: 2 Byte max */
    0x00,
    0x00,                                   /*bInterval: Polling Interval (1 ms)*/

    0x07,                                   /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,                 /* bDescriptorType: */
    CUSTOM_BULK_EP_OUT_ADDR,                    /*bEndpointAddress: Endpoint Address (OUT)*/
    0x02,                                   /* bmAttributes: Interrupt endpoint */
    CUSTOM_BULK_EP_OUT_SIZE,                    /* wMaxPacketSize: 2 Bytes max  */
    0x00,
    0x00                                    /* bInterval: Polling Interval (1 ms) */
};


const uint8_t CustomHID_ReportDescriptor[] = {
 /* USER CODE BEGIN 0 */ 
  0x06, 0x00, 0xff,               /*   USAGE_PAGE (Undefined)                                                          */
  0x09, 0x00,                     /*   USAGE (Undefined)                                                               */
  0xa1, 0x01,                     /*   COLLECTION (Application)                                                        */
  0x85, 0xFD,                     /*   REPORT_ID (253)                                                                 */
  0x06, 0x00, 0xff,               /*   USAGE_PAGE (Undefined)                                                          */
  0x19, 0x01,                     /*   USAGE_MINIMUM                                                                   */
  0x29, 0x3f,                     /*   USAGE_MAXIMUM                                                                   */
  0x15, 0x00,                     /*   LOGICAL_MINIMUM (0)                                                             */
  0x25, 0xff,                     /*   LOGICAL_MAXIMUM (255)                                                           */
  0x75, 0x08,                     /*   REPORT_SIZE (8)                                                                 */
  0x95, 0x3f,                     /*   REPORT_COUNT (63)                                                               */
  0x81, 0x02,                     /*   INPUT (Data,Var,Abs)                                                            */
  0x85, 0xFE,                     /*   REPORT_ID (254)                                                                 */
  0x06, 0x00, 0xff,               /*   USAGE_PAGE (Undefined)                                                          */
  0x19, 0x01,                     /*   USAGE_MINIMUM                                                                   */
  0x29, 0x3f,                     /*   USAGE_MAXIMUM                                                                   */
  0x15, 0x00,                     /*   LOGICAL_MINIMUM (0)                                                             */
  0x25, 0xff,                     /*   LOGICAL_MAXIMUM (255)                                                           */
  0x75, 0x08,                     /*   REPORT_SIZE (8)                                                                 */
  0x95, 0x3f,                     /*   REPORT_COUNT (63)                                                               */
  0x91, 0x02,                     /*   OutPut (Data,Var,Abs)                                                           */
  /* USER CODE END 0 */ 
  0xC0    /*     END_COLLECTION                 */
};

const unsigned char g_nWindowHeadDataArray[] =
{   //Window多点用途页 length=10
    0x05, 0x0d,                     // USAGE_PAGE (Digitizers)
    0x09, 0x04,                     // USAGE (Touch Screen)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, TOUCH_REPORT_ID,          // REPORT_ID (02)
    0x09, 0x22                      // USAGE (Finger)
};

/* Window Touch的报告描述符 */
const unsigned char g_nWindowDataArray_1[] =
{   // Multitouch Length=76*(Contact count maximum)
    0xa1, 0x02,                     // COLLECTION (Logical)
    //0x05, 0x0d,                     // USAGE_PAGE (Digitizers)
    0x09, 0x42,                     // USAGE (Tip Switch)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x01,                     // LOGICAL_MAXIMUM (1)
    0x75, 0x01,                     // REPORT_SIZE (1)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x32,                     // USAGE (In Range)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x47,                     // USAGE (Touch Valid)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x95, 0x05,                     // REPORT_COUNT (5)
    0x81, 0x03,                     // INPUT (Cnst,Ary,Abs)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x09, 0x51,                     // USAGE (Contact Identifier)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x7f,               // LOGICAL_MAXIMUM (4095)
    0x75, 0x10,                     // REPORT_SIZE (16)
    0x55, 0x0e,                     // UNIT_EXPONENT (-2)
    0x65, 0x11,                     // UNIT (cm,SI Linear)
    0x09, 0x30,                     // USAGE (X)
    0x35, 0x00,                     // PHYSICAL_MINIMUM (0)
    0x46, 0x00, 0x00,               // PHYSICAL_MAXIMUM (0)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x46, 0x00, 0x00,               // PHYSICAL_MAXIMUM (0)
    0x09, 0x31,                     // USAGE (Y)
    0x81, 0x02,                     // INPUT (Data,Var,Abs) 

    0x05, 0x0d,                     // USAGE PAGE (Digitizers)
    0x09, 0x48,                     // USAGE (Width)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x49,                     // USAGE (Height)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    
    0xc0                            // END_COLLECTION
};


/* Window Touch的报告描述符 */
const unsigned char g_nWindowDataArray[] =
{   //Length=76*(Contact count maximum)
    0xa1, 0x02,                     // COLLECTION (Logical)
    0x05, 0x0d,                     // USAGE_PAGE (Digitizers)
    0x09, 0x42,                     // USAGE (Tip Switch)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x01,                     // LOGICAL_MAXIMUM (1)
    0x75, 0x01,                     // REPORT_SIZE (1)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x32,                     // USAGE (In Range)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x47,                     // USAGE (Touch Valid)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x95, 0x05,                     // REPORT_COUNT (5)
    0x81, 0x03,                     // INPUT (Cnst,Ary,Abs)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x09, 0x51,                     // USAGE (Contact Identifier)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x7f,               // LOGICAL_MAXIMUM (4095)
    0x75, 0x10,                     // REPORT_SIZE (16)
    0x55, 0x0e,                     // UNIT_EXPONENT (-2)
    0x65, 0x11,                     // UNIT (cm,SI Linear)
    0x09, 0x30,                     // USAGE (X)
    0x35, 0x00,                     // PHYSICAL_MINIMUM (0)
    0x46, 0x00, 0x00,               // PHYSICAL_MAXIMUM (0)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x46, 0x00, 0x00,               // PHYSICAL_MAXIMUM (0)
    0x09, 0x31,                     // USAGE (Y)
    0x81, 0x02,                     // INPUT (Data,Var,Abs) 

    0x05, 0x0d,                     // USAGE PAGE (Digitizers)
    0x09, 0x48,                     // USAGE (Width)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x49,                     // USAGE (Height)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    
    0xc0                            // END_COLLECTION
};

/* Window Touch的报告描述符 */
const unsigned char g_nWindowEndDataArray[] =
{   // Length=52
    0x05, 0x0d,                     // USAGE_PAGE (Digitizers)
    0x09, 0x54,                     // USAGE (Contact count)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)

    0x85, TOUCH_MAXMIN_REPORT_ID,   // REPORT_ID (03)
    0x09, 0x55,                     // USAGE (Contact count maximum)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x7F,                     // LOGICAL_MAXIMUM (128)
    0xb1, 0x02,                     // FEATURE (Data,Var,Abs)
    0xc0,                           // END_COLLECTION

    0x09, 0x0e,                     // USAGE (Device Configuration)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, TOUCH_DEV_CFG_REPORT_ID,  // REPORT_ID (04)
    0x09, 0x23,                     // USAGE (Device Setting)
    0xa1, 0x02,                     // COLLECTION (Logical)
    0x09, 0x52,                     // USAGE (Device mode)
    0x09, 0x53,                     // USAGE (Device Identifier)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x0a,                     // LOGICAL_MAXIMUM (10)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x02,                     // REPORT_COUNT (2)
    0xb1, 0x02,                     // FEATURE (Data,Var,Abs)
    0xc0,                           // END_COLLECTION
    0xc0                            // END_COLLECTION  */
};

const unsigned char g_arrMultiPointsRepDataHead[] =
{
    0x05, 0x0D,        // Usage Page (Digitizer)
    0x09, 0x04,        // Usage (Touch Screen)
    0xA1, 0x01,        // Collection (Application)
    0x85, TOUCH_V2_REPORT_ID,        //   Report ID
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x01,        //   Report Count (1) //version
    0x81, 0x03,        //   Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x54,        //   Usage (Contact Count)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0x80, 0x00,  //   Logical Maximum (128)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x22,        //   Usage (Finger)

};

const unsigned char g_arrMultiPointsRepData[] =
{
    0xA1, 0x02,        //   Collection (Logical)
    0x09, 0x42,        //     Usage (Tip Switch)
    0x15, 0x00,        //     Logical Minimum (0)
    0x25, 0x01,        //     Logical Maximum (1)
    0x75, 0x01,        //     Report Size (1)
    0x95, 0x01,        //     Report Count (1)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x32,        //     Usage (In Range)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x47,        //     Usage (Confidence)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x95, 0x05,        //     Report Count (5)
    0x81, 0x03,        //     Input (Const,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x51,        //     Usage (Contact ID)
    0x75, 0x08,        //     Report Size (8)
    0x95, 0x01,        //     Report Count (1)
    0x25, 0x7F,        //     Logical Maximum (127)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x01,        //     Usage Page (Generic Desktop Ctrls)
    0x75, 0x10,        //     Report Size (16)
    0x55, 0x0E,        //     Unit Exponent (-2)
    0x65, 0x11,        //     Unit (System: SI Linear, Length: Centimeter)
    0x15, 0x00,        //     Logical Minimum (0)
    0x35, 0x00,        //     Physical Minimum (0)
    0x09, 0x30,        //     Usage (X)
    0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
    0x46, 0x06, 0x4A,  //     Physical Maximum (18950) //54 55
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x31,        //     Usage (Y)
    0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
    0x46, 0xA4, 0x29,  //     Physical Maximum (10660) //64 65
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0D,        //     Usage Page (Digitizer)
    0x09, 0x48,        //     Usage (Width)
    0x26, 0x06, 0x4A,  //     Logical Maximum (18950)  //73 74
    0x46, 0x06, 0x4A,  //     Physical Maximum (18950) //76 77
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x49,        //     Usage (Height)
    0x26, 0xA4, 0x29,  //     Logical Maximum (10660)  //83 84
    0x46, 0xA4, 0x29,  //     Physical Maximum (10660) //86 87
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x30,        //     Usage (Tip Pressure)
    0x55, 0x00,        //     Unit Exponent (0)
    0x65, 0x00,        //     Unit (None)
    0x15, 0x00,        //     Logical Minimum (0) //97
    0x26, 0xff, 0x03,  //     Logical Maximum (1023) 99 100
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x3F,        //     Usage (Azimuth)
    0x55, 0x0F,        //     Unit Exponent (-1)
    0x65, 0x14,        //     Unit (System: English Rotation, Length: Degrees)
    0x15, 0x00,        //     Logical Minimum (0)
    0x27, 0x00, 0x00, 0x01, 0x00, //     Logical Maximum (65536)
    0x35, 0x00,        //     Physical Minimum (0)
    0x47, 0xA0, 0x8C, 0x00, 0x00,  //     Physical Maximum (36000)
    0x81, 0x02,        //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0xC0,              //   End Collection
};

const unsigned char g_arrMultiPointsRepDataEnd[] =
{
    0x75, 0x08,        //   Report Size (8)
    0x95, 0x05,        //   Report Count (5)
    0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x05, 0x0D,        //   Usage Page (Digitizer)
    0x85, TOUCH_V2_MAXMIN_REPORT_ID,        //   Report ID (10)
    0x09, 0x55,        //   Usage  (Contact count maximum)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0x80, 0x00,  //   Logical Maximum (128)
    0xB1, 0x02,        //   Feature (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              // End Collection
};

#if (SYSTEM_SUPPORT_INK == SYSTEM_SUPPORT_INK_V1)
/* Window INK的报告描述符 */
const unsigned char g_nInkDataArray[] =
{
    // Ink Length=108
    0x05, 0x0d,                     // USAGE_PAGE (Digitizers)
    0x09, 0x02,                     // USAGE (Pen)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, INK_REPORT_ID,            //   REPORT_ID (Ink)
    0x09, 0x20,                     //   USAGE (Stylus)
    0xa1, 0x00,                     //   COLLECTION (Physical)
    0x15, 0x00,                     //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                     //     LOGICAL_MAXIMUM (1)
    0x09, 0x42,                     //     USAGE (Tip Switch)
    0x75, 0x01,                     //     REPORT_SIZE (1)
    0x95, 0x01,                     //     REPORT_COUNT (1)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x09, 0x32,                     //     USAGE  (In Range)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x09, 0x45,                     //     USAGE (Eraser Switch)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x95, 0x05,                     //     REPORT_COUNT (5)
    0x81, 0x03,                     //     INPUT (Cnst,Var,Abs)
    0x75, 0x08,                     //     REPORT_SIZE (8)
    0x95, 0x01,                     //     REPORT_COUNT (1)
    0x09, 0x5b,                     //     USAGE
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x05, 0x01,                     //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                     //     USAGE (X)
    0x75, 0x10,                     //     REPORT_SIZE (16)
    0x95, 0x01,                     //     REPORT_COUNT (1)
    0xa4,                           //     PUSH
    0x55, 0x0e,                     //     UNIT_EXPONENT (-2)
    0x65, 0x11,                     //     UNIT (cm,SI Linear)
    0x35, 0x00,                     //     PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x7f,               //     PHYSICAL_MAXIMUM (32767)  width 60-61
    0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x09, 0x31,                     //     USAGE (Y)
    0x46, 0xff, 0x7f,               //     PHYSICAL_MAXIMUM (32767)   height 70-71
    0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x09, 0x48,
    0x46, 0xff, 0x7f,               //     PHYSICAL_MAXIMUM (32767)   height 80-81
    0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,
    0x09, 0x49,
    0x46, 0xff, 0x7f,               //     PHYSICAL_MAXIMUM (32767)   height 90-91
    0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,
    0xb4,                           //     POP
    0x05, 0x0d,                     //     USAGE_PAGE (Digitizers)
    0x09, 0x30,                     //     USAGE (Tip Pressure)
    0x26, 0x00, 0x04,               //     LOGICAL_MAXIMUM (1024)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0xc0,                           //   END_COLLECTION
    0xc0
};
#elif (SYSTEM_SUPPORT_INK == SYSTEM_SUPPORT_INK_V2)
const unsigned char g_nInkDataArray[] =
{
    // Ink Length=87
    0x05, 0x0d,                     // USAGE_PAGE (Digitizers)
    0x09, 0x02,                     // USAGE (Pen)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, INK_REPORT_ID,            //   REPORT_ID (Ink)
    0x09, 0x20,                     //   USAGE (Stylus)
    0xa1, 0x00,                     //   COLLECTION (Physical)
    0x09, 0x42,                     //     USAGE (Tip Switch)
    0x09, 0x32,                     //     USAGE  (In Range)
    0x09, 0x45,                     //     USAGE (Eraser Switch)
    0x09, 0x3c,                     //     USAGE (Invert)
    0x15, 0x00,                     //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                     //     LOGICAL_MAXIMUM (1)
    0x75, 0x01,                     //     REPORT_SIZE (1)
    0x95, 0x04,                     //     REPORT_COUNT (4)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x95, 0x01,                     //     REPORT_COUNT (1)
    0x81, 0x03,                     //     INPUT (Cnst,Var,Abs)
    0x09, 0x32,                     //     USAGE (In Range)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x95, 0x02,                     //     REPORT_COUNT (2)
    0x81, 0x03,                     //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,                     //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                     //     USAGE (X)
    0x75, 0x10,                     //     REPORT_SIZE (16)
    0x95, 0x01,                     //     REPORT_COUNT (1)
    0xa4,                           //     PUSH
    0x55, 0x0e,                     //     UNIT_EXPONENT (-2)
    0x65, 0x11,                     //     UNIT (cm,SI Linear)
    0x35, 0x00,                     //     PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x7f,               //     PHYSICAL_MAXIMUM (32767)
    0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0x09, 0x31,                     //     USAGE (Y)
    0x46, 0xff, 0x7f,               //     PHYSICAL_MAXIMUM (32767)
    0x26, 0xff, 0x7f,               //     LOGICAL_MAXIMUM (32767)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0xb4,                           //     POP
    0x05, 0x0d,                     //     USAGE_PAGE (Digitizers)
    0x09, 0x30,                     //     USAGE (Tip Pressure)
    0x26, 0x00, 0x04,               //     LOGICAL_MAXIMUM (1024)
    0x81, 0x02,                     //     INPUT (Data,Var,Abs)
    0xc0,                           //   END_COLLECTION
    0xc0
};
#endif

/* Window Mouse的报告描述符 */
const unsigned char g_nMouseDataArray[] =
{   //length=94
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                     // USAGE (Mouse)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x09, 0x01,                     // USAGE (Pointer)
    0xa1, 0x00,                     // COLLECTION (Physical)
    0x85, MOUSE_REPORT_ID,          // REPORT_ID (1)
    0x05, 0x09,                     // USAGE_PAGE (Button)
    0x19, 0x01,                     // USAGE_MINIMUM (Button 1)
    0x29, 0x03,                     // USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x01,                     // LOGICAL_MAXIMUM (1)
    0x95, 0x03,                     // REPORT_COUNT (3)
    0x75, 0x01,                     // REPORT_SIZE (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x75, 0x05,                     // REPORT_SIZE (5)
    0x81, 0x03,                     // INPUT (Cnst,Var,Abs)
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                     // USAGE (X)
    0x09, 0x31,                     // USAGE (Y)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x7f,               // LOGICAL_MAXIMUM (4095)
    0x35, 0x00,                     // PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x7f,               // PHYSICAL_MAXIMUM (4095)
    0x75, 0x10,                     // REPORT_SIZE (16)
    0x95, 0x02,                     // REPORT_COUNT (2)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x05, 0x0D,                     // USAGE_PAGE (Digitizers)
    0x09, 0x33,                     // USAGE (Touch)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x35, 0x00,                     // PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,               // PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x38,                     // USAGE (Wheel)
    0x15, 0x81,                     // LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                     // LOGICAL_MAXIMUM (127)
    0x35, 0x81,                     // PHYSICAL_MINIMUM (-127)
    0x45, 0x7f,                     // PHYSICAL_MAXIMUM (127)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x06,                     // INPUT (Data,Var,Rel)
    0xc0,                           // END_COLLECTION
    0xc0                            // END_COLLECTION
};

const uint8_t g_UsbDeviceHidGenericReportDescriptor[] = {
 /* USER CODE BEGIN 0 */ 
  0x06, 0x00, 0xff,               /*   USAGE_PAGE (Undefined)                                                          */
  0x09, 0x00,                     /*   USAGE (Undefined)                                                               */
  0xa1, 0x01,                     /*   COLLECTION (Application)                                                        */
  0x85, 0xFD,                     /*   REPORT_ID (253)                                                                 */
  0x06, 0x00, 0xff,               /*   USAGE_PAGE (Undefined)                                                          */
  0x19, 0x01,                     /*   USAGE_MINIMUM                                                                   */
  0x29, 0x3f,                     /*   USAGE_MAXIMUM                                                                   */
  0x15, 0x00,                     /*   LOGICAL_MINIMUM (0)                                                             */
  0x25, 0xff,                     /*   LOGICAL_MAXIMUM (255)                                                           */
  0x75, 0x08,                     /*   REPORT_SIZE (8)                                                                 */
  0x95, 0x3f,                     /*   REPORT_COUNT (63)                                                               */
  0x81, 0x02,                     /*   INPUT (Data,Var,Abs)                                                            */
  0x85, 0xFE,                     /*   REPORT_ID (254)                                                                 */
  0x06, 0x00, 0xff,               /*   USAGE_PAGE (Undefined)                                                          */
  0x19, 0x01,                     /*   USAGE_MINIMUM                                                                   */
  0x29, 0x3f,                     /*   USAGE_MAXIMUM                                                                   */
  0x15, 0x00,                     /*   LOGICAL_MINIMUM (0)                                                             */
  0x25, 0xff,                     /*   LOGICAL_MAXIMUM (255)                                                           */
  0x75, 0x08,                     /*   REPORT_SIZE (8)                                                                 */
  0x95, 0x3f,                     /*   REPORT_COUNT (63)                                                               */
  0x91, 0x02,                     /*   OutPut (Data,Var,Abs)                                                           */
  /* USER CODE END 0 */ 
  0xC0    /*     END_COLLECTION                 */
};

const unsigned char g_cTestBufArray[] =
{   //对管测试用途页 length=60
    0x06, 0x00, 0xff,               // USAGE_PAGE (Undefined)
    0x09, 0x00,                     // USAGE (Undefined)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, BOOT_IN_REPORT_ID,        // REPORT_ID (253)
    0x06, 0x00, 0xff,               // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                     // USAGE (Undefined)
    0x09, 0x02,                     // USAGE (Undefined)
    0x09, 0x03,                     // USAGE (Undefined)
    0x09, 0x04,                     // USAGE (Undefined)
    0x09, 0x05,                     // USAGE (Undefined)
    0x09, 0x06,                     // USAGE (Undefined)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x06,                     // REPORT_COUNT (6)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)

    0x85, BOOT_OUT_REPORT_ID,       // REPORT_ID (254)
    0x06, 0x00, 0xff,               // USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                     // USAGE (Undefined)
    0x09, 0x02,                     // USAGE (Undefined)
    0x09, 0x03,                     // USAGE (Undefined)
    0x09, 0x04,                     // USAGE (Undefined)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x04,                     // REPORT_COUNT (4)
    0xb1, 0x02,                     // FEATURE (Data,Var,Abs)
    0xc0
};

const unsigned char g_nServiceDataArray[] =
{   //PC软件通讯接口 length=62
    0x06, 0x00, 0xff,               // USAGE_PAGE (Undefined)
    0x09, 0x00,                     // USAGE (Undefined)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, FW_IN_REPORT_ID,          // REPORT_ID (0x02)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x09, 0x01,                     // USAGE MIN(1)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x3F,                     // REPORT_COUNT (63)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x01,                     // USAGE MIN(1)
    0x91, 0x02,                     // OUTPUT (Data,Var,Abs)
    0xc0,                           // END_COLLECTION
    0x06, 0x00, 0xff,               // USAGE_PAGE (Undefined)
    0x09, 0x00,                     // USAGE (Undefined)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, FW_OUT_REPORT_ID,         // REPORT_ID (0xfb)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x09, 0x01,                     // USAGE MIN(1)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x3F,                     // REPORT_COUNT (63)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x01,                     // USAGE MIN(1)
    0x91, 0x02,                     // OUTPUT (Data,Var,Abs)
    0xc0,                           // END_COLLECTION
#if (CUSTOMER_REPORT_ID != FW_IN_REPORT_ID)
    0x06, 0x00, 0xff,               // USAGE_PAGE (Undefined)
    0x09, 0x00,                     // USAGE (Undefined)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x85, CUSTOMER_REPORT_ID,       // REPORT_ID (0xfb)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x09, 0x01,                     // USAGE MIN(1)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x3F,                     // REPORT_COUNT (63)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x09, 0x01,                     // USAGE MIN(1)
    0x91, 0x02,                     // OUTPUT (Data,Var,Abs)
    0xc0                            // END_COLLECTION
#endif
};

const uint8_t g_nKeyboardReportDescriptor[] =
{
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                     // USAGE (Keyboard)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x05, 0x07,                     // USAGE_PAGE (Keyboard)
    0x85, KEYBOARD_REPORT_ID,       // REPORT_ID (0x05)

    0x19, 0xe0,                     // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,                     // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x01,                     // LOGICAL_MAXIMUM (1)
    0x75, 0x01,                     // REPORT_SIZE (1)
    0x95, 0x08,                     // REPORT_COUNT (8)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x81, 0x03,                     // INPUT (Cnst,Var,Abs)
    0x95, 0x06,                     // REPORT_COUNT (6)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0xFF,                     // LOGICAL_MAXIMUM (255)
    0x05, 0x07,                     // USAGE_PAGE (Keyboard)
    0x19, 0x00,                     // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,                     // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,                     // INPUT (Data,Ary,Abs)
    0xc0,

    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                     // USAGE (Mouse)
    0xa1, 0x01,                     // COLLECTION (Application)
    0x09, 0x01,                     // USAGE (Pointer)
    0xa1, 0x00,                     // COLLECTION (Physical)
    0x85, MOUSE_REPORT_ID,          // REPORT_ID (1)
    0x05, 0x09,                     // USAGE_PAGE (Button)
    0x19, 0x01,                     // USAGE_MINIMUM (Button 1)
    0x29, 0x03,                     // USAGE_MAXIMUM (Button 3)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x25, 0x01,                     // LOGICAL_MAXIMUM (1)
    0x95, 0x03,                     // REPORT_COUNT (3)
    0x75, 0x01,                     // REPORT_SIZE (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x75, 0x05,                     // REPORT_SIZE (5)
    0x81, 0x03,                     // INPUT (Cnst,Var,Abs)
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                     // USAGE (X)
    0x09, 0x31,                     // USAGE (Y)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x7f,               // LOGICAL_MAXIMUM (4095)
    0x35, 0x00,                     // PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x7f,               // PHYSICAL_MAXIMUM (4095)
    0x75, 0x10,                     // REPORT_SIZE (16)
    0x95, 0x02,                     // REPORT_COUNT (2)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x05, 0x0D,                     // USAGE_PAGE (Digitizers)
    0x09, 0x33,                     // USAGE (Touch)
    0x15, 0x00,                     // LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,               // LOGICAL_MAXIMUM (255)
    0x35, 0x00,                     // PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,               // PHYSICAL_MAXIMUM (255)
    0x75, 0x08,                     // REPORT_SIZE (8)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x02,                     // INPUT (Data,Var,Abs)
    0x05, 0x01,                     // USAGE_PAGE (Generic Desktop)
    0x09, 0x38,                     // USAGE (Wheel)
    0x15, 0x81,                     // LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                     // LOGICAL_MAXIMUM (127)
    0x35, 0x81,                     // PHYSICAL_MINIMUM (-127)
    0x45, 0x7f,                     // PHYSICAL_MAXIMUM (127)
    0x95, 0x01,                     // REPORT_COUNT (1)
    0x81, 0x06,                     // INPUT (Data,Var,Rel)
    0xc0,                           // END_COLLECTION
    0xc0                            // END_COLLECTION
};


/* USB String Descriptors (optional) */
const uint8_t CustomHID_StringLangID[CUSTOMHID_SIZ_STRING_LANGID] =
{
  CUSTOMHID_SIZ_STRING_LANGID,
  USB_DESCRIPTOR_TYPE_STRING,
  0x09,
  0x04
}
; /* LangID = 0x0409: U.S. English */

const uint8_t CustomHID_StringVendor[CUSTOMHID_SIZ_STRING_VENDOR] =
{
  CUSTOMHID_SIZ_STRING_VENDOR, /* Size of Vendor string */
  USB_DESCRIPTOR_TYPE_STRING,             /* bDescriptorType*/
  'C', 0, 'D', 0, 'I', 0, 'T', 0, 'C', 0, ' ', 0, 'C', 0,
  'o', 0, 'd', 0, 'e', 0, 'b', 0, 'u', 0, 'i', 0, 'l', 0, 'd', 0, 's', 0, ' ', 0, 'D', 0, 'r', 0, 'e', 0, 'a', 0, 'm', 0
};

const uint8_t CustomHID_StringProduct[CUSTOMHID_SIZ_STRING_PRODUCT] =
{
  CUSTOMHID_SIZ_STRING_PRODUCT,          /* bLength */
  USB_DESCRIPTOR_TYPE_STRING,        /* bDescriptorType */
  'T', 0, 'o', 0, 'u', 0, 'c', 0, 'h', 0, ' ', 0,
  'D', 0, 'e', 0, 'v', 0, 'i', 0, 'c', 0, 'e', 0
};
const uint8_t CustomHID_StringSerial[CUSTOMHID_SIZ_STRING_SERIAL] =
{
  CUSTOMHID_SIZ_STRING_SERIAL,           /* bLength */
  USB_DESCRIPTOR_TYPE_STRING,            /* bDescriptorType */
  'D', 0, 'r', 0,'i', 0, 'v', 0, 'e', 0, 'r', 0, 'f', 0, 'r', 0, 'e', 0, 'e', 0
};

stReportSection_t g_stUsbKeyboardHidReportDesc = {(uint8_t *)g_nKeyboardReportDescriptor,
                                                  sizeof(g_nKeyboardReportDescriptor)};
stReportSection_t g_stUsbMouseHidReportDesc = {(uint8_t *)g_nMouseDataArray,
                                                sizeof(g_nMouseDataArray)};
stReportSection_t g_stUsbDefaultHidReportDesc = {(uint8_t *)CustomHID_ReportDescriptor,
                                                  sizeof(CustomHID_ReportDescriptor)};
stReportSection_t g_stUsbTouch1HidReportDesc = {(uint8_t *)CustomHID_ReportDescriptor,
                                                 sizeof(CustomHID_ReportDescriptor)};
stReportSection_t g_stUsbInkHidReportDesc = {(uint8_t *)g_nInkDataArray,
                                              sizeof(g_nInkDataArray)};
stReportSection_t g_arrUsbDevDesc[eUsbDevCount] = {(uint8_t *)CustomHID_DeviceDescriptor,
                                                    sizeof(CustomHID_DeviceDescriptor)};
stReportSection_t g_arrUsbCfgDesc[eUsbDevCount] = {(uint8_t *)CustomHID_ConfigDescriptor,
                                                    sizeof(CustomHID_ConfigDescriptor)};
stReportSection_t g_arrHidString[eUsbStrTotal] = {
    {(uint8_t*)CustomHID_StringLangID, CUSTOMHID_SIZ_STRING_LANGID},    ///LangID Pointer and length
    {(uint8_t*)CustomHID_StringVendor, CUSTOMHID_SIZ_STRING_VENDOR},    ///Vendor Pointer and length
    {(uint8_t*)CustomHID_StringProduct, CUSTOMHID_SIZ_STRING_PRODUCT},  ///Product Pointer and length
    {(uint8_t*)CustomHID_StringSerial, CUSTOMHID_SIZ_STRING_SERIAL}};   ///Serial Pointer and length
stReportSection_t *g_arrUsbDescSections[] = {&g_stUsbDefaultHidReportDesc,     //Touch0[] = {&g_stUsbDefaultHidReportDesc,     //Touch0
                                             &g_stUsbKeyboardHidReportDesc,    //Keyboard
                                             &g_stUsbTouch1HidReportDesc,      //Touch1
                                             &g_stUsbMouseHidReportDesc,       //Mouse
                                             &g_stUsbInkHidReportDesc};        //Ink
int32_t g_nReportSectionCount = sizeof(g_arrUsbDescSections)/sizeof(stReportSection_t*);


//保存字符串到flash中
int32_t SaveUsbDescStringFlash(uint8_t *pStr, int32_t nLen, eUsbDescStr_t eUsbDescFlash)
{
    uint8_t arrTmp[FLASH_USB_DESC_LENGTH], *pTmp = (uint8_t *)arrTmp;
    stUsbDescFlash_t *pUsbDescFlash = (stUsbDescFlash_t *)arrTmp;
    const uint16_t arrMaxDescLen[eUsbStrDescTotal] = {32, 32, 256, 256, 800, 800, 800, 800,
                                                      700,
                                                      64, 64, 192, 192, 128, 128};
    uint32_t nTotalDescLen = FLASH_USB_DESC_LENGTH-sizeof(stUsbDescFlash_t);
    uint8_t *pTotalCrcData = &pTmp[sizeof(stUsbDescFlash_t)];
    int32_t bReset = 0;
 
    for (int32_t i = 0; !bReset && i < eUsbStrDescTotal; i++)
    {
        if (sg_pUsbDescFlash->arrDesc[i].nMaxLen != arrMaxDescLen[i])
        {
            bReset = 1;
        }
    }
    if (bReset || sg_pUsbDescFlash->nThisCrc32 != GetCrc32_SW((uint8_t *)sg_pUsbDescFlash, offsetof(stUsbDescFlash_t, nThisCrc32)))
    {
        MemSet((void *)pUsbDescFlash, 0, sizeof(stUsbDescFlash_t));
        for (eUsbDescStr_t eUsbIdx = eUsbDev0Desc; eUsbIdx < eUsbStrDescTotal; eUsbIdx++)
        {
            eUsbDescStr_t eLstUsbIdx = FN_MAX(eUsbDev0Desc, (eUsbDescStr_t)(eUsbIdx-1));
            pUsbDescFlash->arrDesc[eUsbIdx].nOffset = eUsbIdx == eUsbDev0Desc ?
                    sizeof(stUsbDescFlash_t) : (pUsbDescFlash->arrDesc[eLstUsbIdx].nOffset + pUsbDescFlash->arrDesc[eLstUsbIdx].nMaxLen);
            pUsbDescFlash->arrDesc[eUsbIdx].nMaxLen = arrMaxDescLen[eUsbIdx];
        }
        pUsbDescFlash->nThisCrc32 = GetCrc32_SW((uint8_t *)pUsbDescFlash, offsetof(stUsbDescFlash_t, nThisCrc32));

        if (status_success != SaveDataList(eUsbDescData, (uint8_t *)pUsbDescFlash, sizeof(stUsbDescFlash_t)))
        {
            printf("SaveUsbDescStringFlash default error!\n");
            return -1;
        }
    }
    MemCpy(pTmp, sg_pUsbDescFlash, FLASH_USB_DESC_LENGTH);
    if (nLen > pUsbDescFlash->arrDesc[eUsbDescFlash].nMaxLen)
    {
        printf("Warning->SaveUsbDesc eUsbDesc:%d, max:%d, %d\n", eUsbDescFlash, pUsbDescFlash->arrDesc[eUsbDescFlash].nMaxLen, nLen);
    }
    nLen = FN_MIN(nLen, pUsbDescFlash->arrDesc[eUsbDescFlash].nMaxLen);
    pUsbDescFlash->arrDesc[eUsbDescFlash].nLen = nLen;
    uint8_t *pStrCpy = (uint8_t *)&pTmp[pUsbDescFlash->arrDesc[eUsbDescFlash].nOffset];
    MemCpy(pStrCpy, pStr, nLen);

    if (nLen != sg_pUsbDescFlash->arrDesc[eUsbDescFlash].nLen ||
            sg_pUsbDescFlash->nTotalCrc32 != GetCrc32_SW(pTotalCrcData, nTotalDescLen))
    {
//        printf("UsbDescStringFlash nTotalCrc32(0x%08x) != pTmp crc!(0x%08x)\n", sg_pUsbDescFlash->nTotalCrc32, GetCrc32_SW(pTotalCrcData, nTotalDescLen));
        pUsbDescFlash->nTotalCrc32 = GetCrc32_SW(pTotalCrcData, nTotalDescLen);
        pUsbDescFlash->nThisCrc32 = GetCrc32_SW((uint8_t *)pUsbDescFlash, offsetof(stUsbDescFlash_t, nThisCrc32));
        if (status_success != SaveDataList(eUsbDescData, (uint8_t *)pTmp, FLASH_USB_DESC_LENGTH))
        {
            printf("SaveUsbDescStringFlash error!\n");
            return -1;
        }
    }
    return nLen;
}


/**
  * 初始化设备描述符
  */ 
static int32_t InitUSBDevDesc(stUsbEnumInfo_t stEnumInfo)
{
    uint8_t arrDevDesc[CUSTOMHID_SIZ_DEVICE_DESC];

    MemCpy(arrDevDesc, CustomHID_DeviceDescriptor, sizeof(arrDevDesc));

    arrDevDesc[8] = FN_LOBYTE(stEnumInfo.nUSBVid);
    arrDevDesc[9] = FN_HIBYTE(stEnumInfo.nUSBVid);

    arrDevDesc[10] = FN_LOBYTE(stEnumInfo.nUSBPid);
    arrDevDesc[11] = FN_HIBYTE(stEnumInfo.nUSBPid);

    arrDevDesc[12] = FN_LOBYTE(stEnumInfo.nVersion);
    arrDevDesc[13] = FN_HIBYTE(stEnumInfo.nVersion);

    eUsbDescStr_t eDescStr = (eUsbDescStr_t)(eUsbDev0Desc + stEnumInfo.eUsbDev);
    if (!(SaveUsbDescStringFlash(arrDevDesc, sizeof(arrDevDesc), eDescStr) < 0))
    {
        uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
        g_arrUsbDevDesc[stEnumInfo.eUsbDev].Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eDescStr].nOffset];
        g_arrUsbDevDesc[stEnumInfo.eUsbDev].Descriptor_Size = sg_pUsbDescFlash->arrDesc[eDescStr].nLen;
    }
    return 0;
}

//初始化接口描述符
__attribute__((weak)) int32_t InitHidConfigDesc(stUsbEnumInfo_t stEnumInfo, stReportSection_t **ppReportDesc, int32_t nCount)
{
    eUsbCfgType_t eType = stEnumInfo.eUsbCfgType;
    uint8_t arrUsbHidConfigDesc[CUSTOMHID_SIZ_CONFIG_DESC];
    uint8_t *pUsbHidConfigDesc = arrUsbHidConfigDesc;
    uint8_t nInterFaceCount = 0;
    uint16_t nSize = 9;
    int32_t nEpsCount = 0, nEpsPhyOffset = ENDPx_START_ADDR;
    eReportSectionID_t eReportSID = eTouchCommReportSection;
    eUsbDevice_t eUsbDev = stEnumInfo.eUsbDev;

    MemCpy(arrUsbHidConfigDesc, CustomHID_ConfigDescriptor, sizeof(CustomHID_ConfigDescriptor));
    MemSet(&g_stUsbEpsInfo[eUsbDev], 0, sizeof(stUsbEpsInfo_t));

    eReportSID = eTouchCommReportSection;
    MemCpy(&pUsbHidConfigDesc[nSize], CustomHID_ConfigDescriptor_Touch, sizeof(CustomHID_ConfigDescriptor_Touch));
    pUsbHidConfigDesc[nSize + 2] = nInterFaceCount++;
    pUsbHidConfigDesc[nSize + 9 + 7] = FN_LOBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
    pUsbHidConfigDesc[nSize + 9 + 8] = FN_HIBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
    g_arrHidReportMap[pUsbHidConfigDesc[nSize + 2]] = eReportSID; //建立HID描述符和报告描述符的映射关系
    nSize += sizeof(CustomHID_ConfigDescriptor_Touch);

    //初始化以上接口的相关端点属性
    nEpsPhyOffset = InitAEpInfo(eUsbDev, nEpsCount++, USB_ENDPOINT_TYPE_INTERRUPT, CUSTOM_TOUCH0_EP_IN_SIZE,
                                nEpsPhyOffset, CUSTOM_TOUCH0_EP_IN_ADDR, CUSTOM_TOUCH0_EP_OUT_ADDR,
                                USB_TOUCH_COMM_QUEUE_SIZE, CUSTOM_TOUCH0_EP_IN_SIZE, eUsbCfgTouch0CommBit);

    if (eType != eUsbCfgAppDefault && eType != eUsbCfgBootDefault)
    {
        if ((uint8_t)(eType & eUsbCfgBulkBit) == 0 && 1 == MOUSE_INTERFACE_STANDALONE) //非bulk接口下打开mouse
        {
            eReportSID = eMouseReportSection; //Mouse默认带上
            MemCpy(&pUsbHidConfigDesc[nSize], CustomHID_ConfigDescriptor_Mouse, sizeof(CustomHID_ConfigDescriptor_Mouse));
            pUsbHidConfigDesc[nSize + 2] = nInterFaceCount++;
            pUsbHidConfigDesc[nSize + 9 + 7] = FN_LOBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            pUsbHidConfigDesc[nSize + 9 + 8] = FN_HIBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            g_arrHidReportMap[pUsbHidConfigDesc[nSize + 2]] = eReportSID; //建立HID描述符和报告描述符的映射关系
            nSize += sizeof(CustomHID_ConfigDescriptor_Mouse);

            //初始化以上接口的相关端点属性
            nEpsPhyOffset = InitAEpInfo(eUsbDev, nEpsCount++, USB_ENDPOINT_TYPE_INTERRUPT, CUSTOM_MOUSE_EP_IN_SIZE,
                                        nEpsPhyOffset, CUSTOM_MOUSE_EP_IN_ADDR, CUSTOM_MOUSE_EP_OUT_ADDR,
                                        USB_MOUSE_QUEUE_SIZE, CUSTOM_MOUSE_EP_IN_SIZE, eUsbCfgMouseBit);
        }
        else if ((uint8_t)(eType & eUsbCfgBulkBit) != 0)
        {
            MemCpy(&pUsbHidConfigDesc[nSize], CustomHID_ConfigDescriptor_Bulk, sizeof(CustomHID_ConfigDescriptor_Bulk));
            pUsbHidConfigDesc[nSize + 2] = nInterFaceCount++;
            nSize += sizeof(CustomHID_ConfigDescriptor_Bulk);

            //初始化以上接口的相关端点属性
            nEpsPhyOffset = InitAEpInfo(eUsbDev, nEpsCount++, USB_ENDPOINT_TYPE_BULK, CUSTOM_BULK_EP_IN_SIZE,
                                        nEpsPhyOffset, CUSTOM_BULK_EP_IN_ADDR, CUSTOM_BULK_EP_OUT_ADDR,
                                        USB_BULK_QUEUE_SIZE, CUSTOM_BULK_EP_IN_SIZE, eUsbCfgBulkBit);
        }

        if ((uint8_t)(eType & eUsbCfgTouch1Bit) != 0)
        {
            eReportSID = eTouch1ReportSection;
            MemCpy(&pUsbHidConfigDesc[nSize], CustomHID_ConfigDescriptor_Touch1, sizeof(CustomHID_ConfigDescriptor_Touch1));
            pUsbHidConfigDesc[nSize + 2] = nInterFaceCount++;
            pUsbHidConfigDesc[nSize + 9 + 7] = FN_LOBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            pUsbHidConfigDesc[nSize + 9 + 8] = FN_HIBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            g_arrHidReportMap[pUsbHidConfigDesc[nSize + 2]] = eReportSID; //建立HID描述符和报告描述符的映射关系
            nSize += sizeof(CustomHID_ConfigDescriptor_Touch1);

            //初始化以上接口的相关端点属性
            nEpsPhyOffset = InitAEpInfo(eUsbDev, nEpsCount++, USB_ENDPOINT_TYPE_INTERRUPT, CUSTOM_TOUCH1_EP_IN_SIZE,
                                        nEpsPhyOffset, CUSTOM_TOUCH1_EP_IN_ADDR, CUSTOM_TOUCH1_EP_OUT_ADDR,
                                        USB_TOUCH_1_QUEUE_SIZE, CUSTOM_TOUCH1_EP_IN_SIZE, eUsbCfgTouch1Bit);
        }

        if ((uint8_t)(eType & eUsbCfgKeyboardBit) != 0)
        {
            eReportSID = eKeyboardReportSection;
            MemCpy(&pUsbHidConfigDesc[nSize], CustomHID_ConfigDescriptor_KeyBoard, sizeof(CustomHID_ConfigDescriptor_KeyBoard));
            pUsbHidConfigDesc[nSize + 2] = nInterFaceCount++;
            pUsbHidConfigDesc[nSize + 9 + 7] = FN_LOBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            pUsbHidConfigDesc[nSize + 9 + 8] = FN_HIBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            g_arrHidReportMap[pUsbHidConfigDesc[nSize + 2]] = eReportSID; //建立HID描述符和报告描述符的映射关系
            nSize += sizeof(CustomHID_ConfigDescriptor_KeyBoard);

            //初始化以上接口的相关端点属性
            nEpsPhyOffset = InitAEpInfo(eUsbDev, nEpsCount++, USB_ENDPOINT_TYPE_INTERRUPT, CUSTOM_KEYBOARD_EP_IN_SIZE,
                                        nEpsPhyOffset, CUSTOM_KEYBOARD_EP_IN_ADDR, CUSTOM_KEYBOARD_EP_OUT_ADDR,
                                        USB_KEYBOARD_QUEUE_SIZE, CUSTOM_KEYBOARD_EP_IN_SIZE, eUsbCfgKeyboardBit);
        }
        if ((uint8_t)(eType & eUsbCfgInkBit) != 0)
        {
            eReportSID = eInkReportSection;
            MemCpy(&pUsbHidConfigDesc[nSize], CustomHID_ConfigDescriptor_Ink, sizeof(CustomHID_ConfigDescriptor_Ink));
            pUsbHidConfigDesc[nSize + 2] = nInterFaceCount++;
            pUsbHidConfigDesc[nSize + 9 + 7] = FN_LOBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            pUsbHidConfigDesc[nSize + 9 + 8] = FN_HIBYTE(ppReportDesc[eReportSID]->Descriptor_Size);
            g_arrHidReportMap[pUsbHidConfigDesc[nSize + 2]] = eReportSID; //建立HID描述符和报告描述符的映射关系
            nSize += sizeof(CustomHID_ConfigDescriptor_Ink);

            //初始化以上接口的相关端点属性
            nEpsPhyOffset = InitAEpInfo(eUsbDev, nEpsCount++, USB_ENDPOINT_TYPE_INTERRUPT, CUSTOM_INK_EP_IN_SIZE,
                                        nEpsPhyOffset, CUSTOM_INK_EP_IN_ADDR, CUSTOM_INK_EP_OUT_ADDR,
                                        USB_INK_QUEUE_SIZE, CUSTOM_INK_EP_IN_SIZE, eUsbCfgInkBit);
        }
    }
    g_stUsbEpsInfo[eUsbDev].nEpsCount = nEpsCount;

    pUsbHidConfigDesc[4] = nInterFaceCount;
    pUsbHidConfigDesc[2] = FN_LOBYTE(nSize);
    pUsbHidConfigDesc[3] = FN_HIBYTE(nSize);

    eUsbDescStr_t eDescStr = (eUsbDescStr_t)(eUsbDev0CfgDesc + eUsbDev);
    if (!(SaveUsbDescStringFlash(pUsbHidConfigDesc, nSize, eDescStr) < 0))
    {
        uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
        g_arrUsbCfgDesc[stEnumInfo.eUsbDev].Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eDescStr].nOffset];
        g_arrUsbCfgDesc[stEnumInfo.eUsbDev].Descriptor_Size = sg_pUsbDescFlash->arrDesc[eDescStr].nLen;
    }
    return 0;
}


#if TOUCH_V2_REPORT_TYPE_EN
//初始化报告描述符
static int32_t InitHidTouch1ReportDesc(stReportSection_t *pReportSection, stUsbEnumInfo_t stEnumInfo)
{
    eUsbCfgType_t eType = stEnumInfo.eUsbCfgType;
    if((uint8_t)(eType & eUsbCfgTouch1Bit) == 0 ||  //Touch1接口
            eUsbCfgBootDefault == eType || eUsbCfgAppDefault == eType)
    {
        return 0;
    }
    int32_t nWidth = stEnumInfo.nPhysicalX;
    int32_t nHeight = stEnumInfo.nPhysicalY;
    uint8_t arrUsbHidReportDes[2048];
    int32_t nCount = 0;
    uint8_t arrWindowDataArray[1024];
    int32_t nWindowDataArrayLen = FN_MIN(sizeof(g_arrMultiPointsRepData), sizeof(arrWindowDataArray));

    MemCpy(arrWindowDataArray, g_arrMultiPointsRepData, nWindowDataArrayLen);

    arrWindowDataArray[54] = FN_BYTE(nWidth, 0);
    arrWindowDataArray[55] = FN_BYTE(nWidth, 1);
    arrWindowDataArray[64] = FN_BYTE(nHeight, 0);
    arrWindowDataArray[65] = FN_BYTE(nHeight, 1);

    arrWindowDataArray[73] = FN_BYTE(nWidth, 0);
    arrWindowDataArray[74] = FN_BYTE(nWidth, 1);
    arrWindowDataArray[76] = FN_BYTE(nWidth, 0);
    arrWindowDataArray[77] = FN_BYTE(nWidth, 1);

    arrWindowDataArray[83] = FN_BYTE(nHeight, 0);
    arrWindowDataArray[84] = FN_BYTE(nHeight, 1);
    arrWindowDataArray[86] = FN_BYTE(nHeight, 0);
    arrWindowDataArray[87] = FN_BYTE(nHeight, 1);

#if (TOUCH_PRESSURE_LEVELS > 0)
    arrWindowDataArray[99] = FN_BYTE(TOUCH_PRESSURE_LEVELS-1, 0);
    arrWindowDataArray[100] = FN_BYTE(TOUCH_PRESSURE_LEVELS-1, 1);
#endif
    //Window的头信息
    MemCpy(arrUsbHidReportDes + nCount, g_arrMultiPointsRepDataHead, sizeof(g_arrMultiPointsRepDataHead));
    nCount += sizeof(g_arrMultiPointsRepDataHead);

    //Window重复4次的报告描述符
    for(int32_t i = 0; i < 4; i++)
    {
        MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray, sizeof(g_arrMultiPointsRepData));
        nCount += sizeof(g_arrMultiPointsRepData);
    }

    //Window的结束信息
    MemCpy(arrUsbHidReportDes + nCount, g_arrMultiPointsRepDataEnd,sizeof(g_arrMultiPointsRepDataEnd));
    nCount += sizeof(g_arrMultiPointsRepDataEnd);

    eUsbDescStr_t eDescStr = (eUsbDescStr_t)(eUsbDev0HidTouch1Desc + stEnumInfo.eUsbDev);
    if (!(SaveUsbDescStringFlash(arrUsbHidReportDes, nCount, eDescStr) < 0))
    {
        uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
        pReportSection->Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eDescStr].nOffset];
        pReportSection->Descriptor_Size = sg_pUsbDescFlash->arrDesc[eDescStr].nLen;
    }
    return nCount;
}
#else
//初始化报告描述符
static int32_t InitHidTouch1ReportDesc(stReportSection_t *pReportSection, stUsbEnumInfo_t stEnumInfo)
{
    eUsbCfgType_t eType = stEnumInfo.eUsbCfgType;
    if((uint8_t)(eType & eUsbCfgTouch1Bit) == 0 ||  //Touch1接口
            eUsbCfgBootDefault == eType || eUsbCfgAppDefault == eType)
    {
        return 0;
    }
    int32_t nWidth = stEnumInfo.nPhysicalX;
    int32_t nHeight = stEnumInfo.nPhysicalY;
    int32_t bWidthEn = stEnumInfo.bTouchCoordWidthEn;
    uint8_t arrUsbHidReportDes[1024];
    int32_t nCount = 0;
    uint8_t arrWindowDataArray_1[1024];
    uint8_t arrWindowDataArray[1024];
    int32_t nWindowDataArray_1Len = FN_MIN(sizeof(g_nWindowDataArray_1), sizeof(arrWindowDataArray_1));
    int32_t nWindowDataArrayLen = FN_MIN(sizeof(g_nWindowDataArray), sizeof(arrWindowDataArray));

    MemCpy(arrWindowDataArray_1, g_nWindowDataArray_1, nWindowDataArray_1Len);
    MemCpy(arrWindowDataArray, g_nWindowDataArray, nWindowDataArrayLen);

    arrWindowDataArray_1[52] = FN_BYTE(nWidth, 0);
    arrWindowDataArray_1[53] = FN_BYTE(nWidth, 1);
    arrWindowDataArray_1[57] = FN_BYTE(nHeight, 0);
    arrWindowDataArray_1[58] = FN_BYTE(nHeight, 1);

    arrWindowDataArray[54] = FN_BYTE(nWidth, 0);
    arrWindowDataArray[55] = FN_BYTE(nWidth, 1);
    arrWindowDataArray[59] = FN_BYTE(nHeight, 0);
    arrWindowDataArray[60] = FN_BYTE(nHeight, 1);

    //Window的头信息
    MemCpy(arrUsbHidReportDes + nCount, g_nWindowHeadDataArray, sizeof(g_nWindowHeadDataArray));
    nCount += sizeof(g_nWindowHeadDataArray);

    //Window重复一次的报告描述符
    for(int32_t i = 0; i < 1; i++)
    {
        if(bWidthEn == 1)
        {
            MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray_1, nWindowDataArray_1Len);
            nCount += nWindowDataArray_1Len;
        }
        else
        {
            MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray_1, nWindowDataArray_1Len - 11);
            nCount += (nWindowDataArray_1Len - 11);
            arrUsbHidReportDes[nCount] = arrWindowDataArray_1[nWindowDataArray_1Len - 1];
            nCount++;
        }
    }
    //Window重复五次的报告描述符
    for(int32_t i = 0; i < 5; i++)
    {
        if(bWidthEn == 1)
        {
            MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray, nWindowDataArrayLen);
            nCount += nWindowDataArrayLen;
        }
        else
        {
            MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray, nWindowDataArrayLen - 11);
            nCount += (nWindowDataArrayLen - 11);
            arrUsbHidReportDes[nCount] = arrWindowDataArray[nWindowDataArrayLen - 1];
            nCount++;
        }
    }

    //Window的结束信息
    MemCpy(arrUsbHidReportDes + nCount,g_nWindowEndDataArray,sizeof(g_nWindowEndDataArray));
    nCount += sizeof(g_nWindowEndDataArray);

    eUsbDescStr_t eDescStr = (eUsbDescStr_t)(eUsbDev0HidTouch1Desc + stEnumInfo.eUsbDev);
    if (!(SaveUsbDescStringFlash(arrUsbHidReportDes, nCount, eDescStr) < 0))
    {
        uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
        pReportSection->Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eDescStr].nOffset];
        pReportSection->Descriptor_Size = sg_pUsbDescFlash->arrDesc[eDescStr].nLen;
    }
    return nCount;
}
#endif


//初始化Ink报告描述符
static int32_t InitHidInkReportDesc(stReportSection_t *pReportSection, stUsbEnumInfo_t stEnumInfo)
{
    eUsbCfgType_t eType = stEnumInfo.eUsbCfgType;
    if((uint8_t)(eType & eUsbCfgInkBit) == 0 ||   //Windows Ink协议
            eUsbCfgBootDefault == eType || eUsbCfgAppDefault == eType)
    {
        return 0;
    }
    int32_t nWidth = stEnumInfo.nPhysicalX;
    int32_t nHeight = stEnumInfo.nPhysicalY;
    uint8_t arrUsbHidReportDes[1024], *pHidReportDesc = arrUsbHidReportDes;
    int32_t nCount = 0;
    uint8_t arrInkBufArray[512] = {0};

    MemCpy(arrInkBufArray, g_nInkDataArray, sizeof(g_nInkDataArray));
#if (SYSTEM_SUPPORT_INK == SYSTEM_SUPPORT_INK_V1)
    arrInkBufArray[60] = FN_BYTE(nWidth, 0);
    arrInkBufArray[61] = FN_BYTE(nWidth, 1);
    arrInkBufArray[70] = FN_BYTE(nHeight, 0);
    arrInkBufArray[71] = FN_BYTE(nHeight, 1);
    arrInkBufArray[80] = FN_BYTE(nWidth, 0);
    arrInkBufArray[81] = FN_BYTE(nWidth, 1);
    arrInkBufArray[90] = FN_BYTE(nHeight, 0);
    arrInkBufArray[91] = FN_BYTE(nHeight, 1);
#elif (SYSTEM_SUPPORT_INK == SYSTEM_SUPPORT_INK_V2)
    arrInkBufArray[58] = FN_BYTE(nWidth, 0);
    arrInkBufArray[59] = FN_BYTE(nWidth, 1);
    arrInkBufArray[68] = FN_BYTE(nHeight, 0);
    arrInkBufArray[69] = FN_BYTE(nHeight, 1);
#endif

#if (TOUCH_PRESSURE_LEVELS > 0)
#if (SYSTEM_SUPPORT_INK == SYSTEM_SUPPORT_INK_V1)
    arrInkBufArray[103] = FN_BYTE(TOUCH_PRESSURE_LEVELS-1, 0);
    arrInkBufArray[104] = FN_BYTE(TOUCH_PRESSURE_LEVELS-1, 1);
#elif (SYSTEM_SUPPORT_INK == SYSTEM_SUPPORT_INK_V2)
    arrInkBufArray[81] = FN_BYTE(TOUCH_PRESSURE_LEVELS-1, 0);
    arrInkBufArray[82] = FN_BYTE(TOUCH_PRESSURE_LEVELS-1, 1);
#endif
#endif

    for (int32_t i = 0; i < MAX_INK_PENS; i++)
    {
        arrInkBufArray[7] = INK_REPORT_ID + i;
        MemCpy(&pHidReportDesc[nCount], arrInkBufArray, sizeof(g_nInkDataArray));
        nCount += sizeof(g_nInkDataArray);
    }

    eUsbDescStr_t eDescStr = eUsbInkReportDesc;
    if (!(SaveUsbDescStringFlash(arrUsbHidReportDes, nCount, eDescStr) < 0))
    {
        uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
        pReportSection->Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eDescStr].nOffset];
        pReportSection->Descriptor_Size = sg_pUsbDescFlash->arrDesc[eDescStr].nLen;
    }
    return nCount;
}

//初始化报告描述符
int32_t InitHidDefaultReportDesc(stReportSection_t *pReportSection, stUsbEnumInfo_t stEnumInfo)
{
    int32_t nWidth = stEnumInfo.nPhysicalX;
    int32_t nHeight = stEnumInfo.nPhysicalY;
    int32_t bWidthEn = stEnumInfo.bTouchCoordWidthEn;
    eUsbCfgType_t eType = stEnumInfo.eUsbCfgType;
    uint8_t arrUsbHidReportDes[1024];
    int32_t nCount = 0;
    uint8_t arrWindowDataArray_1[1024];
    uint8_t arrWindowDataArray[1024];
    int32_t nWindowDataArray_1Len = FN_MIN(sizeof(g_nWindowDataArray_1), sizeof(arrWindowDataArray_1));
    int32_t nWindowDataArrayLen = FN_MIN(sizeof(g_nWindowDataArray), sizeof(arrWindowDataArray));

    MemCpy(arrWindowDataArray_1, g_nWindowDataArray_1, nWindowDataArray_1Len);
    MemCpy(arrWindowDataArray, g_nWindowDataArray, nWindowDataArrayLen);
    if(eType == eUsbCfgAppDefault)
    {
        MemCpy(&arrUsbHidReportDes[nCount], g_nMouseDataArray, sizeof(g_nMouseDataArray));
        nCount += sizeof(g_nMouseDataArray);
    }
    else if(eType == eUsbCfgBootDefault)
    {
        MemCpy(&arrUsbHidReportDes[nCount], g_UsbDeviceHidGenericReportDescriptor, sizeof(g_UsbDeviceHidGenericReportDescriptor));
        nCount = sizeof(g_UsbDeviceHidGenericReportDescriptor);
    }
    else
    {
        MemCpy(&arrUsbHidReportDes[nCount], g_UsbDeviceHidGenericReportDescriptor, sizeof(g_UsbDeviceHidGenericReportDescriptor));
        nCount = sizeof(g_UsbDeviceHidGenericReportDescriptor);

        MemCpy(&arrUsbHidReportDes[nCount], g_nMouseDataArray, sizeof(g_nMouseDataArray));
        nCount += sizeof(g_nMouseDataArray);

        MemCpy(&arrUsbHidReportDes[nCount], g_nServiceDataArray, sizeof(g_nServiceDataArray));
        nCount += sizeof(g_nServiceDataArray);
        
        arrWindowDataArray_1[52] = FN_BYTE(nWidth, 0);
        arrWindowDataArray_1[53] = FN_BYTE(nWidth, 1);
        arrWindowDataArray_1[57] = FN_BYTE(nHeight, 0);
        arrWindowDataArray_1[58] = FN_BYTE(nHeight, 1);

        arrWindowDataArray[54] = FN_BYTE(nWidth, 0);
        arrWindowDataArray[55] = FN_BYTE(nWidth, 1);
        arrWindowDataArray[59] = FN_BYTE(nHeight, 0);
        arrWindowDataArray[60] = FN_BYTE(nHeight, 1);

        //Window的头信息
        MemCpy(arrUsbHidReportDes + nCount, g_nWindowHeadDataArray, sizeof(g_nWindowHeadDataArray));
        nCount += sizeof(g_nWindowHeadDataArray);

        //Window重复一次的报告描述符
        for(int32_t i = 0; i < 1; i++)
        {
            if(bWidthEn == 1)
            {
                MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray_1, nWindowDataArray_1Len);
                nCount += nWindowDataArray_1Len;
            }
            else
            {
                MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray_1, nWindowDataArray_1Len - 11);
                nCount += (nWindowDataArray_1Len - 11);
                arrUsbHidReportDes[nCount] = arrWindowDataArray_1[nWindowDataArray_1Len - 1];
                nCount++;
            }
        }
        //Window重复五次的报告描述符
        for(int32_t i = 0; i < 5; i++)
        {
            if(bWidthEn == 1)
            {
                MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray, nWindowDataArrayLen);
                nCount += nWindowDataArrayLen;
            }
            else
            {
                MemCpy(&arrUsbHidReportDes[nCount], arrWindowDataArray, nWindowDataArrayLen - 11);
                nCount += (nWindowDataArrayLen - 11);
                arrUsbHidReportDes[nCount] = arrWindowDataArray[nWindowDataArrayLen - 1];
                nCount++;
            }
        }
        //Window的结束信息
        MemCpy(arrUsbHidReportDes + nCount,g_nWindowEndDataArray,sizeof(g_nWindowEndDataArray));
        nCount += sizeof(g_nWindowEndDataArray);
    }

    eUsbDescStr_t eDescStr = (eUsbDescStr_t)(eUsbDev0HidTouch0Desc + stEnumInfo.eUsbDev);
    if (!(SaveUsbDescStringFlash(arrUsbHidReportDes, nCount, eDescStr) < 0))
    {
        uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
        pReportSection->Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eDescStr].nOffset];
        pReportSection->Descriptor_Size = sg_pUsbDescFlash->arrDesc[eDescStr].nLen;
    }
    return nCount;
}

//初始化USB各类描述符
void InitUSBDesc(uint32_t nPoints, stUsbEnumInfo_t stEnumInfo)
{
    g_nUserPoints = nPoints;
    InitUSBDevDesc(stEnumInfo);
    InitHidDefaultReportDesc(&g_stUsbDefaultHidReportDesc, stEnumInfo);
    InitHidTouch1ReportDesc(&g_stUsbTouch1HidReportDesc, stEnumInfo);
    InitHidInkReportDesc(&g_stUsbInkHidReportDesc, stEnumInfo);
    InitHidConfigDesc(stEnumInfo, g_arrUsbDescSections, sizeof(g_arrUsbDescSections)/sizeof(stReportSection_t*));
}


/**
  * @brief  USBD_GetLen
  *         return the string length
   * @param  buf : pointer to the ascii string buffer
  * @retval string length
  */
static uint8_t USBD_GetLen(uint8_t *buf)
{
    uint8_t  len = 0;
    while (*buf != '\0')
    {
        len++;
        buf++;
    }
    return len;
}

/**
  * @brief  USBD_GetString
  *         Convert Ascii string into unicode one
  * @param  desc : descriptor buffer
  * @param  unicode : Formatted string buffer (unicode)
  * @param  len : descriptor length
  * @retval None
  */
static void USBD_GetString(uint8_t *desc, uint8_t *unicode, uint16_t *len)
{
    uint8_t idx = 0;

    if (desc != NULL)
    {
        *len =  USBD_GetLen(desc) * 2 + 2;
        unicode[idx++] = *len;
        unicode[idx++] =  USB_DESCRIPTOR_TYPE_STRING;
        while (*desc != '\0')
        {
            unicode[idx++] = *desc++;
            unicode[idx++] =  0x00;
        }
    }
}

/*
 * 初始化USB相关的字串信息
 * @para pString: 字符串指针
 * @para nLen: 字符串长度
 * @para nIndex: 字符串索引
 * @return: 正确返回0，错误返回负数
 */
int32_t InitUsbStringUnicode(uint8_t* pString, uint8_t *pSrcMemStr, eUsbStrType_t eUsbStrType, eUsbDevice_t eUsbDev)
{
    eUsbDescStr_t arrDescFlashMap[] = {(eUsbDescStr_t)eUsbStrLangID, eUsbDev0VendorStrDesc, eUsbDev0ProductStrDesc, eUsbDev0SNStrDesc,
                                       (eUsbDescStr_t)eUsbStrLangID, eUsbDev1VendorStrDesc, eUsbDev1ProductStrDesc, eUsbDev1SNStrDesc};
    uint16_t nLen = 0;
    if (eUsbStrType < eUsbStrTotal)
    {
        USBD_GetString(pString, pSrcMemStr, &nLen);
        pSrcMemStr[nLen] = '\0';

        eUsbDescStr_t eUsbDescFlash = arrDescFlashMap[(eUsbDev == eUsbDev0) ? eUsbStrType : (eUsbStrType + eUsbStrTotal)];

        if (!(SaveUsbDescStringFlash(pSrcMemStr, nLen, eUsbDescFlash) < 0))
        {
            uint8_t *pFlashStr = (uint8_t *)sg_pUsbDescFlash;
            g_arrHidString[eUsbStrType].Descriptor = (uint8_t *)&pFlashStr[sg_pUsbDescFlash->arrDesc[eUsbDescFlash].nOffset];
            g_arrHidString[eUsbStrType].Descriptor_Size = sg_pUsbDescFlash->arrDesc[eUsbDescFlash].nLen;
        }
    }
    return nLen;
}



