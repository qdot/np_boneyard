//Large chunks of code taken from TONC
//http://user.chem.tue.nl/jakvijn/tonc/
#include "gba.h"
#include "gpioint.h"
#include "textdisp.h"
#include "btnstate.h"
#include "intcont.h"
#include "iinterrupt.h"
#include "simptimer.h"

#include "ExerciseEquipment.h"

//Color Conversion Functions
#define RGB16(r,g,b)  (((b)<<10)+((g)<<5)+(r))

//Size of the block for the snake to eat. 
#define BLOCK_SIZE 6

//The distance we go in pixels on every velocity tick
//This allows for speed scaling for low resolution encoders
#define DISTANCE_PER_TICK 4

//Main game class
//Turned into a class because I like the interrupt container
//class
class CNibbles : public IInterrupt
{
  bool processStep;
  int intCount;
  int p1count;
  bool processSpeed;

  //Mmm silly data structures that didn't make life much easier
  struct point
  {
    point() : x(0), y(0) {}
    point(int _x, int _y) : x(_x), y(_y) {}
    unsigned int x;
    unsigned int y;
  };


  //Draw a pixel on the screen
  void setPixel(point a, short color)
  {
    GBA_BASE_VRAM[a.x + (a.y * 240)] = color;
  }

  //Return the pixel color at the point
  int getPixel(point a)
  {
    return GBA_BASE_VRAM[a.x + (a.y * 240)];
  }

  //Setup the display for the game
  void runGameDisplaySetup()
  {
    point drawPoint;
    
    //Set up screen
    //Mode 3
    GBA_REG_DISPCNT |= (0x3 | (1<<10));
    GBA_REG_DISPCNT &= (0xffff & ~(1 << 7));
    
    drawPoint.x = 0;
    drawPoint.y = 10;
    
    //Clear screen
    for(int i = 0; i < 160; ++i)
      {
	for(int j = 0; j < 240; ++j)
	  {
	    drawPoint.x = j;
	    drawPoint.y = i;
	    setPixel(drawPoint, 0);
	  }
      }
    
    //Draw boundary box
    for(int i = 5; i < 235; ++i)
      {
	drawPoint.x = i;
	drawPoint.y = 10;
	setPixel(drawPoint, 0xffff);
	drawPoint.y = 150;
	setPixel(drawPoint, 0xffff);
      }
    for(int i = 10; i < 150; ++i)
      {
	drawPoint.x = 5;
	drawPoint.y = i;
	setPixel(drawPoint, 0xffff);
	drawPoint.x = 235; 
	setPixel(drawPoint, 0xffff);
      }
  }

  //Interrupt container/handler
  CInterruptCont m_intCont;
  //Digital Sensor Handler
  CGpioInt m_gpio;

  int velCount;
  //Members for random generation
  int m_rand_x, m_rand_c;
  //50Mhz clock implemented in the FPGA
  CSimpTimer m_timer;
  //Status for where the last block was drawn
  point m_blockCorner;
public:
  CNibbles() : m_gpio(&m_intCont), processStep(false), processSpeed(false), intCount(0), p1count(0), velCount(0), m_rand_x(0), m_rand_c(0)
  {
    //Turn on interrupts for sensor 0
    //This is where we have the bike speed sensor hooked up
    *m_gpio.m_intMask = 1;
    *m_gpio.m_intEdge = 1;
    //4 is the vector for the GBA Timer
    m_intCont.Register(this, 4);
    //21 is the vector for the GPIO interrupt
    m_intCont.Register(this, 21);
    m_intCont.Unmask(4);
    m_intCont.Unmask(21);
  }

  virtual void Interrupt(unsigned char vector)
  {
    //If the GBA Timer has looped (Happens at 100hz)
    //Should actually happen on VSYNC so we don't draw
    //in the middle of a refresh, but oh well.
    if(vector == 4)
      {
	++intCount;
	++velCount;
	if(intCount == 10)
	  {
	    //Update speed calculations @ 10hz
	    intCount = 0;
	    processSpeed = true;
	  }
	//Trigger drawing
	processStep = true;
      }
    if(vector == 21)
      {
	//Encoder count from the bike speed sensor
        ++p1count;
      }
  }

  ~CNibbles() {}

  // From:
  // http://remus.rutgers.edu/~rhoads/Code/mwc.c
  
