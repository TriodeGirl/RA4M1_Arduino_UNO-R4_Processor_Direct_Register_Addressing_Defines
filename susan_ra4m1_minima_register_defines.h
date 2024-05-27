/*  Arduino UNO R4 Minima defines for core RA4M1 peripheral operations:
 *
 *  NOTE: Pin allocation is as per R4 Minima, the R4 WiFi has many board-pins with different connections.
 *
 *  Susan Parker, my code stuff may be found here: https://github.com/TriodeGirl
 *
 *  Susan Parker - 28th August 2023.
 *    Put all RA4M1 register defintions into this seperate susan_ra4m1_minima_register_defines.h file
 *
 *  Latest update: 05 Sept 2023
 *
 * This code is "AS IS" without warranty or liability.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed with the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

    See the GNU General Public License for more details.
*/

// Notes:
//
//  For setting up a hardware defined memory area for DMA transfers
//    https://interrupt.memfault.com/blog/noinit-memory
//

// ARM-developer - Accessing memory-mapped peripherals
// https://developer.arm.com/documentation/102618/0100

// ==== Option-Setting Memory  - This is programed into FLASH so can't be changed ====
#define OPTION 0x00000000 // Option Base - See 6.1 page 103
#define SYSTEM_OFS0  ((volatile unsigned int *)(OPTION + 0x0400))  // Option Function Select Register 0
#define SYSTEM_OFS1  ((volatile unsigned int *)(OPTION + 0x0404))  // Option Function Select Register 1
#define OFS1_HOCOFRQ1_2_0   14   // HOCO Frequency Setting 1; 000: 24 MHz, 010: 32 MHz, 100: 48 MHz, 101: 64 MHz


// ==== System & Clock Generation ====
#define SYSTEM 0x40010000 // ICU Base - See 13.2.6 page 233

// Register Write Protection - See section 12
// PRC0 Registers related to the clock generation circuit:
//   SCKDIVCR, SCKSCR, PLLCR, PLLCCR2, MEMWAIT, MOSCCR, HOCOCR, MOCOCR, CKOCR, TRCKCR,
//   OSTDCR, OSTDSR, SLCDSCKCR, EBCKOCR, MOCOUTCR, HOCOUTCR, MOSCWTCR, MOMCR, SOSCCR,
//   SOMCR, LOCOCR, LOCOUTCR, HOCOWTCR, USBCKCR
//
//   *SYSTEM_PRCR = 0xA501;     // Enable writing to the clock registers
//   *SYSTEM_PRCR = 0xA500;     // Disable writing to the clock registers
//
// Restrictions on setting the clock frequency: ICLK = PCLKA = PCLKB, PCLKD = PCLKA = PCLKB, ICLK = FCLKA
// Restrictions on the clock frequency ratio: (N: integer, and up to 64)
// ICLK:FCLK = N:1, ICLK:PCLKA = N: 1, ICLK:PCLKB = N: 1
// ICLK:PCLKC = N:1 or 1:N, ICLK:PCLKD= N:1 or 1:N
// PCLKB:PCLKC = 1:1 or 1:2 or 1:4 or 2:1 or 4:1 or 8:1
//
// After reset:
// 0100 0100 0000 0100 MSBs
// 0100 0100 0100 0100 LSBs
//
// From Arduino Bootloader/IDE
// 0001 0000 0000 0001 MSBs
// 0000 0001 0000 0000 LSBs


#define SYSTEM_PRCR  ((volatile unsigned short *)(SYSTEM + 0xE3FE))    // Protect Register
#define PRCR_PRC0           0   // Enables or disables writing to clock generation registers
#define PRCR_PRC1           1   // En/Dis writing to the low power modes and battery backup function registers
#define PRCR_PRC3           3   // Enables or disables writing to the registers related to the LVD
#define PRCR_PRKEY_7_0      8   // Control write access to the PRCR register.
#define PRCR_PRKEY       0xA5   // PRC Key Code - write to the upper 8 bits

#define SYSTEM_SCKDIVCR  ((volatile unsigned int *)(SYSTEM + 0xE020))  // System Clock Division Control Register
                                // SYSTEM_SCKDIVCR = 10010100  Note: bit-7 is 1, and should be reading as 0
#define SCKDIVCR_PCKD_2_0   0   // Peripheral Module Clock D           = 4; 1/16
#define SCKDIVCR_PCKC_2_0   4   // Peripheral Module Clock C           = 1; 1/2
#define SCKDIVCR_PCKB_2_0   8   // Peripheral Module Clock B           = 0
#define SCKDIVCR_PCKA_2_0  12   // Peripheral Module Clock A           = 0
#define SCKDIVCR_ICK_2_0   24   // System Clock (ICLK) Select          = 0
#define SCKDIVCR_FCK_2_0   28   // Flash Interface Clock (FCLK) Select = 0
#define SYSTEM_SCKSCR  ((volatile unsigned char *)(SYSTEM + 0xE026))  // System Clock Source Control Register
#define SCKSCR_CKSEL_2_0    0   // Clock Source Select - See section 8.2.2
#define SYSTEM_PLLCR   ((volatile unsigned char *)(SYSTEM + 0xE02A))  // PLL Control Register
#define PLLCR_PLLSTP        0   // PLL Stop Control; 0: PLL is operating, 1: PLL is stopped
#define SYSTEM_PLLCCR2 ((volatile unsigned char *)(SYSTEM + 0xE02B))  // PLL Clock Control Register 2
#define PLLCCR2_PLLMUL_4_0  0   // PLL Frequency Multiplication Factor Select
#define PLLCCR2_PLODIV_1_0  6   // PLL Output Frequency Division Ratio Select
#define SYSTEM_MEMWAIT ((volatile unsigned char *)(SYSTEM + 0xE031))  // Memory Wait Cycle Control Register
#define MEMWAIT_MEMWAIT     0   // Memory Wait Cycle Select; 0: No wait, 1: Wait
#define SYSTEM_MOSCCR   ((volatile unsigned char *)(SYSTEM + 0xE032))  // Main Clock Oscillator Control Register
#define MOSCCR_MOSTP        0   // Main Clock Oscillator Stop; 0: Main clock oscillator is operating, 1: MCO is stopped
#define SYSTEM_HOCOCR   ((volatile unsigned char *)(SYSTEM + 0xE036))  // High-Speed On-Chip Oscillator Control Register
#define HOCOCR_HCSTP        0   // HOCO Stop; 0: HOCO is operating, 1: HOCO is stopped
#define SYSTEM_MOCOCR   ((volatile unsigned char *)(SYSTEM + 0xE038))  // Middle-Speed On-Chip Oscillator Control Register
#define MOCOCR_MCSTP        0   // MOCO Stop; 0: MOCO is operating, 1: MOCO is stopped
#define SYSTEM_OSCSF    ((volatile unsigned char *)(SYSTEM + 0xE03C))  // Oscillation Stabilization Flag Register
#define OSCSF_HOCOSF        0   // HOCO Clock Oscillation Stabilization Flag; 0: The HOCO clock is stopped or not stable, 1: The clock is stable
#define OSCSF_MOSCSF        3   // Main Clock Oscillation Stabilization Flag; 0: The Main clock is stopped or not stable, 1: The clock is stable
#define OSCSF_PLLSF         5   // PLL  Clock Oscillation Stabilization Flag; 0: The PLL  clock is stopped or not stable, 1: The clock is stable
#define SYSTEM_CKOCR    ((volatile unsigned char *)(SYSTEM + 0xE03E))  // Clock Out Control Register
#define CKOCR_CKOSEL_2_0    0   // Clock Out Source Select; 000: HOCO, 001: MOCO, 010: LOCO, 011: MOSC, 100: SOSC
#define CKOCR_CKODIV_2_0    4   // Clock Out Input Frequency Division Select; 000: ×1, 001: /2, 010: /4, ... , 111: /128
#define CKOCR_CKOEN         7   // Clock Out Enable; 0: Disable clock out, 1: Enable clock out
#define SYSTEM_TRCKCR   ((volatile unsigned char *)(SYSTEM + 0xE03F))  // Trace Clock Control Register
#define TRCKCR_TRCK_3_0     0   // Trace Clock Operation Frequency Select; 0000: /1, 0001: /2, 0010: /4 ( /2 = value after reset )
#define TRCKCR_TRCKEN       7   // Trace Clock Operating Enable; 0: Disable clock, 1: Enable clock

#define SYSTEM_OSTDCR   ((volatile unsigned char *)(SYSTEM + 0xE040))  // Oscillation Stop Detection Control Register
#define OSTDCR_OSTDIE       0   // Oscillation Stop Detection Interrupt Enable; 0: Disable oscillation stop detection interrupt, 1: Enable OSDI
#define OSTDCR_OSTDE        7   // Oscillation Stop Detection Function Enable; 0: Disable the oscillation stop detection function, 1: Enable OSDF
#define SYSTEM_OSTDSR   ((volatile unsigned char *)(SYSTEM + 0xE041))  // Oscillation Stop Detection Control Register
#define OSTDSR_OSTDF        0   // Oscillation Stop Detection Flag; 0: Main clock oscillation stop not detected, 1: Main clock oscillation stop detected

#define SYSTEM_SLCDSCKCR    ((volatile unsigned char *)(SYSTEM + 0xE050))  // Segment LCD Source Clock Control Register
#define SLCDSCKCR_LCDSCKSEL_2_0  0  // LCD Source Clock Select; 000: LOCO, 001: SOSC, 010: MOSC, 100: HOCO
#define SLCDSCKCR_LCDSCKEN       7  // LCD Source Clock Out Enable; 0: LCD source clock out disabled, 1: LCD source clock out enabled

#define SYSTEM_MOCOUTCR   ((volatile unsigned char *)(SYSTEM + 0xE061))  // MOCO User Trimming Control Register
#define MOCOUTCR_MOCOUTRM_7_0   0  // MOCO User Trimming - See: 8.2.21
#define SYSTEM_HOCOUTCR   ((volatile unsigned char *)(SYSTEM + 0xE062))  // HOCO User Trimming Control Register
#define HOCOUTCR_HOCOUTRM_7_0   0  // HOCO User Trimming - See: 8.2.21

#define SYSTEM_MOSCWTCR ((volatile unsigned char *)(SYSTEM + 0xE0A2))  // Main Clock Oscillator Wait Control Register
#define MOSCWTCR_MSTS_3_0   0   // Main Clock Oscillator Wait Time Setting
#define SYSTEM_HOCOWTCR ((volatile unsigned char *)(SYSTEM + 0xE0A5))  // High-Speed On-Chip Oscillator Wait Control Register
#define HOCOWTCR_MSTS_2_0   0   // HOCO Wait Time Setting

#define SYSTEM_USBCKCR  ((volatile unsigned char *)(SYSTEM + 0xE0D0))  // USB Clock Control Register
#define USBCKCR_USBCLKSEL   0   // USB Clock Source Select; 0: PLL (value after reset), 1: HOCO

#define SYSTEM_MOMCR    ((volatile unsigned char *)(SYSTEM + 0xE413))  // Main Clock Oscillator Mode Oscillation Control Register
#define MOMCR_MODRV1        3   // Main Clock Oscillator Drive Capability 1 Switching; 0: 10 MHz to 20 MHz, 1: 1 MHz to 10 MHz
#define MOMCR_MOSEL         6   // Main Clock Oscillator Switching; 0: Resonator, 1: External clock input

#define SYSTEM_SOSCCR   ((volatile unsigned char *)(SYSTEM + 0xE480))  // Sub-Clock Oscillator Control Register
#define SOSCCR_SOSTP        0   // Sub-Clock Oscillator Stop; 0: Operate the sub-clock oscillator, 1: Stop the sub-clock osc
#define SYSTEM_SOMCR    ((volatile unsigned char *)(SYSTEM + 0xE481))  // Sub-Clock Oscillator Mode Control Register
#define SOMCR_SODRV_1_0     0   // Sub-Clock Oscillator Drive Capability Switching; 00: Normal, 01: Low-power 1, 10: Low-power 2, 11: Low-power 3

#define SYSTEM_LOCOCR   ((volatile unsigned char *)(SYSTEM + 0xE490))  // Low-Speed On-Chip Oscillator Control Register
#define LOCOCR_LCSTP        0   // LOCO Stop; 0: Operate the LOCO clock, 1: Stop the LOCO clock
#define SYSTEM_LOCOUTCR ((volatile unsigned char *)(SYSTEM + 0xE492))  // LOCO User Trimming Control Register
#define LOCOUTCR_LOCOUTRM_7_0   0  // LOCO User Trimming - See: 8.2.20

// Reset Status
#define SYSTEM_RSTSR0   ((volatile unsigned char *)(SYSTEM + 0xE410))  // Reset Status Register 0
#define RSTSR0_PORF         0    // Power-On Reset Detect Flag
#define RSTSR0_LVD0RF       1    // Voltage Monitor 0 Reset Detect Flag
#define RSTSR0_LVD1RF       2    // Voltage Monitor 1 Reset Detect Flag
#define RSTSR0_LVD2RF       3    // Voltage Monitor 2 Reset Detect Flag
#define SYSTEM_RSTSR1   ((volatile unsigned char *)(SYSTEM + 0xE0C0))  // Reset Status Register 1
#define RSTSR1_IWDTRF       0    // Independent Watchdog Timer Reset Detect Flag
#define RSTSR1_WDTRF        1    // Watchdog Timer Reset Detect Flag
#define RSTSR1_SWRF         2    // Software Reset Detect Flag
#define SYSTEM_RSTSR2   ((volatile unsigned char *)(SYSTEM + 0xE411))  // Reset Status Register 2
#define RSTSR2_CWSF         0    // Cold/Warm Start Determination Flag - 0: Cold start, 1: Warm start

// Flash Memory
#define FCACHE_FCACHEE  ((volatile unsigned short *)(SYSTEM + 0xC100))  // Flash Cache Enable Register
#define FCACHEE_FCACHEEN    0    // FCACHE Enable - 0: Disable FCACHE, 1: Enable FCACHE.
#define FCACHE_FCACHEIV ((volatile unsigned short *)(SYSTEM + 0xC100))  // Flash Cache Invalidate Register
#define FCACHEIV_FCACHEIV   0    // Flash Cache Invalidate - 0: Do not invalidate, 1: Invalidate
#define FLCN_DFLEN      ((volatile unsigned char  *)(SYSTEM + 0xC090))  // Data Flash Control Resister
#define DFLEN_DFLEN         0    // Data Flash Access Enable - 0: Access is disabled, 1: Access is enabled


// System Control Block - for Software initiated reset, e.g. needed to reinitialse USB with PC
#define SCBBASE 0xE0000000
#define SCB_AIRCR                  ((volatile unsigned int *)(SCBBASE + 0xED0C))
#define SCB_AIRCR_VECTKEY_Pos      16U                                   // SCB AIRCR: VECTKEY Position
#define SCB_AIRCR_SYSRESETREQ_Pos  2U                                    // SCB AIRCR: SYSRESETREQ Position
#define SCB_AIRCR_SYSRESETREQ_Msk  (1UL << SCB_AIRCR_SYSRESETREQ_Pos)    // SCB AIRCR: SYSRESETREQ Mask
//
// *SCB_AIRCR = ((0x5FA << SCB_AIRCR_VECTKEY_Pos) | SCB_AIRCR_SYSRESETREQ_Msk);
//
// From: https://mcuoneclipse.com/2015/07/01/how-to-reset-an-arm-cortex-m-with-software/


// ==== Clock Frequency Accuracy Measurement Circuit (CAC) ====
#define CACBASE 0x40040000 //
#define CAC_CACR0       ((volatile unsigned char *)(CACBASE + 0x4600))  // CAC Control Register 0
#define CACR0_CFME          0   // Clock Frequency Measurement Enable; 0: Disable, 1: Enable
#define CAC_CACR1       ((volatile unsigned char *)(CACBASE + 0x4601))  // CAC Control Register 1
#define CACR1_CACREFE       0   // CACREF Pin Input Enable; 0: Disable, 1: Enable
#define CACR1_FMCS_2_0      1   // Measurement Target Clock (MTC) Select; 0b010: HOCO clock
#define CACR1_TCSS_1_0      4   // MTC Freq Div Ratio Select; 0b00: None, 0b01: ×1/4 clock, 0b10: ×1/8 clock, 0b11: ×1/32 clock
#define CACR1_EDGES_1_0     6   // Valid Edge Select; 0b00: Rising edge, 0b01: Falling edge, 0b10: Both rising and falling edges
#define CAC_CACR2       ((volatile unsigned char *)(CACBASE + 0x4602))  // CAC Control Register 2
#define CACR2_RPS           0   // Reference Signal Select; 0: CACREF pin input, 1: Internal clock (internally generated signal)
#define CACR2_RSCS_2_0      1   // Measurement Reference Clock (MRC) Select;
#define CACR2_RCDS_1_0      4   // MRC Freq Div Ratio Select; 0b00: ×1/32, 0b01: ×1/128, 0b10: ×1/1024, 0b11: ×1/8192
#define CACR2_DFS_1_0       6   // Digital Filter Select; 0b00: Disable digital filtering, etc.
#define CAC_CAICR       ((volatile unsigned char *)(CACBASE + 0x4603))  // CAC Interrupt Control Register
#define CAICR_FERRIE        0   // Frequency Error Interrupt Request Enable; 1: Enabled
#define CAICR_MENDIE        1   // Measurement End Interrupt Request Enable; 1: Enabled
#define CAICR_OVFIE         2   // Overflow Interrupt Request Enable; 1: Enabled
#define CAICR_FERRFCL       4   // FERRF Clear; When 1 is written to this bit, the FERRF flag is cleared. This bit is read as 0.
#define CAICR_MENDFCL       5   // MENDF Clear;  ditto
#define CAICR_OVFFCL        6   // OVFF Clear;   ditto
#define CAC_CASTR       ((volatile unsigned char *)(CACBASE + 0x4604))  // CAC Status Register
#define CASTR_FERRF         0   // Frequency Error Flag; 0: Clock frequency is within the allowable range, 1: ... frequency error
#define CASTR_MENDF         1   // Measurement End Flag; 0: Measurement is in progress, 1: Measurement ended
#define CASTR_OVFF          2   // Overflow Flag; 0: The counter has not overflowed, 1: The counter overflowed
#define CAC_CAULVR      ((volatile unsigned short *)(CACBASE + 0x4606))  // CAC Upper-Limit Value Setting Register
#define CAC_CALLVR      ((volatile unsigned short *)(CACBASE + 0x4608))  // CAC Lower-Limit Value Setting Register
#define CAC_CACNTBR     ((volatile unsigned short *)(CACBASE + 0x460A))  // CAC Counter Buffer Register


