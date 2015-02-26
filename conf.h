/* 
 * File:   conf.h
 * Author: Bruno
 *
 * Created on 3 février 2015, 21:23
 */

#ifndef CONF_H
#define	CONF_H

/*
 * Boot Segment Write Protect
 * BWRP = WRPROTECT_ON	Boot segment is write-protected
 * BWRP = WRPROTECT_OFF	Boot Segment may be written
 */
#pragma config BWRP = WRPROTECT_OFF

/*
 * Boot Segment Program Flash Code Protection
 * BSS = LARGE_FLASH_HIGH	High Security, Large-sized Boot Flash
 * BSS = HIGH_LARGE_BOOT_CODE	High Security, Large-sized Boot Flash
 * BSS = MEDIUM_FLASH_HIGH	High Security, Medium-sized Boot Flash
 * BSS = HIGH_MEDIUM_BOOT_CODE	High Security, Medium-sized Boot Flash
 * BSS = SMALL_FLASH_HIGH	High Security, Small-sized Boot Flash
 * BSS = HIGH_SMALL_BOOT_CODE	High Security, Small-sized Boot Flash
 * BSS = LARGE_FLASH_STD	Standard Security, Large-sized Boot Flash
 * BSS = STRD_LARGE_BOOT_CODE	Standard Security, Large-sized Boot Flash
 * BSS = MEDIUM_FLASH_STD	Standard Security, Medium-sized Boot Flash
 * BSS = STRD_MEDIUM_BOOT_CODE	Standard Security, Medium-sized Boot Flash
 * BSS = SMALL_FLASH_STD	Standard Security, Small-sized Boot Flash
 * BSS = STRD_SMALL_BOOT_CODE	Standard Security, Small-sized Boot Flash
 * BSS = NO_FLASH	No Boot program Flash segment
 * BSS = NO_BOOT_CODE	No Boot program Flash segment
 */
#pragma config BSS = NO_BOOT_CODE

/*
 * Boot Segment RAM Protection
 * RBS = LARGE_RAM	Large-sized Boot RAM
 * RBS = LARGE_BOOT_RAM	Large-sized Boot RAM
 * RBS = MEDIUM_RAM	Medium-sized Boot RAM
 * RBS = MEDIUM_BOOT_RAM	Medium-sized Boot RAM
 * RBS = SMALL_RAM	Small-sized Boot RAM
 * RBS = SMALL_BOOT_RAM	Small-sized Boot RAM
 * RBS = NO_RAM         No Boot RAM
 * RBS = NO_BOOT_RAM	No Boot RAM
 */
#pragma config RBS = NO_BOOT_RAM

/*
 * Secure Segment Program Write Protect
 * SWRP = WRPROTECT_ON	Secure segment is write-protected
 * SWRP = WRPROTECT_OFF	Secure segment may be written
 */
#pragma config SWRP = WRPROTECT_OFF

/*
 * Secure Segment Program Flash Code Protection
 * SSS = LARGE_FLASH_HIGH	High Security, Large-sized Secure Flash
 * SSS = HIGH_LARGE_SEC_CODE	High Security, Large-sized Secure Flash
 * SSS = MEDIUM_FLASH_HIGH	High Security, Medium-sized Secure Flash
 * SSS = HIGH_MEDIUM_SEC_CODE	High Security, Medium-sized Secure Flash
 * SSS = SMALL_FLASH_HIGH	High Security, Small-sized Secure Flash
 * SSS = HIGH_SMALL_SEC_CODE	High Security, Small-sized Secure Flash
 * SSS = LARGE_FLASH_STD	Standard Security, Large-sized Secure Flash
 * SSS = STRD_LARGE_SEC_CODE	Standard Security, Large-sized Secure Flash
 * SSS = MEDIUM_FLASH_STD	Standard Security, Medium-sized Secure Flash
 * SSS = STRD_MEDIUM_SEC_CODE	Standard Security, Medium-sized Secure Flash
 * SSS = SMALL_FLASH_STD	Standard Security, Small-sized Secure Flash
 * SSS = STRD_SMALL_SEC_CODE	Standard Security, Small-sized Secure Flash
 * SSS = NO_FLASH	No Secure Segment
 * SSS = NO_SEC_CODE	No Secure Segment
 */
#pragma config SSS = NO_SEC_CODE

/*
 * Secure Segment Data RAM Protection
 * RSS = LARGE_RAM	Large-sized Secure RAM
 * RSS = LARGE_SEC_RAM	Large-sized Secure RAM
 * RSS = MEDIUM_RAM	Medium-sized Secure RAM
 * RSS = MEDIUM_SEC_RAM	Medium-sized Secure RAM
 * RSS = SMALL_RAM	Small-sized Secure RAM
 * RSS = SMALL_SEC_RAM	Small-sized Secure RAM
 * RSS = NO_RAM         No Secure RAM
 * RSS = NO_SEC_RAM	No Secure RAM
 */
#pragma config RSS = NO_SEC_RAM

/*
 * General Code Segment Write Protect
 * GWRP = ON	User program memory is write-protected
 * GWRP = OFF	User program memory is not write-protected
 */
#pragma config  GWRP = OFF

