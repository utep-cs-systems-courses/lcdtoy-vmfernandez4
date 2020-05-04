#ifndef buzzer_included
#define buzzer_included

void buzzer_init();
void buzzer_set_period(short cycles);

extern char jurrasic_enable, siren_enable, playing_note; 

// Notes
#define a 300
#define b 450
#define d 650
#define e 800
#define f 415
#define g 440

void buzzer_init();
void timer_delay(unsigned int milliSeconds);
void buzzer_set_period(short cycles);

void songOne();

#endif // included
