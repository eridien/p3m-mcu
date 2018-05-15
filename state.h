#ifndef STATE_H
#define	STATE_H

#include "util.h"
#include "exp-panel.h"

enum states {
  noStateChange = 0,
  pwrOffState, // 1
  splashState, // 2
  mainState,   // 3
  statesCount
};

extern uint8 nextState[statesCount][2][switchesCount];
        
void initState();
void stateSwitchChange(uint8 switchMask, bool swUp);
void stateEnter(uint8 state);

#endif	/* STATE_H */

