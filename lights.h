#ifndef LIGHTS_H
#define	LIGHTS_H

// lights on power-up with switch off TODO

#include "util.h"

#define lgtsI2cAddr   0x41
#define lgtsConfigCmd 0x03
#define lgtsDataCmd   0x01

void lgtsInit();
void chgLights();

void lightsOn();
void lightsOff();

#endif	/* LIGHTS_H */

