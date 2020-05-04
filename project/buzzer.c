#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"


void timer_delay(unsigned int milliSeconds){ // Hence _delay_cycles won't work because of the CPU Clock (Example on LEDS blinking)
    unsigned int counter = 0;
    for (counter = 0; counter <= milliSeconds; counter++){
       __delay_cycles(2000);
    }
}

void buzzer_init(){
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
      timerAUpmode();		/* used to drive speaker */
      P2SEL2 &= ~(BIT6 | BIT7);
      P2SEL &= ~BIT7; 
      P2SEL |= BIT6;
      P2DIR = BIT6;		/* enable output to speaker (P2.6) */

}

void buzzer_set_period(short cycles)
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}

void songOne(){

    buzzer_init();
    for(int jj = 0; jj < 3; jj++){
      
      int tempo = 2500;
  
      led_tgg_hard(1,0);
      buzzer_set_period(a);
      timer_delay(tempo);

      led_tgg_hard(0,1);
      buzzer_set_period(b);
      timer_delay(tempo);

      led_tgg_hard(1,0);
      buzzer_set_period(c);
      timer_delay(tempo-100);

      led_tgg_hard(0,1);
      buzzer_set_period(d);
      timer_delay(3000);

      led_tgg_hard(0,1);
      buzzer_set_period(e);
      timer_delay(tempo+1000);


  
      led_tgg_hard(1,0);
      buzzer_set_period(a);
      timer_delay(tempo);

      led_tgg_hard(0,1);
      buzzer_set_period(b);
      timer_delay(tempo);

      led_tgg_hard(1,0);
      buzzer_set_period(c);
      timer_delay(tempo);

      led_tgg_hard(0,1);
      buzzer_set_period(d);
      timer_delay(3000);

      led_tgg_hard(0,1);
      buzzer_set_period(e);
      timer_delay(7000);


      

    }

      buzzer_set_period(0);

}