/*
 * Copyright (c) 2021-2023 HPMicro
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */


#ifndef HPM_SMIX_H
#define HPM_SMIX_H

typedef struct {
    __R  uint32_t DMAC_ID;                     /* 0x0: DMAC_ID Register */
    __RW uint32_t DMAC_TC_ST;                  /* 0x4: Transfer Complete Status */
    __RW uint32_t DMAC_ABRT_ST;                /* 0x8: Transfer Abort Status */
    __RW uint32_t DMAC_ERR_ST;                 /* 0xC: Transfer Error Status */
    __R  uint8_t  RESERVED0[16];               /* 0x10 - 0x1F: Reserved */
    __RW uint32_t DMAC_CTRL;                   /* 0x20: Control Register */
    __W  uint32_t DMAC_ABRT_CMD;               /* 0x24: Abort Command Register */
    __R  uint8_t  RESERVED1[12];               /* 0x28 - 0x33: Reserved */
    __RW uint32_t DMAC_CHEN;                   /* 0x34: Channel Enable Register */
    __R  uint8_t  RESERVED2[8];                /* 0x38 - 0x3F: Reserved */
    struct {
        __RW uint32_t DMAC_CHCTL_N;            /* 0x40: Channel N Control Register */
        __RW uint32_t DMAC_TTBEAT_N;           /* 0x44: Channel N Source Total Beats Register */
        __RW uint32_t DMAC_SRCADDR_N;          /* 0x48: Channel N Source Register */
        __R  uint8_t  RESERVED0[4];            /* 0x4C - 0x4F: Reserved */
        __RW uint32_t DMAC_DSTADDR_N;          /* 0x50: Channel N Destination Register */
        __R  uint8_t  RESERVED1[4];            /* 0x54 - 0x57: Reserved */
        __RW uint32_t DMAC_LLP_N;              /* 0x58: Channel N Linked List Pointer Register */
        __R  uint8_t  RESERVED2[4];            /* 0x5C - 0x5F: Reserved */
    } CH[26];
    __R  uint8_t  RESERVED3[128];              /* 0x380 - 0x3FF: Reserved */
    __RW uint32_t SRC_CTRL;                    /* 0x400: SRC Control Register */
    __RW uint32_t SRC_MISCFG;                  /* 0x404: SRC Misc Configuration Register */
    __RW uint32_t SRC_STA;                     /* 0x408: SRC Status Register */
    __RW uint32_t SRC_PERIOD;                  /* 0x40C: SRC Period Register */
    __W  uint32_t SRC_ONESHOT_CMD;             /* 0x410: SRC FIFO Resync Register */
    __R  uint8_t  RESERVED4[28];               /* 0x414 - 0x42F: Reserved */
    struct {
        __RW uint32_t SRC_PCTRLN;              /* 0x430: SRC Pair N Control Register */
        __RW uint32_t SRC_DFMT_CTRLN;          /* 0x434: SRC Pair N Data Format Register */
        __RW uint32_t SRC_CCTRLN;              /* 0x438: SRC Pair N Clock Control Register */
        __R  uint32_t SRC_PSTN;                /* 0x43C: SRC Pair N Status Register */
        __RW uint32_t SRC_IDRTN;               /* 0x440: SRC Pair N Ideal Ratio Register */
        __RW uint32_t SRC_ACTRTN;              /* 0x444: SRC Pair N Actual Ratio Register */
        __RW uint32_t SRC_DIN;                 /* 0x448: SRC Pair N Data Input Register */
        __RW uint32_t SRC_DON;                 /* 0x44C: SRC Pair N Data Output Register */
    } PCH[3];
    __R  uint8_t  RESERVED5[880];              /* 0x490 - 0x7FF: Reserved */
    __RW uint32_t SMIX_CALSAT_ST;              /* 0x800: SMIX Cal Saturation Status Register */
    __RW uint32_t SMIX_FDOT_DONE_ST;           /* 0x804: SMIX Fade-Out Done Status Register */
    __R  uint32_t SMIX_DATA_ST;                /* 0x808: SMIX Data Status Register */
    __R  uint8_t  RESERVED6[52];               /* 0x80C - 0x83F: Reserved */
    struct {
        __RW uint32_t SMIX_DSTN_CTRL;          /* 0x840: SMIX Dstination N Control Register */
        __RW uint32_t SMIX_DSTN_GAIN;          /* 0x844: SMIX Dstination N Gain Register */
        __RW uint32_t SMIX_DSTN_MIDX;          /* 0x848: SMIX Dstination N Max Index Register */
        __RW uint32_t SMIX_DSTN_FADEIN;        /* 0x84C: SMIX Dstination N Fade-In Configuration Register */
        __RW uint32_t SMIX_DSTN_FADEOUT;       /* 0x850: SMIX Dstination N Fade-Out Configuration Register */
        __R  uint32_t SMIX_DSTN_ST;            /* 0x854: SMIX Dstination N Status Register */
        __R  uint32_t SMIX_DSTN_DATA;          /* 0x858: SMIX Dstination N Data Out Register */
        __R  uint8_t  RESERVED0[4];            /* 0x85C - 0x85F: Reserved */
        __RW uint32_t SMIX_DSTN_SRC_EN;        /* 0x860: SMIX Dstination N Source Enable Register */
        __RW uint32_t SMIX_DSTN_SRC_ACT;       /* 0x864: SMIX Dstination N Source Activation Register */
        __RW uint32_t SMIX_DSTN_SRC_DEACT;     /* 0x868: SMIX Dstination N Source De-Activation Register */
        __RW uint32_t SMIX_DSTN_SRC_FADEIN_CTRL; /* 0x86C: SMIX Dstination N Source Fade-in Control Register */
        __R  uint32_t SMIX_DSTN_SRC_DEACT_ST;  /* 0x870: SMIX Dstination N Source Deactivation Status Register */
        __RW uint32_t SMIX_DSTN_SRC_MFADEOUT_CTRL; /* 0x874: SMIX Dstination N Source Manual Fade-out Control Register */
        __R  uint8_t  RESERVED1[8];            /* 0x878 - 0x87F: Reserved */
    } SMIX_DCH[2];
    __R  uint8_t  RESERVED7[64];               /* 0x8C0 - 0x8FF: Reserved */
    struct {
        __RW uint32_t SMIX_SRCN_CTRL;          /* 0x900: SMIX Source N Control Register */
        __RW uint32_t SMIX_SRCN_GAIN;          /* 0x904: SMIX Source N Gain Register */
        __RW uint32_t SMIX_SRCN_FADEIN;        /* 0x908: SMIX Source N Fade-in Control Register */
        __RW uint32_t SMIX_SRCN_FADEOUT;       /* 0x90C: SMIX Source N Fade-out Control Register */
        __RW uint32_t SMIX_SRCN_BUFSIZE;       /* 0x910: SMIX Source N Buffer Size Register */
        __RW uint32_t SMIX_SRCN_ST;            /* 0x914: SMIX Source N Status Register */
        __W  uint32_t SMIX_SRCN_DATA;          /* 0x918: SMIX Source N Data Input Register */
        __R  uint8_t  RESERVED0[4];            /* 0x91C - 0x91F: Reserved */
    } SMIX_SCH[14];
} SMIX_Type;


/* Bitfield definition for register: DMAC_ID */
/*
 * REV (RO)
 *
 * Revision
 */
#define SMIX_DMAC_ID_REV_MASK (0x7FFFFUL)
#define SMIX_DMAC_ID_REV_SHIFT (0U)
#define SMIX_DMAC_ID_REV_GET(x) (((uint32_t)(x) & SMIX_DMAC_ID_REV_MASK) >> SMIX_DMAC_ID_REV_SHIFT)

/* Bitfield definition for register: DMAC_TC_ST */
/*
 * CH (W1C)
 *
 * The terminal count status is set when a channel transfer finishes without abort or error events
 */
#define SMIX_DMAC_TC_ST_CH_MASK (0x3FFFFFFUL)
#define SMIX_DMAC_TC_ST_CH_SHIFT (0U)
#define SMIX_DMAC_TC_ST_CH_SET(x) (((uint32_t)(x) << SMIX_DMAC_TC_ST_CH_SHIFT) & SMIX_DMAC_TC_ST_CH_MASK)
#define SMIX_DMAC_TC_ST_CH_GET(x) (((uint32_t)(x) & SMIX_DMAC_TC_ST_CH_MASK) >> SMIX_DMAC_TC_ST_CH_SHIFT)

/* Bitfield definition for register: DMAC_ABRT_ST */
/*
 * CH (W1C)
 *
 * The abort status is set when a channel transfer is aborted
 */
#define SMIX_DMAC_ABRT_ST_CH_MASK (0x3FFFFFFUL)
#define SMIX_DMAC_ABRT_ST_CH_SHIFT (0U)
#define SMIX_DMAC_ABRT_ST_CH_SET(x) (((uint32_t)(x) << SMIX_DMAC_ABRT_ST_CH_SHIFT) & SMIX_DMAC_ABRT_ST_CH_MASK)
#define SMIX_DMAC_ABRT_ST_CH_GET(x) (((uint32_t)(x) & SMIX_DMAC_ABRT_ST_CH_MASK) >> SMIX_DMAC_ABRT_ST_CH_SHIFT)

/* Bitfield definition for register: DMAC_ERR_ST */
/*
 * CH (W1C)
 *
 * The error status is set when a channel transfer encounters the following error events:
 * . Bus error
 * . Unaligned address
 * . Unaligned transfer width
 * . Reserved configuration
 */
#define SMIX_DMAC_ERR_ST_CH_MASK (0x3FFFFFFUL)
#define SMIX_DMAC_ERR_ST_CH_SHIFT (0U)
#define SMIX_DMAC_ERR_ST_CH_SET(x) (((uint32_t)(x) << SMIX_DMAC_ERR_ST_CH_SHIFT) & SMIX_DMAC_ERR_ST_CH_MASK)
#define SMIX_DMAC_ERR_ST_CH_GET(x) (((uint32_t)(x) & SMIX_DMAC_ERR_ST_CH_MASK) >> SMIX_DMAC_ERR_ST_CH_SHIFT)

/* Bitfield definition for register: DMAC_CTRL */
/*
 * SRST (RW)
 *
 * Software Reset
 */
#define SMIX_DMAC_CTRL_SRST_MASK (0x1U)
#define SMIX_DMAC_CTRL_SRST_SHIFT (0U)
#define SMIX_DMAC_CTRL_SRST_SET(x) (((uint32_t)(x) << SMIX_DMAC_CTRL_SRST_SHIFT) & SMIX_DMAC_CTRL_SRST_MASK)
#define SMIX_DMAC_CTRL_SRST_GET(x) (((uint32_t)(x) & SMIX_DMAC_CTRL_SRST_MASK) >> SMIX_DMAC_CTRL_SRST_SHIFT)