// ==== Interrupt Control Unit ====

/* Default Arduino Startup - see section 13.3.2 for Event Number info
 0 33 USBFS_USBI
 1 34 USBFS_USBR
 2 31 USBFS_D0FIFO
 3 32 USBFS_D1FIFO
 4 1E AGT0_AGTI
 ...
*/

#define ICUBASE 0x40000000 // ICU Base - See 13.2.6 page 233
// 32 bits -
#define IELSR 0x6300 // ICU Event Link Setting Register n
#define ICU_IELSR00 ((volatile unsigned int *)(ICUBASE + IELSR))            //
#define ICU_IELSR01 ((volatile unsigned int *)(ICUBASE + IELSR + ( 1 * 4))) //
#define ICU_IELSR02 ((volatile unsigned int *)(ICUBASE + IELSR + ( 2 * 4))) //
#define ICU_IELSR03 ((volatile unsigned int *)(ICUBASE + IELSR + ( 3 * 4))) //
#define ICU_IELSR04 ((volatile unsigned int *)(ICUBASE + IELSR + ( 4 * 4))) //
#define ICU_IELSR05 ((volatile unsigned int *)(ICUBASE + IELSR + ( 5 * 4))) //
#define ICU_IELSR06 ((volatile unsigned int *)(ICUBASE + IELSR + ( 6 * 4))) //
#define ICU_IELSR07 ((volatile unsigned int *)(ICUBASE + IELSR + ( 7 * 4))) //
#define ICU_IELSR08 ((volatile unsigned int *)(ICUBASE + IELSR + ( 8 * 4))) //
#define ICU_IELSR09 ((volatile unsigned int *)(ICUBASE + IELSR + ( 9 * 4))) //
#define ICU_IELSR10 ((volatile unsigned int *)(ICUBASE + IELSR + (10 * 4))) //
#define ICU_IELSR11 ((volatile unsigned int *)(ICUBASE + IELSR + (11 * 4))) //
#define ICU_IELSR12 ((volatile unsigned int *)(ICUBASE + IELSR + (12 * 4))) //
#define ICU_IELSR13 ((volatile unsigned int *)(ICUBASE + IELSR + (13 * 4))) //
#define ICU_IELSR14 ((volatile unsigned int *)(ICUBASE + IELSR + (14 * 4))) //
#define ICU_IELSR15 ((volatile unsigned int *)(ICUBASE + IELSR + (15 * 4))) //
#define ICU_IELSR16 ((volatile unsigned int *)(ICUBASE + IELSR + (16 * 4))) //
#define ICU_IELSR17 ((volatile unsigned int *)(ICUBASE + IELSR + (17 * 4))) //
#define ICU_IELSR18 ((volatile unsigned int *)(ICUBASE + IELSR + (18 * 4))) //
#define ICU_IELSR19 ((volatile unsigned int *)(ICUBASE + IELSR + (19 * 4))) //
#define ICU_IELSR20 ((volatile unsigned int *)(ICUBASE + IELSR + (20 * 4))) //
#define ICU_IELSR21 ((volatile unsigned int *)(ICUBASE + IELSR + (21 * 4))) //
#define ICU_IELSR22 ((volatile unsigned int *)(ICUBASE + IELSR + (22 * 4))) //
#define ICU_IELSR23 ((volatile unsigned int *)(ICUBASE + IELSR + (23 * 4))) //
#define ICU_IELSR24 ((volatile unsigned int *)(ICUBASE + IELSR + (24 * 4))) //
#define ICU_IELSR25 ((volatile unsigned int *)(ICUBASE + IELSR + (25 * 4))) //
#define ICU_IELSR26 ((volatile unsigned int *)(ICUBASE + IELSR + (26 * 4))) //
#define ICU_IELSR27 ((volatile unsigned int *)(ICUBASE + IELSR + (27 * 4))) //
#define ICU_IELSR28 ((volatile unsigned int *)(ICUBASE + IELSR + (28 * 4))) //
#define ICU_IELSR29 ((volatile unsigned int *)(ICUBASE + IELSR + (29 * 4))) //
#define ICU_IELSR30 ((volatile unsigned int *)(ICUBASE + IELSR + (30 * 4))) //
#define ICU_IELSR31 ((volatile unsigned int *)(ICUBASE + IELSR + (31 * 4))) //

// IRQ Event Numbers
#define IRQ_NO_EVENT         0x00
#define IRQ_PORT_IRQ0        0x01
#define IRQ_PORT_IRQ1        0x02
#define IRQ_PORT_IRQ2        0x03
#define IRQ_PORT_IRQ3        0x04
#define IRQ_PORT_IRQ4        0x05
#define IRQ_PORT_IRQ5        0x06
#define IRQ_PORT_IRQ6        0x07
#define IRQ_PORT_IRQ7        0x08
#define IRQ_PORT_IRQ8        0x09
#define IRQ_PORT_IRQ9        0x0A
#define IRQ_PORT_IRQ10       0x0B
#define IRQ_PORT_IRQ11       0x0C
#define IRQ_PORT_IRQ12       0x0D
//      IRQ_PORT_UNUSED      0x0E
#define IRQ_PORT_IRQ14       0x0F
#define IRQ_PORT_IRQ15       0x10
#define IRQ_DMAC0_INT        0x11
#define IRQ_DMAC1_INT        0x12
#define IRQ_DMAC2_INT        0x13
#define IRQ_DMAC3_INT        0x14
#define IRQ_DTC_COMPLETE     0x15
//      IRQ_UNUSED           0x16
#define IRQ_ICU_SNZCANCEL    0x17
#define IRQ_FCU_FRDYI        0x18
#define IRQ_LVD_LVD1         0x19
#define IRQ_LVD_LVD2         0x1A
#define IRQ_VBATT_LVD        0x1B
#define IRQ_MOSC_STOP        0x1C
#define IRQ_SYSTEM_SNZREQ    0x1D
#define IRQ_AGT0_AGTI        0x1E
#define IRQ_AGT0_AGTCMAI     0x1F
#define IRQ_AGT0_AGTCMBI     0x20
#define IRQ_AGT1_AGTI        0x21
#define IRQ_AGT1_AGTCMAI     0x22
#define IRQ_AGT1_AGTCMBI     0x23
#define IRQ_IWDT_NMIUNDF     0x24
#define IRQ_WDT_NMIUNDF      0x25
#define IRQ_RTC_ALM          0x26
#define IRQ_RTC_PRD          0x27
#define IRQ_RTC_CUP          0x28
#define IRQ_ADC140_ADI       0x29
#define IRQ_ADC140_GBADI     0x2A
#define IRQ_ADC140_CMPAI     0x2B
#define IRQ_ADC140_CMPBI     0x2C
#define IRQ_ADC140_WCMPM     0x2D
#define IRQ_ADC140_WCMPUM    0x2E
#define IRQ_ACMP_LP0         0x2F
#define IRQ_ACMP_LP1         0x30
#define IRQ_USBFS_D0FIFO     0x31
#define IRQ_USBFS_D1FIFO     0x32
#define IRQ_USBFS_USBI       0x33
#define IRQ_USBFS_USBR       0x34
#define IRQ_IIC0_RXI         0x35
#define IRQ_IIC0_TXI         0x36
#define IRQ_IIC0_TEI         0x37
#define IRQ_IIC0_EEI         0x38
#define IRQ_IIC0_WUI         0x39
#define IRQ_IIC1_RXI         0x3A
#define IRQ_IIC1_TXI         0x3B
#define IRQ_IIC1_TEI         0x3C
#define IRQ_IIC1_EEI         0x3D
#define IRQ_SSIE0_SSITXI     0x3E
#define IRQ_SSIE0_SSIRXI     0x3F
//      IRQ_UNUSED           0x40
#define IRQ_SSIE0_SSIF       0x41
#define IRQ_CTSU_CTSUWR      0x42
#define IRQ_CTSU_CTSURD      0x43
#define IRQ_CTSU_CTSUFN      0x44
#define IRQ_KEY_INTKR        0x45
#define IRQ_DOC_DOPCI        0x46
#define IRQ_CAC_FERRI        0x47
#define IRQ_CAC_MENDI        0x48
#define IRQ_CAC_OVFI         0x49
#define IRQ_CAN0_ERS         0x4A
#define IRQ_CAN0_RXF         0x4B
#define IRQ_CAN0_TXF         0x4C
#define IRQ_CAN0_RXM         0x4D
#define IRQ_CAN0_TXM         0x4E
#define IRQ_IOPORT_GROUP1    0x4F
#define IRQ_IOPORT_GROUP2    0x50
#define IRQ_IOPORT_GROUP3    0x51
#define IRQ_IOPORT_GROUP4    0x52
#define IRQ_ELC_SWEVT0       0x53
#define IRQ_ELC_SWEVT1       0x54
#define IRQ_POEG_GROUP0      0x55
#define IRQ_POEG_GROUP1      0x56
#define IRQ_GPT0_CCMPA       0x57
#define IRQ_GPT0_CCMPB       0x58
#define IRQ_GPT0_CMPC        0x59
#define IRQ_GPT0_CMPD        0x5A
#define IRQ_GPT0_CMPE        0x5B
#define IRQ_GPT0_CMPF        0x5C
#define IRQ_GPT0_OVF         0x5D
#define IRQ_GPT0_UDF         0x5E
#define IRQ_GPT1_CCMPA       0x5F
#define IRQ_GPT1_CCMPB       0x60
#define IRQ_GPT1_CMPC        0x61
#define IRQ_GPT1_CMPD        0x62
#define IRQ_GPT1_CMPE        0x63
#define IRQ_GPT1_CMPF        0x64
#define IRQ_GPT1_OVF         0x65
#define IRQ_GPT1_UDF         0x66
#define IRQ_GPT2_CCMPA       0x67
#define IRQ_GPT2_CCMPB       0x68
#define IRQ_GPT2_CMPC        0x69
#define IRQ_GPT2_CMPD        0x6A
#define IRQ_GPT2_CMPE        0x6B
#define IRQ_GPT2_CMPF        0x6C
#define IRQ_GPT2_OVF         0x6D
#define IRQ_GPT2_UDF         0x6E
#define IRQ_GPT3_CCMPA       0x6F
#define IRQ_GPT3_CCMPB       0x70
#define IRQ_GPT3_CMPC        0x71
#define IRQ_GPT3_CMPD        0x72
#define IRQ_GPT3_CMPE        0x73
#define IRQ_GPT3_CMPF        0x74
#define IRQ_GPT3_OVF         0x75
#define IRQ_GPT3_UDF         0x76
#define IRQ_GPT4_CCMPA       0x77
#define IRQ_GPT4_CCMPB       0x78
#define IRQ_GPT4_CMPC        0x79
#define IRQ_GPT4_CMPD        0x7A
#define IRQ_GPT4_CMPE        0x7B
#define IRQ_GPT4_CMPF        0x7C
#define IRQ_GPT4_OVF         0x7D
#define IRQ_GPT4_UDF         0x7E
#define IRQ_GPT5_CCMPA       0x7F
#define IRQ_GPT5_CCMPB       0x80
#define IRQ_GPT5_CMPC        0x81
#define IRQ_GPT5_CMPD        0x82
#define IRQ_GPT5_CMPE        0x83
#define IRQ_GPT5_CMPF        0x84
#define IRQ_GPT5_OVF         0x85
#define IRQ_GPT5_UDF         0x86
#define IRQ_GPT6_CCMPA       0x87
#define IRQ_GPT6_CCMPB       0x88
#define IRQ_GPT6_CMPC        0x89
#define IRQ_GPT6_CMPD        0x8A
#define IRQ_GPT6_CMPE        0x8B
#define IRQ_GPT6_CMPF        0x8C
#define IRQ_GPT6_OVF         0x8D
#define IRQ_GPT6_UDF         0x8E
#define IRQ_GPT7_CCMPA       0x8F
#define IRQ_GPT7_CCMPB       0x90
#define IRQ_GPT7_CMPC        0x91
#define IRQ_GPT7_CMPD        0x92
#define IRQ_GPT7_CMPE        0x93
#define IRQ_GPT7_CMPF        0x94
#define IRQ_GPT7_OVF         0x95
#define IRQ_GPT7_UDF         0x96
#define IRQ_GPT_UVWEDGE      0x97
#define IRQ_SCI0_RXI         0x98
#define IRQ_SCI0_TXI         0x99
#define IRQ_SCI0_TEI         0x9A
#define IRQ_SCI0_ERI         0x9B
#define IRQ_SCI0_AM          0x9C
#define IRQ_SCI0_RXI_OR_ERI  0x9D
#define IRQ_SCI1_RXI         0x9E
#define IRQ_SCI1_TXI         0x9F
#define IRQ_SCI1_TEI         0xA0
#define IRQ_SCI1_ERI         0xA1
#define IRQ_SCI1_AM          0xA2
#define IRQ_SCI2_RXI         0xA3
#define IRQ_SCI2_TXI         0xA4
#define IRQ_SCI2_TEI         0xA5
#define IRQ_SCI2_ERI         0xA6
#define IRQ_SCI2_AM          0xA7
#define IRQ_SCI9_RXI         0xA8
#define IRQ_SCI9_TXI         0xA9
#define IRQ_SCI9_TEI         0xAA
#define IRQ_SCI9_ERI         0xAB
#define IRQ_SCI9_AM          0xAC
#define IRQ_SPI0_SPRI        0xAD
#define IRQ_SPI0_SPTI        0xAE
#define IRQ_SPI0_SPII        0xAF
#define IRQ_SPI0_SPEI        0xB0
#define IRQ_SPI0_SPTEND      0xB1
#define IRQ_SPI1_SPRI        0xB2
#define IRQ_SPI1_SPTI        0xB3
#define IRQ_SPI1_SPII        0xB4
#define IRQ_SPI1_SPEI        0xB5
#define IRQ_SPI1_SPTEND      0xB6


// ==== IRQ Control Register ====
#define IRQCR 0x6000        // IRQ Control Register offset
#define ICU_IRQCR00  ((volatile unsigned char  *)(ICUBASE + IRQCR +  0 ))     // Pin IRQ Control Register 0
#define ICU_IRQCR01  ((volatile unsigned char  *)(ICUBASE + IRQCR +  1 ))     // Pin IRQ Control Register 1
#define ICU_IRQCR02  ((volatile unsigned char  *)(ICUBASE + IRQCR +  2 ))     // Pin IRQ Control Register 2
#define ICU_IRQCR03  ((volatile unsigned char  *)(ICUBASE + IRQCR +  3 ))     // Pin IRQ Control Register 3
#define ICU_IRQCR04  ((volatile unsigned char  *)(ICUBASE + IRQCR +  4 ))     // Pin IRQ Control Register 4
#define ICU_IRQCR05  ((volatile unsigned char  *)(ICUBASE + IRQCR +  5 ))     // Pin IRQ Control Register 5
#define ICU_IRQCR06  ((volatile unsigned char  *)(ICUBASE + IRQCR +  6 ))     // Pin IRQ Control Register 6
#define ICU_IRQCR07  ((volatile unsigned char  *)(ICUBASE + IRQCR +  7 ))     // Pin IRQ Control Register 7
#define ICU_IRQCR08  ((volatile unsigned char  *)(ICUBASE + IRQCR +  8 ))     // Pin IRQ Control Register 8
#define ICU_IRQCR09  ((volatile unsigned char  *)(ICUBASE + IRQCR +  9 ))     // Pin IRQ Control Register 9
#define ICU_IRQCR10  ((volatile unsigned char  *)(ICUBASE + IRQCR + 10 ))     // Pin IRQ Control Register 10
#define ICU_IRQCR11  ((volatile unsigned char  *)(ICUBASE + IRQCR + 11 ))     // Pin IRQ Control Register 11
#define ICU_IRQCR12  ((volatile unsigned char  *)(ICUBASE + IRQCR + 12 ))     // Pin IRQ Control Register 12
#define ICU_IRQCR14  ((volatile unsigned char  *)(ICUBASE + IRQCR + 14 ))     // Pin IRQ Control Register 14
#define ICU_IRQCR15  ((volatile unsigned char  *)(ICUBASE + IRQCR + 15 ))     // Pin IRQ Control Register 15

#define ICU_SELSR0  ((volatile unsigned short  *)(ICUBASE + 0x6200))         // SYS Event Link Setting Register

// ==== NVIC Interrupt Controller ====
#define NVICBASE 0xE0000000 // NVIC Interrupt Controller

// asm volatile("dsb"); // <<<< use a DSB instruction to ensure bus-synchronisition for NVIC write operations

#define NVIC_ICTR    ((volatile unsigned int  *)(NVICBASE + 0xE004))              // Interrupt Controller Type Register
// Total number of interrupt lines in groups of 32: [3:0]	INTLINESNUM
// b0000 = 0...32   << RA4M1 returns 0 - Has only 32 Interrupts - See Table 13.3 Interrupt vector table p.237
// b0001 = 33...64
// b0010 = 65...96
// b0011 = 97...128
// b0100 = 129...160
// b0101 = 161...192
// b0110 = 193...224
// b0111 = 225...256


