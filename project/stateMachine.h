#ifndef stateMachine_included
#define stateMachine_included 

extern char state, delay, jaw_count, playing_note, donut_enable;
extern int s1c, s2c, s3c, s4c;

void state_advance();
void loadInstructions();

//extern char section; 

//notes for the jurrasic part theme song
#define Bb5 2145
#define A5  2273
#define C6  1911
#define Eb6 1607
#define F6  1432
#define F5  2863
#define D6  1703
#define Bb6 1073
#define A6  1136
#define Ab6 1204
#define E6  1517

//notes length
#define Eigth   25
#define Quarter 50
#define Half    100
#define Three   150
#define Full    200

#endif // included
