#ifndef BMOTOR_H
#define	BMOTOR_H

#define bmotPinsB 0x39   // set all motor pins to output
#define bmotPinsC 0xf0   // set to output
#define resetLAT  LATC4


enum bmotIdx {
  rotationMotor,  // rstep C5
  pinchMotor,     // tstep C6
  zoomMotor,      // sstep C7
};

extern volatile struct bmotStateStruct bmotState[3];

struct bmotStateStruct {
  uint16 count;    // pulse count, 65535 is infinity; 0 means idle
  uint8  ustep;    // 0 (1/2), 1 (1/8), 2 (1/32
  bool   fwdDir;   // true is fwd, false is reverse
  uint8  rateCount;// pulse period is (1984/rateCount) * 84 usecs
  uint8  intCtr;   // interrupt counter
};

void bmotorInit();
void startBmot(uint8 motor, uint8 ustep, bool fwdDir, uint16 pps, uint16 count);
void stopBmot(uint8 motor);
void bmotAllPwrOff();
void bmotInt(uint8 motor);

#endif	/* BMOTOR_H */