// NVIC IRQ registers - Generic ARM Cortex-M4F; not all are necessiraly present in the RA4M1
// x = 0..7 for each register type, with 32 bits per register, to support up to 240 IRQs
// * Each bit controls one interrupt, identified by its IRQ# (0..239)
// * Register# x = IRQ# DIV 32
// * Bit n in the register = IRQ# MOD 32

#define NVICISER 0xE100     // Interrupt Set-Enable Registers
#define NVIC_ISER0  ((volatile unsigned int  *)(NVICBASE + NVICISER +(0 * 4)))     // Register 0
#define NVIC_ISER1  ((volatile unsigned int  *)(NVICBASE + NVICISER +(1 * 4)))     // Register 1
#define NVIC_ISER2  ((volatile unsigned int  *)(NVICBASE + NVICISER +(2 * 4)))     // Register 2
#define NVIC_ISER3  ((volatile unsigned int  *)(NVICBASE + NVICISER +(3 * 4)))     // Register 3
#define NVIC_ISER4  ((volatile unsigned int  *)(NVICBASE + NVICISER +(4 * 4)))     // Register 4
#define NVIC_ISER5  ((volatile unsigned int  *)(NVICBASE + NVICISER +(5 * 4)))     // Register 5
#define NVIC_ISER6  ((volatile unsigned int  *)(NVICBASE + NVICISER +(6 * 4)))     // Register 6
#define NVIC_ISER7  ((volatile unsigned int  *)(NVICBASE + NVICISER +(7 * 4)))     // Register 7
#define NVICICER 0xE180     // Interrupt Clear-Enable Registers
#define NVIC_ICER0  ((volatile unsigned int  *)(NVICBASE + NVICICER +(0 * 4)))     // Register 0
#define NVIC_ICER1  ((volatile unsigned int  *)(NVICBASE + NVICICER +(1 * 4)))     // Register 1
#define NVIC_ICER2  ((volatile unsigned int  *)(NVICBASE + NVICICER +(2 * 4)))     // Register 2
#define NVIC_ICER3  ((volatile unsigned int  *)(NVICBASE + NVICICER +(3 * 4)))     // Register 3
#define NVIC_ICER4  ((volatile unsigned int  *)(NVICBASE + NVICICER +(4 * 4)))     // Register 4
#define NVIC_ICER5  ((volatile unsigned int  *)(NVICBASE + NVICICER +(5 * 4)))     // Register 5
#define NVIC_ICER6  ((volatile unsigned int  *)(NVICBASE + NVICICER +(6 * 4)))     // Register 6
#define NVIC_ICER7  ((volatile unsigned int  *)(NVICBASE + NVICICER +(7 * 4)))     // Register 7

#define NVICISPR 0xE200     // Interrupt Set-Pending Registers
#define NVIC_ISPR0  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(0 * 4)))     // Register 0
#define NVIC_ISPR1  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(1 * 4)))     // Register 1
#define NVIC_ISPR2  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(2 * 4)))     // Register 2
#define NVIC_ISPR3  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(3 * 4)))     // Register 3
#define NVIC_ISPR4  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(4 * 4)))     // Register 4
#define NVIC_ISPR5  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(5 * 4)))     // Register 5
#define NVIC_ISPR6  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(6 * 4)))     // Register 6
#define NVIC_ISPR7  ((volatile unsigned int  *)(NVICBASE + NVICISPR +(7 * 4)))     // Register 7
#define NVICICPR 0xE280     // Interrupt Clear-Pending Registers
#define NVIC_ICPR0  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(0 * 4)))     // Register 0
#define NVIC_ICPR1  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(1 * 4)))     // Register 1
#define NVIC_ICPR2  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(2 * 4)))     // Register 2
#define NVIC_ICPR3  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(3 * 4)))     // Register 3
#define NVIC_ICPR4  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(4 * 4)))     // Register 4
#define NVIC_ICPR5  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(5 * 4)))     // Register 5
#define NVIC_ICPR6  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(6 * 4)))     // Register 6
#define NVIC_ICPR7  ((volatile unsigned int  *)(NVICBASE + NVICICPR +(7 * 4)))     // Register 7

#define NVICIABR 0xE300     // Interrupt Active Bit Register - Read Only: Read 1 if interrupt in active state
#define NVIC_IABR0  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(0 * 4)))     // Register 0
#define NVIC_IABR1  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(1 * 4)))     // Register 1
#define NVIC_IABR2  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(2 * 4)))     // Register 2
#define NVIC_IABR3  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(3 * 4)))     // Register 3
#define NVIC_IABR4  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(4 * 4)))     // Register 4
#define NVIC_IABR5  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(5 * 4)))     // Register 5
#define NVIC_IABR6  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(6 * 4)))     // Register 6
#define NVIC_IABR7  ((volatile unsigned int  *)(NVICBASE + NVICIABR +(7 * 4)))     // Register 7

// NVIC_IPRx (x=0..59) – Interrupt Priority Registers
// * Supports up to 240 interrupts: 0..239 (32 in RA4M1)
// *  8-bit priority field for each interrupts (4-bit field for RA4M1)
// *  4 priority values per register (MSB aligned bits of each byte for prioity i.e. 0x10 thru 0xF0)
// *  0 = highest priority, F = lowest priority (USBs are all at 0xC0, mS Timer on AGT is 0xE0)
// *  Register# x = IRQ# DIV 4
// *  Byte offset within the register = IRQ# MOD 4
// *  Ex. IRQ85:
// *  85/4 = 21 with remainder 1 (register 21, byte offset 1): Write priority<<8 to NVIC_IPR2
// *  85/32 = 2 with remainder 21: write 1<<21 to NVIC_SER2


#define NVICIPR  0xE400     // Interrupt Priority Register
#define NVIC_IPR00  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(0 * 4)))     // Register 0: NVIC_IPR0 TO 3
#define NVIC_IPR04  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(1 * 4)))     // Register 1
#define NVIC_IPR08  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(2 * 4)))     // Register 2
#define NVIC_IPR12  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(3 * 4)))     // Register 3
#define NVIC_IPR16  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(4 * 4)))     // Register 4
#define NVIC_IPR20  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(5 * 4)))     // Register 5
#define NVIC_IPR24  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(6 * 4)))     // Register 6
#define NVIC_IPR28  ((volatile unsigned int  *)(NVICBASE + NVICIPR +(7 * 4)))     // Register 7

// Also byte accessible:
#define NVIC_IPR00_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  0 ))     // For direct byte access
#define NVIC_IPR01_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  1 ))     // For direct byte access
#define NVIC_IPR02_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  2 ))     // For direct byte access
#define NVIC_IPR03_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  3 ))     // For direct byte access
#define NVIC_IPR04_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  4 ))     // For direct byte access
#define NVIC_IPR05_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  5 ))     // For direct byte access
#define NVIC_IPR06_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  6 ))     // For direct byte access
#define NVIC_IPR07_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  7 ))     // For direct byte access
#define NVIC_IPR08_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  8 ))     // For direct byte access
#define NVIC_IPR09_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR +  9 ))     // For direct byte access
#define NVIC_IPR10_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 10 ))     // For direct byte access
#define NVIC_IPR11_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 11 ))     // For direct byte access
#define NVIC_IPR12_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 12 ))     // For direct byte access
#define NVIC_IPR13_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 13 ))     // For direct byte access
#define NVIC_IPR14_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 14 ))     // For direct byte access
#define NVIC_IPR15_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 15 ))     // For direct byte access
#define NVIC_IPR16_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 16 ))     // For direct byte access
#define NVIC_IPR17_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 17 ))     // For direct byte access
#define NVIC_IPR18_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 18 ))     // For direct byte access
#define NVIC_IPR19_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 19 ))     // For direct byte access
#define NVIC_IPR20_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 20 ))     // For direct byte access
#define NVIC_IPR21_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 21 ))     // For direct byte access
#define NVIC_IPR22_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 22 ))     // For direct byte access
#define NVIC_IPR23_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 23 ))     // For direct byte access
#define NVIC_IPR24_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 24 ))     // For direct byte access
#define NVIC_IPR25_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 25 ))     // For direct byte access
#define NVIC_IPR26_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 26 ))     // For direct byte access
#define NVIC_IPR27_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 27 ))     // For direct byte access
#define NVIC_IPR28_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 28 ))     // For direct byte access
#define NVIC_IPR29_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 29 ))     // For direct byte access
#define NVIC_IPR30_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 30 ))     // For direct byte access
#define NVIC_IPR31_BY  ((volatile unsigned char  *)(NVICBASE + NVICIPR + 31 ))     // For direct byte access



// ==== Event Link Controller ====
#define ELCBASE 0x40040000 // Event Link Controller
#define ELC_ELCR     ((volatile unsigned char  *)(ELCBASE + 0x1000))              // Event Link Controller Register
#define ELC_ELSEGR0  ((volatile unsigned char  *)(ELCBASE + 0x1002))              // Event Link Software Event Generation Register 0
#define ELC_ELSEGR1  ((volatile unsigned char  *)(ELCBASE + 0x1004))              // Event Link Software Event Generation Register 1
#define ELSR 0x1010        // Event Link Setting Registers
#define ELC_ELSR00   ((volatile unsigned short *)(ELCBASE + ELSR +( 0 * 4)))      // ELC_GPTA
#define ELC_ELSR01   ((volatile unsigned short *)(ELCBASE + ELSR +( 1 * 4)))      // ELC_GPTB
#define ELC_ELSR02   ((volatile unsigned short *)(ELCBASE + ELSR +( 2 * 4)))      // ELC_GPTC
#define ELC_ELSR03   ((volatile unsigned short *)(ELCBASE + ELSR +( 3 * 4)))      // ELC_GPTD
#define ELC_ELSR04   ((volatile unsigned short *)(ELCBASE + ELSR +( 4 * 4)))      // ELC_GPTE
#define ELC_ELSR05   ((volatile unsigned short *)(ELCBASE + ELSR +( 5 * 4)))      // ELC_GPTF
#define ELC_ELSR06   ((volatile unsigned short *)(ELCBASE + ELSR +( 6 * 4)))      // ELC_GPTG
#define ELC_ELSR07   ((volatile unsigned short *)(ELCBASE + ELSR +( 7 * 4)))      // ELC_GPTH
#define ELC_ELSR08   ((volatile unsigned short *)(ELCBASE + ELSR +( 8 * 4)))      // ELC_AD00 - ADC14A
#define ELC_ELSR09   ((volatile unsigned short *)(ELCBASE + ELSR +( 9 * 4)))      // ELC_AD01 - ADC14B
#define ELC_ELSR12   ((volatile unsigned short *)(ELCBASE + ELSR +(12 * 4)))      // ELC_DA0 - DAC12
#define ELC_ELSR14   ((volatile unsigned short *)(ELCBASE + ELSR +(14 * 4)))      // ELC_PORT1
#define ELC_ELSR15   ((volatile unsigned short *)(ELCBASE + ELSR +(15 * 4)))      // ELC_PORT2
#define ELC_ELSR16   ((volatile unsigned short *)(ELCBASE + ELSR +(16 * 4)))      // ELC_PORT3
#define ELC_ELSR17   ((volatile unsigned short *)(ELCBASE + ELSR +(17 * 4)))      // ELC_PORT4
#define ELC_ELSR18   ((volatile unsigned short *)(ELCBASE + ELSR +(18 * 4)))      // ELC_CTSU


// ==== Low Power Mode Control ====
#define SYSTEM 0x40010000 // System Registers
#define SYSTEM_SBYCR   ((volatile unsigned short *)(SYSTEM + 0xE00C))      // Standby Control Register
#define SYSTEM_MSTPCRA ((volatile unsigned int   *)(SYSTEM + 0xE01C))      // Module Stop Control Register A

#define MSTP 0x40040000 // Module Registers
#define MSTP_MSTPCRB   ((volatile unsigned int   *)(MSTP + 0x7000))      // Module Stop Control Register B
#define MSTPB2   2 // CAN0
#define MSTPB8   8 // IIC1
#define MSTPB9   9 // IIC0
#define MSTPB11 11 // USBFS
#define MSTPB18 18 // SPI1
#define MSTPB19 19 // SPI0
#define MSTPB22 22 // SCI9
#define MSTPB29 29 // SCI2
#define MSTPB30 30 // SCI1
#define MSTPB31 31 // SCI0

#define MSTP_MSTPCRC   ((volatile unsigned int   *)(MSTP + 0x7004))      // Module Stop Control Register C
#define MSTPC0   0 // CAC   - Clock Frequency Accuracy Measurement Circuit Module
#define MSTPC1   1 // CRC   - Cyclic Redundancy Check Calculator Module
#define MSTPC3   3 // CTSU  - Capacitive Touch Sensing Unit Module
#define MSTPC4   4 // SLCDC - Segment LCD Controller Module
#define MSTPC8   8 // SSIE0 - Serial Sound Interface Enhanced Module
#define MSTPC13 13 // DOC   - Data Operation Circuit Module
#define MSTPC14 14 // ELC   - Event Link Controller Module
#define MSTPC31 31 // SCE5  - SCE5 Module

#define MSTP_MSTPCRD   ((volatile unsigned int   *)(MSTP + 0x7008))      // Module Stop Control Register D
#define MSTPD2   2 // AGT1   - Asynchronous General Purpose Timer 1 Module
#define MSTPD3   3 // AGT0   - Asynchronous General Purpose Timer 0 Module
#define MSTPD5   5 // GPT32x - GPT320 and GPT321 General 32 bit PWM Timer Module
#define MSTPD6   6 // GPT12x - GPT162 thr GPT167 General 16 bit PWM Timer Module
#define MSTPD14 14 // POEG   - Port Output Enable for GPT Module Stop
#define MSTPD16 16 // ADC140 - 14-Bit A/D Converter Module
#define MSTPD19 19 // DAC8   -  8-Bit D/A Converter Module
#define MSTPD20 20 // DAC12  - 12-Bit D/A Converter Module
#define MSTPD29 29 // ACMPLP - Low-Power Analog Comparator Module
#define MSTPD31 31 // OPAMP  - Operational Amplifier Module

// These bits are read as 1, the write value should be 1.
// Bit value 0: Cancel the module-stop state
// Bit value 1: Enter the module-stop state.


// ==== USB 2.0 Full-Speed Module ====
#define USBFSBASE  0x40090000

