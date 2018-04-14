
// PIC16F15354 Configuration Bit Settings

#pragma config FEXTOSC = OFF    // External Oscillator mode selection bits (Oscillator not enabled)
#pragma config RSTOSC = HFINT32 // Power-up default value for COSC bits (HFINTOSC with OSCFRQ= 32 MHz and CDIV = 1:1)
#pragma config CLKOUTEN = OFF   // Clock Out Enable bit (CLKOUT function is disabled; i/o or oscillator function on OSC2)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (FSCM timer enabled)
#pragma config MCLRE = ON       // Master Clear Enable bit (MCLR pin is Master Clear function)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config LPBOREN = OFF    // Low-Power BOR enable bit (ULPBOR disabled)
#pragma config BOREN = OFF       // Brown-out reset enable bits (Brown-out Reset Enabled, SBOREN bit is ignored)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (VBOR) set to 1.9V on LF, and 2.45V on F Devices)
#pragma config ZCD = OFF        // Zero-cross detect disable (Zero-cross detect circuit is disabled at POR.)
#pragma config PPS1WAY = OFF     // Peripheral Pin Select one-way control (The PPSLOCK bit can be cleared and set only once in software)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable bit (Stack Overflow or Underflow will cause a reset)
#pragma config WDTCPS = WDTCPS_31// WDT Period Select bits (Divider ratio 1:0x10000; software control of WDTPS)
#pragma config WDTE = SWDTEN     // WDT operating mode (WDT enabled/disabled by SWDTEN bit in WDTCON0)
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)
#pragma config BBSIZE = BB512   //  (512 char boot block size) 
#pragma config BBEN = ON        //  (Boot Block enabled)
#pragma config SAFEN = OFF      //  (SAF disabled)
#pragma config WRTAPP = OFF     //  (Application Block not write protected)
#pragma config WRTB = OFF       //  (Boot Block not write protected)
#pragma config WRTC = OFF       //  (Configuration Register not write protected)
#pragma config WRTSAF = OFF     //  (SAF not write protected)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (High Voltage on MCLR/Vpp must be used for programming)
#pragma config CP = OFF         // UserNVM Program memory code protection bit (UserNVM code protection disabled)

#include <xc.h>
#include "util.h"
#include "main.h"
#include "i2c.h"
#include "exp.h"
#include "lcd.h"
#include "logo.h"
#include "font708.h"
#include "font813.h"
#include "screens.h"
#include "smot.h"

void main(void) {
  ANSELA = 0; // no analog inputs
  ANSELB = 0; // these &^%$&^ regs cause a lot of trouble
  ANSELC = 0; // they should not default to on and override everything else

  utilInit();
  i2cInit();  
  expInit();
  lcdInit();   
  initLogo();
  initFont708();
  initFont813();
  smotInit();

  uint8  swValues = expReadA();
  uint8  swValChanged = 0;
  uint8  swChgCount[8];
  
  lcdClrAll();
  scrDrawMenu(0, false);
  
  // main event loop
  while(1) {
    // get switch states (swValues) and change flags (swValChanged)
    uint8 swPinValues = expReadA();
    for(uint8 i = 0; i < 8; i++) {
      uint8 mask = (1 << i);
      if((mask & swAllBits) == 0) continue;
      uint8 newval = swPinValues & mask;
      if(newval == (swValues & mask)) 
        swChgCount[i] = 0;
      else if(++swChgCount[i] == 5) {
        swValues = (swValues & ~mask) | newval;
        swValChanged |= mask;
        LATC7 = ((swValues & swTopLft) != 0);
      }
    }
    
    uint8 switchesClosing = (swValChanged & ~swValues);
    if(switchesClosing & swTopLft) scrCursorUp();
    if(switchesClosing & swBotLft) scrCursorDown();
    swValChanged = 0;
  }
}
