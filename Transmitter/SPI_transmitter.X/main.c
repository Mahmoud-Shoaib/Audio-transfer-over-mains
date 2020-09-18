/*
 * File:   main.c
 * Author: Mahmoud_Shoaib
 *
 * 
 */


#include <xc.h>
#include "config.h"          //Include the configuration file.
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16F877A.h>
#define _XTAL_FREQ 16000000  //Set the frequency of the oscillator.


void UART_RX_Init();
void SPI_Master_Init();
void SPI_Write(uint8_t Data);

uint8_t Data;

void main(void) {
    
    UART_RX_Init();
    SPI_Master_Init();
    
    while(1){
    return;
    }
}
void UART_RX_Init()
{
  BRGH = 1;    // Set For High-Speed Baud Rate
  SPBRG = 25;  // Set The Baud Rate To Be 9600 bps
  // Enable The Ascynchronous Serial Port
  SYNC = 0;
  SPEN = 1;
  // Set The RX-TX Pins to be in UART mode (not io)
  TRISC6 = 1;  // As stated in the datasheet
  TRISC7 = 1;  // As stated in the datasheet
  //--[ Enable UART Receiving Interrupts ]--
  RCIE = 1;    // UART Receving Interrupt Enable Bit
  PEIE = 1;    // Peripherals Interrupt Enable Bit
  GIE = 1;     // Global Interrupt Enable Bit
  //------------------
  CREN = 1;    // Enable Data Continous Reception
}


void SPI_Master_Init()
{
  // Set SPI Mode To Master + Set SCK Rate To Fosc/4
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 0;
  SSPM3 = 0;
  // Enable The Synchronous Serial Port
  SSPEN = 1;
  // Configure The Clock Polarity & Phase (SPI Mode Num. 4)
  CKP = 1;
  CKE = 1;
  // Configure The Sampling Time (Let's make it at middle)
  SMP = 0;
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 0; // SCK -> Output
  
}
void SPI_Write(uint8_t Data)
{
  SSPBUF = Data; // Transfer The Data To The Buffer Register
}


void __interrupt() ISR (void)
{
  if (RCIF == 1)
  {
    Data = RCREG;  // Read The Received Data Buffer
    RCIF = 0;             // Clear The Flag
  }
  SPI_Write(Data);
}
