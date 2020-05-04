#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include <stdlib.h>
#include "buzzer.h"

int redrawScreen = 1; 
u_int bgColor =COLOR_BLACK;
 char str[2];
 static int sec =30; //int 

//Bounds rect
AbRectOutline fieldOutline = {
    abRectOutlineGetBounds, abRectOutlineCheck,
    {screenWidth/2-1,screenHeight/2-1},
};
//layerGetBounds needs this v
Layer fieldLayer ={
    (AbShape *) &fieldOutline,
    {screenWidth/2,screenHeight/2},
    {0,0}, {0,0},
    COLOR_RED,
    0
};



AbRect rect ={abRectGetBounds,abRectCheck,{45,3}};
AbRect door ={abRectGetBounds,abRectCheck,{6,10}};
//PLAYER
Layer layer9={
  (AbShape*)&circle3,
  {(screenWidth/2+45), (screenHeight/2+60)},
  {0,0},{0,0},
  COLOR_PURPLE,
  &fieldLayer,
};
//END gate ---------------------------------------
Layer layer5 ={
  (AbShape *)&door,
  {screenWidth/3+75, (screenHeight/2-60)},
  {0,0},{0,0},
  COLOR_GREEN,
  &layer9
};
//Maze blocks------------------------------------
Layer layer4 ={
  (AbShape *)&rect,
  {screenWidth/3, (screenHeight/2-75)},
  {0,0},{0,0},
  COLOR_RED,
  &layer5
};
Layer layer3 ={
  (AbShape *)&rect,
  {screenWidth/3+40, (screenHeight/2)-45},
  {0,0},{0,0},
  COLOR_RED,
  &layer4
};
Layer layer2 ={
  (AbShape *)&rect,
  {screenWidth/3, (screenHeight/2-10)},
  {0,0},{0,0},
  COLOR_RED,
  &layer3
};
Layer layer1 ={
  (AbShape *)&rect,
  {screenWidth/3+40, (screenHeight/2)+25},
  {0,0},{0,0},
  COLOR_RED,
  &layer2
};
Layer layer0 ={
  (AbShape *)&rect,
  {screenWidth/3, (screenHeight/2)+60},
  {0,0},{0,0},
  COLOR_RED,
  &layer1
};
//MOVE layer ---------------------------------------------------------------------------------------------------

typedef struct MovLayer_s{
  Layer *layer;
  Vec2 velocity;
  struct MovLayer_s *next;
} MovLayer;
//defining what layers are going to move and not move
MovLayer ml0 = { &layer9, {1,1}, 0 }; 
MovLayer ml1 ={&layer0,{0,0},0};
MovLayer ml2 ={&layer1,{0,0},0};
MovLayer ml3 ={&layer2,{0,0},0};
MovLayer ml4 ={&layer3,{0,0},0};
MovLayer ml5 ={&layer4,{0,0},0};
MovLayer ml6 ={&layer5,{0,0},0};
//-------------------------------------
void movLayerDraw(MovLayer *movLayers, Layer *layers)
{
  int row, col;
  MovLayer *movLayer;

  and_sr(~8);			/**< disable interrupts (GIE off) */
  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Layer *l = movLayer->layer;
    l->posLast = l->pos;
    l->pos = l->posNext;
  }
  or_sr(8);			/**< disable interrupts (GIE on) */


  for (movLayer = movLayers; movLayer; movLayer = movLayer->next) { /* for each moving layer */
    Region bounds;
    layerGetBounds(movLayer->layer, &bounds);
    lcd_setArea(bounds.topLeft.axes[0], bounds.topLeft.axes[1], 
		bounds.botRight.axes[0], bounds.botRight.axes[1]);
    for (row = bounds.topLeft.axes[1]; row <= bounds.botRight.axes[1]; row++) {
      for (col = bounds.topLeft.axes[0]; col <= bounds.botRight.axes[0]; col++) {
	Vec2 pixelPos = {col, row};
	u_int color = bgColor;
	Layer *probeLayer;
	for (probeLayer = layers; probeLayer; 
	     probeLayer = probeLayer->next) { /* probe all layers, in order */
	  if (abShapeCheck(probeLayer->abShape, &probeLayer->pos, &pixelPos)) {
	    color = probeLayer->color;
	    break; 
	  } /* if probe check */
	} // for checking all layers at col, row
	lcd_writeColor(color); 
      } // for col
    } // for row
  } // for moving layer being updated
}	
/* to move down -----------------------------
 * change velocity to + and change the axes to y */
