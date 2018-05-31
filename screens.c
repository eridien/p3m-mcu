
#include "screens.h"
#include "lcd.h"
#include "font708.h"
#include "font813.h"
#include "strings.h"
#include "options.h"
#include "panel.h"

uint8 menuLines[screenCnt][6] = {
   {mainMenuStr, 
    pasteStr,
    pickStr,
    inspectStr,
    settingsStr,
    menuHelpStr,
   },
    
   {settingsMenuStr,
    calibrateStr,
    pasteStr,
    rotateStr,
    turboStr,
    buttonStr,
   },
   
   {calMenuStr,
    camHomeStr,
    pasteHomeStr,
    rotHomeStr,
    pinchHomeStr,
    goHomeStr,
   },
   
   {pasteSetMenuStr,
    extClickStr,
    pasteClickOption,
    extHoldStr,
    pasteHoldOption,
   },
   
   {pickSetMenuStr,
    rotClickStr,		 
    rotClickOption,
    rotHoldStr,		 
    rotHoldOption,
   },

   {turboSetMenuStr,
    pasteTurboStr,	
    pasteTurboOption,
    rotateTurboStr,
    rotateTurboOption,
   },
    
   {btnSetMenuStr,
    holdDelayStr,
    holdDelayOption,
   },

   {helpMenuStr,
    hm1Str,
    hm2Str,
    hm3Str,
    hm4Str,
    pressSqrStr,
   },
   {helpMenu2Str, 
    hm5Str,
    hm6Str,
    hm7Str,
    hm8Str,
    pressSqrStr,
   },
   {menuNavStr, 
    mn1Str,
    mn2Str,
    mn3Str,
    mn4Str,
    pressSqrStr,
    },
   
   {camCtrlScrn,
    cc1Str,
    lightsStr,
    focusStr,
    zoomStr,      
    pressSqrStr,
    },

   {aboutP3mStr,
    ap1Str,      
    ap2Str,      
    ap3Str,      
    ap4Str,      
    pressSqrStr,
    },
    
    {pasteScreenStr, 
    posSyrStr,
    extRetStr,
    lightsStr,
    focusStr,
    zoomStr,
    },
    
   {pickScreenStr, 
    pickRotStr,
    pickPinchStr,
    lightsStr,
    focusStr,
    zoomStr,
    },
    
   {inspectScreenStr, 
    syringeOutStr,
    pinchOpenStr,
    lightsStr,
    focusStr,
    zoomStr,
    },
    
   {calCamScrnStr,
    ca1Str,	     
    ca2Str,	       
    noiseStr,	     
    cancelStr,	     
    saveStr,
    },

   {calPasteScrnStr,
    cp1Str,	       
    cp2Str,	       
    cp3Str,	       
    cancelStr,	   
    saveStr,
    },	     

   {calRotScrnStr,  
    cr1Str,	       
    cr2Str,	       
    cr3Str,	       
    cancelStr,	   
    saveStr,
    },	    
    
   {calPinchScrnStr,  
    ci1Str,	        
    ci2Str,	        
    noiseStr,	    
    cancelStr,	    
    saveStr,	    
    },	     

   {goHomeScrnStr,      
    gh1Str,	         
    gh2Str,	        
    gh3Str,	        
    gh4Str,	        
    abortStr,	    
    },	     
};

uint8 curCursor, lastCursor;
uint8 curScreen;

void initCursor() {
  curCursor = 1;
  lastCursor = 1;
  for(int i=0; i<menuCnt; i++)
    defCursByMenu[i] = 1;
}
void initScreens() {
  initCursor();
  curScreen = pwrOffScrn;
}

char *menuLine(uint8 line){
  uint8 strIdx = menuLines[curScreen][line];
  if(strIdx < firstOptionCode)
    return romStr(strIdx);
  else
    return optionStr(strIdx);
}