/* Bitfield definition for register: DMAC_ABRT_CMD */
/*
 * CH (WO)
 *
 * Write 1 to force the corresponding channel into abort status
 */
#define SMIX_DMAC_ABRT_CMD_CH_MASK (0x3FFFFFFUL)
#define SMIX_DMAC_ABRT_CMD_CH_SHIFT (0U)
#define SMIX_DMAC_ABRT_CMD_CH_SET(x) (((uint32_t)(x) << SMIX_DMAC_ABRT_CMD_CH_SHIFT) & SMIX_DMAC_ABRT_CMD_CH_MASK)
#define SMIX_DMAC_ABRT_CMD_CH_GET(x) (((uint32_t)(x) & SMIX_DMAC_ABRT_CMD_CH_MASK) >> SMIX_DMAC_ABRT_CMD_CH_SHIFT)

/* Bitfield definition for register: DMAC_CHEN */
/*
 * CH (RO)
 *
 * Write 1 to enable the corresponding channel
 */
#define SMIX_DMAC_CHEN_CH_MASK (0x3FFFFFFUL)
#define SMIX_DMAC_CHEN_CH_SHIFT (0U)
#define SMIX_DMAC_CHEN_CH_GET(x) (((uint32_t)(x) & SMIX_DMAC_CHEN_CH_MASK) >> SMIX_DMAC_CHEN_CH_SHIFT)

/* Bitfield definition for register of struct array CH: DMAC_CHCTL_N */
/*
 * SRCREQSEL (RW)
 *
 * Source DMA request select. Select the request/ack handshake pair that the source device is connected to.
 */
#define SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_MASK (0x7C000000UL)
#define SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_SHIFT (26U)
#define SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_SHIFT) & SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_MASK)
#define SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_MASK) >> SMIX_CH_DMAC_CHCTL_N_SRCREQSEL_SHIFT)

/*
 * DSTREQSEL (RW)
 *
 * Destination DMA request select. Select the request/ack handshake pair that the destination device is connected to.
 */
#define SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_MASK (0x3E00000UL)
#define SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_SHIFT (21U)
#define SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_SHIFT) & SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_MASK)
#define SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_MASK) >> SMIX_CH_DMAC_CHCTL_N_DSTREQSEL_SHIFT)

/*
 * PRIORITY (RW)
 *
 * 0x0: Lower priority
 * 0x1: Higher priority
 */
#define SMIX_CH_DMAC_CHCTL_N_PRIORITY_MASK (0x80000UL)
#define SMIX_CH_DMAC_CHCTL_N_PRIORITY_SHIFT (19U)
#define SMIX_CH_DMAC_CHCTL_N_PRIORITY_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_PRIORITY_SHIFT) & SMIX_CH_DMAC_CHCTL_N_PRIORITY_MASK)
#define SMIX_CH_DMAC_CHCTL_N_PRIORITY_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_PRIORITY_MASK) >> SMIX_CH_DMAC_CHCTL_N_PRIORITY_SHIFT)

/*
 * SRCBURSTSIZE (RW)
 *
 * 0x0: 1 beat per transfer
 * 0x1: 2 beats per transfer
 * 0x2: 4 beats per transfer
 * 0x3: 8 beats per transfer
 * 0x4: 16 beats per transfer
 * 0x5: 32 beats per transfer
 * 0x6: 64 beats per transfer
 * 0x7: 128 beats per transfer
 */
#define SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_MASK (0x78000UL)
#define SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_SHIFT (15U)
#define SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_SHIFT) & SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_MASK)
#define SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_MASK) >> SMIX_CH_DMAC_CHCTL_N_SRCBURSTSIZE_SHIFT)

/*
 * SRCWIDTH (RW)
 *
 * Source Transfer Beat Size:
 * 0x0: Byte transfer
 * 0x1: Half-word transfer
 * 0x2: Word transfer
 */
#define SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_MASK (0x6000U)
#define SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_SHIFT (13U)
#define SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_SHIFT) & SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_MASK)
#define SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_MASK) >> SMIX_CH_DMAC_CHCTL_N_SRCWIDTH_SHIFT)

/*
 * DSTWIDTH (RW)
 *
 * Destination Transfer Beat Size:
 * 0x0: Byte transfer
 * 0x1: Half-word transfer
 * 0x2: Word transfer
 */
#define SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_MASK (0x1800U)
#define SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_SHIFT (11U)
#define SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_SHIFT) & SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_MASK)
#define SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_MASK) >> SMIX_CH_DMAC_CHCTL_N_DSTWIDTH_SHIFT)

/*
 * SRCMODE (RW)
 *
 * DMA Source handshake mode
 * 0x0: Normal mode
 * 0x1: Handshake mode
 */
#define SMIX_CH_DMAC_CHCTL_N_SRCMODE_MASK (0x400U)
#define SMIX_CH_DMAC_CHCTL_N_SRCMODE_SHIFT (10U)
#define SMIX_CH_DMAC_CHCTL_N_SRCMODE_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_SRCMODE_SHIFT) & SMIX_CH_DMAC_CHCTL_N_SRCMODE_MASK)
#define SMIX_CH_DMAC_CHCTL_N_SRCMODE_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_SRCMODE_MASK) >> SMIX_CH_DMAC_CHCTL_N_SRCMODE_SHIFT)

/*
 * DSTMODE (RW)
 *
 * DMA Destination handshake mode
 * 0x0: Normal mode
 * 0x1: Handshake mode
 */
#define SMIX_CH_DMAC_CHCTL_N_DSTMODE_MASK (0x200U)
#define SMIX_CH_DMAC_CHCTL_N_DSTMODE_SHIFT (9U)
#define SMIX_CH_DMAC_CHCTL_N_DSTMODE_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_DSTMODE_SHIFT) & SMIX_CH_DMAC_CHCTL_N_DSTMODE_MASK)
#define SMIX_CH_DMAC_CHCTL_N_DSTMODE_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_DSTMODE_MASK) >> SMIX_CH_DMAC_CHCTL_N_DSTMODE_SHIFT)

/*
 * SRCADDRCTRL (RW)
 *
 * 0x0: Increment address
 * 0x1: Decrement address
 * 0x2: Fixed address
 * 0x3: Reserved, setting the field with this value triggers an error exception
 */
#define SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_MASK (0x180U)
#define SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_SHIFT (7U)
#define SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_SHIFT) & SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_MASK)
#define SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_MASK) >> SMIX_CH_DMAC_CHCTL_N_SRCADDRCTRL_SHIFT)

/*
 * DSTADDRCTRL (RW)
 *
 * 0x0: Increment address
 * 0x1: Decrement address
 * 0x2: Fixed address
 * 0x3: Reserved, setting the field with this value triggers an error exception
 */
#define SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_MASK (0x60U)
#define SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_SHIFT (5U)
#define SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_SHIFT) & SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_MASK)
#define SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_MASK) >> SMIX_CH_DMAC_CHCTL_N_DSTADDRCTRL_SHIFT)

/*
 * ABRT_INT_EN (RW)
 *
 * Abort interrupt enable
 */
#define SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_MASK (0x8U)
#define SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_SHIFT (3U)
#define SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_SHIFT) & SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_MASK)
#define SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_MASK) >> SMIX_CH_DMAC_CHCTL_N_ABRT_INT_EN_SHIFT)

/*
 * ERR_INT_EN (RW)
 *
 * Err interrupt enable
 */
#define SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_MASK (0x4U)
#define SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_SHIFT (2U)
#define SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_SHIFT) & SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_MASK)
#define SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_MASK) >> SMIX_CH_DMAC_CHCTL_N_ERR_INT_EN_SHIFT)

/*
 * TC_INT_EN (RW)
 *
 * TC interrupt enable
 */
#define SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_MASK (0x2U)
#define SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_SHIFT (1U)
#define SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_SHIFT) & SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_MASK)
#define SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_MASK) >> SMIX_CH_DMAC_CHCTL_N_TC_INT_EN_SHIFT)

/*
 * EN (RW)
 *
 * channel enable bit
 */
#define SMIX_CH_DMAC_CHCTL_N_EN_MASK (0x1U)
#define SMIX_CH_DMAC_CHCTL_N_EN_SHIFT (0U)
#define SMIX_CH_DMAC_CHCTL_N_EN_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_CHCTL_N_EN_SHIFT) & SMIX_CH_DMAC_CHCTL_N_EN_MASK)
#define SMIX_CH_DMAC_CHCTL_N_EN_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_CHCTL_N_EN_MASK) >> SMIX_CH_DMAC_CHCTL_N_EN_SHIFT)

/* Bitfield definition for register of struct array CH: DMAC_TTBEAT_N */
/*
 * NUM (RW)
 *
 * the total number of source beats
 */
#define SMIX_CH_DMAC_TTBEAT_N_NUM_MASK (0xFFFFFFFFUL)
#define SMIX_CH_DMAC_TTBEAT_N_NUM_SHIFT (0U)
#define SMIX_CH_DMAC_TTBEAT_N_NUM_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_TTBEAT_N_NUM_SHIFT) & SMIX_CH_DMAC_TTBEAT_N_NUM_MASK)
#define SMIX_CH_DMAC_TTBEAT_N_NUM_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_TTBEAT_N_NUM_MASK) >> SMIX_CH_DMAC_TTBEAT_N_NUM_SHIFT)

/* Bitfield definition for register of struct array CH: DMAC_SRCADDR_N */
/*
 * PTR (RW)
 *
 * source address
 */
#define SMIX_CH_DMAC_SRCADDR_N_PTR_MASK (0xFFFFFFFFUL)
#define SMIX_CH_DMAC_SRCADDR_N_PTR_SHIFT (0U)
#define SMIX_CH_DMAC_SRCADDR_N_PTR_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_SRCADDR_N_PTR_SHIFT) & SMIX_CH_DMAC_SRCADDR_N_PTR_MASK)
#define SMIX_CH_DMAC_SRCADDR_N_PTR_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_SRCADDR_N_PTR_MASK) >> SMIX_CH_DMAC_SRCADDR_N_PTR_SHIFT)

/* Bitfield definition for register of struct array CH: DMAC_DSTADDR_N */
/*
 * PTR (RW)
 *
 * destination address
 */
