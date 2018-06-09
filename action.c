/*
 TODO: home camera should cancel home hold
 */

#include "action.h"
#include "util.h"
#include "panel.h"
#include "lcd.h"
#include "logo.h"
#include "screens.h"
#include "options.h"
#include "bmotor.h"
#include "smot.h"
#include "lights.h"

#define cameraAction 200
#define menuAction   201

uint8 rockerActionTbl[10][5] = {
  {cameraAction,  lightsAction,focusAction,         zoomInAction,zoomOutAction},
  {menuAction,    cursorUpAction,cursorDownAction,  escMenuAction,okMenuAction},
  {pasteScreen,   syringeInAction,syringeOutAction, extrudeAction,retractAction},
  {pickScreen,    rotateFwdAction, rotateBakAction, pinchInAction,pinchOutAction},
  {inspectScreen, 0,syringeOutAction,               0,pinchOutAction},
  
  {calCamScrn,    zoomOutAction,zoomOutAction,      escMenuAction,saveAction},
  {calPasteScrn,  syringeInAction,syringeOutAction, escMenuAction,saveAction},
  {calRotScrn,    rotateFwdAction, rotateBakAction, escMenuAction,saveAction},
  {calPinchScrn,  pinchInAction,pinchOutAction,     escMenuAction,saveAction},
  {goHomeScrn,    abortAction,abortAction,          abortAction,abortAction},
};

uint16 logoStartTimeStamp;
int8 focusDir = 1;

void doAction(uint8 action, uint8 swIdx) {
  swIdx -= 2;
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
//      beep(1);
      initCursor();
      logoStartTimeStamp = timer();
      logoShowLogo();
      lightsOn();
      return;

    case pwrOffAction:
      lcdOff(); 
      bmotAllPwrOff();
      smotAllPwrOff();
      lightsOff();
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
      
    case lightsAction:  chgLights(); break;
    
    case focusAction:   
      startSmot(focusMotor, focusDir, 100, 65535);
      focusDir = -focusDir;
      actionOnSwUp[swIdx] = focusEndAction;
      break;
    case focusEndAction: stopSmot(focusMotor); break;
      
    case zoomInAction:   
      startBmot(zoomMotor, 1, true, 500, 65535);
      actionOnSwUp[swIdx] = zoomEndAction;
      break;
    case zoomOutAction:   
      startBmot(zoomMotor, 1, false, 500, 65535);
      actionOnSwUp[swIdx] = zoomEndAction;
      break;
    case zoomEndAction: stopBmot(zoomMotor); break;
    
    case syringeInAction:  beep(1); break;
    case syringeOutAction: beep(1); break;
    case extrudeAction:    beep(1); break;
    case retractAction:    beep(1); break;
    
    case rotateFwdAction: 
      startBmot(rotateMotor, 4, true, 200, 65535);
      startBmot(pinchMotor, 4, true, 200, 65535);
      actionOnSwUp[swIdx] = rotateEndAction;
      break;
    case rotateBakAction:
      startBmot(rotateMotor, 4, false, 200, 65535);
      startBmot(pinchMotor, 4, false, 200, 65535);
      actionOnSwUp[swIdx] = rotateEndAction;
      break;
    case rotateEndAction: 
      stopBmot(rotateMotor); 
      stopBmot(pinchMotor); 
      break;
    
    case pinchInAction: 
      startBmot(pinchMotor, 4, true, 200, 65535);
      actionOnSwUp[swIdx] = pinchEndAction;
      break;
    case pinchOutAction:
      startBmot(pinchMotor, 4, false, 200, 65535);
      actionOnSwUp[swIdx] = pinchEndAction;
      break;
    case pinchEndAction: 
      stopBmot(pinchMotor); 
      break;
  }
}

const uint8 screenByMenuAndLine[menuCnt][menuLineCnt] = {
  {pasteScreen,pickScreen,inspectScreen,settingsMenu},
  {calMenu,pasteSetMenu,pickSetMenu,turboSetMenu,btnSetMenu},
  {calCamScrn,calPasteScrn,calRotScrn,calPinchScrn,goHomeScrn},
};

bool  cameraMode;
bool  turboMode;
uint8 actionOnTurboStart;
uint8 actionOnTurboEnd;
uint8 actionOnHoldStart[6];
uint8 actionOnSwUp[rockerCount];

void handleHomeSwUpDwn(bool swUp) {
  if(!swUp) {  // switch down
    if(actionOnTurboStart) {
      turboMode = true;
      doAction(actionOnTurboStart, 0); 
      return;
    }
  } else { // switch up
    if(turboMode) {
      turboMode = false;
      if(actionOnTurboEnd) doAction(actionOnTurboEnd, 0); 
    }
    else if(!cameraMode) {
      switch (curScreen) {
        case mainMenu:      doAction(scrOfs+helpMenuScrn, 0);  break;
        case helpMenuScrn:  doAction(scrOfs+helpMenu2Scrn, 0); break;
        case helpMenu2Scrn: doAction(scrOfs+menuNavScrn, 0);   break;
        case menuNavScrn:   doAction(scrOfs+camCtrlScrn, 0);   break;
        case camCtrlScrn:   doAction(scrOfs+aboutScrn, 0);     break;
        default: doAction(scrOfs+mainMenu, 0);
      }
    }
    cameraMode = turboMode = false;
  }
}

void doRockerAction(uint8 actMode, uint8 swIdx) {
  for(int tblIdx=0; tblIdx < 5; tblIdx++) {
    if(rockerActionTbl[tblIdx][0] == actMode) {
       doAction(rockerActionTbl[tblIdx][swIdx-2+1], swIdx);  
       return;
    }
  }
}

volatile bool   swHoldWaiting[6];
volatile uint16 swDownTimestamp[6];

void handleSwUpDown(uint8 swIdx, bool swUp) {
  if(curScreen == pwrOffScrn && swIdx != swPwrIdx) return;
  
  if(!swUp) {                   // switch down
    swDownTimestamp[swIdx] = timer();
    swHoldWaiting[swIdx]   = true;
  } else {                      // switch up
    swHoldWaiting[swIdx] = false;
    if(actionOnSwUp[swIdx-2]) {
      doAction(actionOnSwUp[swIdx-2], 0);
      actionOnSwUp[swIdx-2] = 0;
    }
  }
  
  if(swIdx == swHomeIdx)        // home switch
    handleHomeSwUpDwn(swUp);
  
  else if(swIdx == swPwrIdx) {  // power switch
    if(swUp) doAction(pwrOffAction, 0);
    else     doAction(pwrOnAction, 0);
  } 
  else {                        // rocker switch
    if(swUp) {
      if(turboMode) {
        turboMode = false;
        if(actionOnTurboEnd) doAction(actionOnTurboEnd, 0);
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
    doAction(scrOfs + mainMenu, 0);
  
  else if(swHoldWaiting[swIdx] && 
          (timer() - swDownTimestamp[swIdx]) > optHoldTime) {
    swHoldWaiting[swIdx] = false;
//    if(swIdx == swHomeIdx) {
//      if (curScreen == mainMenu) doAction(scrOfs+menuHelp, 0);
//    } 
    if(actionOnHoldStart[swIdx]) doAction(actionOnHoldStart[swIdx], 0);
  }
}