/*
 * General Segment Code Protection
 * GSS = HIGH	High Security Code Protection is Enabled
 * GSS = ON	Standard Security Code Protection is Enabled
 * GSS = OFF	User program memory is not code-protected
 */
#pragma config GSS = OFF

/*
 * Oscillator Mode
 * FNOSC = FRC          Internal Fast RC (FRC)
 * FNOSC = FRCPLL	Internal Fast RC (FRC) w/ PLL
 * FNOSC = PRI          Primary Oscillator (XT, HS, EC)
 * FNOSC = PRIPLL	Primary Oscillator (XT, HS, EC) w/ PLL
 * FNOSC = SOSC         Secondary Oscillator (SOSC)
 * FNOSC = LPRC         Low Power RC Oscillator (LPRC)
 * FNOSC = FRCDIV16	Internal Fast RC (FRC) divide by 16
 * FNOSC = LPRCDIVN	Internal Fast RC (FRC) with divide by N
 */
#pragma config FNOSC = PRIPLL

/*
 * Internal External Switch Over Mode
 * IESO = OFF	Start-up device with user-selected oscillator source
 * IESO = ON	Start-up device with FRC, then automatically switch to user-selected oscillator source when ready
 */
#pragma config IESO = OFF

/*
 * Primary Oscillator Source
 * POSCMD = EC	EC Oscillator Mode
 * POSCMD = XT	XT Oscillator Mode
 * POSCMD = HS	HS Oscillator Mode
 * POSCMD = NONE	Primary Oscillator Disabled
 */
#pragma config POSCMD = XT

/*
 * OSC2 Pin Function
 * OSCIOFNC = ON	OSC2 pin has digital I/O function
 * OSCIOFNC = OFF	OSC2 pin has clock out function
 */
#pragma config OSCIOFNC = OFF

/*
 * Peripheral Pin Select Configuration
 * IOL1WAY = OFF	Allow Multiple Re-configurations
 * IOL1WAY = ON         Allow Only One Re-configuration
 */
#pragma config IOL1WAY = OFF

/*
 * Clock Switching and Monitor
 * FCKSM = CSECME	Both Clock Switching and Fail-Safe Clock Monitor are enabled
 * FCKSM = CSECMD	Clock switching is enabled, Fail-Safe Clock Monitor is disabled
 * FCKSM = CSDCMD	Both Clock Switching and Fail-Safe Clock Monitor are disabled
 */
#pragma config FCKSM = CSDCMD

/*
 * Watchdog Timer Postscaler
 * WDTPOST = PS1	1 : 1
 * WDTPOST = PS2	1 : 2
 * WDTPOST = PS4	1 : 4
 * WDTPOST = PS8	1 : 8
 * WDTPOST = PS16	1 : 16
 * WDTPOST = PS32	1 : 32
 * WDTPOST = PS64	1 : 64
 * WDTPOST = PS128	1 : 128
 * WDTPOST = PS256	1 : 256
 * WDTPOST = PS512	1 : 512
 * WDTPOST = PS1024	1 : 1,024
 * WDTPOST = PS2048	1 : 2,048
 * WDTPOST = PS4096	1 : 4,096
 * WDTPOST = PS8192	1 : 8,192
 * WDTPOST = PS16384	1 : 16,384
 * WDTPOST = PS32768	1 : 32,768
 */
#pragma config WDTPOST = PS1

/*
 * WDT Prescaler
 * WDTPRE = PR32	1 : 32
 * WDTPRE = PR128	1 : 128
 */
#pragma config WDTPRE = PR32

/*
 * Watchdog Timer Window
 * WINDIS = ON	Watchdog Timer in Window mode
 * WINDIS = OFF	Watchdog Timer in Non-Window mode
 */
#pragma config WINDIS = OFF

/*
 * Watchdog Timer Enable
 * FWDTEN = OFF	Watchdog timer enabled/disabled by user software
 * FWDTEN = ON	Watchdog timer always enabled
 */
#pragma config FWDTEN = OFF

/*
 * POR Timer Value
 * FPWRT = PWR1         Disabled
 * FPWRT = PWR2         2ms
 * FPWRT = PWR4         4ms
 * FPWRT = PWR8         8ms
 * FPWRT = PWR16	16ms
 * FPWRT = PWR32	32ms
 * FPWRT = PWR64	64ms
 * FPWRT = PWR128	128ms
 */
#pragma config FPWRT = PWR1

/*
 * Alternate I2C pins
 * ALTI2C = ON	I2C mapped to ASDA1/ASCL1 pins
 * ALTI2C = OFF	I2C mapped to SDA1/SCL1 pins
 */
#pragma config ALTI2C = OFF

/*
 * Comm Channel Select
 * ICS = PGD3	Communicate on PGC3/EMUC3 and PGD3/EMUD3
 * ICS = PGD2	Communicate on PGC2/EMUC2 and PGD2/EMUD2
 * ICS = PGD1	Communicate on PGC1/EMUC1 and PGD1/EMUD1
 */
#pragma config ICS = PGD1

/*
 * JTAG Port Enable
 * JTAGEN = OFF	JTAG is Disabled
 * JTAGEN = ON	JTAG is Enabled
 */
#pragma config JTAGEN = OFF

#endif	/* CONF_H */

