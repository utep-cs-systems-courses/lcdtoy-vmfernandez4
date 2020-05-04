#include <msp430.h>
#include "stateMachine.h"
#include "switches.h"
#include <lcdutils.h>
#include<lcddraw.h>
#include "renderings.h"

//the following defines the varialbes needed for the stateMachine
char state = 3, switch_state_change, delay, donut_enable = 0, toggle_animation = 1;

//the following displayes the instructions for when the program is first loaded into the msp430
void loadInstructions()
{
  drawInstructions();
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


