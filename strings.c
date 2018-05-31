
#include "strings.h"
#include "util.h"

// stringsrom is defined in font708.as
extern const uint16 stringsrom;

uint16 stringsromAddr;
 
void initStringsRom() {
  stringsromAddr = ((uint16) &stringsrom) & 0x7fff;
}

char glblRomString[MAX_ROMSTR_SIZE];
uint8 glblRomStringId = 255;

char *romStr(uint8 strId) {
  if(strId == glblRomStringId) return glblRomString;
  glblRomStringId = strId;
  
  uint16 romPtr = stringsromAddr + strRomOfs[strId];
  char *strptr = glblRomString;
  while(true) {
    uint16 word = getRomWord(romPtr++);
    
    char chr = word >> 7;
    if(chr == eol) {
      *strptr = 0;
      return glblRomString;
    }
    *strptr++ = chr;
    
    chr = word & 0x7f;
    if(chr == eol) {
      *strptr = 0;
      return glblRomString;
    }
    *strptr++ = chr;
  }
  return glblRomString;
}

