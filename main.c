/* 
 * File:   main.c
 * Author: Bruno
 *
 * Created on 3 février 2015, 21:14
 */

#define FCY 40000000UL

#include <xc.h>
//#include <libpic30.h>
//#include <string.h>

#include "bool.h"
#include "conf.h"
#include "display.h"
#include "vt100.h"

#define VSYNC PORTAbits.RA0
#define BUSY  PORTAbits.RA1

#define busy_assert()       BUSY = 0
#define busy_disassert()    BUSY = 1

// 1 cycle = 2 pixels
#define HSYNC_PERIOD    399 // 31.77 us / 400 (800) pixels
#define HSYNC_LENGTH    48  // 3.77 us / 48(96) pixels
#define PIXEL_START     49  // 72 (144=96+40+8) pixels (ajusted with an oscilloscope)
#define PIXEL_END       392  // 96 + 40 + 8 + 640 = 784 pixels

#define VSYNC_PERIOD    524 // 525 lines
#define VSYNC_LENGTH    2 // 2 lines
#define VIDEO_START      35 // 2 + 25 + 8 = 35 lines
#define VIDEO_END        515 // 2 + 25 + 8 + 480 = 515 lines

#define DMA_BUFFER_SIZE 40 + 1
char __attribute__((space(dma),noload)) dma_scanline_buffer[DMA_BUFFER_SIZE];
char __attribute__((space(dma),noload)) dma_trash_buffer;

volatile unsigned char data;
volatile unsigned char data_pending;

void __attribute__((interrupt(auto_psv))) _SPI2Interrupt(void) {
    busy_assert();
    IFS2bits.SPI2IF = 0;
    data = SPI2BUF;
    data_pending = TRUE;
}

void print(const char* string) {
    while (*string != 0) {
        display_put_char(*string);
        string++;
    }
}