void drawScreen(bool cursorOnly) {
  if(!cursorOnly) {
    lcdClrPage(0);
    lcdClrPage(1);
    font813WriteStr(0,  0, 0, menuLine(0));
    font813WriteStr(1, -8, 0, menuLine(0));
  }
  if(curScreen < menuCnt) {
    if(!cursorOnly || curCursor == 1 || lastCursor == 1) {
      lcdClrPage(2);
      font708WriteStr(2,  0, 0, menuLine(1), curCursor == 1);
    }
    if(!cursorOnly || curCursor == 2 || lastCursor == 2) {
      lcdClrPage(3);
      font708WriteStr(3,  2, 0,  menuLine(2), curCursor == 2);
    }
    if(!cursorOnly || curCursor == 2 || lastCursor == 2
                   || curCursor == 3 || lastCursor == 3) {
      lcdClrPage(4);
      font708WriteStr(9, -6, 0, menuLine(2), curCursor == 2);
      font708WriteStr(4,  3, 0, menuLine(3), curCursor == 3);
    }
    if(!cursorOnly || curCursor == 3 || lastCursor == 3
                   || curCursor == 4 || lastCursor == 4) {
      lcdClrPage(5);
      font708WriteStr(9, -5, 0, menuLine(3), curCursor == 3);
      font708WriteStr(5,  6, 0, menuLine(4), curCursor == 4);
    }
    if(!cursorOnly || curCursor == 4 || lastCursor == 4) {
      lcdClrPage(6);
      font708WriteStr(6, -2, 0, menuLine(4), curCursor == 4);
    }
    if(!cursorOnly || curCursor == 5 || lastCursor == 5) {
      lcdClrPage(7);
      font708WriteStr(7,  0, 0, menuLine(5), curCursor == 5);
    }
  } else {
    font708WriteStr(2,  0, 0, menuLine(1), false);
    font708WriteStr(3,  2, 0, menuLine(2), false);
    font708WriteStr(9, -6, 0, menuLine(2), false);
    font708WriteStr(4,  3, 0, menuLine(3), false);
    font708WriteStr(9, -5, 0, menuLine(3), false);
    font708WriteStr(5,  6, 0, menuLine(4), false);
    font708WriteStr(6, -2, 0, menuLine(4), false);
    font708WriteStr(7,  0, 0, menuLine(5), false);
  }
}

const uint8 cursorLines[] = {
  4,  // mainMenu
  3,  // settingsMenu
  4,  // pasteSettingsMenu
};

const uint8 cursorDist[] = {
  1,  // mainMenu
  1,  // settingsMenu
  2,  // pasteSettingsMenu
};

const uint8 parentMenu[menuCnt] = {
  mainMenu,      // mainMenu
  mainMenu,      // settingsMenu
  settingsMenu,  // pasteSettingsMenu
};

const uint8 menuSelScreen[menuCnt][menuLineCnt] = {
  {pasteScreen, pickScreen, inspectScreen, settingsMenu}, // mainMenu
  {pasteSetMenuStr},                                      // settingsMenu
};

uint8 defCursByMenu[menuCnt];

bool scrCursorUp(bool oneOnly) {
  uint8 dist = (oneOnly ? 1 : cursorDist[curScreen]);
  if(curCursor > dist) {
    curCursor -= dist;
    drawScreen(true);
    lastCursor = curCursor;
    defCursByMenu[curScreen] = curCursor;
    return true;
  } else
    return false;
}

bool scrCursorDown(bool oneOnly) {
  uint8 dist = (oneOnly ? 1 : cursorDist[curScreen]);
  if(curCursor < cursorLines[curScreen] - dist + 1) {
    curCursor += dist;
    drawScreen(true);
    lastCursor = curCursor;
    defCursByMenu[curScreen] = curCursor;
    return true;
  } else
    return false;
}

uint8 editingOption = 0;

void openOptionField(uint8 optCode) {
  editingOption = optCode;
  scrCursorDown(true);
}