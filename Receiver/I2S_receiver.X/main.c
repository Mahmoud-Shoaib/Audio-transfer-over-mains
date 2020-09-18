/*
 * File:   main.c
 * Author: Mahmoud_Shoaib
 *
 * 
 */


#include <xc.h>
#include "config.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <p32xxxx.h>
#define _XTAL_FREQ 16000000  //Set the frequency of the oscillator.

uint32_t buffer = 0;
uint8_t x = 0;

void main(void) {
    
    RPA0Rbits.RPA0R = 0000;   // Set RA0 as SS.
    RPA1Rbits.RPA1R = 0000;   // Set RA1 as SDI.
    RPB1Rbits.RPB1R = 0011;   // Set RB1 as SDO.
    RPB3Rbits.RPB3R = 0001;   // Set RB3 as TX.
    RPA4Rbits.RPA4R = 0010;   // Set RA4 as RX.
    
    SPI1CON2bits.AUDMOD = 0b00;  // I2S Mode.
    SPI1CON2bits.AUDMONO = 0;    // Data is Stereo.
    SPI1CON2bits.AUDEN = 1;      // Audio protocol enabled.
    SPI1CON2bits.IGNROV = 1;     // Ignore Receive Overflow bit.
    SPI1CON2bits.IGNTUR = 1;     // Ignore Transmit Underrun bit.
    SPI1CONbits.DISSDI = 0;      // SDI pin is controlled by the SPI module.
    SPI1CONbits.MSTEN = 1;       // Master mode.
    SPI1CONbits.CKP = 1;
    SPI1CONbits.CKE = 1;
    SPI1CONbits.MODE16 = 1;     // 24-bit Data, 32-bit FIFO.
    SPI1CONbits.MODE32 = 1;
    SPI1CONbits.ON = 1;          // SPI Peripheral is enabled.
    SPI1BRG = 30;
    SPI1CONSET = 0x00008000;
    
    
    U1MODEbits.ON = 1;          //  UARTx is enabled.
    U1MODEbits.UEN = 0b00;      //  UxTX and UxRX pins are enabled and used.
    U1MODEbits.BRGH = 1;        //  High-Speed mode.
    U1MODEbits.PDSEL = 0b00;    //  8-bit data, no parity.
    U1MODEbits.STSEL = 0;       //  1 Stop bit.
    U1STAbits.URXISEL = 0b10;   //  Interrupt flag bit is asserted while receive buffer is 3/4 or more full.
    U1STAbits.URXEN = 1;        //  UARTx Receiver is enabled.
    U1BRG = 103;
    
    while(1){
        return;
    }
}


void __attribute__((interrupt)) U1InterruptHandler() 
{
    if(U1STAbits.URXDA) {
        if(x == 0) {
            buffer = U1RXREG;
            x = 1;
        }
        else if(x == 1) {
            buffer = U1RXREG >> 8;
            x = 2;
        }
        else if(x == 2){
            buffer = U1RXREG >> 16;
            SPI1BUF = buffer;
            x = 0;
        }
    
    }
}