/*
 * Copyright (c) 2022 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "usbd_core.h"
#include "usbd_hid.h"
#include "user_config.h"


#if !(USBD_BOOT_TEST || USBD_APP_TEST)
void usbd_configure_done_callback(void)
{
    ;
}
#endif

#if USBD_BOOT_TEST
/*!< hidraw in endpoint */
#define BOOT_IN_EP                      0x81
#define BOOT_IN_SIZE                    64
#define BOOT_IN_INTERVAL                1

/*!< hidraw out endpoint */
#define BOOT_OUT_EP                     0x01
#define BOOT_OUT_EP_SIZE                64
#define BOOT_OUT_EP_INTERVAL            1

#define BOOT_USBD_VID                   0x1FF7
#define BOOT_USBD_PID                   0x0F0A
#define BOOT_USBD_MAX_POWER             100
#define BOOT_USBD_LANGID_STRING         1033

/*!< config descriptor size */
#define BOOT_HID_CONFIG_DESC_SIZ        (9 + 9 + 9 + 7 + 7)

/*!< custom hid report descriptor size */
#define BOOT_HID_REPORT_DESC_SIZE       46

/*!< global descriptor */
static const uint8_t boot_hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, BOOT_USBD_VID, BOOT_USBD_PID, 0x0002, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(BOOT_HID_CONFIG_DESC_SIZ, 0x01, 0x01, USB_CONFIG_BUS_POWERED, BOOT_USBD_MAX_POWER),
    /************** Descriptor of Custom interface *****************/
    0x09,                           /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE,  /* bDescriptorType: Interface descriptor type */
    0x00,                           /* bInterfaceNumber: Number of Interface */
    0x00,                           /* bAlternateSetting: Alternate setting */
    0x02,                           /* bNumEndpoints */
    0x03,                           /* bInterfaceClass: HID */
    0x00,                           /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                           /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                              /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID ********************/
    0x09,                           /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID,        /* bDescriptorType: HID */
    0x00,                           /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                           /* bCountryCode: Hardware target country */
    0x01,                           /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                           /* bDescriptorType */
    BOOT_HID_REPORT_DESC_SIZE,      /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom in endpoint ********************/
    0x07,                           /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* bDescriptorType: */
    BOOT_IN_EP,                     /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                           /* bmAttributes: Interrupt endpoint */
    WBVAL(BOOT_IN_SIZE),            /* wMaxPacketSize: 4 Byte max */
    BOOT_IN_INTERVAL,               /* bInterval: Polling Interval */
    /******************** Descriptor of Custom out endpoint ********************/
    0x07,                           /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT,   /* bDescriptorType: */
    BOOT_OUT_EP,                    /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                           /* bmAttributes: Interrupt endpoint */
    WBVAL(BOOT_OUT_EP_SIZE),        /* wMaxPacketSize: 4 Byte max */
    BOOT_OUT_EP_INTERVAL,           /* bInterval: Polling Interval */
    /* 73 */
    /*
     * string0 descriptor
     */
    USB_LANGID_INIT(BOOT_USBD_LANGID_STRING),
    /*
     * string1 descriptor
     */
    0x14,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    /*
     * string2 descriptor
     */
    0x26,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'C', 0x00,                  /* wcChar0 */
    'h', 0x00,                  /* wcChar1 */
    'e', 0x00,                  /* wcChar2 */
    'r', 0x00,                  /* wcChar3 */
    'r', 0x00,                  /* wcChar4 */
    'y', 0x00,                  /* wcChar5 */
    'U', 0x00,                  /* wcChar6 */
    'S', 0x00,                  /* wcChar7 */
    'B', 0x00,                  /* wcChar8 */
    ' ', 0x00,                  /* wcChar9 */
    'H', 0x00,                  /* wcChar10 */
    'I', 0x00,                  /* wcChar11 */
    'D', 0x00,                  /* wcChar12 */
    ' ', 0x00,                  /* wcChar13 */
    'D', 0x00,                  /* wcChar14 */
    'E', 0x00,                  /* wcChar15 */
    'M', 0x00,                  /* wcChar16 */
    'O', 0x00,                  /* wcChar17 */
    /*
     * string3 descriptor
     */
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    /*
     * device qualifier descriptor
     */
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
#endif
    0x00
};

