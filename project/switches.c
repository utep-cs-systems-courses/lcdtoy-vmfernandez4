#include <msp430.h>
#include "switches.h"
#include "buzzer.h"
#include "stateMachine.h"


char SW0_DOWN, SW1_DOWN, SW2_DOWN, SW3_DOWN, state, switch_state_changed, donut_enable;
    //section;

static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);  /* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES); /* if switch down, sense up */
  return p2val; 
}

void
switch_init()                   /* setup switches */
{
  P2REN |= SWITCHES;            /* enables resistors for switches */
  P2IE = SWITCHES;              /* enable interrupts from switches */
  P2OUT |= SWITCHES;            /* pull-u[s for switches */
  P2DIR &= ~SWITCHES;           /* set switches' bits for input */
  switch_update_interrupt_sense();
  //led_update(); 
}

//updates the enables according to the state set
void set_enables(char jurrasic, char donut)
{
  jurrasic_enable = jurrasic;
  donut_enable = donut;
}

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  SW0_DOWN = (p2val & SW0) ? 0 : 1; /* when SW0 is up */
  SW1_DOWN = (p2val & SW1) ? 0 : 1; /* when SW1 is up */
  SW2_DOWN = (p2val & SW2) ? 0 : 1; /* when SW2 is up */
  SW3_DOWN = (p2val & SW3) ? 0 : 1; /* when SW3 is up */
  switch_state_changed = 1;

  //below updates the staes according to which button are pressed
  if(SW0_DOWN){ state = 0; set_enables(0, 0);}
  if(SW1_DOWN){ state = 4; set_enables(0, 1);}
  if(SW2_DOWN){ state = 2; set_enables(1, 0);}
  if(SW3_DOWN){ state = 3; set_enables(0, 0);}
  if(!SW1_DOWN && donut_enable){state = 1;}
 }