#define USBFS_SYSCFG     ((volatile unsigned short *)(USBFSBASE + 0x0000))
#define USBFS_SYSSTS0    ((volatile unsigned short *)(USBFSBASE + 0x0004))
#define USBFS_DVSTCTR0   ((volatile unsigned short *)(USBFSBASE + 0x0008))
#define USBFS_CFIFO      ((volatile unsigned short *)(USBFSBASE + 0x0014))
#define USBFS_D0FIFO     ((volatile unsigned short *)(USBFSBASE + 0x0018))
#define USBFS_D1FIFO     ((volatile unsigned short *)(USBFSBASE + 0x001C))
#define USBFS_CFIFOL     ((volatile unsigned char  *)(USBFSBASE + 0x0014))
#define USBFS_D0FIFOL    ((volatile unsigned char  *)(USBFSBASE + 0x0018))
#define USBFS_D1FIFOL    ((volatile unsigned char  *)(USBFSBASE + 0x001C))
#define USBFS_CFIFOSEL   ((volatile unsigned short *)(USBFSBASE + 0x0020))
#define USBFS_D0FIFOSEL  ((volatile unsigned short *)(USBFSBASE + 0x0028))
#define USBFS_D1FIFOSEL  ((volatile unsigned short *)(USBFSBASE + 0x002C))
#define USBFS_CFIFOCTR   ((volatile unsigned short *)(USBFSBASE + 0x0022))
#define USBFS_D0FIFOCTR  ((volatile unsigned short *)(USBFSBASE + 0x002A))
#define USBFS_D1FIFOCTR  ((volatile unsigned short *)(USBFSBASE + 0x002E))
#define USBFS_INTENB0    ((volatile unsigned short *)(USBFSBASE + 0x0030))
#define USBFS_INTENB1    ((volatile unsigned short *)(USBFSBASE + 0x0032))
#define USBFS_BRDYENB    ((volatile unsigned short *)(USBFSBASE + 0x0036))
#define USBFS_NRDYENB    ((volatile unsigned short *)(USBFSBASE + 0x0038))
#define USBFS_BEMPENB    ((volatile unsigned short *)(USBFSBASE + 0x003A))
#define USBFS_SOFCFG     ((volatile unsigned short *)(USBFSBASE + 0x003C))
#define USBFS_INTSTS0    ((volatile unsigned short *)(USBFSBASE + 0x0040))
#define USBFS_INTSTS1    ((volatile unsigned short *)(USBFSBASE + 0x0042))
#define USBFS_BRDYSTS    ((volatile unsigned short *)(USBFSBASE + 0x0046))
#define USBFS_NRDYSTS    ((volatile unsigned short *)(USBFSBASE + 0x0048))
#define USBFS_BEMPSTS    ((volatile unsigned short *)(USBFSBASE + 0x004A))
#define USBFS_FRMNUM     ((volatile unsigned short *)(USBFSBASE + 0x004C))
#define USBFS_USBREQ     ((volatile unsigned short *)(USBFSBASE + 0x0054))
#define USBFS_USBVAL     ((volatile unsigned short *)(USBFSBASE + 0x0056))
#define USBFS_USBINDX    ((volatile unsigned short *)(USBFSBASE + 0x0058))
#define USBFS_USBLENG    ((volatile unsigned short *)(USBFSBASE + 0x005A))
#define USBFS_DCPCFG     ((volatile unsigned short *)(USBFSBASE + 0x005C))
#define USBFS_DCPMAXP    ((volatile unsigned short *)(USBFSBASE + 0x005E))
#define USBFS_DCPCTR     ((volatile unsigned short *)(USBFSBASE + 0x0060))
#define USBFS_PIPESEL    ((volatile unsigned short *)(USBFSBASE + 0x0064))
#define USBFS_PIPECFG    ((volatile unsigned short *)(USBFSBASE + 0x0068))
#define USBFS_PIPEMAXP   ((volatile unsigned short *)(USBFSBASE + 0x006C))
#define USBFS_PIPEPERI   ((volatile unsigned short *)(USBFSBASE + 0x006E))
#define USBFS_PIPE1CTR   ((volatile unsigned short *)(USBFSBASE + 0x0070))
#define USBFS_PIPE2CTR   ((volatile unsigned short *)(USBFSBASE + 0x0072))
#define USBFS_PIPE3CTR   ((volatile unsigned short *)(USBFSBASE + 0x0074))
#define USBFS_PIPE4CTR   ((volatile unsigned short *)(USBFSBASE + 0x0076))
#define USBFS_PIPE5CTR   ((volatile unsigned short *)(USBFSBASE + 0x0078))
#define USBFS_PIPE6CTR   ((volatile unsigned short *)(USBFSBASE + 0x007A))
#define USBFS_PIPE7CTR   ((volatile unsigned short *)(USBFSBASE + 0x007C))
#define USBFS_PIPE8CTR   ((volatile unsigned short *)(USBFSBASE + 0x007E))
#define USBFS_PIPE9CTR   ((volatile unsigned short *)(USBFSBASE + 0x0080))
#define USBFS_PIPE1TRE   ((volatile unsigned short *)(USBFSBASE + 0x0090))
#define USBFS_PIPE2TRE   ((volatile unsigned short *)(USBFSBASE + 0x0094))
#define USBFS_PIPE3TRE   ((volatile unsigned short *)(USBFSBASE + 0x0098))
#define USBFS_PIPE4TRE   ((volatile unsigned short *)(USBFSBASE + 0x009C))
#define USBFS_PIPE5TRE   ((volatile unsigned short *)(USBFSBASE + 0x00A0))
#define USBFS_PIPE1TRN   ((volatile unsigned short *)(USBFSBASE + 0x0092))
#define USBFS_PIPE2TRN   ((volatile unsigned short *)(USBFSBASE + 0x0096))
#define USBFS_PIPE3TRN   ((volatile unsigned short *)(USBFSBASE + 0x009A))
#define USBFS_PIPE4TRN   ((volatile unsigned short *)(USBFSBASE + 0x009E))
#define USBFS_PIPE5TRN   ((volatile unsigned short *)(USBFSBASE + 0x00A2))
#define USBFS_DEVADD0    ((volatile unsigned short *)(USBFSBASE + 0x00D0))
#define USBFS_DEVADD1    ((volatile unsigned short *)(USBFSBASE + 0x00D2))
#define USBFS_DEVADD2    ((volatile unsigned short *)(USBFSBASE + 0x00D4))
#define USBFS_DEVADD3    ((volatile unsigned short *)(USBFSBASE + 0x00D6))
#define USBFS_DEVADD4    ((volatile unsigned short *)(USBFSBASE + 0x00D8))
#define USBFS_DEVADD5    ((volatile unsigned short *)(USBFSBASE + 0x00DA))
#define USBFS_USBMC      ((volatile unsigned short *)(USBFSBASE + 0x00CC))
#define USBFS_USBBCCTRL0 ((volatile unsigned short *)(USBFSBASE + 0x00B0))


// ==== 14-Bit A/D Converter ====
#define ADCBASE 0x40050000 /* ADC Base */
                                                                           // N/C = Pin not connected; N/A = No pin for LQFP64 package
// 16 bit registers
#define ADC140_ADDR00   ((volatile unsigned short *)(ADCBASE + 0xC020))      // A1 (P000 AN00 AMP+)
#define ADC140_ADDR01   ((volatile unsigned short *)(ADCBASE + 0xC020 +  2)) // A2 (P001 AN01 AMP-)
#define ADC140_ADDR02   ((volatile unsigned short *)(ADCBASE + 0xC020 +  4)) // A3 (P002 AN02 AMPO)
#define ADC140_ADDR03   ((volatile unsigned short *)(ADCBASE + 0xC020 +  6)) // N/C
#define ADC140_ADDR04   ((volatile unsigned short *)(ADCBASE + 0xC020 +  8)) // N/C
#define ADC140_ADDR05   ((volatile unsigned short *)(ADCBASE + 0xC020 + 10)) // Aref (P010 AN05 VrefH0)
#define ADC140_ADDR06   ((volatile unsigned short *)(ADCBASE + 0xC020 + 12)) // N/C
#define ADC140_ADDR07   ((volatile unsigned short *)(ADCBASE + 0xC020 + 14)) // TxLED (P012 AN07)
#define ADC140_ADDR08   ((volatile unsigned short *)(ADCBASE + 0xC020 + 16)) // RxLED (P013 AN08)
#define ADC140_ADDR09   ((volatile unsigned short *)(ADCBASE + 0xC020 + 18)) // A0 (P014 AN09 DAC)
#define ADC140_ADDR10   ((volatile unsigned short *)(ADCBASE + 0xC020 + 20)) // N/C
#define ADC140_ADDR11   ((volatile unsigned short *)(ADCBASE + 0xC020 + 22)) // N/A
#define ADC140_ADDR12   ((volatile unsigned short *)(ADCBASE + 0xC020 + 24)) // N/A
#define ADC140_ADDR13   ((volatile unsigned short *)(ADCBASE + 0xC020 + 26)) // N/A
#define ADC140_ADDR14   ((volatile unsigned short *)(ADCBASE + 0xC020 + 28)) // N/A
#define ADC140_ADDR16   ((volatile unsigned short *)(ADCBASE + 0xC040))      // R10/R26 net (P500 AN16)
#define ADC140_ADDR17   ((volatile unsigned short *)(ADCBASE + 0xC040 +  2)) // SWD-pin8 SCI1_TxD (P501 AN17)
#define ADC140_ADDR18   ((volatile unsigned short *)(ADCBASE + 0xC040 +  4)) // SWD-pin7 SCI1_RxD (P502 AN18)
#define ADC140_ADDR19   ((volatile unsigned short *)(ADCBASE + 0xC040 +  6)) // D4 (P103 AN19)
#define ADC140_ADDR20   ((volatile unsigned short *)(ADCBASE + 0xC040 +  8)) // D5 (P102 AN20 ADtrg0)
#define ADC140_ADDR21   ((volatile unsigned short *)(ADCBASE + 0xC040 + 10)) // A4 (P101 AN21 SDA)
#define ADC140_ADDR22   ((volatile unsigned short *)(ADCBASE + 0xC040 + 12)) // A5 (P100 AN20 SCL)
#define ADC140_ADDR23   ((volatile unsigned short *)(ADCBASE + 0xC040 + 14)) // N/A
#define ADC140_ADDR24   ((volatile unsigned short *)(ADCBASE + 0xC040 + 16)) // N/A
#define ADC140_ADDR25   ((volatile unsigned short *)(ADCBASE + 0xC040 + 18)) // N/A
#define ADC140_ADDBLDR  ((volatile unsigned short *)(ADCBASE + 0xC018))    // A/D Data Duplexing Register
#define ADC140_ADTSDR   ((volatile unsigned short *)(ADCBASE + 0xC01A))    // A/D conversion result of temperature sensor output
#define ADC140_ADOCDR   ((volatile unsigned short *)(ADCBASE + 0xC01C))    // A/D result of internal reference voltage
#define ADC140_ADDBLDRA ((volatile unsigned short *)(ADCBASE + 0xC084))    // A/D Data Duplexing Register A
#define ADC140_ADDBLDRB ((volatile unsigned short *)(ADCBASE + 0xC086))    // A/D Data Duplexing Register B
// A/D Control Register
#define ADC140_ADCSR      ((volatile unsigned short *)(ADCBASE + 0xC000))
#define ADCSR_DBLANS_4_0   0 // Double Trigger Channel Select
#define ADCSR_GBADIE       6 // Group B Scan End Interrupt Enable
#define ADCSR_DBLE         7 // Double Trigger Mode Select
#define ADCSR_EXTRG        8 // Trigger Select
#define ADCSR_TRGE         9 // Trigger Start Enable
#define ADCSR_ADHSC       10 // A/D Conversion Mode Select - 0: High-speed; 1: Low-power
#define ADCSR_ADCS_1_0    13 // Scan Mode Select
#define ADCSR_ADST        15 // A/D Conversion Start
// A/D Channel Select Register A0
#define ADC140_ADANSA0    ((volatile unsigned short *)(ADCBASE + 0xC004))
#define ADANSA0_ANSA00     0 // A/D Conversion Channel Select - 1: Associated input channel selected
#define ADANSA0_ANSA01     1
#define ADANSA0_ANSA02     2
#define ADANSA0_ANSA03     3
#define ADANSA0_ANSA04     4
#define ADANSA0_ANSA05     5
#define ADANSA0_ANSA06     6
#define ADANSA0_ANSA07     7
#define ADANSA0_ANSA08     8
#define ADANSA0_ANSA09     9
#define ADANSA0_ANSA10    10
#define ADANSA0_ANSA11    11
#define ADANSA0_ANSA12    12
#define ADANSA0_ANSA13    13
#define ADANSA0_ANSA14    14
// A/D Channel Select Register A1
#define ADC140_ADANSA1    ((volatile unsigned short *)(ADCBASE + 0xC006))
#define ADANSA1_ANSA16     0 // A/D Conversion Channel Select - 1: Associated input channel selected
#define ADANSA1_ANSA17     1
#define ADANSA1_ANSA18     2
#define ADANSA1_ANSA19     3
#define ADANSA1_ANSA20     4
#define ADANSA1_ANSA21     5
#define ADANSA1_ANSA22     6
#define ADANSA1_ANSA23     7
#define ADANSA1_ANSA24     8
#define ADANSA1_ANSA25     9
// A/D-Converted Value Addition/Average Channel Select Register 0
#define ADC140_ADADS0     ((volatile unsigned short *)(ADCBASE + 0xC008))
#define ADADS0_ADS00       0 // A/D Conversion Channel Select - 1: Associated input channel selected
#define ADADS0_ADS01       1
#define ADADS0_ADS02       2
#define ADADS0_ADS03       3
#define ADADS0_ADS04       4
#define ADADS0_ADS05       5
#define ADADS0_ADS06       6
#define ADADS0_ADS07       7
#define ADADS0_ADS08       8
#define ADADS0_ADS09       9
#define ADADS0_ADS10      10
#define ADADS0_ADS11      11
#define ADADS0_ADS12      12
#define ADADS0_ADS13      13
#define ADADS0_ADS14      14
// A/D-Converted Value Addition/Average Channel Select Register 1
#define ADC140_ADADS1     ((volatile unsigned short *)(ADCBASE + 0xC00A))
#define ADADS1_ADS16       0 // A/D Conversion Channel Select - 1: Associated input channel selected
#define ADADS1_ADS17       1
#define ADADS1_ADS18       2
#define ADADS1_ADS19       3
#define ADADS1_ADS20       4
#define ADADS1_ADS21       5
#define ADADS1_ADS22       6
#define ADADS1_ADS23       7
#define ADADS1_ADS24       8
#define ADADS1_ADS25       9
// A/D Control Extended Register
#define ADC140_ADCER      ((volatile unsigned short *)(ADCBASE + 0xC00E))
#define ADCER_ADPCR_1_0    1 // A/D Conversion Accuracy Specify - 0b00: 12-bit accuracy; 0b11: 14-bit accuracy
#define ADCER_ACE          5 // A/D Data Register Automatic Clearing Enable
#define ADCER_DIAGVAL_1_0  8 // Self-Diagnosis Conversion Voltage Select
#define ADCER_DIAGLD      10 // Self-Diagnosis Mode Select
#define ADCER_DIAGM       11 // Self-Diagnosis Enable
#define ADCER_ADRFMT      15 // A/D Data Register Format Select - 0: Flush-right; 1: Flush-left
// A/D Conversion Start Trigger Select Register
#define ADC140_ADSTRGR    ((volatile unsigned short *)(ADCBASE + 0xC010))
#define ADSTRGR_TRSB_5_0   0 // A/D Conversion Start Trigger Select for Group B
#define ADSTRGR_TRSA_5_0   8 // A/D Conversion Start Trigger Select for Group A OR single scan mode
// A/D Conversion Extended Input Control Register
#define ADC140_ADEXICR    ((volatile unsigned short *)(ADCBASE + 0xC012))
#define ADEXICR_TSSAD      0 // Temperature Sensor Output A/DConverted Value Addition/Average Mode Select
#define ADEXICR_OCSAD      1 // Internal Reference Voltage A/DConverted Value Addition/Average Mode Select
#define ADEXICR_TSSA       8 // Temperature Sensor Output A/D Conversion Select
#define ADEXICR_OCSA       9 // Internal Reference Voltage A/D Conversion Select
// A/D Channel Select Register B0
#define ADC140_ADANSB0    ((volatile unsigned short *)(ADCBASE + 0xC014))
#define ADANSB0_ANSB00     0 // A/D Conversion Channel Select - 1: Associated input channel selected
#define ADANSB0_ANSB01     1
#define ADANSB0_ANSB02     2
#define ADANSB0_ANSB03     3
#define ADANSB0_ANSB04     4
#define ADANSB0_ANSA05     5
#define ADANSB0_ANSA06     6
#define ADANSB0_ANSA07     7
#define ADANSB0_ANSA08     8
#define ADANSB0_ANSA09     9
#define ADANSB0_ANSA10    10
#define ADANSB0_ANSA11    11
#define ADANSB0_ANSA12    12
#define ADANSB0_ANSA13    13
#define ADANSB0_ANSA14    14
// A/D Channel Select Register B1
#define ADC140_ADANSB1    ((volatile unsigned short *)(ADCBASE + 0xC016))
#define ADANSB1_ANSA16     0 // A/D Conversion Channel Select - 1: Associated input channel selected
#define ADANSB1_ANSA17     1
#define ADANSB1_ANSA18     2
#define ADANSB1_ANSA19     3
#define ADANSB1_ANSA20     4
#define ADANSB1_ANSA21     5
#define ADANSB1_ANSA22     6
#define ADANSB1_ANSA23     7
#define ADANSB1_ANSA24     8
#define ADANSB1_ANSA25     9
// A/D Self-Diagnosis Data Register
#define ADC140_ADRD       ((volatile unsigned short *)(ADCBASE + 0xC01E))
// A/D Group Scan Priority Control Register
#define ADC140_ADGSPCR    ((volatile unsigned short *)(ADCBASE + 0xC080))
#define ADGSPCR_PGS        0 // Group A Priority Control Setting
#define ADGSPCR_GBRSCN     1 // Group B Restart Setting
#define ADGSPCR_GBRP      15 // Group B Single Scan Continuous Start
// A/D Compare Function Control Register
#define ADC140_ADCMPCR    ((volatile unsigned short *)(ADCBASE + 0xC090))
#define ADCMPCR_CMPAB_1_0  0 // Window A/B Composite Conditions Setting
#define ADCMPCR_CMPBE      9 // Compare Window B Operation Enable
#define ADCMPCR_CMPAE     11 // Compare Window A Operation Enable
#define ADCMPCR_CMPBIE    13 // Compare B Interrupt Enable
#define ADCMPCR_WCMPE     14 // Window Function Setting
#define ADCMPCR_CMPAIE    15 // Compare A Interrupt Enable
// A/D Compare Function Window Registers
#define ADC140_ADCMPANSR0 ((volatile unsigned short *)(ADCBASE + 0xC094)) // A/D Compare Function Window A Channel Select Register 0
#define ADC140_ADCMPANSR1 ((volatile unsigned short *)(ADCBASE + 0xC096)) // A/D Compare Function Window A Channel Select Register 1
#define ADC140_ADCMPLR0   ((volatile unsigned short *)(ADCBASE + 0xC098)) // A/D Compare Function Window A Comparison Condition Setting Register 0
#define ADC140_ADCMPLR1   ((volatile unsigned short *)(ADCBASE + 0xC09A)) // A/D Compare Function Window A Comparison Condition Setting Register 1
#define ADC140_ADCMPDR0   ((volatile unsigned short *)(ADCBASE + 0xC09C)) // A/D Compare Function Window A Lower-Side Level Setting Register
#define ADC140_ADCMPDR1   ((volatile unsigned short *)(ADCBASE + 0xC09E)) // A/D Compare Function Window A Upper-Side Level Setting Register
#define ADC140_ADWINLLB   ((volatile unsigned short *)(ADCBASE + 0xC0A8)) // A/D Compare Function Window B Lower-Side Level Setting Register
#define ADC140_ADWINULB   ((volatile unsigned short *)(ADCBASE + 0xC0AA)) // A/D Compare Function Window B Upper-Side Level Setting Register
#define ADC140_ADCMPSR0   ((volatile unsigned short *)(ADCBASE + 0xC0A0)) // A/D Compare Function Window A Channel Status Register 0
#define ADC140_ADCMPSR1   ((volatile unsigned short *)(ADCBASE + 0xC0A2)) // A/D Compare Function Window A Channel Status Register 1

// 8 bit registers
 // A/D-Converted Value Addition/Average Count Select Register
