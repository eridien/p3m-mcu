
#include <xc.h>

#include "util.h"
#include "panel.h"
#include "i2c.h"
#include "action.h"

uint8 swMask[switchesCount] = {
  swHomeMask,   swPwrMask, 
  swTopLftMask, swBotLftMask, 
  swTopRgtMask, swBotRgtMask
};

uint8 curSwitches;
uint8 debounceCount[switchesCount];


void panelInit() {
    // expander chip setup
    i2cSendTwoBytes(i2cPanelAddr, IOCON,  0x22); // not seq op, no slew, not open-drain, active-high
    i2cSendTwoBytes(i2cPanelAddr, GPIO, 0xff);           // start buzz off, high
    i2cSendTwoBytes(i2cPanelAddr, IODIR, swAllSwMask);   // all switches are input, buzz is output
    i2cSendTwoBytes(i2cPanelAddr, GPINTEN, 0);           // pin ints off
    
    curSwitches = swAllSwMask;
}

uint8 panelReadA() {
    return i2cReadByte(i2cPanelAddr, GPIO);
}

void panelWriteA(uint8 data) {
    i2cSendTwoBytes(i2cPanelAddr, GPIO, data);
}

uint8 panelSwPinValues() {
  return (i2cReadByte(i2cPanelAddr, INTCAP) & swAllSwMask); 
}

uint16 beepTimestamp;
bool beeping = false;

void beep() {
  beepTimestamp = timer();
  beeping = true;
  panelWriteA(~buzzMask);
}

void stopBeep() {
  panelWriteA(buzzMask);
  beeping = false;
}

void switchChk() {
  uint8 swPinValues = panelReadA();
  for(uint8 swIdx = 0; swIdx < switchesCount; swIdx++) {
    uint8 mask = swMask[swIdx];
    uint8 newval = swPinValues & mask;
    if(newval == (curSwitches & mask)) {
      debounceCount[swIdx] = 0;
    } else if(++debounceCount[swIdx] == 5) {
      curSwitches = (curSwitches & ~mask) | newval;
      handleSwUpDown(swIdx, (newval != 0));
    }
  }
  if((curScreen == pwrOffScrn) && (curSwitches & swPwrMask) == 0)
    doAction(pwrOnAction, 0);
}