#define SMIX_CH_DMAC_DSTADDR_N_PTR_MASK (0xFFFFFFFFUL)
#define SMIX_CH_DMAC_DSTADDR_N_PTR_SHIFT (0U)
#define SMIX_CH_DMAC_DSTADDR_N_PTR_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_DSTADDR_N_PTR_SHIFT) & SMIX_CH_DMAC_DSTADDR_N_PTR_MASK)
#define SMIX_CH_DMAC_DSTADDR_N_PTR_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_DSTADDR_N_PTR_MASK) >> SMIX_CH_DMAC_DSTADDR_N_PTR_SHIFT)

/* Bitfield definition for register of struct array CH: DMAC_LLP_N */
/*
 * PTR (RW)
 *
 * the address pointer for the linked list descriptor
 */
#define SMIX_CH_DMAC_LLP_N_PTR_MASK (0xFFFFFFFFUL)
#define SMIX_CH_DMAC_LLP_N_PTR_SHIFT (0U)
#define SMIX_CH_DMAC_LLP_N_PTR_SET(x) (((uint32_t)(x) << SMIX_CH_DMAC_LLP_N_PTR_SHIFT) & SMIX_CH_DMAC_LLP_N_PTR_MASK)
#define SMIX_CH_DMAC_LLP_N_PTR_GET(x) (((uint32_t)(x) & SMIX_CH_DMAC_LLP_N_PTR_MASK) >> SMIX_CH_DMAC_LLP_N_PTR_SHIFT)

/* Bitfield definition for register: SRC_CTRL */
/*
 * OVLD_ERR_IE (RW)
 *
 * Overload Interrupt Enable
 * Enables the overload interrupt.
 * 1 -- interrupt enabled
 * 0 -- interrupt disabled
 */
#define SMIX_SRC_CTRL_OVLD_ERR_IE_MASK (0x40000000UL)
#define SMIX_SRC_CTRL_OVLD_ERR_IE_SHIFT (30U)
#define SMIX_SRC_CTRL_OVLD_ERR_IE_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_OVLD_ERR_IE_SHIFT) & SMIX_SRC_CTRL_OVLD_ERR_IE_MASK)
#define SMIX_SRC_CTRL_OVLD_ERR_IE_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_OVLD_ERR_IE_MASK) >> SMIX_SRC_CTRL_OVLD_ERR_IE_SHIFT)

/*
 * WRSV0 (RW)
 *
 * Reserved
 */
#define SMIX_SRC_CTRL_WRSV0_MASK (0x3FFFFFE0UL)
#define SMIX_SRC_CTRL_WRSV0_SHIFT (5U)
#define SMIX_SRC_CTRL_WRSV0_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_WRSV0_SHIFT) & SMIX_SRC_CTRL_WRSV0_MASK)
#define SMIX_SRC_CTRL_WRSV0_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_WRSV0_MASK) >> SMIX_SRC_CTRL_WRSV0_SHIFT)

/*
 * P2_EN (RW)
 *
 * Pair 2 Enable
 */
#define SMIX_SRC_CTRL_P2_EN_MASK (0x10U)
#define SMIX_SRC_CTRL_P2_EN_SHIFT (4U)
#define SMIX_SRC_CTRL_P2_EN_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_P2_EN_SHIFT) & SMIX_SRC_CTRL_P2_EN_MASK)
#define SMIX_SRC_CTRL_P2_EN_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_P2_EN_MASK) >> SMIX_SRC_CTRL_P2_EN_SHIFT)

/*
 * P1_EN (RW)
 *
 * Pair 1 Enable
 */
#define SMIX_SRC_CTRL_P1_EN_MASK (0x8U)
#define SMIX_SRC_CTRL_P1_EN_SHIFT (3U)
#define SMIX_SRC_CTRL_P1_EN_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_P1_EN_SHIFT) & SMIX_SRC_CTRL_P1_EN_MASK)
#define SMIX_SRC_CTRL_P1_EN_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_P1_EN_MASK) >> SMIX_SRC_CTRL_P1_EN_SHIFT)

/*
 * P0_EN (RW)
 *
 * Pair 0 Enable
 */
#define SMIX_SRC_CTRL_P0_EN_MASK (0x4U)
#define SMIX_SRC_CTRL_P0_EN_SHIFT (2U)
#define SMIX_SRC_CTRL_P0_EN_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_P0_EN_SHIFT) & SMIX_SRC_CTRL_P0_EN_MASK)
#define SMIX_SRC_CTRL_P0_EN_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_P0_EN_MASK) >> SMIX_SRC_CTRL_P0_EN_SHIFT)

/*
 * SRST (RW)
 *
 * Software Reset
 * This bit is self-clear bit. Once it is been written as 1, it will generate a software reset signal inside ASRC. After 9 cycles of the ASRC processing clock, this reset process will stop, and this bit will be cleared automatically.
 */
#define SMIX_SRC_CTRL_SRST_MASK (0x2U)
#define SMIX_SRC_CTRL_SRST_SHIFT (1U)
#define SMIX_SRC_CTRL_SRST_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_SRST_SHIFT) & SMIX_SRC_CTRL_SRST_MASK)
#define SMIX_SRC_CTRL_SRST_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_SRST_MASK) >> SMIX_SRC_CTRL_SRST_SHIFT)

/*
 * EN (RW)
 *
 * Module Enable
 */
#define SMIX_SRC_CTRL_EN_MASK (0x1U)
#define SMIX_SRC_CTRL_EN_SHIFT (0U)
#define SMIX_SRC_CTRL_EN_SET(x) (((uint32_t)(x) << SMIX_SRC_CTRL_EN_SHIFT) & SMIX_SRC_CTRL_EN_MASK)
#define SMIX_SRC_CTRL_EN_GET(x) (((uint32_t)(x) & SMIX_SRC_CTRL_EN_MASK) >> SMIX_SRC_CTRL_EN_SHIFT)

/* Bitfield definition for register: SRC_MISCFG */
/*
 * WRSV0 (RW)
 *
 * Reserved
 */
#define SMIX_SRC_MISCFG_WRSV0_MASK (0xFFFFF000UL)
#define SMIX_SRC_MISCFG_WRSV0_SHIFT (12U)
#define SMIX_SRC_MISCFG_WRSV0_SET(x) (((uint32_t)(x) << SMIX_SRC_MISCFG_WRSV0_SHIFT) & SMIX_SRC_MISCFG_WRSV0_MASK)
#define SMIX_SRC_MISCFG_WRSV0_GET(x) (((uint32_t)(x) & SMIX_SRC_MISCFG_WRSV0_MASK) >> SMIX_SRC_MISCFG_WRSV0_SHIFT)

/*
 * NUMP2 (RW)
 *
 * Number of channels in Pair 2
 * 0000- 0channels (Pair is disabled)
 * ...
 * 1010- 10channels
 */
#define SMIX_SRC_MISCFG_NUMP2_MASK (0xF00U)
#define SMIX_SRC_MISCFG_NUMP2_SHIFT (8U)
#define SMIX_SRC_MISCFG_NUMP2_SET(x) (((uint32_t)(x) << SMIX_SRC_MISCFG_NUMP2_SHIFT) & SMIX_SRC_MISCFG_NUMP2_MASK)
#define SMIX_SRC_MISCFG_NUMP2_GET(x) (((uint32_t)(x) & SMIX_SRC_MISCFG_NUMP2_MASK) >> SMIX_SRC_MISCFG_NUMP2_SHIFT)

/*
 * NUMP1 (RW)
 *
 * Number of channels in Pair 1
 * 0000- 0channels (Pair is disabled)
 * ...
 * 1010- 10channels
 */
#define SMIX_SRC_MISCFG_NUMP1_MASK (0xF0U)
#define SMIX_SRC_MISCFG_NUMP1_SHIFT (4U)
#define SMIX_SRC_MISCFG_NUMP1_SET(x) (((uint32_t)(x) << SMIX_SRC_MISCFG_NUMP1_SHIFT) & SMIX_SRC_MISCFG_NUMP1_MASK)
#define SMIX_SRC_MISCFG_NUMP1_GET(x) (((uint32_t)(x) & SMIX_SRC_MISCFG_NUMP1_MASK) >> SMIX_SRC_MISCFG_NUMP1_SHIFT)

/*
 * NUMP0 (RW)
 *
 * Number of channels in Pair 0
 * 0000- 0channels (Pair is disabled)
 * ...
 * 1010- 10channels
 */
#define SMIX_SRC_MISCFG_NUMP0_MASK (0xFU)
#define SMIX_SRC_MISCFG_NUMP0_SHIFT (0U)
#define SMIX_SRC_MISCFG_NUMP0_SET(x) (((uint32_t)(x) << SMIX_SRC_MISCFG_NUMP0_SHIFT) & SMIX_SRC_MISCFG_NUMP0_MASK)
#define SMIX_SRC_MISCFG_NUMP0_GET(x) (((uint32_t)(x) & SMIX_SRC_MISCFG_NUMP0_MASK) >> SMIX_SRC_MISCFG_NUMP0_SHIFT)

/* Bitfield definition for register: SRC_STA */
/*
 * WRSVI1 (RO)
 *
 * Reserved
 */
#define SMIX_SRC_STA_WRSVI1_MASK (0xFFFF0000UL)
#define SMIX_SRC_STA_WRSVI1_SHIFT (16U)
#define SMIX_SRC_STA_WRSVI1_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_WRSVI1_MASK) >> SMIX_SRC_STA_WRSVI1_SHIFT)

/*
 * TQOL (RO)
 *
 * task queue overloaded.
 */
#define SMIX_SRC_STA_TQOL_MASK (0x8000U)
#define SMIX_SRC_STA_TQOL_SHIFT (15U)
#define SMIX_SRC_STA_TQOL_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_TQOL_MASK) >> SMIX_SRC_STA_TQOL_SHIFT)

/*
 * OD_OVFL (RO)
 *
 * Asserted when overflow of the output buffer is found for corresponding conversion group.
 */
#define SMIX_SRC_STA_OD_OVFL_MASK (0x3800U)
#define SMIX_SRC_STA_OD_OVFL_SHIFT (11U)
#define SMIX_SRC_STA_OD_OVFL_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_OD_OVFL_MASK) >> SMIX_SRC_STA_OD_OVFL_SHIFT)

/*
 * ID_UDFL (RO)
 *
 * Asserted when underflow of the input buffer is found for corresponding conversion group.
 */
#define SMIX_SRC_STA_ID_UDFL_MASK (0x700U)
#define SMIX_SRC_STA_ID_UDFL_SHIFT (8U)
#define SMIX_SRC_STA_ID_UDFL_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_ID_UDFL_MASK) >> SMIX_SRC_STA_ID_UDFL_SHIFT)

/*
 * DA (RO)
 *
 * output data available status for corresponding converion pair. dma_req[6:4]
 */