#define ADC140_ADADC      ((volatile unsigned char  *)(ADCBASE + 0xC00C))
#define ADADC_ADC_2_0      0 // Count Select - 0b000: 1-time conversion; 0b011: 4-time conversion (three additions)
#define ADC_AVG_1      0b000 // Single Conversion, no averaging
#define ADC_AVG_2      0b001 // 2 conversions, with averaging
#define ADC_AVG_3      0b010 // 3 conversions, with averaging
#define ADC_AVG_4      0b011 // 4 conversions, with averaging
#define ADC_AVG_16     0b101 // 16 conversions, with averaging - ONLY for 12-bit ADC mode
#define ADADC_AVEE         7 //
// A/D Disconnection Detection Control Register
#define ADC140_ADDISCR    ((volatile unsigned char  *)(ADCBASE + 0xC07A)) // A/D Disconnection Detection Control Register
#define ADDISCR_ADNDIS_3_0    0 // Precharge/Discharge period
#define ADDISCR_PCHG          4 // Precharge/Discharge select
// A/D High-Potential/Low-Potential Reference Voltage Control Register
#define ADC140_ADHVREFCNT ((volatile unsigned char  *)(ADCBASE + 0xC08A))
#define ADHVREFCNT_HVSEL_1_0  0 // High-Potential Reference Voltage Select:
#define ADC_HVSEL_AVCC0    0b00 // AVCC0
#define ADC_HVSEL_VREFH0   0b01 // VREFH0
#define ADC_HVSEL_INTREF   0b10 // Internal ref
#define ADC_HVSEL_INTND    0b11 // Internal node discharge
#define ADHVREFCNT_LVSEL      4 // Low-Potential Reference Voltage Select - 0: AVSS0; 1: VREFL0
#define ADHVREFCNT_ADSLP      7 // Sleep
// A/D Compare Function Window Registers
#define ADC140_ADWINMON   ((volatile unsigned char  *)(ADCBASE + 0xC08C)) // A/D Compare Function Window A/B Status Monitor Register
#define ADC140_ADCMPANSER ((volatile unsigned char  *)(ADCBASE + 0xC092)) // A/D Compare Function Window A Extended Input Select Register
#define ADC140_ADCMPLER   ((volatile unsigned char  *)(ADCBASE + 0xC093)) // A/D Compare Function Window A Extended Input Comparison Condition Setting Register
#define ADC140_ADCMPSER   ((volatile unsigned char  *)(ADCBASE + 0xC0A4)) // A/D Compare Function Window A Extended Input Channel Status Register
#define ADC140_ADCMPBNSER ((volatile unsigned char  *)(ADCBASE + 0xC0A6)) // A/D Compare Function Window B Channel Select Register
#define ADC140_ADCMPBSR   ((volatile unsigned char  *)(ADCBASE + 0xC0AC)) // A/D Compare Function Window B Status Register

// 35.2.14 A/D Sampling State Register n (ADSSTRn) (n = 00 to 14, L, T, O)
#define ADC140_ADSSTR00 ((volatile unsigned char *)(ADCBASE + 0xC0E0))      // AN00
#define ADC140_ADSSTR01 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  1)) // AN01
#define ADC140_ADSSTR02 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  2)) // AN02
#define ADC140_ADSSTR03 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  3)) // AN03
#define ADC140_ADSSTR04 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  4)) // AN04
#define ADC140_ADSSTR05 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  5)) // AN05
#define ADC140_ADSSTR06 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  6)) // AN06
#define ADC140_ADSSTR07 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  7)) // AN07
#define ADC140_ADSSTR08 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  8)) // AN08
#define ADC140_ADSSTR09 ((volatile unsigned char *)(ADCBASE + 0xC0E0 +  9)) // AN09
#define ADC140_ADSSTR10 ((volatile unsigned char *)(ADCBASE + 0xC0E0 + 10)) // AN10
#define ADC140_ADSSTR11 ((volatile unsigned char *)(ADCBASE + 0xC0E0 + 11)) // AN11
#define ADC140_ADSSTR12 ((volatile unsigned char *)(ADCBASE + 0xC0E0 + 12)) // AN12
#define ADC140_ADSSTR13 ((volatile unsigned char *)(ADCBASE + 0xC0E0 + 13)) // AN13
#define ADC140_ADSSTR14 ((volatile unsigned char *)(ADCBASE + 0xC0E0 + 14)) // AN14
#define ADC140_ADSSTRL  ((volatile unsigned char *)(ADCBASE + 0xC0DD))      // AN16 to AN25
#define ADC140_ADSSTRT  ((volatile unsigned char *)(ADCBASE + 0xC0DE))      // Temp
#define ADC140_ADSSTRO  ((volatile unsigned char *)(ADCBASE + 0xC0DF))      // Iref


// ==== 12-Bit D/A Converter ====
#define DACBASE 0x40050000          // DAC Base - DAC output on A0 (P014 AN09 DAC)
#define DAC12_DADR0    ((volatile unsigned short *)(DACBASE + 0xE000))      // D/A Data Register 0
#define DAC12_DACR     ((volatile unsigned char  *)(DACBASE + 0xE004))      // D/A Control Register
#define DAC12_DADPR    ((volatile unsigned char  *)(DACBASE + 0xE005))      // DADR0 Format Select Register
#define DAC12_DAADSCR  ((volatile unsigned char  *)(DACBASE + 0xE006))      // D/A A/D Synchronous Start Control Register
#define DAC12_DAVREFCR ((volatile unsigned char  *)(DACBASE + 0xE007))      // D/A VREF Control Register


// =========== Ports ============
#define PORTBASE 0x40040000 /* Port Base */

// 19.2.1 Port Control Registers, to control/access multiple pins with a single operation
// Not all options defined for each register, expand as needed

#define PCNTR1 0x0000  // Port n Control Register 1 - 32bit access
#define PORT0_PCNTR1 ((volatile unsigned int *)(PORTBASE + PCNTR1))              //
#define PORT1_PCNTR1 ((volatile unsigned int *)(PORTBASE + PCNTR1 + (1 * 0x20))) //
#define PORT2_PCNTR1 ((volatile unsigned int *)(PORTBASE + PCNTR1 + (2 * 0x20))) //
#define PORT3_PCNTR1 ((volatile unsigned int *)(PORTBASE + PCNTR1 + (3 * 0x20))) //
#define PORT4_PCNTR1 ((volatile unsigned int *)(PORTBASE + PCNTR1 + (4 * 0x20))) //
#define PORT5_PCNTR1 ((volatile unsigned int *)(PORTBASE + PCNTR1 + (5 * 0x20))) //

#define PODR 0x0000  // Port n Control Register 1 - Output Data
#define PORT0_PODR ((volatile unsigned short *)(PORTBASE + PODR))              //
#define PORT1_PODR ((volatile unsigned short *)(PORTBASE + PODR + (1 * 0x20))) //
#define PORT2_PODR ((volatile unsigned short *)(PORTBASE + PODR + (2 * 0x20))) //
#define PORT3_PODR ((volatile unsigned short *)(PORTBASE + PODR + (3 * 0x20))) //
#define PORT4_PODR ((volatile unsigned short *)(PORTBASE + PODR + (4 * 0x20))) //
#define PORT5_PODR ((volatile unsigned short *)(PORTBASE + PODR + (5 * 0x20))) //

#define PDR  0x0002  // Port n Control Register 1 - Pin Direction
#define PORT0_PDR ((volatile unsigned short *)(PORTBASE + PDR))              //
#define PORT1_PDR ((volatile unsigned short *)(PORTBASE + PDR + (1 * 0x20))) //
#define PORT2_PDR ((volatile unsigned short *)(PORTBASE + PDR + (2 * 0x20))) //
#define PORT3_PDR ((volatile unsigned short *)(PORTBASE + PDR + (3 * 0x20))) //
#define PORT4_PDR ((volatile unsigned short *)(PORTBASE + PDR + (4 * 0x20))) //
#define PORT5_PDR ((volatile unsigned short *)(PORTBASE + PDR + (5 * 0x20))) //

#define PIDR  0x0006  // Port n Control Register 2 - Pin State - For reading
#define PORT0_PIDR ((volatile unsigned short *)(PORTBASE + PIDR))              //
#define PORT1_PIDR ((volatile unsigned short *)(PORTBASE + PIDR + (1 * 0x20))) //
#define PORT2_PIDR ((volatile unsigned short *)(PORTBASE + PIDR + (2 * 0x20))) //
#define PORT3_PIDR ((volatile unsigned short *)(PORTBASE + PIDR + (3 * 0x20))) //
#define PORT4_PIDR ((volatile unsigned short *)(PORTBASE + PIDR + (4 * 0x20))) //
#define PORT5_PIDR ((volatile unsigned short *)(PORTBASE + PIDR + (5 * 0x20))) //

#define PORR 0x0008  // Port n Control Register 3 - Pin Reset (to Low Output)
#define PORT0_PORR ((volatile unsigned short *)(PORTBASE + PORR))              //
#define PORT1_PORR ((volatile unsigned short *)(PORTBASE + PORR + (1 * 0x20))) //
#define PORT2_PORR ((volatile unsigned short *)(PORTBASE + PORR + (2 * 0x20))) //
#define PORT3_PORR ((volatile unsigned short *)(PORTBASE + PORR + (3 * 0x20))) //
#define PORT4_PORR ((volatile unsigned short *)(PORTBASE + PORR + (4 * 0x20))) //
#define PORT5_PORR ((volatile unsigned short *)(PORTBASE + PORR + (5 * 0x20))) //

#define POSR  0x000A  // Port n Control Register 3 - Pin Set (to High Output)
#define PORT0_POSR ((volatile unsigned short *)(PORTBASE + POSR))              //
#define PORT1_POSR ((volatile unsigned short *)(PORTBASE + POSR + (1 * 0x20))) //
#define PORT2_POSR ((volatile unsigned short *)(PORTBASE + POSR + (2 * 0x20))) //
#define PORT3_POSR ((volatile unsigned short *)(PORTBASE + POSR + (3 * 0x20))) //
#define PORT4_POSR ((volatile unsigned short *)(PORTBASE + POSR + (4 * 0x20))) //
#define PORT5_POSR ((volatile unsigned short *)(PORTBASE + POSR + (5 * 0x20))) //

// 19.2.5 Port mn Pin Function Select Register (PmnPFS/PmnPFS_HA/PmnPFS_BY) (m = 0 to 9; n = 00 to 15)
// 32 bits - Use for setting pin functions to other than default pin I/O

#define PMISC_PWPR ((volatile unsigned char  *)(PORTBASE + 0x0D03))            // Write-Protect Register - 19.2.6
#define PWPR_PFSWE  6  // PmnPFS Register Write Enable; 0: Writing to PmnPFS register disabled, 1: Writing enabled.
#define PWPR_B0WI   7  // PFSWE Bit Write Disable; 0: Writing to the PFSWE bit enabled, 1: Writing disabled

// Note: These are the pin allocations for the R4 Minima - The WiFI has diferent pin assignments
// Note: Pin IRQxx on R4 Minima pins D0, D1, D2, D3, D8, D12, D13, D15, D16, D17, D18, D19 only

#define P000PFS 0x0800  // Port 0 Pin Function Select Register
#define PFS_P000PFS ((volatile unsigned int *)(PORTBASE + P000PFS))            // A1 / D15 - AN00 - AMP0+ - IRQ6
#define PFS_P001PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 1 * 4))) // A2 / D16 - AN01 - AMP0- - IRQ7
#define PFS_P002PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 2 * 4))) // A3 / D17 - AN03 - AMP0O - IRQ2
#define PFS_P003PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 3 * 4))) // N/C - AN03
#define PFS_P004PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 4 * 4))) // N/C - AN04 - IRQ3
#define PFS_P005PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 5 * 4))) // N/A - AN11 - IRQ10
#define PFS_P006PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 6 * 4))) // N/A - AN12
#define PFS_P007PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 7 * 4))) // N/A - AN13
#define PFS_P008PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 8 * 4))) // N/A - AN14
// #define PFS_P009PFS ((volatile unsigned int *)(PORTBASE + P000PFS + ( 9 * 4))) // Does not exist
#define PFS_P010PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (10 * 4))) // N/A - AN05
#define PFS_P011PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (11 * 4))) // N/C - AN06 - IRQ15
#define PFS_P012PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (12 * 4))) // TxLED - AN07
#define PFS_P013PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (13 * 4))) // RxLED - AN08
#define PFS_P014PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (14 * 4))) // A0 / D14 - AN09 - DAC0
#define PFS_P015PFS ((volatile unsigned int *)(PORTBASE + P000PFS + (15 * 4))) // N/C - AN10 - IRQ7

#define P100PFS 0x0840  // Port 1 Pin Function Select Register
#define PFS_P100PFS ((volatile unsigned int *)(PORTBASE + P100PFS))            // A5 / D19 - MISOA - SCL1 - IRQ2
#define PFS_P101PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 1 * 4))) // A4 / D18 - MOSIA - SDA1 - IRQ1
#define PFS_P102PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 2 * 4))) // D5 - RSPCKA
#define PFS_P103PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 3 * 4))) // D4 - SSLA0
#define PFS_P104PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 4 * 4))) // D3 - IRQ1
#define PFS_P105PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 5 * 4))) // D2 - IRQ0
#define PFS_P106PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 6 * 4))) // D6
#define PFS_P107PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 7 * 4))) // D7
#define PFS_P108PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 8 * 4))) // SWD p2 SWDIO
#define PFS_P109PFS ((volatile unsigned int *)(PORTBASE + P100PFS + ( 9 * 4))) // D11 / MOSI
#define PFS_P110PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (10 * 4))) // D12 / MISO - IRQ3
#define PFS_P111PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (11 * 4))) // D13 / SCLK - IRQ4
#define PFS_P112PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (12 * 4))) // D10 / CS
#define PFS_P113PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (13 * 4))) // N/C
#define PFS_P114PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (14 * 4))) // N/A
#define PFS_P115PFS ((volatile unsigned int *)(PORTBASE + P100PFS + (15 * 4))) // N/A

#define P200PFS 0x0880  // Port 2 Pin Function Select Register
#define PFS_P200PFS ((volatile unsigned int *)(PORTBASE + P200PFS))           // NMI
#define PFS_P201PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (1 * 4))) // MD
#define PFS_P202PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (2 * 4))) // N/A
#define PFS_P203PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (3 * 4))) // N/A
#define PFS_P204PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (4 * 4))) // LOVE (Heart Pad on underside of board)
#define PFS_P205PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (5 * 4))) // N/C - IRQ1 - CLKOUT
#define PFS_P206PFS ((volatile unsigned int *)(PORTBASE + P200PFS + (6 * 4))) // N/C - IRQ0
// Pins P212, P213, P214, and P215 are Crystal functions, or N/C

#define P300PFS 0x08C0  // Port 3 Pin Function Select Register
#define PFS_P300PFS ((volatile unsigned int *)(PORTBASE + P300PFS))            // SWD p4 SWCLK
#define PFS_P301PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 1 * 4))) // D0 / RxD - IRQ6
#define PFS_P302PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 2 * 4))) // D1 / TxD - IRQ5
#define PFS_P303PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 3 * 4))) // D9
#define PFS_P304PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 4 * 4))) // D8  - IRQ9
#define PFS_P305PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 5 * 4))) // N/C - IRQ8
#define PFS_P306PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 6 * 4))) // N/C
#define PFS_P307PFS ((volatile unsigned int *)(PORTBASE + P300PFS + ( 7 * 4))) // N/C

#define P400PFS 0x0900  // Port 4 Pin Function Select Register
#define PFS_P400PFS ((volatile unsigned int *)(PORTBASE + P400PFS))            // N/C - IRQ0
#define PFS_P401PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 1 * 4))) // N/C - IRQ5
#define PFS_P402PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 2 * 4))) // N/C - IRQ4
#define PFS_P403PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 3 * 4))) // N/A
#define PFS_P404PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 4 * 4))) // N/A
#define PFS_P405PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 5 * 4))) // N/A
#define PFS_P406PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 6 * 4))) // N/A
#define PFS_P407PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 7 * 4))) // ADTRG0 - R7, R8, and R9 divider.
#define PFS_P408PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 8 * 4))) // N/C - IRQ7
#define PFS_P409PFS ((volatile unsigned int *)(PORTBASE + P400PFS + ( 9 * 4))) // N/C - IRQ6
#define PFS_P410PFS ((volatile unsigned int *)(PORTBASE + P400PFS + (10 * 4))) // N/C - IRQ5
#define PFS_P411PFS ((volatile unsigned int *)(PORTBASE + P400PFS + (11 * 4))) // N/C - IRQ4
#define PFS_P412PFS ((volatile unsigned int *)(PORTBASE + P400PFS + (12 * 4))) // N/A
#define PFS_P413PFS ((volatile unsigned int *)(PORTBASE + P400PFS + (13 * 4))) // N/A
#define PFS_P414PFS ((volatile unsigned int *)(PORTBASE + P400PFS + (14 * 4))) // N/A - IRQ9
#define PFS_P415PFS ((volatile unsigned int *)(PORTBASE + P400PFS + (15 * 4))) // N/A - IRQ8

#define P500PFS 0x0940  // Port 5 Pin Function Select Register
#define PFS_P500PFS ((volatile unsigned int *)(PORTBASE + P500PFS))           // AN016 in R10/R26 divider
#define PFS_P501PFS ((volatile unsigned int *)(PORTBASE + P500PFS + (1 * 4))) // SWD p8 TxD - IRQ11
#define PFS_P502PFS ((volatile unsigned int *)(PORTBASE + P500PFS + (2 * 4))) // SWD p7 RxD - IRQ12
#define PFS_P503PFS ((volatile unsigned int *)(PORTBASE + P500PFS + (3 * 4))) // N/A
#define PFS_P504PFS ((volatile unsigned int *)(PORTBASE + P500PFS + (4 * 4))) // N/A
#define PFS_P505PFS ((volatile unsigned int *)(PORTBASE + P500PFS + (5 * 4))) // N/A - IRQ14