void mlAdvanceDown(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
      if ((shapeBoundary.topLeft.axes[1] < fence->topLeft.axes[1]) ||
	  (shapeBoundary.botRight.axes[1] > fence->botRight.axes[1]) ) {
        int velocity= -ml->velocity.axes[1];
        newPos.axes[1] += (2*velocity);
      }	/**< if outside of fence */
      
      ml->layer->posNext = newPos;
    } /**< for  y axis */
  } /**< for ml */
  
/*to move up-----------------------------
 * change velocity to - and change the axes to y */
void mlAdvanceUp(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Sub(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
      if ((shapeBoundary.topLeft.axes[1] < fence->topLeft.axes[1]) ||
	  (shapeBoundary.botRight.axes[1] > fence->botRight.axes[1]) ) {
        int velocity= -ml->velocity.axes[1];
        newPos.axes[1] -= (2*velocity);
      }	/**< if outside of fence */
      ml->layer->posNext = newPos;
    } /**< for  y axis */
  } /**< for ml */
/*to move Right------------------------------
 * change velocity to + and the axes to x */
void mlAdvanceLeft(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Add(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
      if ((shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0]) ||
	  (shapeBoundary.botRight.axes[0] > fence->botRight.axes[0]) ) {
        int velocity= -ml->velocity.axes[0];
        newPos.axes[0] += (2*velocity);
      }	/**< if outside of fence */
      ml->layer->posNext = newPos;
    } /**< for  x axis */
} /**< for ml */

/*to move left-----------------------------
 * change velocity to - and the axes to x */
void mlAdvanceRight(MovLayer *ml, Region *fence)
{
  Vec2 newPos;
  u_char axis;
  Region shapeBoundary;
  for (; ml; ml = ml->next) {
    vec2Sub(&newPos, &ml->layer->posNext, &ml->velocity);
    abShapeGetBounds(ml->layer->abShape, &newPos, &shapeBoundary);
      if ((shapeBoundary.topLeft.axes[0] < fence->topLeft.axes[0]) ||
	  (shapeBoundary.botRight.axes[0] > fence->botRight.axes[0]) ) {
        int velocity= -ml->velocity.axes[0];
        newPos.axes[0] -= (2*velocity);
      }	/**< if outside of fence */
      ml->layer->posNext = newPos;
    } /**< for  x axis */
  } /**< for ml */

/* This collision detection is only for blocks on the left 
 * since I had to check different points */
char collisionDetection(MovLayer *maze, MovLayer *player){
    char collision =0;
    u_char axis;
    Region mazeBound;
    Region playerBound;
    for(; maze; maze=maze->next){
        abShapeGetBounds(maze->layer->abShape,&maze->layer->pos, &mazeBound);
        abShapeGetBounds(player->layer->abShape, &player->layer->pos, &playerBound);
        if (playerBound.topLeft.axes[0]<= mazeBound.botRight.axes[0] && (playerBound.topLeft.axes[1] <= mazeBound.botRight.axes[1] && playerBound.topLeft.axes[1]>=mazeBound.topLeft.axes[1])||
            (playerBound.botRight.axes[0] <= mazeBound.botRight.axes[0] && (playerBound.botRight.axes[1]<= mazeBound.botRight.axes[1] && playerBound.botRight.axes[1]>=mazeBound.topLeft.axes[1]))){
            collision = 1; 
        }
    }
    
    return collision;
}
/* This collision is to check the blocks to
 * the right including the winning gate */
char collisionDetection2(MovLayer *maze, MovLayer *player){
    char collision =0;
    u_char axis;
    Region mazeBound;
    Region playerBound;
    for(; maze; maze=maze->next){
        abShapeGetBounds(maze->layer->abShape,&maze->layer->pos, &mazeBound);
        abShapeGetBounds(player->layer->abShape, &player->layer->pos, &playerBound);
        Vec2 topR; 
        topR.axes[0] = playerBound.botRight.axes[0];
        topR.axes[1] = playerBound.topLeft.axes[1];
        if ((topR.axes[1]<=mazeBound.botRight.axes[1]&&topR.axes[1]>=mazeBound.topLeft.axes[1])&&(topR.axes[0]>=mazeBound.topLeft.axes[0])||((playerBound.botRight.axes[1]>=mazeBound.topLeft.axes[1]&&playerBound.botRight.axes[1]<=mazeBound.botRight.axes[1])&&playerBound.botRight.axes[0]>=mazeBound.topLeft.axes[0])){
                collision = 1; 
            }
    }
    return collision;
}
Region fieldFence;