int main(void) {
    // Init Clock 50.4 MHz
    CLKDIVbits.PLLPRE = 5 - 2; // Input/5
    PLLFBDbits.PLLDIV = 84 - 2; // 84
    CLKDIVbits.PLLPOST = 0b00; // Output/2

    // Init I/O
    AD1PCFGL = 0x1FFF;
    TRISAbits.TRISA0 = 0;
    TRISAbits.TRISA1 = 0;
    VSYNC = 0;

    // Init display
    display_reset();

    dma_scanline_buffer[DMA_BUFFER_SIZE-1] = 0x00;

    // Configure SPI (VGA - RGB)
    SPI1CON1bits.DISSDO = 0; // SDO1 pin is controlled by the module
    SPI1CON1bits.MODE16 = 0; // Communication is byte-wide (8 bits)
    SPI1CON1bits.MSTEN = 0; // Slave mode
    _RP12R = 0b00111; //  RP12 tied to SPI1 Data Output.
    _SCK1R = 10; // Input SCK1 on pin RP10.
    
    // Configure DMA0 in order to send pixels
    DMACS0 = 0;
    DMA0PAD = (unsigned int)&SPI1BUF;
    DMA0STA = __builtin_dmaoffset(dma_scanline_buffer);
    DMA0CNT = DMA_BUFFER_SIZE - 1;
    DMA0CONbits.SIZE = 1; // Byte
    DMA0CONbits.DIR = 1;  // Read from DMA RAM address, write to peripheral address
    DMA0CONbits.HALF = 1; // Initiate block transfer complete interrupt when half of the data has been moved
    DMA0CONbits.AMODE = 0b00; // Register Indirect with Post-Increment mode
    DMA0CONbits.MODE = 0b01; // One-Shot, Ping-Pong modes disabled
    DMA0REQbits.IRQSEL = 0b0001010; // SPI1 Transfer Done

    // Configure DMA1 to read nothing.....
    DMA1PAD = (unsigned int)&SPI1BUF;
    DMA1STA = __builtin_dmaoffset(&dma_trash_buffer);
    DMA1CNT = DMA_BUFFER_SIZE - 1;
    DMA1CONbits.SIZE = 1; // Byte
    DMA1CONbits.DIR = 0;  // Read from peripheral address, write to DMA RAM address
    DMA1CONbits.AMODE = 0b01; // Register Indirect without Post-Increment mode
    DMA1CONbits.MODE = 0b01;// One-Shot, Ping-Pong modes disabled
    DMA1REQbits.IRQSEL = 0b0001010; // SPI1 Transfer Done

    // Configure timer 2 to generate the pixel clock period (1 cycle = 2 pixels)
    T2CONbits.TCS = 0; // Select internal instruction cycle clock
    T2CONbits.TGATE = 0; // Disable Gated Timer mode
    T2CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR2 = 0;
    PR2 = 1;

    // Configure output compare module 2 to generate the pixel clock in using timer 2
    OC2CONbits.OCTSEL = 0;  // Timer2 is the clock source
    OC2CONbits.OCM = 0b101; // Continuous Pulse mode
    OC2R = 0;
    OC2RS = 1;
    _RP10R = 0b10011;  // Output OC2 (pixel clock) on pin RP10.

    // Configure timer 3 to generate HSYNC period
    _T3CKR = 10; // RP10 (pixel clock) is used as external clock
    T3CONbits.TCS = 1; // External clock from T3CK pin
    T3CONbits.TGATE = 0; // Disable Gated Timer mode
    T3CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR3 = 0;
    PR3 = HSYNC_PERIOD;

    // Configure output compare module 3 to generate HSYNC PWM in using timer 3
    OC3CONbits.OCTSEL = 1;  // Timer3 is the clock source
    OC3CONbits.OCM = 0b110; // PWM mode without fault protection
    OC3R = HSYNC_LENGTH;
    OC3RS = HSYNC_LENGTH; // OC2RS is loaded in OC2R on every timer period
    _RP11R = 0b10100;  // Output OC3 (HSYNC) on pin RP11.

    // Configure timer 4 to generate VSYNC
    _T4CKR = 11; // RP11 (HSYNC) is used as external clock
    T4CONbits.TCS = 1; // External clock from T4CK pin
    T4CONbits.TGATE = 0; // Disable Gated Timer mode
    T4CONbits.TCKPS = 0b00; // Select 1:1 Prescaler
    TMR4 = 0;
    PR4 = VSYNC_PERIOD;

    // Generate VSYNC (sofwtare emulated Output Compare Module)
    IPC2bits.T3IP = 6; // Set Timer3 Interrupt Priority Level
    IFS0bits.T3IF = 0; // Clear Timer3 Interrupt Flag
    IEC0bits.T3IE = 1; // Enable Timer3 interrupt

    // Configure output compare module 1 to start DMA transfert in using timer 3
    OC1CONbits.OCTSEL = 1;  // Timer3 is the clock source
    OC1CONbits.OCM = 0b011; // Toggle Mode
    IPC0bits.OC1IP = 6;
    IFS0bits.OC1IF = 0;
    IEC0bits.OC1IE = 1;
    OC1R = PIXEL_START;

    // Configure output compare module 4 to end DMA transfert in using timer 3
    OC4CONbits.OCTSEL = 1;  // Timer3 is the clock source
    OC4CONbits.OCM = 0b011; // Toggle Mode
    IPC6bits.OC4IP = 6;
    IFS1bits.OC4IF = 0;
    IEC1bits.OC4IE = 1;
    OC4R = PIXEL_END;

    // Configure SPI : SMP=0; CKE=1; CKP=0;
    SPI2CON1bits.MODE16 = 0; // Communication is byte-wide (8 bits)
    SPI2CON1bits.SMP = 0;
    SPI2CON1bits.CKE = 1;
    SPI2CON1bits.CKP = 0;
    SPI2CON1bits.MSTEN = 0; // Slave mode
    SPI2CON1bits.DISSDO = 0; // SDO2 pin is controlled by the module
    SPI2CON1bits.SSEN = 1; // SS2 pin used for Slave mode
    _SS2R = 9; // Input SS2 on pin RP9.
    _SCK2R = 8; // Input SCK2 on pin RP8.
    _SDI2R = 7; // Input SDI2 on pin RP7.
    _RP5R = 0b01010; //  RP5 tied to SPI2 Data Output.
    IPC8bits.SPI2IP = 5;
    IFS2bits.SPI2IF = 0;
    IEC2bits.SPI2IE = 1;

    data = 0;
    data_pending = FALSE;
    busy_disassert();
    SPI2STATbits.SPIEN = 1;
    
    // Start peripherals
    T4CONbits.TON = 1; // Start VSYNC
    T3CONbits.TON = 1; // Start HSYNC
    T2CONbits.TON = 1; // Start pixel clock

    while(1) {
        if (data_pending == TRUE) {
            if (data == ESCAPE) {
                vt100_buffer[vt100_buffer_count++] = data;
            }
            else if (vt100_buffer_count == 0) {
                display_put_char(data);
            }
            else {
                if (vt100_buffer_count < VT100_BUFFER_SIZE) {
                    vt100_buffer[vt100_buffer_count++] = data;
                    vt100_handler();
                }
                else {
                    print("VT100 BUFFER OVERFLOW !");
                    vt100_clear_buffer();
                }
           }
            data_pending = FALSE;
            busy_disassert();
        }
        display_vsync_cursor_blinking();
    }
}