  /* Choose a value for a from this list
     1791398085 1929682203 1683268614 1965537969 1675393560
     1967773755 1517746329 1447497129 1655692410 1606218150
     2051013963 1075433238 1557985959 1781943330 1893513180
     1631296680 2131995753 2083801278 1873196400 1554115554
  */

  // Returns 0 to 2^31-1
  int GenerateRandom() {
    const unsigned int a=1791398085;
    const unsigned int ah=a>>16;
    const unsigned int al=a&65535;
    
    unsigned int xh = m_rand_x>>16, xl = m_rand_x & 65535;
    
    unsigned long microseconds;
    m_timer.GetCount(&microseconds);
    
    m_rand_x = m_rand_x * a + m_rand_c + microseconds;
    m_rand_c = xh*ah + ((xh*al) >> 16) + ((xl*ah) >> 16);
    /* thanks to Don Mitchell for this correction */
    if (xl*al >= ~m_rand_c + 1) m_rand_c++;  
    return 0x7fffffff & m_rand_x;
  }

  int GenerateRandomInRange(int x) {
    return(GenerateRandom()%x);
  }
  void SeedRandom(int x) {
	m_rand_x = x;
  }

  //Finds a random place where it can draw a block 
  //(inside boundary, not through snake) and draws it.
  void drawBlock()
  {
    bool genFinished = false;
    int startX = 0, startY = 0;
    while(!genFinished)
      {
	startX = 0;
	startY = 0;
	while(startX < 10)
	  {
	    startX = GenerateRandomInRange(225);
	  }
	while(startY < 10)
	  {
	    startY = GenerateRandomInRange(145);
	  }
	genFinished = true;
	for(int i = 0; i < BLOCK_SIZE; ++i)
	  {
	    for(int j = 0; j < BLOCK_SIZE; ++j)
	      {
		if(getPixel(point(startX + i, startY + j)) > 0)
		  {
		    genFinished = false;
		    break;
		  }
	      }
	    if(!genFinished) break;
	  }
      }
    m_blockCorner = point(startX, startY);
    for(int i = 0; i < BLOCK_SIZE; ++i)
      {
	for(int j = 0; j < BLOCK_SIZE; ++j)
	  {
	    setPixel(point(startX+i, startY+j), RGB16(0, 31, 0));
	  }
      }
  }
  
  //If we've eaten the block, we call this to erase it from the screen
  void eraseBlock()
  {
    int startX = m_blockCorner.x;
    int startY = m_blockCorner.y;
    for(int i = 0; i < BLOCK_SIZE; ++i)
      {
	for(int j = 0; j < BLOCK_SIZE; ++j)
	  {
	    setPixel(point(startX+i, startY+j), 0);
	  }
      }
  }