#define SMIX_SRC_STA_DA_MASK (0xE0U)
#define SMIX_SRC_STA_DA_SHIFT (5U)
#define SMIX_SRC_STA_DA_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_DA_MASK) >> SMIX_SRC_STA_DA_SHIFT)

/*
 * DN (RO)
 *
 * input data needed status for corresponding converion pair. dma_req[3:1]
 */
#define SMIX_SRC_STA_DN_MASK (0x1CU)
#define SMIX_SRC_STA_DN_SHIFT (2U)
#define SMIX_SRC_STA_DN_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_DN_MASK) >> SMIX_SRC_STA_DN_SHIFT)

/*
 * OVLD_ERR (W1C)
 *
 * Overload Error. W1C. Will clear all overload status bits.
 */
#define SMIX_SRC_STA_OVLD_ERR_MASK (0x1U)
#define SMIX_SRC_STA_OVLD_ERR_SHIFT (0U)
#define SMIX_SRC_STA_OVLD_ERR_SET(x) (((uint32_t)(x) << SMIX_SRC_STA_OVLD_ERR_SHIFT) & SMIX_SRC_STA_OVLD_ERR_MASK)
#define SMIX_SRC_STA_OVLD_ERR_GET(x) (((uint32_t)(x) & SMIX_SRC_STA_OVLD_ERR_MASK) >> SMIX_SRC_STA_OVLD_ERR_SHIFT)

/* Bitfield definition for register: SRC_PERIOD */
/*
 * P56K (RW)
 *
 * LSB0 is assumed to be 0
 */
#define SMIX_SRC_PERIOD_P56K_MASK (0xFFFF0000UL)
#define SMIX_SRC_PERIOD_P56K_SHIFT (16U)
#define SMIX_SRC_PERIOD_P56K_SET(x) (((uint32_t)(x) << SMIX_SRC_PERIOD_P56K_SHIFT) & SMIX_SRC_PERIOD_P56K_MASK)
#define SMIX_SRC_PERIOD_P56K_GET(x) (((uint32_t)(x) & SMIX_SRC_PERIOD_P56K_MASK) >> SMIX_SRC_PERIOD_P56K_SHIFT)

/*
 * P76K (RW)
 *
 * LSB0 is assumed to be 0
 */
#define SMIX_SRC_PERIOD_P76K_MASK (0xFFFFU)
#define SMIX_SRC_PERIOD_P76K_SHIFT (0U)
#define SMIX_SRC_PERIOD_P76K_SET(x) (((uint32_t)(x) << SMIX_SRC_PERIOD_P76K_SHIFT) & SMIX_SRC_PERIOD_P76K_MASK)
#define SMIX_SRC_PERIOD_P76K_GET(x) (((uint32_t)(x) & SMIX_SRC_PERIOD_P76K_MASK) >> SMIX_SRC_PERIOD_P76K_SHIFT)

/* Bitfield definition for register: SRC_ONESHOT_CMD */
/*
 * OUTFIFO_RESYNC (WO)
 *
 * Resync output fifos
 */
#define SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_MASK (0x70000UL)
#define SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_SHIFT (16U)
#define SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_SET(x) (((uint32_t)(x) << SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_SHIFT) & SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_MASK)
#define SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_GET(x) (((uint32_t)(x) & SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_MASK) >> SMIX_SRC_ONESHOT_CMD_OUTFIFO_RESYNC_SHIFT)

/*
 * INFIFO_RESYNC (WO)
 *
 * Resync input fifo pointers
 */
#define SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_MASK (0x7U)
#define SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_SHIFT (0U)
#define SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_SET(x) (((uint32_t)(x) << SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_SHIFT) & SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_MASK)
#define SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_GET(x) (((uint32_t)(x) & SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_MASK) >> SMIX_SRC_ONESHOT_CMD_INFIFO_RESYNC_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_PCTRLN */
/*
 * ZEROBUF (RW)
 *
 * initialize the FIFO contents as zero during initialization
 */
#define SMIX_PCH_SRC_PCTRLN_ZEROBUF_MASK (0x10000UL)
#define SMIX_PCH_SRC_PCTRLN_ZEROBUF_SHIFT (16U)
#define SMIX_PCH_SRC_PCTRLN_ZEROBUF_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_ZEROBUF_SHIFT) & SMIX_PCH_SRC_PCTRLN_ZEROBUF_MASK)
#define SMIX_PCH_SRC_PCTRLN_ZEROBUF_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_ZEROBUF_MASK) >> SMIX_PCH_SRC_PCTRLN_ZEROBUF_SHIFT)

/*
 * BUFSTALL (RW)
 *
 * stall the calculation when input data is not available, or the output buffer is not available
 */
#define SMIX_PCH_SRC_PCTRLN_BUFSTALL_MASK (0x8000U)
#define SMIX_PCH_SRC_PCTRLN_BUFSTALL_SHIFT (15U)
#define SMIX_PCH_SRC_PCTRLN_BUFSTALL_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_BUFSTALL_SHIFT) & SMIX_PCH_SRC_PCTRLN_BUFSTALL_MASK)
#define SMIX_PCH_SRC_PCTRLN_BUFSTALL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_BUFSTALL_MASK) >> SMIX_PCH_SRC_PCTRLN_BUFSTALL_SHIFT)

/*
 * BYPASS_POLY (RW)
 *
 * bypass the poly interpolation stage
 */
#define SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_MASK (0x4000U)
#define SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_SHIFT (14U)
#define SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_SHIFT) & SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_MASK)
#define SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_MASK) >> SMIX_PCH_SRC_PCTRLN_BYPASS_POLY_SHIFT)

/*
 * DA_DMAEN (RW)
 *
 * Output data available DMA enable
 */
#define SMIX_PCH_SRC_PCTRLN_DA_DMAEN_MASK (0x2000U)
#define SMIX_PCH_SRC_PCTRLN_DA_DMAEN_SHIFT (13U)
#define SMIX_PCH_SRC_PCTRLN_DA_DMAEN_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_DA_DMAEN_SHIFT) & SMIX_PCH_SRC_PCTRLN_DA_DMAEN_MASK)
#define SMIX_PCH_SRC_PCTRLN_DA_DMAEN_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_DA_DMAEN_MASK) >> SMIX_PCH_SRC_PCTRLN_DA_DMAEN_SHIFT)

/*
 * DN_DMAEN (RW)
 *
 * Input data needed DMA enable
 */
#define SMIX_PCH_SRC_PCTRLN_DN_DMAEN_MASK (0x1000U)
#define SMIX_PCH_SRC_PCTRLN_DN_DMAEN_SHIFT (12U)
#define SMIX_PCH_SRC_PCTRLN_DN_DMAEN_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_DN_DMAEN_SHIFT) & SMIX_PCH_SRC_PCTRLN_DN_DMAEN_MASK)
#define SMIX_PCH_SRC_PCTRLN_DN_DMAEN_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_DN_DMAEN_MASK) >> SMIX_PCH_SRC_PCTRLN_DN_DMAEN_SHIFT)

/*
 * DAIE (RW)
 *
 * Output data available interrupt enable
 */
#define SMIX_PCH_SRC_PCTRLN_DAIE_MASK (0x800U)
#define SMIX_PCH_SRC_PCTRLN_DAIE_SHIFT (11U)
#define SMIX_PCH_SRC_PCTRLN_DAIE_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_DAIE_SHIFT) & SMIX_PCH_SRC_PCTRLN_DAIE_MASK)
#define SMIX_PCH_SRC_PCTRLN_DAIE_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_DAIE_MASK) >> SMIX_PCH_SRC_PCTRLN_DAIE_SHIFT)

/*
 * DNIE (RW)
 *
 * Input data needed interrupt enable
 */
#define SMIX_PCH_SRC_PCTRLN_DNIE_MASK (0x400U)
#define SMIX_PCH_SRC_PCTRLN_DNIE_SHIFT (10U)
#define SMIX_PCH_SRC_PCTRLN_DNIE_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_DNIE_SHIFT) & SMIX_PCH_SRC_PCTRLN_DNIE_MASK)
#define SMIX_PCH_SRC_PCTRLN_DNIE_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_DNIE_MASK) >> SMIX_PCH_SRC_PCTRLN_DNIE_SHIFT)

/*
 * IDR (RW)
 *
 * Use Ideal Ratio
 * When USR=0, this bit has no usage.
 * When USR=1 and IDR=0, ASRC internal measured ratio will be used.
 * When USR=1 and IDR=1, idea ratio from the interface register ASRIDRH, ASRIDRL will be used. It is suggested to manually set ASRCFG:POSTMOD, ASRCFG:PREMOD according to Table 1-27 in this case.
 */
#define SMIX_PCH_SRC_PCTRLN_IDR_MASK (0x100U)
#define SMIX_PCH_SRC_PCTRLN_IDR_SHIFT (8U)
#define SMIX_PCH_SRC_PCTRLN_IDR_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_IDR_SHIFT) & SMIX_PCH_SRC_PCTRLN_IDR_MASK)
#define SMIX_PCH_SRC_PCTRLN_IDR_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_IDR_MASK) >> SMIX_PCH_SRC_PCTRLN_IDR_SHIFT)

/*
 * USR (RW)
 *
 * Use ratio as the input to ASRC. This bit is used in conjunction with IDR control bit.
 */
#define SMIX_PCH_SRC_PCTRLN_USR_MASK (0x80U)
#define SMIX_PCH_SRC_PCTRLN_USR_SHIFT (7U)
#define SMIX_PCH_SRC_PCTRLN_USR_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_USR_SHIFT) & SMIX_PCH_SRC_PCTRLN_USR_MASK)
#define SMIX_PCH_SRC_PCTRLN_USR_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_USR_MASK) >> SMIX_PCH_SRC_PCTRLN_USR_SHIFT)

/*
 * POSTMOD (RW)
 *
 * Post-Processing Configuration
 * 00 – Select Upsampling-by-2
 * 01 – Select Direct-Connection
 * 10 – Select Downsampling-by-2
 * These bits will be read/write by user if ASLCT=0, and can also be automatically updated by the ASRC internal logic if ASLCT=1. These bits set the selection of the post-processing configuration.
 */
#define SMIX_PCH_SRC_PCTRLN_POSTMOD_MASK (0x60U)
#define SMIX_PCH_SRC_PCTRLN_POSTMOD_SHIFT (5U)
#define SMIX_PCH_SRC_PCTRLN_POSTMOD_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_POSTMOD_SHIFT) & SMIX_PCH_SRC_PCTRLN_POSTMOD_MASK)
#define SMIX_PCH_SRC_PCTRLN_POSTMOD_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_POSTMOD_MASK) >> SMIX_PCH_SRC_PCTRLN_POSTMOD_SHIFT)

