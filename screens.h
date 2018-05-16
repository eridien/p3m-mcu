#ifndef SCREENS_H
#define	SCREENS_H

#include "util.h"

extern uint8 cursor;

enum menus {
    mainMenu    = 0,
    menuHelp    = 1,
    menuHelp2   = 2,
    menuHelp3   = 3,
    pasteScreen = 4,
    menusCount
};

enum strings {
    blankStr = 0,
    mainMenuStr,
    pasteStr,
    pickStr,
    inspectStr,
    settingsStr,
    menuHelpStr,
    helpMenuStr,
    hm1Str,
    hm2Str,
    hm3Str,
    hm4Str,
    hm5Str,
    helpMenu2Str,
    hm6Str,
    hm7Str,
    hm8Str,
    hm9Str,
    hm10Str,
    hm11Str,
    hm12Str,
    hm13Str,
    pasteScreenStr,
    stringsCount
};

extern const char *string[stringsCount];
extern uint8 menuLines[menusCount][6];

void scrDrawMenu(uint8 menu, bool screenOnly, bool cursorOnly);
void scrRedrawMenu(uint8 menu);
void scrCursorUp();
void scrCursorDown(uint8 menuIdx);

#endif	/* SCREENS_H */

