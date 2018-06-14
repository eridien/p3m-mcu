#ifndef EXPANDER_H
#define	EXPANDER_H

// moving fast between switches dosn't work  TODO

#include "util.h"   

#define swDebounceTime 16  // 2 ms @ 127.2 usecs/tick
#define beepMs 50

#define i2cPanelAddr 0x20

// reg addresses in expander chip
#define IODIR     0x00
#define GPINTEN   0x02  // int enable
#define expINTCON 0x04  // 1: compare to def val, 0: any change
#define INTF      0x07  // int flags, read-only
#define INTCAP    0x08  // pin values at time int event occured, read-only
#define GPIO      0x09
#define IOCON     0x0B  // when bank = 0

enum swIdx {
    swHomeIdx = 0,
    swPwrIdx,
    swBotRgtIdx,
    swBotLftIdx,
    swTopRgtIdx,
    swTopLftIdx,
    switchesCount
};

#define rockerCount 4
extern uint8 curSwitches;
extern uint8 swMask[switchesCount];
extern bool beeping;
extern uint16 beepTimestamp;

// switches
#define swHomeMask     0x01
#define swPwrMask      0x02
#define swBotRgtMask   0x04
#define swBotLftMask   0x08
#define swTopRgtMask   0x10
#define swTopLftMask   0x20
#define buzzMask       0x80

#define swAllSwMask  (swTopLftMask|swBotLftMask|swTopRgtMask|  \
                        swBotRgtMask|swHomeMask|swPwrMask)

void  panelInit();
void  switchChk();
void  beep();
void  stopBeep();

#endif	/* EXPANDER_H */

