
#include <xc.h>
#include "util.h"
#include "main.h"
#include "i2c.h"

#define CLK LATB1
#define SDA LATB2

void dly() {
    int x=1;
}

// bit bang a sequence to unlock slaves waiting for clock
void i2cHardReset() {
  int x;

  SSP2CON1bits.SSPEN = 0;
  CLK = 1;
  SDA = 1;
  ODCB1  = 1;
  ODCB2  = 1;
  TRISB1 = 0;
  TRISB2 = 0;

  CLK = 1;
  SDA = 0;  // start bit
  dly();
  CLK = 0;
  dly();
  SDA = 1;
  for(uint8 i=0; i<9; i++) {
      CLK = 1;
      dly();
      CLK = 0;
  }
  dly();
  SDA = 0;
  dly();
  CLK = 1;
  dly();
  SDA = 1;  // stop bit
}

void i2cInit() {    
  i2cHardReset();

  TRISB1 = 1;
  TRISB2 = 1;
  SSP2CLKPPS = 0x09;           // RB1
  SSP2DATPPS = 0x0A;           // RB2
  RB1PPS     = 0x17;           // SCL2
  RB2PPS     = 0x18;           // SDA2

  SSP2ADD    = 0x11;         // baud rate 400 khz
//    SSP2ADD    = 0x44;           // baud rate 100 khz
  SSP2STATbits.CKE   = 1;      // SMBus 2V input
  SSP2CON1bits.SSPM  = 0b1000; // I2C Master mode, clock=FOSC/(4*(SSPxADD+1))
  SSP2CON1bits.SSPEN = 1;      // Enables the serial port
  SSP2IF = 0;    
}

void waitSSP2IF() {
  while(!SSP2IF);
  SSP2IF = 0;
}

void i2cStartSending(uint8 chipAddr) {  
  SSP2CON2bits.SEN = 1;        // send start bit
  waitSSP2IF();
        
  SSP2BUF = (uint8) (chipAddr << 1);
  waitSSP2IF();
}
void i2cSendByte(uint8 byte) {
  SSP2BUF = byte;
  waitSSP2IF();
}

void i2cStopSending() {
  SSP2CON2bits.PEN = 1;        // send stop bit
  waitSSP2IF();
}

void i2cSendTwoBytes(uint8 chipAddr, uint8 byte1, uint8 byte2) {
  i2cStartSending(chipAddr);
  i2cSendByte(byte1);
  i2cSendByte(byte2);
  i2cStopSending();
}

uint8 i2cReadByte(uint8 chipAddr, uint8 regAddr) {
  SSP2CON2bits.SEN = 1;        // send start bit
  waitSSP2IF();

  SSP2BUF = (uint8) (chipAddr << 1);
  waitSSP2IF();

  SSP2BUF = regAddr;
  waitSSP2IF();

  SSP2CON2bits.RSEN = 1;        // send restart bit
  waitSSP2IF();

  SSP2BUF = (uint8) ((chipAddr << 1) | 1); // with read bit set
  waitSSP2IF();

  SSP2CON2bits.RCEN = 1;        // receive enable
  waitSSP2IF();

  uint8 byt = SSP2BUF;          // byte from slave

  SSP2CON2bits.PEN = 1;         // send stop bit
  waitSSP2IF();

  return byt;
}