/*
 * PREMOD (RW)
 *
 * Pre-Processing Configuration
 * 00 – Select Upsampling-by-2
 * 01 – Select Direct-Connection
 * 10 – Select Downsampling-by-2
 * 11 – Select passthrough mode. In this case, POSTMOD[1-0],HF[1:0] have no use.
 * These bits will be read/write by user if ASLCT=0, and can also be automatically updated by the ASRC internal logic if ASLCT=1. These bits set the selection of the pre-processing configuration.
 */
#define SMIX_PCH_SRC_PCTRLN_PREMOD_MASK (0x6U)
#define SMIX_PCH_SRC_PCTRLN_PREMOD_SHIFT (1U)
#define SMIX_PCH_SRC_PCTRLN_PREMOD_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_PREMOD_SHIFT) & SMIX_PCH_SRC_PCTRLN_PREMOD_MASK)
#define SMIX_PCH_SRC_PCTRLN_PREMOD_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_PREMOD_MASK) >> SMIX_PCH_SRC_PCTRLN_PREMOD_SHIFT)

/*
 * ASLCT (RW)
 *
 * Auto selection for processing options
 */
#define SMIX_PCH_SRC_PCTRLN_ASLCT_MASK (0x1U)
#define SMIX_PCH_SRC_PCTRLN_ASLCT_SHIFT (0U)
#define SMIX_PCH_SRC_PCTRLN_ASLCT_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_PCTRLN_ASLCT_SHIFT) & SMIX_PCH_SRC_PCTRLN_ASLCT_MASK)
#define SMIX_PCH_SRC_PCTRLN_ASLCT_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PCTRLN_ASLCT_MASK) >> SMIX_PCH_SRC_PCTRLN_ASLCT_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_DFMT_CTRLN */
/*
 * IWD (RW)
 *
 * These three bits will determine the bitwidth for the audio data into ASRC
 * 2’b00 -- 24-bit audio data.
 * 2’b01 -- 16-bit audio data.
 * 2’b10 -- 8-bit audio data.
 * others -- Reserved
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_IWD_MASK (0x60000UL)
#define SMIX_PCH_SRC_DFMT_CTRLN_IWD_SHIFT (17U)
#define SMIX_PCH_SRC_DFMT_CTRLN_IWD_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_IWD_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_IWD_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_IWD_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_IWD_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_IWD_SHIFT)

/*
 * IMSB (RW)
 *
 * This bit will determine the data alignment of the input FIFO.
 * 1-- MSB aligned.
 * 0-- LSB aligned.
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_IMSB_MASK (0x10000UL)
#define SMIX_PCH_SRC_DFMT_CTRLN_IMSB_SHIFT (16U)
#define SMIX_PCH_SRC_DFMT_CTRLN_IMSB_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_IMSB_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_IMSB_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_IMSB_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_IMSB_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_IMSB_SHIFT)

/*
 * WRSV1 (RW)
 *
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_MASK (0x8000U)
#define SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_SHIFT (15U)
#define SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_WRSV1_SHIFT)

/*
 * OWD (RW)
 *
 * These three bits will determine the bitwidth for the audio data out of ASRC
 * 1’b0 -- 24-bit audio data.
 * 1’b1 -- 16-bit audio data.
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_OWD_MASK (0x4000U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OWD_SHIFT (14U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OWD_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_OWD_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_OWD_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_OWD_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_OWD_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_OWD_SHIFT)

/*
 * OMSB (RW)
 *
 * This bit will determine the data alignment of the output FIFO.
 * 1-- MSB aligned.
 * 0-- LSB aligned.
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_OMSB_MASK (0x2000U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OMSB_SHIFT (13U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OMSB_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_OMSB_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_OMSB_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_OMSB_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_OMSB_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_OMSB_SHIFT)

/*
 * OSIGN (RW)
 *
 * This bit will determine the sign extension option of the output FIFO.
 * 1-- Sign extension.
 * 0-- No sign extension.
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_MASK (0x1000U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_SHIFT (12U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_OSIGN_SHIFT)

/*
 * OUTF_THR (RW)
 *
 * the threshold for output FIFO
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_MASK (0xFC0U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_SHIFT (6U)
#define SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_OUTF_THR_SHIFT)

/*
 * INF_THR (RW)
 *
 * the threshold for input FIFO
 */
#define SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_MASK (0x3FU)
#define SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_SHIFT (0U)
#define SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_SHIFT) & SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_MASK)
#define SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_MASK) >> SMIX_PCH_SRC_DFMT_CTRLN_INF_THR_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_CCTRLN */
/*
 * WRSV (RW)
 *
 */
#define SMIX_PCH_SRC_CCTRLN_WRSV_MASK (0xFFF00000UL)
#define SMIX_PCH_SRC_CCTRLN_WRSV_SHIFT (20U)
#define SMIX_PCH_SRC_CCTRLN_WRSV_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_WRSV_SHIFT) & SMIX_PCH_SRC_CCTRLN_WRSV_MASK)
#define SMIX_PCH_SRC_CCTRLN_WRSV_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_WRSV_MASK) >> SMIX_PCH_SRC_CCTRLN_WRSV_SHIFT)

/*
 * OCD (RW)
 *
 * Output Clock Divider
 * Specify the divide ratio of the output clock divider . The divide ratio may range from 1 to 8 (ICD[2:0] = 000 to 111)
 */
#define SMIX_PCH_SRC_CCTRLN_OCD_MASK (0xE0000UL)
#define SMIX_PCH_SRC_CCTRLN_OCD_SHIFT (17U)
#define SMIX_PCH_SRC_CCTRLN_OCD_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_OCD_SHIFT) & SMIX_PCH_SRC_CCTRLN_OCD_MASK)
#define SMIX_PCH_SRC_CCTRLN_OCD_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_OCD_MASK) >> SMIX_PCH_SRC_CCTRLN_OCD_SHIFT)

/*
 * OCP (RW)
 *
 * Output Clock Prescaler
 * Specify the prescaling factor of the input prescaler  The prescaling ratio may be any power of 2 from 1 to 128.
 */
#define SMIX_PCH_SRC_CCTRLN_OCP_MASK (0x1C000UL)
#define SMIX_PCH_SRC_CCTRLN_OCP_SHIFT (14U)
#define SMIX_PCH_SRC_CCTRLN_OCP_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_OCP_SHIFT) & SMIX_PCH_SRC_CCTRLN_OCP_MASK)
#define SMIX_PCH_SRC_CCTRLN_OCP_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_OCP_MASK) >> SMIX_PCH_SRC_CCTRLN_OCP_SHIFT)

/*
 * ICD (RW)
 *
 * Input Clock Divider
 * Specify the divide ratio of the input clock divider . The divide ratio may range from 1 to 8 (ICD[2:0] = 000 to 111)
 */
#define SMIX_PCH_SRC_CCTRLN_ICD_MASK (0x3800U)
#define SMIX_PCH_SRC_CCTRLN_ICD_SHIFT (11U)
#define SMIX_PCH_SRC_CCTRLN_ICD_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_ICD_SHIFT) & SMIX_PCH_SRC_CCTRLN_ICD_MASK)
#define SMIX_PCH_SRC_CCTRLN_ICD_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_ICD_MASK) >> SMIX_PCH_SRC_CCTRLN_ICD_SHIFT)

/*
 * ICP (RW)
 *
 * Input Clock Prescaler
 * Specify the prescaling factor of the input prescaler  The prescaling ratio may be any power of 2 from 1 to 128.
 */
#define SMIX_PCH_SRC_CCTRLN_ICP_MASK (0x700U)
#define SMIX_PCH_SRC_CCTRLN_ICP_SHIFT (8U)
#define SMIX_PCH_SRC_CCTRLN_ICP_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_ICP_SHIFT) & SMIX_PCH_SRC_CCTRLN_ICP_MASK)
#define SMIX_PCH_SRC_CCTRLN_ICP_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_ICP_MASK) >> SMIX_PCH_SRC_CCTRLN_ICP_SHIFT)

/*
 * OCS (RW)
 *
 * Output clock source selection
 */
#define SMIX_PCH_SRC_CCTRLN_OCS_MASK (0xF0U)
#define SMIX_PCH_SRC_CCTRLN_OCS_SHIFT (4U)
#define SMIX_PCH_SRC_CCTRLN_OCS_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_OCS_SHIFT) & SMIX_PCH_SRC_CCTRLN_OCS_MASK)
#define SMIX_PCH_SRC_CCTRLN_OCS_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_OCS_MASK) >> SMIX_PCH_SRC_CCTRLN_OCS_SHIFT)

/*
 * ICS (RW)
 *
 * Input clock source selection
 */
#define SMIX_PCH_SRC_CCTRLN_ICS_MASK (0xFU)
#define SMIX_PCH_SRC_CCTRLN_ICS_SHIFT (0U)
#define SMIX_PCH_SRC_CCTRLN_ICS_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_CCTRLN_ICS_SHIFT) & SMIX_PCH_SRC_CCTRLN_ICS_MASK)
#define SMIX_PCH_SRC_CCTRLN_ICS_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_CCTRLN_ICS_MASK) >> SMIX_PCH_SRC_CCTRLN_ICS_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_PSTN */
/*
 * OUTF_FILL (RO)
 *
 * The fillings of the output FIFO
 */
#define SMIX_PCH_SRC_PSTN_OUTF_FILL_MASK (0x7F000000UL)
#define SMIX_PCH_SRC_PSTN_OUTF_FILL_SHIFT (24U)
#define SMIX_PCH_SRC_PSTN_OUTF_FILL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_OUTF_FILL_MASK) >> SMIX_PCH_SRC_PSTN_OUTF_FILL_SHIFT)

/*
 * INF_FILL (RO)
 *
 * The fillings of the input FIFO
 */
#define SMIX_PCH_SRC_PSTN_INF_FILL_MASK (0xFF8000UL)
#define SMIX_PCH_SRC_PSTN_INF_FILL_SHIFT (15U)
#define SMIX_PCH_SRC_PSTN_INF_FILL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_INF_FILL_MASK) >> SMIX_PCH_SRC_PSTN_INF_FILL_SHIFT)

/*
 * IC_CNT (RO)
 *
 * Input channel counter
 */
#define SMIX_PCH_SRC_PSTN_IC_CNT_MASK (0x7800U)
#define SMIX_PCH_SRC_PSTN_IC_CNT_SHIFT (11U)
#define SMIX_PCH_SRC_PSTN_IC_CNT_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_IC_CNT_MASK) >> SMIX_PCH_SRC_PSTN_IC_CNT_SHIFT)