#define PFS_PODR   0  // Pin Output Data     - 0: Low output; 1: High output
#define PFS_PIDR   1  // Pin Input State     - Read 0: Low level; 1: High level
#define PFS_PDR    2  // Pin Direction       - 0: Input (input pin); 1: Output (output pin)
#define PFS_PCR    4  // Pull-up Control     - 1: Enable internal pull-up
#define PFS_NCODR  6  // N-Channel Open Drain Control - 1: NMOS open-drain output.
#define PFS_DSCR  10  // Port Drive Capability - 1: Middle drive; Default 0: Low drive
#define PFS_EOR   12  // Event on Rising     - 1: Detect rising edge  - Set EOR and EOF both to 1
#define PFS_EOF   13  // Event on Falling    - 1: Detect falling edge - ... for Detect both edges
#define PFS_ISEL  14  // IRQ Input Enable    - 1: Used as an IRQn input pin.
#define PFS_ASEL  15  // Analog Input Enable - 1: Used as an analog pin.
#define PFS_PMR   16  // Pin Mode Control    - 1: Used as an I/O port for peripheral functions
#define PFS_PSEL_4_0 24 // Peripheral Function Select

// 16 bit register access
#define PFS_P100PFS_HA ((volatile unsigned short *)(PORTBASE + 0x0842))
#define PFS_P115PFS_HA ((volatile unsigned short *)(PORTBASE + 0x0842 + (15 * 2)))
#define PFS_P200PFS_HA ((volatile unsigned short *)(PORTBASE + 0x0882))
#define PFS_P206PFS_HA ((volatile unsigned short *)(PORTBASE + 0x0882 + (06 * 2))) //

// 8 bits - Used for fast pin set/clear operations of register lower byte
#define PFS_P000PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803))            // A1
#define PFS_P001PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 1 * 4))) // A2
#define PFS_P002PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 2 * 4))) // A3
#define PFS_P003PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 3 * 4))) // N/C
#define PFS_P004PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 4 * 4))) // N/C
#define PFS_P005PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 5 * 4))) // N/A
#define PFS_P006PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 6 * 4))) // N/A
#define PFS_P007PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 7 * 4))) // N/A
#define PFS_P008PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 8 * 4))) // N/A
// #define PFS_P009PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + ( 9 * 4))) //
#define PFS_P010PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + (10 * 4))) // N/A
#define PFS_P011PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + (11 * 4))) // N/C - P58 - VREFL0 << WTF!!!
#define PFS_P012PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + (12 * 4))) // TxLED - VREFH
#define PFS_P013PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + (13 * 4))) // RxLED - VREFL
#define PFS_P014PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + (14 * 4))) // A0 - DAC0
#define PFS_P015PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0803 + (15 * 4))) // N/C

#define PFS_P100PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843))            // A5
#define PFS_P101PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 1 * 4))) // A4
#define PFS_P102PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 2 * 4))) // D5
#define PFS_P103PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 3 * 4))) // D4
#define PFS_P104PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 4 * 4))) // D3
#define PFS_P105PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 5 * 4))) // D2
#define PFS_P106PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 6 * 4))) // D6
#define PFS_P107PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 7 * 4))) // D7
#define PFS_P108PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 8 * 4))) // SWDIO
#define PFS_P109PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + ( 9 * 4))) // D11 / MOSI
#define PFS_P110PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (10 * 4))) // D12 / MISO
#define PFS_P111PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (11 * 4))) // D13 / SCLK
#define PFS_P112PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (12 * 4))) // D10 / CS
#define PFS_P113PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (13 * 4))) // N/C
#define PFS_P114PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (14 * 4))) // N/A
#define PFS_P115PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0843 + (15 * 4))) // N/A

#define PFS_P200PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883))            // NMI input
#define PFS_P201PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883 + (01 * 4))) // MD
#define PFS_P202PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883 + (02 * 4))) // N/A
#define PFS_P203PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883 + (03 * 4))) // N/A
#define PFS_P204PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883 + (04 * 4))) // LOVE (Heart Pad on underside of board)
#define PFS_P205PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883 + (05 * 4))) // N/C
#define PFS_P206PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x0883 + (06 * 4))) // N/C

#define PFS_P212PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08B3))            // EXTAL
#define PFS_P213PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08B3 + (01 * 4))) // XTAL
#define PFS_P214PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08B3 + (02 * 4))) // N/C XCOUT (P214 IN)
#define PFS_P215PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08B3 + (03 * 4))) // N/C XCIN  (P215 IN)

#define PFS_P300PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3))            // SWCLK (P300)
#define PFS_P301PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (01 * 4))) // D0 / RxD (P301)
#define PFS_P302PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (02 * 4))) // D1 / TxD (P302)
#define PFS_P303PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (03 * 4))) // D9
#define PFS_P304PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (04 * 4))) // D8
#define PFS_P305PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (05 * 4))) // N/C
#define PFS_P306PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (06 * 4))) // N/C
#define PFS_P307PFS_BY ((volatile unsigned char  *)(PORTBASE + 0x08C3 + (07 * 4))) // N/C


// ==== General PWM Timer (GPT) ====
#define GPTBASE 0x40070000 /* PWM Base */

#define GTWP 0x8000  // General PWM Timer Write-Protection Register
#define GPT320_GTWP ((volatile unsigned int *)(GPTBASE + GTWP))
#define GPT321_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0100))
#define GPT162_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0200))
#define GPT163_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0300))
#define GPT164_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0400))
#define GPT165_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0500))
#define GPT166_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0600))
#define GPT167_GTWP ((volatile unsigned int *)(GPTBASE + GTWP + 0x0700))

#define GTSTR 0x8004  // General PWM Timer Software Start Register
#define GPT320_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR))
#define GPT321_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0100))
#define GPT162_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0200))
#define GPT163_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0300))
#define GPT164_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0400))
#define GPT165_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0500))
#define GPT166_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0600))
#define GPT167_GTSTR ((volatile unsigned int *)(GPTBASE + GTSTR + 0x0700))

#define GTSTP 0x8008  // General PWM Timer Software Stop Register
#define GPT320_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP))
#define GPT321_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0100))
#define GPT162_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0200))
#define GPT163_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0300))
#define GPT164_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0400))
#define GPT165_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0500))
#define GPT166_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0600))
#define GPT167_GTSTP ((volatile unsigned int *)(GPTBASE + GTSTP + 0x0700))

#define GTCLR 0x800C  // General PWM Timer Software Clear Register
#define GPT320_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR))
#define GPT321_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0100))
#define GPT162_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0200))
#define GPT163_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0300))
#define GPT164_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0400))
#define GPT165_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0500))
#define GPT166_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0600))
#define GPT167_GTCLR ((volatile unsigned int *)(GPTBASE + GTCLR + 0x0700))

#define GTSSR 0x8010  // General PWM Timer Start Source Select Register
#define GPT320_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR))
#define GPT321_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0100))
#define GPT162_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0200))
#define GPT163_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0300))
#define GPT164_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0400))
#define GPT165_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0500))
#define GPT166_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0600))
#define GPT167_GTSSR ((volatile unsigned int *)(GPTBASE + GTSSR + 0x0700))

#define GTPSR 0x8014  // General PWM Timer Stop Source Select Register
#define GPT320_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR))
#define GPT321_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0100))
#define GPT162_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0200))
#define GPT163_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0300))
#define GPT164_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0400))
#define GPT165_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0500))
#define GPT166_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0600))
#define GPT167_GTPSR ((volatile unsigned int *)(GPTBASE + GTPSR + 0x0700))

#define GTCSR 0x8018  // General PWM Timer Clear Source Select Register
#define GPT320_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR))
#define GPT321_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0100))
#define GPT162_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0200))
#define GPT163_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0300))
#define GPT164_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0400))
#define GPT165_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0500))
#define GPT166_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0600))
#define GPT167_GTCSR ((volatile unsigned int *)(GPTBASE + GTCSR + 0x0700))

#define GTUPSR 0x801C  // General PWM Timer Up Count Source Select Register
#define GPT320_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR))
#define GPT321_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0100))
#define GPT162_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0200))
#define GPT163_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0300))
#define GPT164_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0400))
#define GPT165_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0500))
#define GPT166_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0600))
#define GPT167_GTUPSR ((volatile unsigned int *)(GPTBASE + GTUPSR + 0x0700))

#define GTDNSR 0x8020  // General PWM Timer Down Count Source Select Register
#define GPT320_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR))
#define GPT321_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0100))
#define GPT162_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0200))
#define GPT163_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0300))
#define GPT164_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0400))
#define GPT165_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0500))
#define GPT166_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0600))
#define GPT167_GTDNSR ((volatile unsigned int *)(GPTBASE + GTDNSR + 0x0700))

#define GTICASR 0x8024 // General PWM Timer Input Capture Source Select Register A
#define GTICBSR 0x8028 // General PWM Timer Input Capture Source Select Register B

#define GTCR 0x802C // General PWM Timer Control Register
#define GPT320_GTCR ((volatile unsigned int *)(GPTBASE + GTCR))
#define GPT321_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0100))
#define GPT162_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0200))
#define GPT163_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0300))
#define GPT164_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0400))
#define GPT165_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0500))
#define GPT166_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0600))
#define GPT167_GTCR ((volatile unsigned int *)(GPTBASE + GTCR + 0x0700))

#define GTUDDTYC 0x8030 // General PWM Timer Count Direction and Duty Setting Register
#define GPT320_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC))
#define GPT321_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0100))
#define GPT162_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0200))
#define GPT163_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0300))
#define GPT164_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0400))
#define GPT165_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0500))
#define GPT166_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0600))
#define GPT167_GTUDDTYC ((volatile unsigned int *)(GPTBASE + GTUDDTYC + 0x0700))

#define GTIOR 0x8034 // General PWM Timer I/O Control Register
#define GPT320_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR))
#define GPT321_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0100))
#define GPT162_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0200))
#define GPT163_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0300))
#define GPT164_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0400))
#define GPT165_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0500))
#define GPT166_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0600))
#define GPT167_GTIOR ((volatile unsigned int *)(GPTBASE + GTIOR + 0x0700))

#define GTST 0x803C // General PWM Timer Status Register
#define GPT320_GTST ((volatile unsigned int *)(GPTBASE + GTST))
#define GPT321_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0100))
#define GPT162_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0200))
#define GPT163_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0300))
#define GPT164_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0400))
#define GPT165_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0500))
#define GPT166_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0600))
#define GPT167_GTST ((volatile unsigned int *)(GPTBASE + GTST + 0x0700))

#define GTBER 0x8040 // General PWM Timer Buffer Enable Register
#define GPT320_GTBER ((volatile unsigned int *)(GPTBASE + GTBER))
#define GPT321_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0100))
#define GPT162_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0200))
#define GPT163_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0300))
#define GPT164_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0400))
#define GPT165_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0500))
#define GPT166_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0600))
#define GPT167_GTBER ((volatile unsigned int *)(GPTBASE + GTBER + 0x0700))

// Note: GTCNT can only be written to after the counting stops
#define GTCNT 0x8048 // General PWM Timer Counter
#define GPT320_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT))
#define GPT321_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0100))
#define GPT162_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0200))
#define GPT163_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0300))
#define GPT164_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0400))
#define GPT165_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0500))
#define GPT166_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0600))
#define GPT167_GTCNT ((volatile unsigned int *)(GPTBASE + GTCNT + 0x0700))

#define GTCCRA 0x804C // General PWM Timer Compare Capture Register A
#define GPT320_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA))
#define GPT321_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0100))
#define GPT162_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0200))
#define GPT163_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0300))
#define GPT164_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0400))
#define GPT165_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0500))
#define GPT166_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0600))
#define GPT167_GTCCRA ((volatile unsigned int *)(GPTBASE + GTCCRA + 0x0700))

#define GTCCRB 0x8050 // General PWM Timer Compare Capture Register B
#define GPT320_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB))
#define GPT321_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0100))
#define GPT162_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0200))
#define GPT163_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0300))
#define GPT164_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0400))
#define GPT165_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0500))
#define GPT166_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0600))
#define GPT167_GTCCRB ((volatile unsigned int *)(GPTBASE + GTCCRB + 0x0700))

#define GTCCRC 0x8054 // General PWM Timer Compare Capture Register C
#define GPT320_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC))
#define GPT321_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0100))
#define GPT162_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0200))
#define GPT163_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0300))
#define GPT164_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0400))
#define GPT165_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0500))
#define GPT166_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0600))
#define GPT167_GTCCRC ((volatile unsigned int *)(GPTBASE + GTCCRC + 0x0700))

//#define GTCCRD 0x805C // General PWM Timer Compare Capture Register D  - See page 425 22.2.19
#define GTCCRD 0x8058 // General PWM Timer Compare Capture Register D
#define GPT320_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD))
#define GPT321_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0100))
#define GPT162_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0200))
#define GPT163_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0300))
#define GPT164_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0400))
#define GPT165_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0500))
#define GPT166_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0600))
#define GPT167_GTCCRD ((volatile unsigned int *)(GPTBASE + GTCCRD + 0x0700))

//#define GTCCRE 0x8058 // General PWM Timer Compare Capture Register E  - As above...
#define GTCCRE 0x805C // General PWM Timer Compare Capture Register E
#define GPT320_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE))
#define GPT321_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0100))
#define GPT162_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0200))
#define GPT163_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0300))
#define GPT164_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0400))
#define GPT165_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0500))
#define GPT166_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0600))
#define GPT167_GTCCRE ((volatile unsigned int *)(GPTBASE + GTCCRE + 0x0700))

#define GTCCRF 0x8060 // General PWM Timer Compare Capture Register F
#define GPT320_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF))
#define GPT321_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0100))
#define GPT162_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0200))
#define GPT163_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0300))
#define GPT164_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0400))
#define GPT165_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0500))
#define GPT166_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0600))
#define GPT167_GTCCRF ((volatile unsigned int *)(GPTBASE + GTCCRF + 0x0700))

#define GTPR 0x8064 // General PWM Timer Cycle Setting Register
#define GPT320_GTPR ((volatile unsigned int *)(GPTBASE + GTPR))
#define GPT321_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0100))
#define GPT162_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0200))
#define GPT163_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0300))
#define GPT164_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0400))
#define GPT165_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0500))
#define GPT166_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0600))
#define GPT167_GTPR ((volatile unsigned int *)(GPTBASE + GTPR + 0x0700))

#define GTPBR 0x8068 // General PWM Timer Cycle Setting Buffer Register
#define GPT320_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR))
#define GPT321_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0100))
#define GPT162_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0200))
#define GPT163_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0300))
#define GPT164_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0400))
#define GPT165_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0500))
#define GPT166_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0600))
#define GPT167_GTPBR ((volatile unsigned int *)(GPTBASE + GTPBR + 0x0700))

// ====  Asynchronous General Purpose Timer (AGT) =====
#define AGTBASE 0x40084000
#define AGT0_AGT    ((volatile unsigned short *)(AGTBASE))         // AGT Counter Register
#define AGT1_AGT    ((volatile unsigned short *)(AGTBASE + 0x100))
#define AGT0_AGTCMA ((volatile unsigned short *)(AGTBASE + 0x002)) // AGT Compare Match A Register
#define AGT1_AGTCMA ((volatile unsigned short *)(AGTBASE + 0x102))
#define AGT0_AGTCMB ((volatile unsigned short *)(AGTBASE + 0x004)) // AGT Compare Match B Register
#define AGT1_AGTCMB ((volatile unsigned short *)(AGTBASE + 0x104))

// 8 bit registers
#define AGT0_AGTCR    ((volatile unsigned char  *)(AGTBASE + 0x008))  // AGT Control Register
#define AGT1_AGTCR    ((volatile unsigned char  *)(AGTBASE + 0x108))  //
#define AGTCR_TSTART 0  // R/W - AGT Count Start; 1: Count starts, 0: Count stops
#define AGTCR_TCSTF  1  // R   - AGT Count Status Flag; 1: Count in progress, 0: Count is stopped
#define AGTCR_TSTOP  2  // W   - AGT Count Forced Stop; 1: The count is forcibly stopped, 0: Writing 0 is invalid!!!
#define AGT0_AGTMR1   ((volatile unsigned char  *)(AGTBASE + 0x009))  // AGT Mode Register 1
#define AGT1_AGTMR1   ((volatile unsigned char  *)(AGTBASE + 0x109))  //
#define AGT0_AGTMR2   ((volatile unsigned char  *)(AGTBASE + 0x00A))  // AGT Mode Register 2
#define AGT1_AGTMR2   ((volatile unsigned char  *)(AGTBASE + 0x10A))  //
#define AGT0_AGTIOC   ((volatile unsigned char  *)(AGTBASE + 0x00C))  // AGT I/O Control Register
#define AGT1_AGTIOC   ((volatile unsigned char  *)(AGTBASE + 0x10C))  //
#define AGTIOC_TOE   2  // AGTOn Output Enable
#define AGT0_AGTISR   ((volatile unsigned char  *)(AGTBASE + 0x00D))  // AGT Event Pin Select Register
#define AGT1_AGTISR   ((volatile unsigned char  *)(AGTBASE + 0x10D))  //
#define AGT0_AGTCMSR  ((volatile unsigned char  *)(AGTBASE + 0x00E))  // AGT Compare Match Function Select Register
#define AGT1_AGTCMSR  ((volatile unsigned char  *)(AGTBASE + 0x10E))  //
#define AGT0_AGTIOSEL ((volatile unsigned char  *)(AGTBASE + 0x00F))  // AGT Pin Select Register
#define AGT1_AGTIOSEL ((volatile unsigned char  *)(AGTBASE + 0x10F))  //

