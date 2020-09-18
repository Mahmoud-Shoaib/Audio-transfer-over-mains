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

void SPI_Slave_Init();
void UART_TX_Init(void);
void UART_Write(uint8_t data);

uint8_t Data;

void main(void) {
    
    SPI_Slave_Init();
    UART_TX_Init();
    
    while(1){
    return;
    }
}

void UART_TX_Init(void)
{
  BRGH = 1; // Set For High-Speed Baud Rate
  SPBRG = 25; // Set The Baud Rate To Be 9600 bps
  //--[ Enable The Ascynchronous Serial Port ]--
  SYNC = 0;
  SPEN = 1;
  //--[ Set The RX-TX Pins to be in UART mode (not io) ]--
  TRISC6 = 1;  // As stated in the datasheet
  TRISC7 = 1;  // As stated in the datasheet
  TXEN = 1; // Enable UART Transmission
}

void UART_Write(uint8_t data)
{
  while(!TRMT);
  TXREG = data;
}

void SPI_Slave_Init()
{
  // Set Spi Mode To Slave + SS Enabled 
  SSPM0 = 0;
  SSPM1 = 0;
  SSPM2 = 1;
  SSPM3 = 0;
  // Enable The Synchronous Serial Port
  SSPEN = 1;
  // Configure The Clock Polarity & Phase (SPI Mode Num. 4)
  CKP = 1;
  CKE = 1;
  // Clear The SMP Bit
  SMP = 0;
  // Configure The IO Pins For SPI Master Mode
  TRISC5 = 0; // SDO -> Output
  TRISC4 = 1; // SDI -> Input
  TRISC3 = 1; // SCK -> Input
  PCFG3 = 0; // Set SS (RA5/AN4) To Be Digital IO
  PCFG2 = 1;
  PCFG1 = 0;
  PCFG0 = 0;
  TRISA5 = 1; // SS -> Input
  // Enable Interrupts. Comment Them To Disable interrupts(NOT Recommended)
  SSPIE = 1;
  PEIE = 1;
  GIE = 1;
}

void __interrupt() ISR(void)
{
  if(SSPIF)
  {
    Data = SSPBUF; // Read The Buffer
    SSPIF = 0; // Clear The Interrupt Flag
  }
  UART_Write(Data);
}