/*
 * OC_CNT (RO)
 *
 * Output channel counter
 */
#define SMIX_PCH_SRC_PSTN_OC_CNT_MASK (0x780U)
#define SMIX_PCH_SRC_PSTN_OC_CNT_SHIFT (7U)
#define SMIX_PCH_SRC_PSTN_OC_CNT_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_OC_CNT_MASK) >> SMIX_PCH_SRC_PSTN_OC_CNT_SHIFT)

/*
 * OTSK_OL (RO)
 *
 * Output task is overloaded
 */
#define SMIX_PCH_SRC_PSTN_OTSK_OL_MASK (0x40U)
#define SMIX_PCH_SRC_PSTN_OTSK_OL_SHIFT (6U)
#define SMIX_PCH_SRC_PSTN_OTSK_OL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_OTSK_OL_MASK) >> SMIX_PCH_SRC_PSTN_OTSK_OL_SHIFT)

/*
 * ITSK_OL (RO)
 *
 * Input task is overloaded.
 */
#define SMIX_PCH_SRC_PSTN_ITSK_OL_MASK (0x20U)
#define SMIX_PCH_SRC_PSTN_ITSK_OL_SHIFT (5U)
#define SMIX_PCH_SRC_PSTN_ITSK_OL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_ITSK_OL_MASK) >> SMIX_PCH_SRC_PSTN_ITSK_OL_SHIFT)

/*
 * OD_OVFL (RO)
 *
 * Out data overflowed
 */
#define SMIX_PCH_SRC_PSTN_OD_OVFL_MASK (0x10U)
#define SMIX_PCH_SRC_PSTN_OD_OVFL_SHIFT (4U)
#define SMIX_PCH_SRC_PSTN_OD_OVFL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_OD_OVFL_MASK) >> SMIX_PCH_SRC_PSTN_OD_OVFL_SHIFT)

/*
 * ID_UDFL (RO)
 *
 * In data underflowed
 */
#define SMIX_PCH_SRC_PSTN_ID_UDFL_MASK (0x8U)
#define SMIX_PCH_SRC_PSTN_ID_UDFL_SHIFT (3U)
#define SMIX_PCH_SRC_PSTN_ID_UDFL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_ID_UDFL_MASK) >> SMIX_PCH_SRC_PSTN_ID_UDFL_SHIFT)

/*
 * DA (RO)
 *
 * Output data available
 */
#define SMIX_PCH_SRC_PSTN_DA_MASK (0x4U)
#define SMIX_PCH_SRC_PSTN_DA_SHIFT (2U)
#define SMIX_PCH_SRC_PSTN_DA_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_DA_MASK) >> SMIX_PCH_SRC_PSTN_DA_SHIFT)

/*
 * DN (RO)
 *
 * Input data needed
 */
#define SMIX_PCH_SRC_PSTN_DN_MASK (0x2U)
#define SMIX_PCH_SRC_PSTN_DN_SHIFT (1U)
#define SMIX_PCH_SRC_PSTN_DN_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_DN_MASK) >> SMIX_PCH_SRC_PSTN_DN_SHIFT)

/*
 * INI_DONE (RO)
 *
 * INI done
 */
#define SMIX_PCH_SRC_PSTN_INI_DONE_MASK (0x1U)
#define SMIX_PCH_SRC_PSTN_INI_DONE_SHIFT (0U)
#define SMIX_PCH_SRC_PSTN_INI_DONE_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_PSTN_INI_DONE_MASK) >> SMIX_PCH_SRC_PSTN_INI_DONE_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_IDRTN */
/*
 * VAL (RW)
 *
 * The value of the ideal ratio, with 24 bits fractional parts
 */
#define SMIX_PCH_SRC_IDRTN_VAL_MASK (0xFFFFFFFFUL)
#define SMIX_PCH_SRC_IDRTN_VAL_SHIFT (0U)
#define SMIX_PCH_SRC_IDRTN_VAL_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_IDRTN_VAL_SHIFT) & SMIX_PCH_SRC_IDRTN_VAL_MASK)
#define SMIX_PCH_SRC_IDRTN_VAL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_IDRTN_VAL_MASK) >> SMIX_PCH_SRC_IDRTN_VAL_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_ACTRTN */
/*
 * VAL (RW)
 *
 * The value of the actual ratio, with 24 bits fractional parts
 */
#define SMIX_PCH_SRC_ACTRTN_VAL_MASK (0xFFFFFFFFUL)
#define SMIX_PCH_SRC_ACTRTN_VAL_SHIFT (0U)
#define SMIX_PCH_SRC_ACTRTN_VAL_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_ACTRTN_VAL_SHIFT) & SMIX_PCH_SRC_ACTRTN_VAL_MASK)
#define SMIX_PCH_SRC_ACTRTN_VAL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_ACTRTN_VAL_MASK) >> SMIX_PCH_SRC_ACTRTN_VAL_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_DIN */
/*
 * VAL (RW)
 *
 * The input data register
 */
#define SMIX_PCH_SRC_DIN_VAL_MASK (0xFFFFFFFFUL)
#define SMIX_PCH_SRC_DIN_VAL_SHIFT (0U)
#define SMIX_PCH_SRC_DIN_VAL_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DIN_VAL_SHIFT) & SMIX_PCH_SRC_DIN_VAL_MASK)
#define SMIX_PCH_SRC_DIN_VAL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DIN_VAL_MASK) >> SMIX_PCH_SRC_DIN_VAL_SHIFT)

/* Bitfield definition for register of struct array PCH: SRC_DON */
/*
 * VAL (RW)
 *
 * The output data register
 */
#define SMIX_PCH_SRC_DON_VAL_MASK (0xFFFFFFFFUL)
#define SMIX_PCH_SRC_DON_VAL_SHIFT (0U)
#define SMIX_PCH_SRC_DON_VAL_SET(x) (((uint32_t)(x) << SMIX_PCH_SRC_DON_VAL_SHIFT) & SMIX_PCH_SRC_DON_VAL_MASK)
#define SMIX_PCH_SRC_DON_VAL_GET(x) (((uint32_t)(x) & SMIX_PCH_SRC_DON_VAL_MASK) >> SMIX_PCH_SRC_DON_VAL_SHIFT)

/* Bitfield definition for register: SMIX_CALSAT_ST */
/*
 * DST (W1C)
 *
 * DST CAL_SAT_ERR. W1C
 */
#define SMIX_SMIX_CALSAT_ST_DST_MASK (0xC0000000UL)
#define SMIX_SMIX_CALSAT_ST_DST_SHIFT (30U)
#define SMIX_SMIX_CALSAT_ST_DST_SET(x) (((uint32_t)(x) << SMIX_SMIX_CALSAT_ST_DST_SHIFT) & SMIX_SMIX_CALSAT_ST_DST_MASK)
#define SMIX_SMIX_CALSAT_ST_DST_GET(x) (((uint32_t)(x) & SMIX_SMIX_CALSAT_ST_DST_MASK) >> SMIX_SMIX_CALSAT_ST_DST_SHIFT)

/*
 * RRSV (RO)
 *
 * Reserved
 */
#define SMIX_SMIX_CALSAT_ST_RRSV_MASK (0x3FFFC000UL)
#define SMIX_SMIX_CALSAT_ST_RRSV_SHIFT (14U)
#define SMIX_SMIX_CALSAT_ST_RRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_CALSAT_ST_RRSV_MASK) >> SMIX_SMIX_CALSAT_ST_RRSV_SHIFT)

/*
 * SRC (W1C)
 *
 * SRC CAL_SAT_ERR. W1C
 */
#define SMIX_SMIX_CALSAT_ST_SRC_MASK (0x3FFFU)
#define SMIX_SMIX_CALSAT_ST_SRC_SHIFT (0U)
#define SMIX_SMIX_CALSAT_ST_SRC_SET(x) (((uint32_t)(x) << SMIX_SMIX_CALSAT_ST_SRC_SHIFT) & SMIX_SMIX_CALSAT_ST_SRC_MASK)
#define SMIX_SMIX_CALSAT_ST_SRC_GET(x) (((uint32_t)(x) & SMIX_SMIX_CALSAT_ST_SRC_MASK) >> SMIX_SMIX_CALSAT_ST_SRC_SHIFT)

/* Bitfield definition for register: SMIX_FDOT_DONE_ST */
/*
 * DST (W1C)
 *
 * DST fadeout done. W1C
 */
#define SMIX_SMIX_FDOT_DONE_ST_DST_MASK (0xC0000000UL)
#define SMIX_SMIX_FDOT_DONE_ST_DST_SHIFT (30U)
#define SMIX_SMIX_FDOT_DONE_ST_DST_SET(x) (((uint32_t)(x) << SMIX_SMIX_FDOT_DONE_ST_DST_SHIFT) & SMIX_SMIX_FDOT_DONE_ST_DST_MASK)
#define SMIX_SMIX_FDOT_DONE_ST_DST_GET(x) (((uint32_t)(x) & SMIX_SMIX_FDOT_DONE_ST_DST_MASK) >> SMIX_SMIX_FDOT_DONE_ST_DST_SHIFT)

/*
 * RRSV1 (RO)
 *
 * Reserved
 */
#define SMIX_SMIX_FDOT_DONE_ST_RRSV1_MASK (0x3FFFC000UL)
#define SMIX_SMIX_FDOT_DONE_ST_RRSV1_SHIFT (14U)
#define SMIX_SMIX_FDOT_DONE_ST_RRSV1_GET(x) (((uint32_t)(x) & SMIX_SMIX_FDOT_DONE_ST_RRSV1_MASK) >> SMIX_SMIX_FDOT_DONE_ST_RRSV1_SHIFT)

/*
 * SRC (W1C)
 *
 * SRC fadeout done. W1C
 */
#define SMIX_SMIX_FDOT_DONE_ST_SRC_MASK (0x3FFFU)
#define SMIX_SMIX_FDOT_DONE_ST_SRC_SHIFT (0U)
#define SMIX_SMIX_FDOT_DONE_ST_SRC_SET(x) (((uint32_t)(x) << SMIX_SMIX_FDOT_DONE_ST_SRC_SHIFT) & SMIX_SMIX_FDOT_DONE_ST_SRC_MASK)
#define SMIX_SMIX_FDOT_DONE_ST_SRC_GET(x) (((uint32_t)(x) & SMIX_SMIX_FDOT_DONE_ST_SRC_MASK) >> SMIX_SMIX_FDOT_DONE_ST_SRC_SHIFT)

/* Bitfield definition for register: SMIX_DATA_ST */
/*
 * DST_DA (RO)
 *
 * DST data available
 */
