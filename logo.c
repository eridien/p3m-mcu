
#include <xc.h>
#include "util.h"
#include "logo.h"
#include "lcd.h"

// logotable is defined in logotable.asm
extern const uint16 logotable;

uint16 logoAddr;
uint16 logoIndex;
uint16 logoWord;

void initLogo() {
  logoAddr = (uint16) &logotable;
  logoIndex = 0;
}

uint8 getNext7bitsRunlen() {
    uint16 ofs = logoIndex >> 1;
    if((logoIndex & 1) == 0) {
        uint16 addr = logoAddr + ofs;
        NVMCON1bits.NVMREGS = 0;
        NVMADRL = addr & 0xff;
        NVMADRH = addr >> 8;
        NVMCON1bits.RD = 1;
        logoWord = ((uint16) NVMDATH << 8) | NVMDATL;   
        logoIndex++;
        return logoWord >> 7;
    } else {
        logoIndex++;
        return logoWord & 0x7f;
    }
}

uint16 getNextLogoRunlen() {
    uint16 totalRun = 0;
    uint8 run = getNext7bitsRunlen();
    while(run == 127) {
      totalRun += run;
      run = getNext7bitsRunlen();
    }
    totalRun += run;
    return totalRun;
}

uint8 page, word, wordIdx, pixel;

void addBitsToWord(uint16 len) {
    for(uint16 i = 0; i < len;  i++) {
        word >>= 1;
        word |= (pixel << 7);
        if(++wordIdx == 8) {
            lcdPageBuf[lcdPageBufIdx++] = word;
            word = wordIdx = 0;
            if(lcdPageBufIdx == 128) {
                lcdSendPageBuf();
                if(++page == 7) return;
                lcdPageBufIdx = 0;
                lcdClrPageBuf();
                lcdSendCmd(0xb0 + page);
                lcdSendCmd(0x04); // col idx<3:0> -> 0
                lcdSendCmd(0x10); // col idx<7:4> -> 0
            }
        }
    }
}

void logoShowLogo() {
    lcdClrAll();
    initLogo();
    lcdPageBufIdx = page = word = wordIdx = pixel = 0;
    lcdSendCmd(0xb0 + page);
    lcdSendCmd(0x04); // col (low  nibble) => 4
    lcdSendCmd(0x10); // col (high nibble) => 0
    while(page < 7) {
      uint16 len = getNextLogoRunlen();
      addBitsToWord(len);
      pixel = 1 - pixel;
    }
    lcdSendCmd(0xaf); 
}
