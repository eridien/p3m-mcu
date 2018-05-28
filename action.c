
#include "action.h"
#include "util.h"
#include "panel.h"
#include "lcd.h"
#include "logo.h"
#include "screens.h"
#include "options.h"
#include "bmotor.h"
#include "smot.h"

uint16 logoStartTimeStamp;

void doAction(uint8 action) {
chkAction:
  if(action >= scrOfs) {
    uint8 scrnIdx = action - scrOfs;
    if(scrnIdx == curScreen) return;
    curScreen = scrnIdx;
    if(curScreen < menuCnt) 
      curCursor = defCursByMenu[curScreen];
    lcdClrAll();
    drawScreen(false);
    return;
  }
  switch (action) {
    case pwrOnAction:
      curScreen = logoScrn;
      beep(1);
      initCursor();
      logoStartTimeStamp = timer();
      logoShowLogo();
      return;

    case pwrOffAction:
      lcdOff(); 
      bmotAllPwrOff();
      smotAllPwrOff();
      curScreen = pwrOffScrn;
      return;
      
    case cursorUpAction:   scrCursorUp(false);   return;
    case cursorDownAction: scrCursorDown(false); return;
    case escMenuAction: 
      action = scrOfs + parentMenu[curScreen]; 
      goto chkAction;
    case okMenuAction:
      action = scrOfs + menuSelScreen[curScreen][curCursor-1];
      goto chkAction;
      
    case lightsAction:  beep(1);
    case focusAction:   beep(1);
    case zoomInAction:  beep(1);
    case zoomOutAction: beep(1);
  
    case syringeInAction:  beep(1);
    case syringeOutAction: beep(1);
    case extrudeAction:    beep(1);
    case retractAction:    beep(1); break;
  }
}

const uint8 screenByMenuAndLine[menuCnt][menuLineCnt] = {
  {pasteScreen,pickScreen,inspectScreen,settingsMenu},
  {pasteSettingsMenu},  
};

bool  cameraMode;
bool  turboMode;
uint8 actionOnTurboStart;
uint8 actionOnTurboEnd;
uint8 actionOnHoldStart;

void handleHomeSwUpDwn(bool swUp) {
  if(!swUp) {  // switch down
    if(actionOnTurboStart) {
      turboMode = true;
      doAction(actionOnTurboStart); 
      return;
    }
    switch (curScreen) {
      case menuHelp:  doAction(scrOfs+menuHelp2); break;
      case menuHelp2: doAction(scrOfs+menuHelp3); break;
      case menuHelp3: doAction(scrOfs+mainMenu);  break;
    }
  } else { // switch up
    if(turboMode) {
      turboMode = false;
      if(actionOnTurboEnd) doAction(actionOnTurboEnd);
    }
    if(!cameraMode && curScreen != menuHelp  &&
                        curScreen != menuHelp2 &&
                        curScreen != menuHelp3) 
      doAction(scrOfs+mainMenu);
    
    cameraMode = turboMode = false;
  }
}

#define cameraAction 200
#define menuAction   201

uint8 actionTable[5][5] = {
  {cameraAction,   lightsAction,      focusAction,  zoomInAction, zoomOutAction},
  {menuAction,   cursorUpAction, cursorDownAction, escMenuAction,  okMenuAction},
  {pasteScreen, syringeInAction, syringeOutAction, extrudeAction, retractAction},
  {pickScreen,    0,0,0,0},
  {inspectScreen, 0,0,0,0},
};

void doRockerAction(uint8 actMode, uint8 swIdx) {
  for(int tblIdx=0; tblIdx < 5; tblIdx++) {
    if(actionTable[tblIdx][0] == actMode) {
       doAction(actionTable[tblIdx][swIdx-2+1]);  
       return;
    }
  }
}

volatile bool   swHoldWaiting[6];
volatile uint16 swDownTimestamp[6];

void handleSwUpDown(uint8 swIdx, bool swUp) {
  if(!swUp) {
    swDownTimestamp[swIdx] = timer();
    swHoldWaiting[swIdx]   = true;
  } else {
    swHoldWaiting[swIdx] = false;
  }
  
  if(swIdx == swHomeIdx)        // home switch
    handleHomeSwUpDwn(swUp);
  
  else if(swIdx == swPwrIdx) {  // power switch
    if(swUp) doAction(pwrOffAction);
    else     doAction(pwrOnAction);
  } 
  else {                        // rocker switch
    if(swUp) {
      if(turboMode) {
        turboMode = false;
        if(actionOnTurboEnd) doAction(actionOnTurboEnd);
      }
      return;
    }
    if(cameraMode = ((curSwitches & swHomeMask) == 0)) {
      doRockerAction(cameraAction, swIdx);
      return;
    } 
    if(curScreen < menuCnt) {  // menu
      doRockerAction(menuAction, swIdx); 
      return;
    }
    else { // paste, pick, or inspect screen
      doRockerAction(curScreen, swIdx); 
      return;
    }
  }
}

void timeoutChk(uint8 swIdx) {
  if((curScreen == logoScrn) && 
          (timer() - logoStartTimeStamp) > LOGO_DUR)
    doAction(scrOfs + mainMenu);
  
  else if(swHoldWaiting[swIdx] && 
          (timer() - swDownTimestamp[swIdx]) > optHoldTime) {
    swHoldWaiting[swIdx] = false;
    if(swIdx == swHomeIdx) {
      if (curScreen == mainMenu) doAction(scrOfs+menuHelp);
    } 
    if(actionOnHoldStart) doAction(actionOnHoldStart);
  }
}