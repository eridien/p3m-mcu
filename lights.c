
#include "lights.h"
#include "util.h"
#include "i2c.h"


#define lgtsI2cAddr   0x41
#define lgtsDataLat   0x01
#define lgtsConfigCmd 0x03

uint8 curLightsVal;

void lgtsInit() {
  i2cSendTwoBytes(lgtsI2cAddr, lgtsConfigCmd, 0); // all outputs
  lightsOn();
}

void chgLights() {
  switch (curLightsVal) {
    case 0x00: curLightsVal = 0x0f; break;
    case 0x0f: curLightsVal = 0x01; break;
    case 0x08: curLightsVal = 0x00; break;
    default: curLightsVal <<= 1;
  }
  i2cSendTwoBytes(lgtsI2cAddr, lgtsDataCmd, ~curLightsVal);
}

void lightsOn() {
  curLightsVal = 0;
  chgLights();
}

void lightsOff() {
  curLightsVal = 0x08;
  chgLights();
}
