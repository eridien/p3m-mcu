#ifndef SMOT_H
#define	SMOT_H


#include "main.h"
#include "util.h"

enum smotIdx {
  focusMotor,  
  syringeMotor,  
  extrudeMotor,
};

#define smotDirFwd +1
#define smotDirBwd -1

#define smoteTris TRISA
#define smotpTris TRISA
#define smotfTris TRISC

#define smoteLat LATA
#define smotpLat LATA
#define smotfLat LATC

struct smotStateStruct {
  uint16 count;       // pulse count, 65535 is infinity; 0 means idle
  bool   pwrOn;       // not tri-stated -> powered on
  int8   phase;       // rotor phase
  int8   dir;         // smotDirFwd or smotDirBwd
  uint8  rateCount;   // pulse period is (1984/rateCount) * 96 usecs
  uint8  intCtr;      // interrupt counter
};

extern volatile struct smotStateStruct smotState[3];

void smotInit();
void startSmot(uint8 motorsmotIdx, int dir, uint16 pps, uint16 count);
void stopSmot(uint8 motor);
void smotAllPwrOff();

void smotInt(uint8 motor);

#endif	/* SMOT_H */

// power is red

// -------- phases ----------
// schematic     3  2  1  0
// pin idx       0  1  2  3
// Color        Bl Pi Ye Or
//              {1, 1, 0, 0},
//              {0, 1, 1, 0},
//              {0, 0, 1, 1},
//              {1, 0, 0, 1}


