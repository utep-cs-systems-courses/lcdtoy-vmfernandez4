#ifndef switches_included
#define switches_included

#define SW0 BIT0           /* switch0 is p2.0 */
#define SW1 BIT1           /* switch1 is p2.1 */
#define SW2 BIT2
#define SW3 BIT3
#define SWITCHES (SW0 | SW1 | SW2 | SW3)       /* only 1 switch on this board */

void switch_init();
void switch_interrupt_handler();

extern char SW0_DOWN, SW1_DOWN, SW2_DOWN, SW3_DOWN, switch_state_changed; /* effectively boolean */

#endif //included
