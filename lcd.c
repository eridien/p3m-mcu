
#include "util.h"
#include "lcd.h"
#include "i2c.h"
#include "logo.h"
#include "font708.h"
#include "font813.h"
#include <stdio.h>

uint8 lcdPageBuf[128];
uint8 lcdPageBufIdx;

void lcdSendCmdByte(uint8 cmd) {
    i2cSendByte(lcdCmdCtrl);
    i2cSendByte(cmd);
}

void lcdInit() {
  i2cStartSending(i2cLcdAddr);
    
	lcdSendCmdByte(0xae);//--turn off oled panel
    
	lcdSendCmdByte(0xd5);//--set display clock divide ratio/oscillator frequency
	lcdSendCmdByte(0x80);//--set divide ratio
    
	lcdSendCmdByte(0xa8);//--set multiplex ratio(1 to 64)
	lcdSendCmdByte(0x3f);//--1/64 duty
    
	lcdSendCmdByte(0xd3);//-set vertical offset
	lcdSendCmdByte(0x00);//-not offset
    
	lcdSendCmdByte(0x8d);//--set Charge Pump enable/disable
	lcdSendCmdByte(0x14);//--set(0x14) enable
    
	lcdSendCmdByte(0x40);//--set start line address to 0
    
	lcdSendCmdByte(0xa6);//--set not inverted black/white
  
	lcdSendCmdByte(0xC8);//--set not rotated immediately (0xc0 is rotated)
    
	lcdSendCmdByte(0xa4);//  Disable Entire Display On
    
	lcdSendCmdByte(0xa1);//--set segment re-map 128 to 0 (no horiz flip))
    
	lcdSendCmdByte(0xda);//--set com pins hardware configuration
	lcdSendCmdByte(0x12); // 0x12 for 1106?  0x02 for 1306? (causes interlace))
    
	lcdSendCmdByte(0x81);//--set contrast control register -- BRIGHTNESS
	lcdSendCmdByte(0xff);
    
	lcdSendCmdByte(0xd9);//--set pre-charge period
	lcdSendCmdByte(0xf1);
    
	lcdSendCmdByte(0xdb);//--set vcomh
	lcdSendCmdByte(0x40);
  
 	i2cStopSending();   
}

void lcdSendCmd(uint8 cmd) {
    i2cStartSending(i2cLcdAddr);
    lcdSendCmdByte(cmd);
    i2cStopSending();   
}

void lcdSendData(uint8 data) {
    i2cStartSending(i2cLcdAddr);
    i2cSendByte(lcdDataCtrl);
    i2cSendByte(data);
    i2cStopSending();   
}

void lcdClrPageBuf() {
    for(uint8 i=0; i < 128; i++)
        lcdPageBuf[i] = 0;
}

void lcdSendPageBuf() {
    i2cStartSending(i2cLcdAddr);
    i2cSendByte(lcdContDataCtrl);
    for(uint8 i=0; i < 128; i++) {
        i2cSendByte(lcdPageBuf[i]);
    }
    i2cStopSending();       
}

void lcdClrPage(uint8 page) {
    lcdSendCmd(0xb0 + page);
    lcdSendCmd(0x04); // col idx<3:0> -> 0
    lcdSendCmd(0x10); // col idx<7:4> -> 0
    i2cStartSending(i2cLcdAddr);
    i2cSendByte(lcdContDataCtrl);
    for(uint8 i=0; i < 132; i++)
      i2cSendByte(0);
    i2cStopSending();       
    lcdClrPageBuf();
}

void lcdClrAll() {
  for(uint8 page=0; page < 8; page++) lcdClrPage(page);
}

void lcdWriteStr(uint16 font, uint8 page, int8 rowOfs, uint8 col, 
                              const char *str, bool cursor) {
    lcdPageBufIdx = col;
    
    for(const char* p = str; *p; p++) {
        switch(font) {
            case 708: font708Chr2pageBuf(*p, rowOfs, cursor); break;           
            case 813: font813Chr2pageBuf(*p, rowOfs); break;
        }
    }
    if(page != 9) {
        lcdSendCmd(0xb0 + page);
        lcdSendCmd(0x04);
        lcdSendCmd(0x10); 
        lcdSendPageBuf();
        lcdClrPageBuf();
    }
}

void lcdOn() { 
  lcdInit();
  lcdSendCmd(0xaf); 
  
  lcdSendCmd(0xC8);//--set not rotated    (immediate)   (kludge for problem)
  lcdSendCmd(0xA1);//--set not horiz flip (before data) (kludge for problem)

}
void lcdOff() { 
    lcdSendCmd(0xae); 
}
