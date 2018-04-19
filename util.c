
#include <xc.h>
#include "util.h"

void utilInit() {
    LATC7  = 0;
    TRISC7 = 0; // fault line used as debug pin
    
    T0ASYNC             = 0;   // sync clock
    T016BIT             = 1;   // 16-bit counter
    T0CON1bits.T0CS     = 5;   // src clk is MFINTOSC (0.5 mhz)
    T0CON1bits.T0CKPS   = 6;   // prescaler  is 1:64 (8 khz, 128 usecs)
    T0EN   = 1;                // enable timer0
}

void dbg() {
    LATC7 = 1;
    volatile int x=1;
    LATC7 = 0;
}

void dbgToggle() {
    LATC7 = !LATC7;
}

uint16 timer() {
  uint8 low = TMR0L;
  return(TMR0H << 8) | low;
}

void delay128Usecs(uint16 ticks) {
  uint16 start = timer();
  while (timer() != (start + ticks));
}

void delayMs(uint16 ms) {
  uint16 start = timer();
  while (timer() != (start + (ms * 1000)/clkPeriod));
}

uint16 getRomWord(uint16 addr) {
    NVMCON1bits.NVMREGS = 0;
    NVMADRL = addr & 0xff;
    NVMADRH = addr >> 8;
    NVMCON1bits.RD = 1;
    return  ((uint16) NVMDATH << 8) | NVMDATL;   
}