// Note: With the Arduino IDE setup, it does not appear possible to use the WatchDog timers
// ====  Watchdog Timer (WDT) =====
#define WDTBASE 0x40044200
#define WDT_WDTCR    ((volatile unsigned short *)(WDTBASE + 0x02))  // WDT Control Register
#define WDT_WDTSR    ((volatile unsigned short *)(WDTBASE + 0x04))  // WDT Status Register

// 8 bit registers
#define WDT_WDTRR    ((volatile unsigned char  *)(WDTBASE + 0x00))  // WDT Refresh Register
#define WDT_WDTRCR   ((volatile unsigned char  *)(WDTBASE + 0x06))  // WDT Reset Control Register
#define WDT_WDTCSTPR ((volatile unsigned char  *)(WDTBASE + 0x06))  // WDT Count Stop Control Register

// ====  Independant Watchdog Timer (IWDT) =====
#define IWDTBASE 0x40044000
#define IWDT_IWDTSR    ((volatile unsigned short *)(IWDTBASE + 0x04))  // WDT Status Register

// 8 bit registers
#define IWDT_IWDTRR    ((volatile unsigned char  *)(IWDTBASE + 0x00))  // WDT Refresh Register


// ==== 28. Serial Communications Interface (SCI) ====
#define SCIBASE 0x40070000
// Receive Data Register
#define SCI0_RDR ((volatile unsigned char  *)(SCIBASE + 0x0005))
#define SCI1_RDR ((volatile unsigned char  *)(SCIBASE + 0x0025))
#define SCI2_RDR ((volatile unsigned char  *)(SCIBASE + 0x0045))
#define SCI9_RDR ((volatile unsigned char  *)(SCIBASE + 0x0125))
// Receive 9-bit Data Register
#define SCI0_RDRHL ((volatile unsigned short *)(SCIBASE + 0x0010))
#define SCI1_RDRHL ((volatile unsigned short *)(SCIBASE + 0x0030))
#define SCI2_RDRHL ((volatile unsigned short *)(SCIBASE + 0x0050))
#define SCI9_RDRHL ((volatile unsigned short *)(SCIBASE + 0x0130))
// Receive FIFO Data Register H
#define SCI0_FRDRH ((volatile unsigned char *)(SCIBASE + 0x0010))
#define SCI1_FRDRH ((volatile unsigned char *)(SCIBASE + 0x0030))
// Receive FIFO Data Register L
#define SCI0_FRDRL ((volatile unsigned char *)(SCIBASE + 0x0011))
#define SCI1_FRDRL ((volatile unsigned char *)(SCIBASE + 0x0031))
// Receive FIFO Data Register HL
#define SCI0_FRDRHL ((volatile unsigned short *)(SCIBASE + 0x0010))
#define SCI1_FRDRHL ((volatile unsigned short *)(SCIBASE + 0x0030))
// Transmit Data Register
#define SCI0_TDR ((volatile unsigned char  *)(SCIBASE + 0x0003))
#define SCI1_TDR ((volatile unsigned char  *)(SCIBASE + 0x0023))
#define SCI2_TDR ((volatile unsigned char  *)(SCIBASE + 0x0043))
#define SCI9_TDR ((volatile unsigned char  *)(SCIBASE + 0x0123))
// Transmit 9-bit Data Register
#define SCI0_TDRHL ((volatile unsigned short *)(SCIBASE + 0x000E))
#define SCI1_TDRHL ((volatile unsigned short *)(SCIBASE + 0x002E))
#define SCI2_TDRHL ((volatile unsigned short *)(SCIBASE + 0x004E))
#define SCI9_TDRHL ((volatile unsigned short *)(SCIBASE + 0x012E))
// Transmit FIFO Data Register H
#define SCI0_FTDRH ((volatile unsigned char *)(SCIBASE + 0x000E))
#define SCI1_FTDRH ((volatile unsigned char *)(SCIBASE + 0x002E))
// Transmit FIFO Data Register L
#define SCI0_FTDRL ((volatile unsigned char *)(SCIBASE + 0x000F))
#define SCI1_FTDRL ((volatile unsigned char *)(SCIBASE + 0x002F))
// Transmit FIFO Data Register HL
#define SCI0_FTDRHL ((volatile unsigned short *)(SCIBASE + 0x000E))
#define SCI1_FTDRHL ((volatile unsigned short *)(SCIBASE + 0x002E))
// Serial Mode Register
#define SCI0_SMR ((volatile unsigned char  *)(SCIBASE + 0x0000))
#define SCI1_SMR ((volatile unsigned char  *)(SCIBASE + 0x0020))
#define SCI2_SMR ((volatile unsigned char  *)(SCIBASE + 0x0040))
#define SCI9_SMR ((volatile unsigned char  *)(SCIBASE + 0x0120))
#define SCI_CKS_1_0  0  // Clock Select
#define SCI_MP       2  // Multi-Processor Mode
#define SCI_STOP     3  // Stop Bit Length
#define SCI_PM       4  // Parity Mode
#define SCI_PE       5  // Parity Enable
#define SCI_CHR      6  // Character Length (default 8 bit), in combination with the CHR1 bit in SCMR
#define SCI_CM       7  // Communication Mode
// Serial Control Register
#define SCI0_SCR ((volatile unsigned char  *)(SCIBASE + 0x0002))
#define SCI1_SCR ((volatile unsigned char  *)(SCIBASE + 0x0022))
#define SCI2_SCR ((volatile unsigned char  *)(SCIBASE + 0x0042))
#define SCI9_SCR ((volatile unsigned char  *)(SCIBASE + 0x0122))
#define SCR_CKE_1_0  0  // Clock Enable
#define SCR_TEIE     2  // Transmit End Interrupt Enable
#define SCR_MPIE     3  // Multi-Processor Interrupt Enable
#define SCR_RE       4  // Receive Enable
#define SCR_TE       5  // Transmit Enable
#define SCR_RIE      6  // Receive Interrupt Enable
#define SCR_TIE      7  // Transmit Interrupt Enable
// Serial Status Register
#define SCI0_SSR ((volatile unsigned char  *)(SCIBASE + 0x0004))
#define SCI1_SSR ((volatile unsigned char  *)(SCIBASE + 0x0024))
#define SCI2_SSR ((volatile unsigned char  *)(SCIBASE + 0x0044))
#define SCI9_SSR ((volatile unsigned char  *)(SCIBASE + 0x0124))
#define SSR_TEND      2  // Transmit End Flag
#define SSR_RDRF      6  // Receive Data Full Flag
#define SSR_TDRE      7  // Transmit Data Empty Flag
// Serial Status Register FIFO
#define SCI0_SSR_FIFO ((volatile unsigned char  *)(SCIBASE + 0x0004))
#define SCI1_SSR_FIFO ((volatile unsigned char  *)(SCIBASE + 0x0024))
#define SSR_DR        0  // Receive Data Ready Flag
#define SSR_TEND      2  // Transmit End Flag
#define SSR_RDF       6  // Receive FIFO Data Full Flag
#define SSR_TDFE      7  // Transmit FIFO Data Empty Flag
// Smart Card Mode Register - and other settings
#define SCI0_SCMR ((volatile unsigned char  *)(SCIBASE + 0x0006))
#define SCI1_SCMR ((volatile unsigned char  *)(SCIBASE + 0x0026))
#define SCI2_SCMR ((volatile unsigned char  *)(SCIBASE + 0x0046))
#define SCI9_SCMR ((volatile unsigned char  *)(SCIBASE + 0x0126))
#define SCMR_SMIF     0  // Smart Card Interface Mode Select; 0: Non-smart card interface mode
#define SCMR_SINV     2  // Transmitted/Received Data Invert
#define SCMR_SDIR     3  // Transmitted/Received Data Transfer Direction
#define SCMR_CHR1     4  // Character Length 1
#define SCMR_BCP2     7  // Base Clock Pulse 2
// Bit Rate Register
#define SCI0_BRR ((volatile unsigned char  *)(SCIBASE + 0x0001))
#define SCI1_BRR ((volatile unsigned char  *)(SCIBASE + 0x0021))
#define SCI2_BRR ((volatile unsigned char  *)(SCIBASE + 0x0041))
#define SCI9_BRR ((volatile unsigned char  *)(SCIBASE + 0x0121))
// Modulation Duty Register
#define SCI0_MDDR ((volatile unsigned char  *)(SCIBASE + 0x0012))
#define SCI1_MDDR ((volatile unsigned char  *)(SCIBASE + 0x0032))
#define SCI2_MDDR ((volatile unsigned char  *)(SCIBASE + 0x0052))
#define SCI9_MDDR ((volatile unsigned char  *)(SCIBASE + 0x0132))
// Serial Extended Mode Register
#define SCI0_SEMR ((volatile unsigned char  *)(SCIBASE + 0x0007))
#define SCI1_SEMR ((volatile unsigned char  *)(SCIBASE + 0x0027))
#define SCI2_SEMR ((volatile unsigned char  *)(SCIBASE + 0x0047))
#define SCI9_SEMR ((volatile unsigned char  *)(SCIBASE + 0x0127))
#define SEMR_BRME     2  // Bit Rate Modulation Enable
#define SEMR_ABCSE    3  // Asynchronous Mode Extended Base Clock Select 1
#define SEMR_ABCS     4  // Asynchronous Mode Base Clock Select
#define SEMR_NFEN     5  // Digital Noise Filter Function Enable
#define SEMR_BGDM     6  // Baud Rate Generator Double-Speed Mode Select
#define SEMR_RXDESEL  7  // Asynchronous Start Bit Edge Detection Select
// Noise Filter Setting Register
#define SCI0_SNFR ((volatile unsigned char  *)(SCIBASE + 0x0008))
#define SCI1_SNFR ((volatile unsigned char  *)(SCIBASE + 0x0028))
#define SCI2_SNFR ((volatile unsigned char  *)(SCIBASE + 0x0048))
#define SCI9_SNFR ((volatile unsigned char  *)(SCIBASE + 0x0128))
// Line Status Register
#define SCI0_LSR ((volatile unsigned short *)(SCIBASE + 0x0018))
#define SCI1_LSR ((volatile unsigned short *)(SCIBASE + 0x0038))
// Serial Port Register
#define SCI0_SPTR ((volatile unsigned char  *)(SCIBASE + 0x001C))
#define SCI1_SPTR ((volatile unsigned char  *)(SCIBASE + 0x003C))
#define SCI2_SPTR ((volatile unsigned char  *)(SCIBASE + 0x005C))
#define SCI9_SPTR ((volatile unsigned char  *)(SCIBASE + 0x013C))



// ==== 29. I2C Bus Interface ====
#define IICBASE 0x40050000
// I2C Bus Control Register 1
#define IIC0_ICCR1 ((volatile unsigned char  *)(IICBASE + 0x3000))
#define IIC1_ICCR1 ((volatile unsigned char  *)(IICBASE + 0x3100)) // 0x9F
#define ICCR1_SDAI     0  // SDA Line Monitor - R
#define ICCR1_SCLI     1  // SCL Line Monitor - R
#define ICCR1_SDAO     2  // SDA Output Control/Monitor - R/W
#define ICCR1_SCLO     3  // SCL Output Control/Monitor - R/W
#define ICCR1_SOWP     4  // SCLO/SDAO Write Protect
#define ICCR1_CLO      5  // Extra SCL Clock Cycle Output
#define ICCR1_IICRST   6  // I2C Bus Interface Internal Reset
#define ICCR1_ICE      7  // I2C Bus Interface Enable
// I2C Bus Control Register 2
#define IIC0_ICCR2 ((volatile unsigned char  *)(IICBASE + 0x3001))
#define IIC1_ICCR2 ((volatile unsigned char  *)(IICBASE + 0x3101)) // 0x00
#define ICCR2_ST       1  // Start Condition Issuance Request
#define ICCR2_RS       2  // Restart Condition Issuance Request
#define ICCR2_SP       3  // Stop Condition Issuance Request
#define ICCR2_TRS      5  // Transmit/Receive Mode
#define ICCR2_MST      6  // Master/Slave Mode
#define ICCR2_BBSY     7  // Bus Busy Detection Flag
// I2C Bus Mode Register 1
#define IIC0_ICMR1 ((volatile unsigned char  *)(IICBASE + 0x3002))
#define IIC1_ICMR1 ((volatile unsigned char  *)(IICBASE + 0x3102)) // 0x28
#define ICMR1_BC_2_0   0  // Bit Counter
#define ICMR1_BCWP     3  // BC Write Protect
#define ICMR1_CKS_2_0  4  // Internal Reference Clock Select
#define ICMR1_MTWP     7  // MST/TRS Write Protect
// I2C Bus Mode Register 2
#define IIC0_ICMR2 ((volatile unsigned char  *)(IICBASE + 0x3003))
#define IIC1_ICMR2 ((volatile unsigned char  *)(IICBASE + 0x3103)) // 0x05
#define ICMR2_TMOS     0  // Timeout Detection Time Select
#define ICMR2_TMOL     1  // Timeout L Count Control
#define ICMR2_TMOH     2  // Timeout H Count Control
#define ICMR2_SDDL_2_0 4  // SDA Output Delay Counter
#define ICMR2_DLCS     7  // SDA Output Delay Clock Source Select
// I2C Bus Mode Register 3
#define IIC0_ICMR3 ((volatile unsigned char  *)(IICBASE + 0x3004))
#define IIC1_ICMR3 ((volatile unsigned char  *)(IICBASE + 0x3104)) // 0x00
#define ICMR3_NF_1_0   0  // Noise Filter Stage Select
#define ICMR3_ACKBR    2  // Receive Acknowledge
#define ICMR3_ACKBT    3  // Transmit Acknowledge
#define ICMR3_ACKWP    4  // ACKBT Write Protect
#define ICMR3_RDRFS    5  // RDRF Flag Set Timing Select
#define ICMR3_WAIT     6  // WAIT - 0: No WAIT SCLn is not held low during the period between 9th clock cycle and 1ST clock cycle
#define ICMR3_SMBS     7  // SMBus/I2C-Bus Select
// I2C Bus Function Enable Register
#define IIC0_ICFER ((volatile unsigned char  *)(IICBASE + 0x3005))
#define IIC1_ICFER ((volatile unsigned char  *)(IICBASE + 0x3105)) // 0x77
#define ICFER_TMOE     0  // Timeout Function Enable
#define ICFER_MALE     1  // Master Arbitration-Lost Detection Enable
#define ICFER_NALE     2  // NACK Transmission Arbitration-Lost Detection Enable
#define ICFER_SALE     3  // Slave Arbitration-Lost Detection Enable
#define ICFER_NACKE    4  // NACK Reception Transfer Suspension Enable
#define ICFER_NFE      5  // Digital Noise Filter Circuit Enable
#define ICFER_SCLE     6  // SCL Synchronous Circuit Enable
// I2C Bus Status Enable Register
#define IIC0_ICSER ((volatile unsigned char  *)(IICBASE + 0x3006))
#define IIC1_ICSER ((volatile unsigned char  *)(IICBASE + 0x3106)) // 0x00
#define ICSER_SAR0E    0  // Slave Address Register 0 Enable
#define ICSER_SAR1E    1  // Slave Address Register 1 Enable
#define ICSER_SAR2E    2  // Slave Address Register 2 Enable
#define ICSER_GCAE     3  // General Call Address Enable
#define ICSER_DIDE     5  // Device ID Address Detection Enable
#define ICSER_HOAE     7  // Host Address Enable
// I2C Bus Interupt Enable Register
#define IIC0_ICIER ((volatile unsigned char  *)(IICBASE + 0x3007))
#define IIC1_ICIER ((volatile unsigned char  *)(IICBASE + 0x3107)) // 0xB3 Interrupts
#define ICIER_TMOIE    0  // Timeout Interrupt Request Enable
#define ICIER_ALIE     1  // Arbitration-Lost Interrupt Request Enable
#define ICIER_STIE     2  // Start Condition Detection Interrupt Request Enable
#define ICIER_SPIE     3  // Stop Condition Detection Interrupt Request Enable
#define ICIER_NAKIE    4  // NACK Reception Interrupt Request Enable
#define ICIER_RIE      5  // Receive Data Full Interrupt Request Enable
#define ICIER_TEIE     6  // Transmit End Interrupt Request Enable
#define ICIER_TIE      7  // Transmit Data Empty Interrupt Request Enable
// I2C Bus Status Register 1
#define IIC0_ICSR1 ((volatile unsigned char  *)(IICBASE + 0x3008))
#define IIC1_ICSR1 ((volatile unsigned char  *)(IICBASE + 0x3108)) // 0x00
#define ICSR1_AAS0     0  // Slave Address 0 Detection Flag
#define ICSR1_AAS1     1  // Slave Address 1 Detection Flag
#define ICSR1_AAS2     2  // Slave Address 2 Detection Flag
#define ICSR1_GCA      3  // General Call Address Detection Flag
#define ICSR1_DID      5  // Device ID Address Detection Flag
#define ICSR1_HOA      7  // Host Address Detection Flag
// I2C Bus Status Register 2
#define IIC0_ICSR2 ((volatile unsigned char  *)(IICBASE + 0x3009))
#define IIC1_ICSR2 ((volatile unsigned char  *)(IICBASE + 0x3109)) // 0x00
#define ICSR2_TMOF     0  // Timeout Detection Flag
#define ICSR2_AL       1  // Arbitration-Lost Flag
#define ICSR2_START    2  // Start Condition Detection Flag
#define ICSR2_STOP     3  // Stop Condition Detection Flag
#define ICSR2_NACKF    4  // NACK Detection Flag
#define ICSR2_RDRF     5  // Receive Data Full Flag
#define ICSR2_TEND     6  // Transmit End Flag
#define ICSR2_TDRE     7  // Transmit Data Empty Flag
// ...
// I2C Bus Bit Rate Low-Level Register
#define IIC0_ICBRL ((volatile unsigned char  *)(IICBASE + 0x3010))
#define IIC1_ICBRL ((volatile unsigned char  *)(IICBASE + 0x3110)) // 0xFB
#define ICBRL_4_0  0  // Bit Rate Low-Level Period - 5-bits
// I2C Bus Bit Rate High-Level Register
#define IIC0_ICBRH ((volatile unsigned char  *)(IICBASE + 0x3011))
#define IIC1_ICBRH ((volatile unsigned char  *)(IICBASE + 0x3111)) // 0xFA
#define ICBRH_4_0  0  // Bit Rate High-Level Period
// ...
// I2C Bus Transmit Data Register
#define IIC0_ICDRT ((volatile unsigned char  *)(IICBASE + 0x3012))
#define IIC1_ICDRT ((volatile unsigned char  *)(IICBASE + 0x3112))
// I2C Bus Receive Data Register
#define IIC0_ICDRR ((volatile unsigned char  *)(IICBASE + 0x3013))
#define IIC1_ICDRR ((volatile unsigned char  *)(IICBASE + 0x3113))

