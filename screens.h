#ifndef SCREENS_H
#define	SCREENS_H

#include "util.h"
#include "screens.h"

#define menuLineCnt 5
#define splashScreenIdx 255
#define menuIdxStart     0
#define menuIdxEnd       7
#define screenIdxStart   7

#define pwrOffScrn 254
#define logoScrn   255

enum menusAndScreens {
  // menuIdxStart
  mainMenu,          
  settingsMenu,  
  calMenu,
  pasteSetMenu, 
  pickSetMenu,
  turboSetMenu,
  btnSetMenu,

  // screenIdxStart
  helpMenuScrn,          
  helpMenu2Scrn,         
  menuNavScrn,         
  camCtrlScrn,
  aboutScrn,

  pasteScreen,       
  pickScreen ,       
  inspectScreen, 
  
  calCamScrn,
  calPasteScrn,
  calRotScrn,
  calPinchScrn,
  goHomeScrn,
  
  screenIdxEnd
};

#define menuCnt      (  menuIdxEnd -   menuIdxStart)
#define screenIdxLen (screenIdxEnd - screenIdxStart)
#define screenCnt screenIdxEnd

extern       uint8 curScreen;
extern       uint8 curCursor;
extern       uint8 defCursByMenu[menuCnt];
extern       uint8 editingOption;
extern const uint8 parentMenu[menuCnt];
extern const uint8 menuSelScreen[menuCnt][5];

void initScreens();
void initCursor();
void drawScreen(bool cursorOnly);
bool scrCursorUp(bool oneOnly);
bool scrCursorDown(bool oneOnly);
void openOptionField(uint8 optCode);

#endif	/* SCREENS_H */