  //Main game loop function
  int RunGame()
  {
    //Keeps the status of the GBA buttons
    CBtnState btnState;
    //Bike velocity calculator and history
    //100 is the number of time segments to keep in 
    //the history. Since we have .1s time segments, this
    //will keep a 10s history. We don't really need that 
    //much, but it's a just in case thing.
    CExerciseEquipment bike(100);
    int score = 0;
    int drawColor = 30;
    int collisionColor = 0;
    point player1point;
    bool gameOver = false;
    int state;

    //Set up the GBA timer to throw interrupts at 100hz. 
    int intPeriod = 262192/100;
    (intPeriod < 0xffff) ? GBA_REG_TM1D = 0xffff - intPeriod : GBA_REG_TM1D = 0;

    //timer on, 64 cpu ticks per clock tick, interrupt on, interrupt on overflow
    GBA_REG_TM1CNT = 0x00c1;

    //Seed the random number generator using whatever value the
    //timer is at right now. Not exactly a good seeding value since
    //it will occur at the same place every time we boot, but oh well.
    SeedRandom(GBA_REG_TM1D);    

    while(1)
      {
	//Create a new text display and history list on every
	//cycle of the game. Cheap way of not having to deal with
	//handling display contexts.
	CTextDisp textDisp;
	CDLCList<point> pointHistory;
	int pointHistoryLimit = 10;
	drawColor = 30;

	//Push the starting point into the list
	pointHistory.PushBack(point(120, 80));
	
	CDLLNode<point>* pointHistoryCursor = pointHistory.GetHeadNode();

	//Set up start/reset screen
	textDisp.SetupDisplay();
	textDisp.Clear();
	if(gameOver)
	  {
	    textDisp.printf("Game over\n");
	    textDisp.printf("Score: %d\n", score);
	  }
	gameOver = false;
	score = 0;
	textDisp.printf("Press Start\n");
	while(!btnState.KeyHit(CBtnState::START_BUTTON))
	  {
	    btnState.PollKeys();
	  }
	textDisp.Clear();
	runGameDisplaySetup();
	
	//Set 1/2 starting positions
	player1point.x = 120;
	player1point.y = 80;
	state = 0;

	//Draw the first block and start the game
	drawBlock();
	while(1)
	  {
	    //10hz Speed update
	    if(processSpeed)
	      {
		//Add the number of ticks in the last .1s
		bike.AddTickPeriod(p1count);
		processSpeed = false;
		p1count = 0;
		//IF we're below a certain velocity, start the fadeout sequence
		if(bike.GetVelocity(10) < 0.5 && pointHistory.GetListSize() > 1 )
		  {
		    drawColor -= 3;
		    if(drawColor == 0)
		      {
			//If we've faded out completely, game over
			break;
		      }
		    //Redraw the snake in fading colors
		    CDLLNode<point>* drawPoint = pointHistory.GetHeadNode();
		    setPixel(drawPoint->GetPrevNode()->GetNodeData(), drawColor);
		    while(drawPoint->GetNextNode() != pointHistory.GetHeadNode())
		      {
			setPixel(drawPoint->GetNodeData(), drawColor);
			drawPoint = drawPoint->GetNextNode();
		      }
		  }
		else		
		  {
		    //Snake recovery. If the user starts pedaling again,
		    //it restores energy.
		    if(drawColor < 30) drawColor += 3;
		  }
	      }
	    //Control and redraw, happening at 100hz
	    if(processStep)
	      {
		//Update button status, and check for moves
		btnState.PollKeys();
		//Turn left
		if(btnState.KeyHit(CBtnState::B_BUTTON))
		  {
		    if(state == 0) state = 3;
		    else --state;
		  }
		//Turn right
		else if(btnState.KeyHit(CBtnState::A_BUTTON))
		  {
		    if(state == 3) state = 0;
		    else ++state;
		  }
		
		//Assuming we're going in a direction, draw more snake
		if(bike.GetVelocity(10) > 0.1)
		  {
		    //Velocity calculation. If enough time has elapsed,
		    //we can draw another x pixels (x = DISTANCE_PER_TICK)
		    if(velCount > (100/(bike.GetVelocity(10)*10)))
		      {
		
			for(int i = 0; i < DISTANCE_PER_TICK; ++i)
			{
			  //state refers to the direction we're going
			  switch(state)
			    {
			    case 0: 
			      player1point.x += 1;
			      break;
			    case 1: 
			      player1point.y -= 1;
			      break;
			    case 2: 
			      player1point.x -= 1;
			      break;
			    case 3: 
			      player1point.y += 1;
			      break;
			    }
			  //Collision Detection
			  collisionColor = getPixel(player1point);
			  if (collisionColor != 0)
			    {
			      //Eating a green brick: +1 point and longer snake
			      if(collisionColor == RGB16(0, 31, 0))
				{
				  eraseBlock();
				  drawBlock();
				  pointHistory.SetHeadNode(pointHistoryCursor->GetNextNode());
				  pointHistoryLimit += 10;
				  ++score;
				}
			      //Eating yourself or the wall: Game Over
			      else
				{
				  gameOver = true;
				  break;
				}
			    }
			  //If the snake size has grown, push new points onto the list
			  if(pointHistory.GetListSize() < pointHistoryLimit)
			    {
			      pointHistory.PushBack(player1point);
			      setPixel(player1point, drawColor);
			      pointHistoryCursor = pointHistoryCursor->GetNextNode();
			    }
			  //Else just keep running through the list, since it's
			  //circular anyways
			  else
			    {
			      setPixel(pointHistoryCursor->GetNodeData(), 0x0);
			      setPixel(player1point, drawColor);
			      pointHistoryCursor->SetNodeData(player1point);
			      pointHistoryCursor = pointHistoryCursor->GetNextNode();
			    }		  
			}
			velCount = 0;
			if(gameOver) break;
		      }
		  }
		//Reset state
		processStep = false;
	      }
	  } 
	//Uh oh, we ran into something, trigger the game over message
	gameOver = true;
      }
    return 0;
  }
};

//Stub to start the game
int main()
{
  CNibbles game;
  game.RunGame();
  return 0;
}
