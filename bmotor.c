
#include <xc.h>
#include "util.h"
#include "panel.h"
#include "bmotor.h"

void bmotorInit() {
  bmotAllPwrOff();
  // all big motor pins are outputs
  TRISB = TRISB & ~bmotPinsB;
  TRISC = TRISC & ~bmotPinsC;
}

volatile struct bmotStateStruct bmotState[3];

// count must be > 0,  65535 == forever
void startBmot(uint8 motor, uint8 ustep, bool fwdDir, uint16 pps, uint16 count) {
  bmotState[motor].ustep   = ustep << 3;
  bmotState[motor].fwdDir  = fwdDir;
  bmotState[motor].rateCount = 1984 / pps;  // pps = 10..1984
  bmotState[motor].intCtr = 0;
  bmotState[motor].count = count;
  resetLAT = 1; // reset off, all pwr on
}

void stopBmot(uint8 motor) {
   bmotState[motor].count = 0;
}

void bmotAllPwrOff() {
  for(int i=0; i<3; i++) bmotState[i].count = 0;
  resetLAT = 0; // power down
}

uint8 pulseDelay;

// big motor interrupt routine
void bmotInt(uint8 motor) {
  volatile struct bmotStateStruct *pState = &bmotState[motor];
  
  if (pState->count && (++pState->intCtr == pState->rateCount)) {
    pState->intCtr = 0;
    if(pState->count < 65535) pState->count--;
    LATB = (LATB & 0xe6) | pState->ustep | pState->fwdDir;
    pulseDelay = 2;
    switch(motor) {
      case 0: LATC5 = 0;  while(--pulseDelay > 0) NOP(); LATC5 = 1;  break;
      case 1: LATC6 = 0;  while(--pulseDelay > 0) NOP(); LATC6 = 1;  break;
      case 2: LATC7 = 0;  while(--pulseDelay > 0) NOP(); LATC7 = 1;  break;
    }
  }
}