#define SMIX_SMIX_DATA_ST_DST_DA_MASK (0xC0000000UL)
#define SMIX_SMIX_DATA_ST_DST_DA_SHIFT (30U)
#define SMIX_SMIX_DATA_ST_DST_DA_GET(x) (((uint32_t)(x) & SMIX_SMIX_DATA_ST_DST_DA_MASK) >> SMIX_SMIX_DATA_ST_DST_DA_SHIFT)

/*
 * DST_UNDL (RO)
 *
 * DST data underflow
 */
#define SMIX_SMIX_DATA_ST_DST_UNDL_MASK (0x30000000UL)
#define SMIX_SMIX_DATA_ST_DST_UNDL_SHIFT (28U)
#define SMIX_SMIX_DATA_ST_DST_UNDL_GET(x) (((uint32_t)(x) & SMIX_SMIX_DATA_ST_DST_UNDL_MASK) >> SMIX_SMIX_DATA_ST_DST_UNDL_SHIFT)

/*
 * RRSV1 (RO)
 *
 * Reserved
 */
#define SMIX_SMIX_DATA_ST_RRSV1_MASK (0xFFFC000UL)
#define SMIX_SMIX_DATA_ST_RRSV1_SHIFT (14U)
#define SMIX_SMIX_DATA_ST_RRSV1_GET(x) (((uint32_t)(x) & SMIX_SMIX_DATA_ST_RRSV1_MASK) >> SMIX_SMIX_DATA_ST_RRSV1_SHIFT)

/*
 * SRC_DN (RO)
 *
 * SRC data needed
 */
#define SMIX_SMIX_DATA_ST_SRC_DN_MASK (0x3FFFU)
#define SMIX_SMIX_DATA_ST_SRC_DN_SHIFT (0U)
#define SMIX_SMIX_DATA_ST_SRC_DN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DATA_ST_SRC_DN_MASK) >> SMIX_SMIX_DATA_ST_SRC_DN_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_CTRL */
/*
 * WRSV (RW)
 *
 * Reserved
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_MASK (0xFFE00000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_SHIFT (21U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_WRSV_SHIFT)

/*
 * DATA_UNFL_IE (RW)
 *
 * Data Underflow Error IntEn
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_MASK (0x100000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_SHIFT (20U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DATA_UNFL_IE_SHIFT)

/*
 * THRSH (RW)
 *
 * FIFO threshold for DMA or Int. >= will generate req.  Must be greater or equal than 8. The read burst of DMA should make the fillings in the buffer be greater than 4.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_MASK (0xFF000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_SHIFT (12U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_THRSH_SHIFT)

/*
 * CALSAT_INT_EN (RW)
 *
 * Cal Saturation IntEn
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_MASK (0x800U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_SHIFT (11U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_CALSAT_INT_EN_SHIFT)

/*
 * DA_INT_EN (RW)
 *
 * Data Available IntEn
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_MASK (0x400U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_SHIFT (10U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DA_INT_EN_SHIFT)

/*
 * ADEACTFADEOUT_EN (RW)
 *
 * AutoDeactAfterFadeOut_En:
 * Asserted to enter de-activated mode after fade-out done
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_MASK (0x200U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_SHIFT (9U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_ADEACTFADEOUT_EN_SHIFT)

/*
 * FADEOUT_DONE_IE (RW)
 *
 * Fade-Out interrupt enable
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_MASK (0x100U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_FADEOUT_DONE_IE_SHIFT)

/*
 * DST_DEACT (RW)
 *
 * de-activate the destination channel
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_MASK (0x80U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_SHIFT (7U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_DEACT_SHIFT)

/*
 * DST_ACT (RW)
 *
 * activate the destination channel
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_MASK (0x40U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_SHIFT (6U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_ACT_SHIFT)

/*
 * DSTFADOUT_MEN (RW)
 *
 * Manual FadeOut_Ctrl for destionation. Auto clear.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_MASK (0x20U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_SHIFT (5U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_MEN_SHIFT)

/*
 * DSTFADOUT_AEN (RW)
 *
 * Automatically FadeOut_Ctrl for destionation. Only effective after DST_AFADEOUT is assigned a non-zero value
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_MASK (0x10U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_SHIFT (4U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADOUT_AEN_SHIFT)

/*
 * DSTFADIN_EN (RW)
 *
 * FadeIn_Ctrl for destionation. Auto clear.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_MASK (0x8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_SHIFT (3U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DSTFADIN_EN_SHIFT)

/*
 * DST_EN (RW)
 *
 * Dst enabled. When disabled, clear the FIFO pointers.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_MASK (0x4U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_SHIFT (2U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_DST_EN_SHIFT)

/*
 * SOFTRST (RW)
 *
 * Soft reset
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_MASK (0x2U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_SHIFT (1U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_SOFTRST_SHIFT)

/*
 * MIXER_EN (RW)
 *
 * mixer function enable.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_MASK (0x1U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_CTRL_MIXER_EN_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_GAIN */
/*
 * WRSV (RW)
 *
 * Reserved
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_MASK (0xFFFF8000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_SHIFT (15U)
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_GAIN_WRSV_SHIFT)

/*
 * VAL_14_0 (RW)
 *
 * Unsigned Int, with 12 fractional bits. . The top 3 bits are for shift. Same as SHFT_CTR[2:0]
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_MASK (0x7FFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_GAIN_VAL_14_0_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_MIDX */
/*
 * MAX_IDX (RW)
 *
 * The total length of the dst stream -1. If zero, means there is no  end of the stream.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_MASK (0xFFFFFFFFUL)
#define SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_MIDX_MAX_IDX_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_FADEIN */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_MASK (0xFFF00000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_SHIFT (20U)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_WRSV_SHIFT)

/*
 * DELTA (RW)
 *
 * Fade-in delta for linear fading in from 0 to 1 (about at most 20s for 48kHz sampled sound)
 * (Using only top 14 bits for mul)
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_MASK (0xFFFFFUL)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_FADEIN_DELTA_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_FADEOUT */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_MASK (0xFFF00000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_SHIFT (20U)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_WRSV_SHIFT)

/*
 * DELTA (RW)
 *
 * Fade out in 2^DELTA samples.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_MASK (0xFFFFFUL)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_FADEOUT_DELTA_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_ST */
/*
 * RRSV (RO)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_RRSV_MASK (0xFFFF8000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_RRSV_SHIFT (15U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_RRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_ST_RRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_ST_RRSV_SHIFT)

/*
 * FIFO_FILLINGS (RO)
 *
 * destination channel output FIFO fillings
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_FIFO_FILLINGS_MASK (0x7FC0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_FIFO_FILLINGS_SHIFT (6U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_FIFO_FILLINGS_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_ST_FIFO_FILLINGS_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_ST_FIFO_FILLINGS_SHIFT)

/*
 * FDOUT_DONE (RO)
 *
 * Fade-Out Done. W1C
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_FDOUT_DONE_MASK (0x20U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_FDOUT_DONE_SHIFT (5U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_FDOUT_DONE_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_ST_FDOUT_DONE_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_ST_FDOUT_DONE_SHIFT)

/*
 * CALSAT (RO)
 *
 * Saturate Error Found. W1C
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_CALSAT_MASK (0x10U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_CALSAT_SHIFT (4U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_CALSAT_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_ST_CALSAT_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_ST_CALSAT_SHIFT)

/*
 * DA (RO)
 *
 * Data Available
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_DA_MASK (0x8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_DA_SHIFT (3U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_DA_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_ST_DA_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_ST_DA_SHIFT)

/*
 * MODE (RO)
 *
 * The modes are:
 * Mode 0: Disabled: after reset. Program the registers, and SMIX_DSTn_CTRL [DST_EN] to enter Mode 1.
 * Mode 1: Enabled and not-activated. wait for SMIX_DSTn_CTRL [DSTFADIN_EN] or SMIX_DSTn_CTRL [DST_ACT], jump to Mode 3 or Mode 4 based on whether Fade-in enabled.
 * Mode 3: Enabled and activated and fade-in in progress: Can not be fade out. Will send data to DMA. Jump to Mode 4 after fadin op done.
 * Mode 4: Enabled and activated and done fade-in, no fade-out yet: Can be fade out. Will send data to DMA.
 * Mode 5: Enabled and activated and fade-out in progress: After faded out OP. Will send data to DMA. Will transfer to mode 6 or mode 7 depending on the SMIX_DSTn_CTRL [ADeactFadeOut_En] cfg
 * Mode 6: Enabled and activated and faded-out: faded out is done. Will send data to DMA. Will transfer to mode 7 if manual deactivated.
 * Mode 7: Enabled and De-activated: If configured to enter this mode, after auto or manuallly fade-out, or after manual de-activated. Won't send data to DMA. Won't gen data avail signals. Intf register can be programmed. Will change to Mode 3 or Mode 4  after manual ACT or Fade-in CMD. Will transfer to Mode 0 if SMIX_DSTn_CTRL [DST_EN] is assigned 0. To support a new stream or, to continue the old stream after a pause.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_MODE_MASK (0x7U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_MODE_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_ST_MODE_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_ST_MODE_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_ST_MODE_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_DATA */
/*
 * VAL (RO)
 *
 * Output data buffer
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_DATA_VAL_MASK (0xFFFFFFFFUL)
#define SMIX_SMIX_DCH_SMIX_DSTN_DATA_VAL_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_DATA_VAL_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_DATA_VAL_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_DATA_VAL_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_SRC_EN */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_MASK (0xFFFFFF00UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_WRSV_SHIFT)

/*
 * VAL (RW)
 *
 * After enabled, Data needed req will be asserted. DMA can feed in data. The channel will join in the sum operation of mixer operation.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_MASK (0xFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_EN_VAL_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_SRC_ACT */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_MASK (0xFFFFFF00UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_WRSV_SHIFT)

/*
 * VAL (WO)
 *
 * Manually Activate the channel
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_MASK (0xFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_ACT_VAL_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_SRC_DEACT */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_MASK (0xFFFFFF00UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_WRSV_SHIFT)

/*
 * VAL (WO)
 *
 * Manually DeActivate the channel
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_MASK (0xFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_VAL_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_SRC_FADEIN_CTRL */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_MASK (0xFFFFFF00UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_WRSV_SHIFT)

/*
 * AOP (RW)
 *
 * Asserted to start fade-in operation. When the amplification factors are stable, auto clear.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_MASK (0xFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_FADEIN_CTRL_AOP_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_SRC_DEACT_ST */
