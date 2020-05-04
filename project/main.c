#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>


#include "stateMachine.h"
#include "switches.h"
#include "buzzer.h"

#define GREEN_LED BIT6; //green led

//defines everything you need for the lcd toy
int redrawScreen = 1;


void main()
{

  P1DIR |= GREEN_LED; //turns on green led when cpu is on
  P1OUT |= GREEN_LED;

  //initialization
  configureClocks();
  lcd_init();
  buzzer_init();
  switch_init();
  loadInstructions();
  enableWDTInterrupts();
  or_sr(0x8); //enable interrupts
  
  for(;;)
  {
    while(!redrawScreen)
    { 
     P1OUT &= ~GREEN_LED; //turns off green led when cpu is off
      or_sr(0x10); //cpu off
    }
    P1OUT |= GREEN_LED; //turns on the green led when cpu is on
    redrawScreen = 0;
    state_advance(); //advacnes state in state machine
  }
}

void wdt_c_handler()
{

  P1OUT |= GREEN_LED; //turns on green led when cpu is on
  static char  blink = 0;

  blink++;
  P1OUT &= ~GREEN_LED;  //turns of the green led when cpu is off
}
