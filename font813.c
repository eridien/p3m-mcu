#include "util.h"
#include "lcd.h"

// font813 is defined in font813.as
extern const uint16 font0813;

uint16 font813Addr;

char font813chrExpand1[] = {'3'};

void initFont813() {
  font813Addr = (uint16) &font0813;
}

void font813Chr2pageBuf(uint8 ascii, int8 rowOfs) {
  uint8 wid = 8;
  for(uint8 i=0; i < sizeof(font813chrExpand1); i++)
      if (ascii == font813chrExpand1[i]) wid = 9;

  if(lcdPageBufIdx + wid > 128) return;

  uint16 romOfs = (uint16) (ascii - 32) * 8;
  for(uint8 i = 0; i < wid; i++) {
      uint16 word = getRomWord(font813Addr + romOfs++);
      if(rowOfs < 0) word >>= -rowOfs;
      if(rowOfs > 0) word <<=  rowOfs;
      lcdPageBuf[lcdPageBufIdx] |= (word & 0xff);
      lcdPageBufIdx++;
  }   
}

void font813WriteStr(uint8 page, int8 rowOfs, uint8 col, const char *str) {
    lcdWriteStr(813, page, rowOfs, col, str, false);
}