/*
 * DST_DEACT (RO)
 *
 * Asserted when in de-active mode
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_DST_DEACT_MASK (0x80000000UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_DST_DEACT_SHIFT (31U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_DST_DEACT_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_DST_DEACT_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_DST_DEACT_SHIFT)

/*
 * RRSV (RO)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_RRSV_MASK (0x7FFFFF00UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_RRSV_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_RRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_RRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_RRSV_SHIFT)

/*
 * SRC_DEACT_ST (RO)
 *
 * Asserted when in de-active mode
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_SRC_DEACT_ST_MASK (0xFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_SRC_DEACT_ST_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_SRC_DEACT_ST_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_SRC_DEACT_ST_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_DEACT_ST_SRC_DEACT_ST_SHIFT)

/* Bitfield definition for register of struct array SMIX_DCH: SMIX_DSTN_SRC_MFADEOUT_CTRL */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_MASK (0xFFFFFF00UL)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_SHIFT (8U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_WRSV_SHIFT)

/*
 * OP (RW)
 *
 * Asserted to start fade-out operation. When the amplification factors are stable, auto clear.
 */
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_MASK (0xFFU)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_SHIFT (0U)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_SET(x) (((uint32_t)(x) << SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_SHIFT) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_MASK)
#define SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_GET(x) (((uint32_t)(x) & SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_MASK) >> SMIX_SMIX_DCH_SMIX_DSTN_SRC_MFADEOUT_CTRL_OP_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_CTRL */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_MASK (0xFFC00000UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_SHIFT (22U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_WRSV_SHIFT)

/*
 * FIFO_RESET (RW)
 *
 * Asserted to reset FIFO pointer. Cleared to exit reset state.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_MASK (0x200000UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_SHIFT (21U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FIFO_RESET_SHIFT)

/*
 * THRSH (RW)
 *
 * FIFO threshold for DMA or Int. <= will generate req. Must be greater or equal than 8. This threshold is also used to trgger the internal FIR operation. To avoid the reading and writing to the same address in the memory block, the threshold should greater than 4.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_MASK (0x1FE000UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_SHIFT (13U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_THRSH_SHIFT)

/*
 * CALSAT_INT_EN (RW)
 *
 * Cal Saturation IntEn
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_MASK (0x1000U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_SHIFT (12U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_CALSAT_INT_EN_SHIFT)

/*
 * DN_INT_EN (RW)
 *
 * Data Needed IntEn
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_MASK (0x800U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_SHIFT (11U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_DN_INT_EN_SHIFT)

/*
 * SHFT_CTRL (RW)
 *
 * Shift operation after FIR
 * 0: no shift (when no rate change or rate /2)
 * 1: x2 (rate x2, or rate x3)
 * 2: x4 (rate x4, rate x6)
 * 3: x8 (rate x8, rate x12)
 * others: reserved
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_MASK (0x700U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_SHIFT (8U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_SHFT_CTRL_SHIFT)

/*
 * AUTODEACTAFTERFADEOUT_EN (RW)
 *
 * Asserted to enter de-activated mode after fade-out done
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_MASK (0x80U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_SHIFT (7U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_AUTODEACTAFTERFADEOUT_EN_SHIFT)

/*
 * FADEOUT_DONE_IE (RW)
 *
 * Fade-Out interrupt enable
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_MASK (0x40U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_SHIFT (6U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_FADEOUT_DONE_IE_SHIFT)

/*
 * RATECONV (RW)
 *
 * 0: no rate conversion
 * 1: up-conversion x2
 * 2: up-conversion x3
 * 3: up-conversion x4
 * 4: up-conversion x6
 * 5: up-conversion x8
 * 6: up-conversion x12
 * 7: down-conversion /2
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_MASK (0x7U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_CTRL_RATECONV_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_GAIN */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_MASK (0xFFFF8000UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_SHIFT (15U)
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_GAIN_WRSV_SHIFT)

/*
 * VAL (RW)
 *
 * Unsigned Int, with 12 fractional bits. The top 3 bits are for shift. Same as SHFT_CTR[2:0].
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_MASK (0x7FFFU)
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_GAIN_VAL_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_FADEIN */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_MASK (0xFFF00000UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_SHIFT (20U)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_WRSV_SHIFT)

/*
 * DELTA (RW)
 *
 * Fade -in confg.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_MASK (0xFFFFFUL)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_FADEIN_DELTA_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_FADEOUT */
/*
 * WRSV (RW)
 *
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_MASK (0xFFF00000UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_SHIFT (20U)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_WRSV_SHIFT)

/*
 * DELTA (RW)
 *
 * Fade out in 2^DELTA samples.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_MASK (0xFFFFFUL)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_FADEOUT_DELTA_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_BUFSIZE */
/*
 * MAXIDX (RW)
 *
 * unit as 16-bits per sample. Zero means no length limit. = Act Len-1.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_MASK (0xFFFFFFFFUL)
#define SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_BUFSIZE_MAXIDX_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_ST */
/*
 * FIFO_FILLINGS (RO)
 *
 * The fillings of input FIFO.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FIFO_FILLINGS_MASK (0x7FC00UL)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FIFO_FILLINGS_SHIFT (10U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FIFO_FILLINGS_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_ST_FIFO_FILLINGS_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_ST_FIFO_FILLINGS_SHIFT)

/*
 * FDOUT_DONE (W1C)
 *
 * Fade-Out Done. W1C
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_MASK (0x200U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_SHIFT (9U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_ST_FDOUT_DONE_SHIFT)

/*
 * CALSAT (W1C)
 *
 * Calculation saturation status. W1C
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_MASK (0x100U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_SHIFT (8U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_ST_CALSAT_SHIFT)

/*
 * DN (RO)
 *
 * Data needed flag
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_DN_MASK (0x80U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_DN_SHIFT (7U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_DN_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_ST_DN_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_ST_DN_SHIFT)

/*
 * FIRPHASE (RO)
 *
 * the poly phase counter
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FIRPHASE_MASK (0x78U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FIRPHASE_SHIFT (3U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_FIRPHASE_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_ST_FIRPHASE_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_ST_FIRPHASE_SHIFT)

/*
 * MODE (RO)
 *
 * The modes are:
 * Mode 0: Disabled: after reset. Program the registers, and SMIX_DSTx_SRC_EN[n] to enter Mode 1.
 * Mode 1: Enabled but not activated: After Enabled. Data needed signal can send out, can receive DMA data. Will enter Mode 2 after manual ACT or Fade-in CMD
 * Mode 2: Enabled and activated and buffer feed-in in progress: Can not be fade out. Will consume data from DMA. If not enter due to Fade-in CMD, will enter Mode 4, else enter Mode 3. This mode is used to make the channel in MIX only after initial data are ready, thus will not stall mix operation due to the lackness of data of this channel omly.
 * Mode 3: Enabled and activated and fade-in in progress: Can not be fade out. Will consume data from DMA.
 * Mode 4: Enabled and activated and done fade-in, no fade-out yet: Can be fade out. Will consume data from DMA.
 * Mode 5: Enabled and activated and fade-out in progress: After faded out done. Will consume data from DMA. Will transfer to mode 6 or mode 7 depending on the SMIX_SRCn_CTRL[AutoDeactAfterFadeOut_En] cfg
 * Mode 6: Enabled and activated and faded-out: faded out is done. Will consume data from DMA. Will transfer to mode 7 if manual deactivated.
 * Mode 7: Enabled and De-activated: If configured to enter this mode, after auto or manuallly fade-out, or after manual de-activated. Won't consume data from DMA. Won't gen data needed signals. Intf register can be programmed. Will change to Mode 2 after manual ACT or Fade-in CMD. Will transfer to Mode 0 if SMIX_DSTx_SRC_EN[n] is assigned 0. To support a new stream or, to continue the old stream after a pause.
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_MODE_MASK (0x7U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_MODE_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_ST_MODE_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_ST_MODE_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_ST_MODE_SHIFT)

/* Bitfield definition for register of struct array SMIX_SCH: SMIX_SRCN_DATA */
/*
 * VAL (WO)
 *
 * Data input register
 */
#define SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_MASK (0xFFFFFFFFUL)
#define SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_SHIFT (0U)
#define SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_SET(x) (((uint32_t)(x) << SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_SHIFT) & SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_MASK)
#define SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_GET(x) (((uint32_t)(x) & SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_MASK) >> SMIX_SMIX_SCH_SMIX_SRCN_DATA_VAL_SHIFT)



/* CH register group index macro definition */
#define SMIX_CH_0 (0UL)
#define SMIX_CH_1 (1UL)
#define SMIX_CH_2 (2UL)
#define SMIX_CH_3 (3UL)
#define SMIX_CH_4 (4UL)
#define SMIX_CH_5 (5UL)
#define SMIX_CH_6 (6UL)
#define SMIX_CH_7 (7UL)
#define SMIX_CH_8 (8UL)
#define SMIX_CH_9 (9UL)
#define SMIX_CH_10 (10UL)
#define SMIX_CH_11 (11UL)
#define SMIX_CH_12 (12UL)
#define SMIX_CH_13 (13UL)
#define SMIX_CH_14 (14UL)
#define SMIX_CH_15 (15UL)
#define SMIX_CH_16 (16UL)
#define SMIX_CH_17 (17UL)
#define SMIX_CH_18 (18UL)
#define SMIX_CH_19 (19UL)
#define SMIX_CH_20 (20UL)
#define SMIX_CH_21 (21UL)
#define SMIX_CH_22 (22UL)
#define SMIX_CH_23 (23UL)
#define SMIX_CH_24 (24UL)
#define SMIX_CH_25 (25UL)

/* PCH register group index macro definition */
#define SMIX_PCH_0 (0UL)
#define SMIX_PCH_1 (1UL)
#define SMIX_PCH_2 (2UL)

/* SMIX_DCH register group index macro definition */
#define SMIX_SMIX_DCH_0 (0UL)
#define SMIX_SMIX_DCH_1 (1UL)

/* SMIX_SCH register group index macro definition */
#define SMIX_SMIX_SCH_0 (0UL)
#define SMIX_SMIX_SCH_1 (1UL)
#define SMIX_SMIX_SCH_2 (2UL)
#define SMIX_SMIX_SCH_3 (3UL)
#define SMIX_SMIX_SCH_4 (4UL)
#define SMIX_SMIX_SCH_5 (5UL)
#define SMIX_SMIX_SCH_6 (6UL)
#define SMIX_SMIX_SCH_7 (7UL)
#define SMIX_SMIX_SCH_8 (8UL)
#define SMIX_SMIX_SCH_9 (9UL)
#define SMIX_SMIX_SCH_10 (10UL)
#define SMIX_SMIX_SCH_11 (11UL)
#define SMIX_SMIX_SCH_12 (12UL)
#define SMIX_SMIX_SCH_13 (13UL)


#endif /* HPM_SMIX_H */