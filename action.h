#ifndef STATE_H
#define	STATE_H

#include "util.h"
#include "panel.h"
#include "screens.h"

#define logoMs 2000

enum actions {
  noOp,
  pwrOnAction,
  pwrOffAction,
  startLogoAction,
  cursorUpAction,
  cursorDownAction,
  settingsUpAction,
  settingsDownAction,
  escMenuAction,
  okMenuAction,
  lightsAction,
  focusAction,
  focusEndAction,
  zoomInAction,
  zoomOutAction,
  zoomEndAction,
  syringeInAction,
  syringeOutAction, 
  extrudeAction,
  retractAction,
  rotateFwdAction, 
  rotateBakAction, 
  rotateEndAction, 
  pinchInAction,
  pinchOutAction,
  pinchEndAction,
  saveAction,
  abortAction,
  
  actionCount  // go to screen actions start here
};
#define scrOfs actionCount

extern uint8 actionOnSwUp[rockerCount];
extern uint8 actionOnTurboStart;
extern uint8 actionOnTurboEnd;
extern uint8 actionOnHoldStart[6];

void timeoutChk(uint8 switchMask);
void handleSwUpDown(uint8 switchMask, bool swUp);
void doAction(uint8 action, uint8 swIdx);

#endif	/* STATE_H */

