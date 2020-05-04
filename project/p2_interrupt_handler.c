#include <msp430.h>
#include "switches.h"

/* Switch on p1 (S2) */
void
__interrupt_vec(PORT2_VECTOR) Port_1(){
  if (P2IFG & SWITCHES){               /* did a button cause this interrupt */
    P2IFG &= ~SWITCHES;                /* clear peding sw interrupts */
    switch_interrupt_handler();  /* single handler for all switches */
  }
}