// Slave Address Register L0-2
#define IIC0_SARL0 ((volatile unsigned char  *)(IICBASE + 0x300A))
#define IIC1_SARL0 ((volatile unsigned char  *)(IICBASE + 0x310A))
#define IIC0_SARL1 ((volatile unsigned char  *)(IICBASE + 0x300C))
#define IIC1_SARL1 ((volatile unsigned char  *)(IICBASE + 0x310C))
#define IIC0_SARL2 ((volatile unsigned char  *)(IICBASE + 0x300E))
#define IIC1_SARL2 ((volatile unsigned char  *)(IICBASE + 0x310E))
#define SARL_SVA0    0  // 10-Bit Address LSB
#define SARL_SVA_6_0 1  // 7-Bit Address/10-Bit Address Lower Bits
// Slave Address Register U0-2
#define IIC0_SARU0 ((volatile unsigned char  *)(IICBASE + 0x300B))
#define IIC1_SARU0 ((volatile unsigned char  *)(IICBASE + 0x310B))
#define IIC0_SARU1 ((volatile unsigned char  *)(IICBASE + 0x300D))
#define IIC1_SARU1 ((volatile unsigned char  *)(IICBASE + 0x310D))
#define IIC0_SARU2 ((volatile unsigned char  *)(IICBASE + 0x300F))
#define IIC1_SARU2 ((volatile unsigned char  *)(IICBASE + 0x310F))
#define SARU_FS      0  // 7-Bit/10-Bit Address Format Select - 0: 7-bit
#define SARU_SVA_1_0 1  // 10-Bit Address Upper Bits

// I2C Bus Wakeup Unit Register
#define IIC0_ICWUR  ((volatile unsigned char  *)(IICBASE + 0x3016))
#define ICWUR_WUAFA     0  // Wakeup Analog Filter Additional Selection
#define ICWUR_WUACK     4  // ACK bit for Wakeup Mode
#define ICWUR_WUF       5  // Wakeup Event Occurrence Flag
#define ICWUR_WUIE      6  // Wakeup Interrupt Request Enable
#define ICWUR_WUE       7  // Wakeup Function Enable
// I2C Bus Wakeup Unit Register
#define IIC0_ICWUR2 ((volatile unsigned char  *)(IICBASE + 0x3017))
#define ICWUR2_WUSEN    0  // Wake-up Function Synchronous Enable
#define ICWUR2_WUASYF   1  // Wake-up Function Asynchronous Operation Status Flag
#define ICWUR2_WUSFY    2  // Wake-up Function Synchronous Operation Status Flag


// ==== 31. Serial Peripheral Interface (SPI) ====
#define SPIBASE 0x40070000
// SPI Control Register
#define SPI0_SPCR    ((volatile unsigned char  *)(SPIBASE + 0x2000))
#define SPI1_SPCR    ((volatile unsigned char  *)(SPIBASE + 0x2100))
#define SPCR_SPMS   0  // SPI Mode Select
#define SPCR_TXMD   1  // Communications Operating Mode
#define SPCR_MODFEN 2  // Mode Fault Error Detection Enable
#define SPCR_MSTR   3  // SPI Master/Slave Mode Select
#define SPCR_SPEIE  4  // SPI Error Interrupt Enable
#define SPCR_SPTIE  5  // Transmit Buffer Empty Interrupt Enable
#define SPCR_SPE    6  // SPI Function Enable
#define SPCR_SPRIE  7  // SPI Receive Buffer Full Interrupt Enable
// SPI Slave Select Polarity Register
#define SPI0_SSLP    ((volatile unsigned char  *)(SPIBASE + 0x2001))
#define SPI1_SSLP    ((volatile unsigned char  *)(SPIBASE + 0x2101))
#define SSLP_SSL0P  0  // SSL0 Signal Polarity Setting
#define SSLP_SSL1P  1  // SSL1 Signal Polarity Setting
#define SSLP_SSL2P  2  // SSL2 Signal Polarity Setting
#define SSLP_SSL3P  3  // SSL3 Signal Polarity Setting
// SPI Pin Control Register
#define SPI0_SPPCR   ((volatile unsigned char  *)(SPIBASE + 0x2002))
#define SPI1_SPPCR   ((volatile unsigned char  *)(SPIBASE + 0x2102))
#define SPPCR_SPLP   0  // SPI Loopback - data inverted
#define SPPCR_SPLP2  1  // SPI Loopback 2 - data not inverted
#define SPPCR_MOIFV  4  // MOSI Idle Fixed Value
#define SPPCR_MOIFE  5  // MOSI Idle Value Fixing Enable
// SPI Status Register
#define SPI0_SPSR    ((volatile unsigned char  *)(SPIBASE + 0x2003))
#define SPI1_SPSR    ((volatile unsigned char  *)(SPIBASE + 0x2103))
#define SPSR_OVRF   0  // Overrun Error Flag
#define SPSR_IDLNF  1  // SPI Idle Flag
#define SPSR_MODF   2  // Mode Fault Error Flag
#define SPSR_PERF   3  // Parity Error Flag
#define SPSR_UDRF   4  // Underrun Error Flag
#define SPSR_SPTEF  5  // SPI Transmit Buffer Empty Flag
#define SPSR_SPRF   7  // SPI Receive Buffer Full Flag
// SPI Data Register
#define SPI0_SPDR    ((volatile unsigned int   *)(SPIBASE + 0x2004))
#define SPI1_SPDR    ((volatile unsigned int   *)(SPIBASE + 0x2104))
#define SPI0_SPDR_HA ((volatile unsigned short *)(SPIBASE + 0x2004))
#define SPI1_SPDR_HA ((volatile unsigned short *)(SPIBASE + 0x2104))
// SPI Bit Rate Register
#define SPI0_SPBR    ((volatile unsigned char  *)(SPIBASE + 0x200A))
#define SPI1_SPBR    ((volatile unsigned char  *)(SPIBASE + 0x210A))
#define SPBR_0_7     0  // 0x00 = 24MHz; 0x01 = 12MHz; 0x02 = 8.0MHz; 0x03 = 6.0MHz; 0x04 = 4.8MHz; 0x05 = 4.0MHz
#define SPI_BR_24MHz  0x00
#define SPI_BR_12MHz  0x01
#define SPI_BR_8M0Hz  0x02
#define SPI_BR_6M0Hz  0x03
#define SPI_BR_4M8Hz  0x04
#define SPI_BR_4M0Hz  0x05
// SPI Data Control Register
#define SPI0_SPDCR   ((volatile unsigned char  *)(SPIBASE + 0x200B))
#define SPI1_SPDCR   ((volatile unsigned char  *)(SPIBASE + 0x210B))
#define SPDCR_SPRDTD 4  // SPI Receive/Transmit Data Select
#define SPDCR_SPLW   5  // SPI Word Access/Halfword Access Specification
// SPI Clock Delay Register
#define SPI0_SPCKD   ((volatile unsigned char  *)(SPIBASE + 0x200C))
#define SPI1_SPCKD   ((volatile unsigned char  *)(SPIBASE + 0x210C))
// SPI Slave Select Negation Delay Register
#define SPI0_SSLND   ((volatile unsigned char  *)(SPIBASE + 0x200D))
#define SPI1_SSLND   ((volatile unsigned char  *)(SPIBASE + 0x210D))
// SPI Next-Access Delay Register
#define SPI0_SPND    ((volatile unsigned char  *)(SPIBASE + 0x200E))
#define SPI1_SPND    ((volatile unsigned char  *)(SPIBASE + 0x210E))
// SPI Control Register 2
#define SPI0_SPCR2   ((volatile unsigned char  *)(SPIBASE + 0x200F))
#define SPI1_SPCR2   ((volatile unsigned char  *)(SPIBASE + 0x210F))
#define SPCR2_SPPE   0  // Parity Enable
#define SPCR2_SPOE   1  // Parity Mode
#define SPCR2_SPIIE  2  // SPI Idle Interrupt Enable
#define SPCR2_PTE    3  // Parity Self-Testing
#define SPCR2_SCKASE 4  // RSPCK Auto-Stop Function Enable
// SPI Command Register 0
#define SPI0_SPCMD0  ((volatile unsigned short *)(SPIBASE + 0x2010))
#define SPI1_SPCMD0  ((volatile unsigned short *)(SPIBASE + 0x2110))
#define SPCMD0_CPHA      0  // RSPCK Phase Setting
#define SPCMD0_CPOL      1  // RSPCK Polarity Setting
#define SPCMD0_BRDV_0_1  2  // Bit Rate Division Setting
#define SPCMD0_SSLA_0_2  4  // SSL Signal Assertion Setting
#define SPCMD0_SPB_0_3   8  // SPI Data Length Setting
#define SPCMD0_LSBF     12  // SPI LSB First
#define SPCMD0_SPNDEN   13  // SPI Next-Access Delay Enable
#define SPCMD0_SLNDEN   14  // SSL Negation Delay Setting Enable
#define SPCMD0_SCKDEN   15  // RSPCK Delay Setting Enable
#define SPI_BITS_08 0b0100 // 8 bits
#define SPI_BITS_09 0b1000 // 9 bits
#define SPI_BITS_10 0b1001 // 10 bits
#define SPI_BITS_11 0b1010 // 11 bits
#define SPI_BITS_12 0b1011 // 12 bits
#define SPI_BITS_13 0b1100 // 13 bits
#define SPI_BITS_14 0b1101 // 14 bits
#define SPI_BITS_15 0b1110 // 15 bits
#define SPI_BITS_16 0b1111 // 16 bits
#define SPI_BITS_20 0b0000 // 20 bits
#define SPI_BITS_24 0b0001 // 24 bits
#define SPI_BITS_32 0b0010 // 32 bits.
#define SPI_MASTER 1
#define SPI_SLAVE  0
#define SPI_IRQEN  1
#define SPI_POLL   0
#define SPI1       1
#define SPI0       0


// Serial Sound Interface Enhanced (SSIE)
#define AUDIO_DATA_SIGNED  // I2S audio data is signed

#define SSIEBASE 0x40040000          // SSIE Base - Bit Clock output on D10
#define SSIE0_SSICR  ((volatile unsigned int *)(SSIEBASE + 0xE000))   // Control Register
#define SSICR_REN        0  // 1: Enable reception.
#define SSICR_TEN        1  // 1: Enable transmission.
#define SSICR_MUEN       3  // 1: Enable muting
#define SSICR_CKDV_3_0   4  // 1: Select Bit Clock Division Ratio
#define SSICR_DEL        8  // 0: Delay of 1 cycle of SSIBCK; 1: No delay
#define SSICR_PDTA       9  // Select Placement Data Alignment
#define SSICR_SDTA      10  // Select Serial Data Alignment
#define SSICR_SPDP      11  // Select Serial Padding Polarity
#define SSICR_LRCKP     12  // Select the Initial Value and Polarity of LR Clock/Frame
#define SSICR_BCKP      13  // Select Bit Clock Polarity
#define SSICR_MST       14  // Master Enable
#define SSICR_SWL_2_0   16  // Select System Word Length
#define SSICR_DWL_2_0   19  // Select Data Word Length
#define SSICR_IIEN      25  // Idle Mode Interrupt Output Enable
#define SSICR_ROIEN     26  // Receive Overflow Interrupt Output Enable
#define SSICR_RUIEN     27  // Receive Underflow Interrupt Output Enable
#define SSICR_TOIEN     28  // Transmit Overflow Interrupt Output Enable
#define SSICR_TUIEN     29  // transmit Underflow Interrupt Output Enable
#define SSICR_CKS       30  // Select Audio Clock for Master mode - 0: AUDIO_CLK, 1: GTIOC1A (GPT output).
#define SSIE0_SSISR  ((volatile unsigned int *)(SSIEBASE + 0xE004))   // Status Register (SSISR)
#define SSISR_IIRQ      25  // Idle Mode Status Flag
#define SSISR_ROIRQ     26  // Receive Overflow Error Status Flag
#define SSISR_RUIRQ     27  // Receive Underflow Error Status Flag
#define SSISR_TOIRQ     28  // Transmit Overflow Error Status Flag
#define SSISR_TUIRQ     29  // transmit Underflow Error Status Flag
#define SSIE0_SSIFCR ((volatile unsigned int *)(SSIEBASE + 0xE010))   // FIFO Control Register
#define SSIFCR_RFRST     0  // 1: Set a receive data FIFO reset condition.
#define SSIFCR_TFRST     1  // 1: Set a transmit data FIFO reset condition.
#define SSIFCR_RIE       2  // 1: Enable receive data full interrupts.
#define SSIFCR_TIE       3  // 1: Enable transmit data empty interrupts.
#define SSIFCR_AUCKE    31  // 1: Enable supply of AUDIO_MCK in Master Mode.
#define SSIE0_SSIFSR ((volatile unsigned int *)(SSIEBASE + 0xE014))   // FIFO Status Register
#define SSIFSR_RDF       0  // Receive Data Full Flag
#define SSIFSR_RDC_3_0   8  // Number of receive FIFO data indication flag
#define SSIFSR_TDE      16  // Transmit Data Empty Flag
#define SSIFSR_TDC_3_0  24  // Number of transmit FIFO data indication flag
#ifdef AUDIO_DATA_SIGNED
#define SSIE0_SSIFTDR    ((volatile signed int   *)(SSIEBASE + 0xE018))  // Transmit FIFO Data Register - 18 to 32 bit
#define SSIE0_SSIFTDR_HA ((volatile signed short *)(SSIEBASE + 0xE018))  // Transmit FIFO Data Register - 16 bit
#define SSIE0_SSIFTDR_BY ((volatile signed char  *)(SSIEBASE + 0xE018))  // Transmit FIFO Data Register - 8 bit
#define SSIE0_SSIFRDR    ((volatile signed int   *)(SSIEBASE + 0xE01C))  // Receive FIFO Data Register
#define SSIE0_SSIFRDR_HA ((volatile signed short *)(SSIEBASE + 0xE01C))  // Receive FIFO Data Register
#define SSIE0_SSIFRDR_BY ((volatile signed char  *)(SSIEBASE + 0xE01C))  // Receive FIFO Data Register
#else
#define SSIE0_SSIFTDR    ((volatile unsigned int   *)(SSIEBASE + 0xE018))  // Transmit FIFO Data Register - 18 to 32 bit
#define SSIE0_SSIFTDR_HA ((volatile unsigned short *)(SSIEBASE + 0xE018))  // Transmit FIFO Data Register - 16 bit
#define SSIE0_SSIFTDR_BY ((volatile unsigned char  *)(SSIEBASE + 0xE018))  // Transmit FIFO Data Register - 8 bit
#define SSIE0_SSIFRDR    ((volatile unsigned int   *)(SSIEBASE + 0xE01C))  // Receive FIFO Data Register
#define SSIE0_SSIFRDR_HA ((volatile unsigned short *)(SSIEBASE + 0xE01C))  // Receive FIFO Data Register
#define SSIE0_SSIFRDR_BY ((volatile unsigned char  *)(SSIEBASE + 0xE01C))  // Receive FIFO Data Register
#endif
#define SSIE0_SSITDMR    ((volatile unsigned int   *)(SSIEBASE + 0xE020))  // TDM Mode Register
#define SSITDMR_OMOD_1_0 0  // Audio Format Select; 00: I2S format, 10: Monaural format
#define SSITDMR_LRCONT   8  // 1: Enables LRCK/FS continuation
#define SSITDMR_BCKASTP  9  // 0: Always outputs BCK to the SSIBCK pin, 1: Automatically controls output
#define SSIE0_SSISCR  ((volatile unsigned int *)(SSIEBASE + 0xE024))   //  Status Control Register
#define SSISCR_RDFS_2_0  0  // RDF Setting Condition; 000: SSIFRDR has one... stage of data, to 111: SSIFRDR has eight
#define SSISCR_TDFS_2_0  8  // TDE Setting Condition; 000: SSIFTDR has one... stage of free space, to 111: SSIFTDR has eight


// 27. USB 2.0 Full-Speed Module (USBFS) - Partial fragment to check configs - and enable USB module to be turned off
#define USBFSBASE 0x40090000          // USBFS Base
#define USBFS_SYSCFG  ((volatile unsigned short *)(USBFSBASE + 0x0000))   // System Configuration Control Register
#define SYSCFG_USBE     0  // USBFS Operation Enable; 0: Disabled, 1: Enabled
#define SYSCFG_SCKE    10  // USB Clock Enable
#define USBFS_INTENB0 ((volatile unsigned short *)(USBFSBASE + 0x0030))   // Interrupt Enable Register 0
#define INTENB0_VBSE   15  // VBUS Interrupt Enable; 0: Interrupt output disabled, 1: ... enabled

