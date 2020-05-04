#include <msp430.h>
#include "stateMachine.h"
#include "switches.h"
#include <lcdutils.h>
#include<lcddraw.h>
#include "renderings.h"

//the following defines the varialbes needed for the stateMachine
char state = 3, switch_state_change, delay, donut_enable = 0, toggle_animation = 1;

//the following is the music score for the quick version of jurrasic park theme song
int music_score[54][2] =
  {
  {Bb5, Three}, {Bb5, Eigth}, {A5, Eigth}, {Bb5, Half}, {C6, Eigth}, {C6, Half}, {Eb6, Eigth},
  {Eb6, Three}, {D6, Eigth}, {Bb5, Eigth}, {C6, Half}, {A5, Eigth}, {F5, Quarter}, {D6, Eigth},
  {Bb5, Eigth}, {C6, Three}, {F6, Eigth}, {Bb5, Eigth}, {Eb6, Half}, {D6, Eigth}, {D6, Half},
  {C6, Eigth}, {C6, Full}, {Bb6, Eigth}, {A6, Eigth}, {Bb6, Quarter}, {F6, Quarter},
  {Eb6, Quarter}, {Bb6, Eigth}, {A6, Eigth}, {Bb6, Half}, {F6, Quarter}, {Bb5, Quarter},
  {Ab6, Full}, {Bb6, Eigth}, {A6, Eigth}, {Bb6, Quarter}, {F6, Eigth}, {E6, Eigth}, {F6, Quarter},
  {Eb6, Eigth},{D6, Eigth}, {Eb6, Quarter}, {C6, Eigth}, {Bb5, Eigth}, {C6, Quarter}, {Bb6, Eigth},
  {A6, Eigth}, {Bb6, Quarter}, {F6, Eigth}, {E6, Eigth}, {F6, Half}, {A6, Full}, {Bb6, Full}
  }; 

//the following displayes the instructions for when the program is first loaded into the msp430
void loadInstructions()
{
  drawInstructions();
}

//The following function instructs on how to play the specifies not and its delay (total time) 
void play_note()
{
  if(playing_note %3 == 0) //toggles the animation every third note
    animate();
  delay = music_score[playing_note][1];
  buzzer_set_period(music_score[playing_note][0]);
}

//this function animates the t-rex using two different position for the arms and mouth
void animate()
{
  if(toggle_animation) //switches the animation between the two stages
  {
    drawTriangle(53, 90, 3, 6, 1, 1, 3, COLOR_RED);
    drawTriangle(59, 81, 1, 6, 1, 1, 2, COLOR_BLACK);
    drawTriangle(72, 82, 1, 6, 1, 1, 3, COLOR_RED); 
  }
  else
  {
    drawTriangle(53, 90, 3, 6, 1, 1, 3, COLOR_BLACK);
    drawTriangle(59, 81, 1, 6, 1, 1, 2, COLOR_RED);
    drawTriangle(71, 81, 1, 6, 1, 1, 3, COLOR_BLACK); 
  }
  toggle_animation++;
  toggle_animation = toggle_animation % 2; //toggles animation between two states : 0 - inital and 1 
}

//the following function is the state machine which advances the program
void state_advance()
{
  delay = 100; 
  switch(state)
  {
  case 0: //call to draw the hourglass in renderings
    if(switch_state_changed)
    {
      buzzer_set_period(0);
      drawHourGlass();
      switch_state_changed = 0; 
    }
  break;
    
  case 1://calls to draw the donute
    if(switch_state_changed)
    {
      buzzer_set_period(0);
      clearScreen(COLOR_YELLOW);
      drawString5x7(23, 12, "Here is a donut", COLOR_BLACK, COLOR_YELLOW);
      drawDonut();
      switch_state_changed = 0;
    }
  break;
    
  case 2: //calls to activate the jurrasic themed section
    if(switch_state_changed)
    {
      clearScreen(COLOR_RED);
      drawJurrasicPark();
      switch_state_changed = 0;
    }
    play_note(); //playes the music note
    break;
    
  case 3: //calls to display the instructions
    if(switch_state_changed)
    {
      drawInstructions();
      buzzer_set_period(0);
      switch_state_changed = 0;
    }
  break;
  
  case 4: //calls to draw the eaten donut state when the button is held pressed down
    if(switch_state_changed)
    {
      buzzer_set_period(0);
      drawString5x7(55, 115, "NOM", COLOR_BLACK, COLOR_YELLOW);
      drawEatenDonut(60, 48, 12, 12, 3, COLOR_YELLOW);
    }
    break;
  }
}


