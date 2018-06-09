#ifndef I2C_H
#define	I2C_H


#include "main.h"
        
void i2cInit();
void i2cStartSending(uint8 chipAddr);
void i2cSendByte(uint8 byte);
void i2cStopSending();
void i2cSendTwoBytes(uint8 chipAddr, uint8 byte1, uint8 byte2);
uint8 i2cReadByte(uint8 chipAddr, uint8 regAddr);

#endif	/* I2C_H */