/*!< custom hid report descriptor */
static const uint8_t boot_hid_report_desc[BOOT_HID_REPORT_DESC_SIZE] = {
    /* USER CODE BEGIN 0 */ 
  0x06, 0x00, 0xff,               	/*   USAGE_PAGE (Undefined)     */
  0x09, 0x00,                     	/*   USAGE (Undefined)          */
  0xa1, 0x01,                     	/*   COLLECTION (Application)   */
  0x85, 0xFD,                     	/*   REPORT_ID (253)            */
  0x06, 0x00, 0xff,               	/*   USAGE_PAGE (Undefined)     */
  0x19, 0x01,                     	/*   USAGE_MINIMUM              */
  0x29, 0x3f,                     	/*   USAGE_MAXIMUM              */
  0x15, 0x00,                     	/*   LOGICAL_MINIMUM (0)        */
  0x25, 0xff,                     	/*   LOGICAL_MAXIMUM (255)      */
  0x75, 0x08,                     	/*   REPORT_SIZE (8)            */
  0x95, 0x3f,                     	/*   REPORT_COUNT (63)          */
  0x81, 0x02,                     	/*   INPUT (Data,Var,Abs)       */
  0x85, 0xFE,                     	/*   REPORT_ID (254)            */
  0x06, 0x00, 0xff,               	/*   USAGE_PAGE (Undefined)     */
  0x19, 0x01,                     	/*   USAGE_MINIMUM              */
  0x29, 0x3f,                     	/*   USAGE_MAXIMUM              */
  0x15, 0x00,                     	/*   LOGICAL_MINIMUM (0)        */
  0x25, 0xff,                     	/*   LOGICAL_MAXIMUM (255)      */
  0x75, 0x08,                     	/*   REPORT_SIZE (8)            */
  0x95, 0x3f,                     	/*   REPORT_COUNT (63)          */
  0x91, 0x02,                     	/*   OutPut (Data,Var,Abs)      */
  /* USER CODE END 0 */ 
  0xC0    /*     END_COLLECTION                 */
};

static USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[64];
static USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t send_buffer[64];

#define HID_STATE_IDLE 0
#define HID_STATE_BUSY 1

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t custom_state;

void usbd_configure_done_callback(void)
{
    /* setup first out ep read transfer */
    usbd_ep_start_read(BOOT_OUT_EP, read_buffer, 64);
}

static void boot_hid_in_callback(uint8_t ep, uint32_t nbytes)
{
    USB_LOG_RAW("actual in len:%d\r\n", nbytes);
    custom_state = HID_STATE_IDLE;
}

static void boot_hid_out_callback(uint8_t ep, uint32_t nbytes)
{
    USB_LOG_RAW("actual out len:%d\r\n", nbytes);
    usbd_ep_start_read(BOOT_OUT_EP, read_buffer, 64);
    read_buffer[0] = 0x02;    /* IN: report id */
    usbd_ep_start_write(BOOT_IN_EP, read_buffer, nbytes);
}

static struct usbd_endpoint boot_in_ep = {
    .ep_cb = boot_hid_in_callback,
    .ep_addr = BOOT_IN_EP
};

static struct usbd_endpoint boot_out_ep = {
    .ep_cb = boot_hid_out_callback,
    .ep_addr = BOOT_OUT_EP
};

/* function ------------------------------------------------------------------*/
/**
 * @brief            boot hid init
 * @pre              none
 * @param[in]        none
 * @retval           none
 */
void boot_hid_init(void)
{
    usbd_desc_register(boot_hid_descriptor);
    usbd_add_interface(usbd_hid_alloc_intf(boot_hid_report_desc, BOOT_HID_REPORT_DESC_SIZE));
    usbd_add_endpoint(&boot_in_ep);
    usbd_add_endpoint(&boot_out_ep);

    usbd_initialize();
}

void boot_hid_test(void)
{
    memset(send_buffer, 0, 64);
    send_buffer[0] = 0xFC;
    send_buffer[1] = 0x30;
    send_buffer[2] = 0x31;

    usbd_ep_start_write(BOOT_IN_EP, send_buffer, 12);
}
#endif