
#include <xc.h>
#include "util.h"
#include "smot.h"
#include "bmotor.h"
#include "lcd.h"
#include "panel.h"
#include "options.h"
#include "action.h"

volatile uint8 DBG;

volatile uint16 time;

void utilInit() {
  T0ASYNC             =  0;   // sync clock
  T016BIT             =  0;   // 8-bit counter
  T0CON1bits.T0CS     =  5;   // src clk is MFINTOSC (500 khz)
  T0CON1bits.T0CKPS   =  0;   // prescaler  is 1:1 ( 2 usecs)
  TMR0H               = 41;   // wraps at count 42 (84 usecs, 11.9 khz)
  TMR0IF              =  0;   // int flag
  T0EN                =  1;   // enable timer0
  TMR0IE              =  1;   // enable timer int
  PEIE                =  1;   // enable peripheral ints
  GIE                 =  1;   // enable all ints
}

void dly() {
    int x=1;
}

void dbg(bool high) {
  resetLAT = high;
}
void dbgCnt(uint8 count) {
  resetLAT = 0;
  dly();  dly(); dly(); dly();
  for (int i=0; i < count; i++) {
    resetLAT = 1;
    dly(); 
    resetLAT = 0;
  }
  dly();  dly(); dly(); dly();
}

// each tick is 1 ms
uint16 timer() {
  GIE = 0;
  uint16 timeTemp = time;
  GIE = 1;
  return timeTemp;
}

void delayMs(uint16 ms) {
  uint16 start = timer();
  while (timer() != (start + ms)) NOP();
}

volatile uint8 subTime;
volatile uint8 motorCtr = 0;

// global interrupt routine
// interrupts every 84 usecs (11.9 KHz))
void interrupt globalInt() {
  if(++subTime == 12) {
    subTime = 0;
    time++; // 1 ms
  }
  if(motorCtr < 3)
    smotInt(motorCtr);   // each motor called every 500 us
  else 
    bmotInt(motorCtr-3); // each motor called every 500 us
  if(++motorCtr == 6)
    motorCtr = 0;
  
  TMR0IF = 0; // only int source
}

uint16 getRomWord(uint16 addr) {
    NVMCON1bits.NVMREGS = 0;
    NVMADRL = addr & 0xff;
    NVMADRH = addr >> 8;
    NVMCON1bits.RD = 1;
    return  ((uint16) NVMDATH << 8) | NVMDATL;   
}

