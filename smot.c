
#include <xc.h>
#include "util.h"
#include "smot.h"
#include "bmotor.h"

// 0:extrudeMotor, 1:pasteMotor, 2:focusMotor, 
uint8 smotPortMask[3] = {0xf0, 0x0f, 0x0f};
uint8 smotPortValue[3][4] = { // motor, phase
    {0x30, 0x60, 0xc0, 0x90},
    {0x03, 0x06, 0x0c, 0x09},
    {0x03, 0x06, 0x0c, 0x09}
};

volatile struct smotStateStruct smotState[3];

void smotInit() {
  smotAllPwrOff();
  for(uint8 motor=0; motor < 3; motor++) {
    stopSmot(motor);
    smotState[motor].phase = 0;
  }        
}

void smotPwrOnOff(uint8 motor, bool on) {
  if (smotState[motor].pwrOn != on) {
    uint8 tris;
    switch (motor) {
      case extrudeMotor: tris = smoteTris; break;
      case syringeMotor:   tris = smotpTris; break;
      case focusMotor:   tris = smotfTris; break;
    }
    tris = (tris & ~smotPortMask[motor]);
    if(!on) tris = tris | smotPortMask[motor];
    switch (motor) {
      case extrudeMotor: smoteTris = tris; break;
      case syringeMotor:   smotpTris = tris; break;
      case focusMotor:   smotfTris = tris; break;
    }
    smotState[motor].pwrOn = on;
  }
}

void smotAllPwrOff(){
  for(uint8 motor=0; motor < 3; motor++) {
    smotState[motor].count = 0;
    smotPwrOnOff(motor, false);
  };
}

void startSmot(uint8 motor, int dir, uint16 pps, uint16 count) {
  smotState[motor].dir       = dir;
  smotState[motor].rateCount = 2000 / pps;  // pps = 10..200
  smotState[motor].intCtr    = 0;
  smotState[motor].count     = count;
  smotPwrOnOff(motor, true);
}
// 500 usecs  200 pps => 5ms

void stopSmot(uint8 motor) {
   smotState[motor].count = 0;
}

// small motor interrupt routine
void smotInt(uint8 motor) {
  volatile struct smotStateStruct *pState = &smotState[motor];

  if (pState->count && (++(pState->intCtr) == pState->rateCount)) {
    pState->intCtr = 0;
    if(pState->count < 65535) pState->count--;
    uint8 phase = smotState[motor].phase =
                 (smotState[motor].phase + pState->dir) & 3;
    uint8 latVal;
    switch (motor) {
      case extrudeMotor: latVal = smoteLat; break;
      case syringeMotor:   latVal = smotpLat; break;
      case focusMotor:   latVal = smotfLat; break;
    }
    latVal = (latVal & ~smotPortMask[motor]) | 
                        smotPortValue[motor][phase];
    switch (motor) {
      case extrudeMotor: smoteLat = latVal; break;
      case syringeMotor:   smotpLat = latVal; break;
      case focusMotor:   smotfLat = latVal; break;
    }
  }
}