void main(){
  configureClocks();
  lcd_init();
  shapeInit();
  clearScreen(COLOR_BLACK);
  layerInit(&layer0);
  layerInit(&layer1);
  layerInit(&layer2);
  layerInit(&layer3);
  layerInit(&layer4);
  layerInit(&layer5);
  layerInit(&layer9);
  layerDraw(&layer0);
  buzzer_init();
  p2sw_init(15);
  layerGetBounds(&fieldLayer, &fieldFence);
  or_sr(0x8);
  enableWDTInterrupts();
  while(1){
    while(!redrawScreen) {
        or_sr(0x10);
    }
    if(sec>=1){
    movLayerDraw(&ml0, &layer9);
    movLayerDraw(&ml1, &layer0);
    movLayerDraw(&ml2, &layer1);
    movLayerDraw(&ml3, &layer2);
    movLayerDraw(&ml4, &layer3);
    movLayerDraw(&ml5, &layer4);
    movLayerDraw(&ml6, &layer5);
    redrawScreen=0;
    }
    }
}
void wdt_c_handler(){
  static short count =0; 
 /* if they collide, change the sec to 0 to change to loosing screen */
    u_int switches = p2sw_read();
      if( collisionDetection(&ml1,&ml0)||
      collisionDetection2(&ml2,&ml0)||
      collisionDetection(&ml3,&ml0)||
      collisionDetection2(&ml4,&ml0)||
      collisionDetection(&ml5,&ml0)){
      sec=0;

 /* if the winning rectangle detects collision, move to winning screen */
    }if(collisionDetection2(&ml6,&ml0)){
      clearScreen(COLOR_GREEN);
      drawString5x7(10,50,"WINNER.", COLOR_BLACK,COLOR_GREEN);
    }
/* game over screen and the loosing melody starts */
   if(sec==0){
       buzzer_advance_frequency();
      clearScreen(COLOR_RED);
      drawString5x7(10,50,"GAME OVER :(", COLOR_BLACK,COLOR_RED);
      
      
    }
    //moving left
    if((switches&(1<<0))==0){
      ml0.velocity.axes[1]=0;
      ml0.velocity.axes[0]=1;
      mlAdvanceRight(&ml0,&fieldFence);
      redrawScreen = 1; 
    }
    //moving down
    if((switches&(1<<1))==0){
        ml0.velocity.axes[0]=0;
        ml0.velocity.axes[1]=1;
        mlAdvanceDown(&ml0,&fieldFence);
        redrawScreen = 1; 
    //moving up
    }if((switches&(1<<2))==0){
        ml0.velocity.axes[1]=1;
        ml0.velocity.axes[0]=0;
        mlAdvanceUp(&ml0,&fieldFence);
        redrawScreen = 1; 
    //moving right
    }if((switches&(1<<3))==0){
         ml0.velocity.axes[0]=1;
        ml0.velocity.axes[1]=0;
        mlAdvanceLeft(&ml0,&fieldFence);
        redrawScreen = 1; 
    }

//updates time using interrupts. (close to a sec)
  if(count==180){
     
    if(sec>=10){
        itoa(sec,str,10);
    drawString5x7(1,150,"TIMER",COLOR_GREEN,COLOR_BLACK);
    drawString5x7(40,150,str,COLOR_GREEN,COLOR_BLACK);
    redrawScreen =1;
    }
    else if(sec<=10){
    itoa(sec,str,10);
    drawString5x7(1,150,"TIMER",COLOR_GREEN,COLOR_BLACK);
    drawString5x7(40,150,"0",COLOR_GREEN,COLOR_BLACK);
    drawString5x7(46,150,str,COLOR_GREEN,COLOR_BLACK);
    redrawScreen =1;
    }
    sec--;
    //reset interrupts
    count = 0;
  }
  count++;
}

