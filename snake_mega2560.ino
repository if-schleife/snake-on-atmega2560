/**************************************************************************
  This is a library for several Adafruit displays based on ST77* drivers.

  Works with the Adafruit 1.8" TFT Breakout w/SD card
    ----> http://www.adafruit.com/products/358
  The 1.8" TFT shield
    ----> https://www.adafruit.com/product/802
  The 1.44" TFT breakout
    ----> https://www.adafruit.com/product/2088
  as well as Adafruit raw 1.8" TFT display
    ----> http://www.adafruit.com/products/618

  Check out the links above for our tutorials and wiring diagrams.
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional).

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 **************************************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>


#ifdef ADAFRUIT_HALLOWING
  #define TFT_CS        39 // Hallowing display control pins: chip select
  #define TFT_RST       37 // Display reset
  #define TFT_DC        38 // Display data/command select
  #define TFT_BACKLIGHT  7 // Display backlight pin
#else
  // For the breakout board, you can use any 2 or 3 pins.
  // These pins will also work for the 1.8" TFT shield.
  #define TFT_CS        10
  #define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
  #define TFT_DC         8
#endif

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique
// to each board and not reassignable. For Arduino Uno: MOSI = pin 11 and
// SCLK = pin 13. This is the fastest mode of operation and is required if
// using the breakout board's microSD card.

// For 1.44" and 1.8" TFT with ST7735 (including HalloWing) use:
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// For 1.54" TFT with ST7789:
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

float p = 3.1415926;
const int VRx = A10; // Connect to Analog Pin 0
const int VRy = A11; // Connect to Analog Pin 1
const int SW = 4; // Connect to Digital Pin 4

#define right 1
#define down  2
#define up    3
#define left  0

bool hungry = true;
bool gamestatus = true;

uint16_t pos_x =24; //multiple of 8
uint16_t pos_y =40; //multiple of 8

int direction =1;
uint8_t pos_food_x=0, pos_food_y=0;  //food xy position
bool lock_move=false;  //so you can only input 1 value/loopcycle

struct body* top=NULL;  //init snake



void setup(void) {
  Serial.begin(9600);
  Serial.print(F("Hello! ST77xx TFT Test"));
  randomSeed(analogRead(0));

 
////////////////////////
////Joystick///////////
  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode (VRx, INPUT);
  pinMode (VRy, INPUT);
//////////////////////


#ifdef ADAFRUIT_HALLOWING
  // HalloWing is a special case. It uses a ST7735R display just like the
  // breakout board, but the orientation and backlight control are different.
  tft.initR(INITR_HALLOWING);        // Initialize HalloWing-oriented screen
  pinMode(TFT_BACKLIGHT, OUTPUT);
  digitalWrite(TFT_BACKLIGHT, HIGH); // Backlight on
#else
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

  // OR use this initializer (uncomment) if using a 1.44" TFT:
  //tft.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

  // OR use this initializer (uncomment) if using a 0.96" 180x60 TFT:
  //tft.initR(INITR_MINI160x80);  // Init ST7735S mini display

  // OR use this initializer (uncomment) if using a 1.54" 240x240 TFT:
  //tft.init(240, 240);           // Init ST7789 240x240
#endif

 
  

  

  Serial.println(F("Initialized"));

  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);



  generate_new_food();
  delay(5);

}




///////////////////////////////////////////////////////////////
//Disclamer///////////////////////////////////////////////////
//This is a test project, the sample code is without any warranties. Use at your own risk!
/////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
//This is my own variation of the game snake,
//using a single linked list with global variables
////////////////////////////////////////////////////////////

 /*
 ////TODO////
 output of amount of eaten food 
 aftergame screen
 restart with switch on joystick
 ///////////
 */

 
////////////////////////////loop///////////////////////////////////////////////////////////////////////////////////////////
void loop() {

while(gamestatus){


      joystick_input();
      collision_check_border();
      collision_check_snake() ; 
      collission_food();
      generate_food();
      starting_snakelenght(5);      //defines the lenght of the startsnake AND removes Tail
      push();                       //creates new snake bodypart
      snake_output();               //creates snake on display
    
     delay(150);
  }
  ///Endgamescreen
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(50, 50);
  tft.println("Looser");
  delay(150);
}

//////////////////////////loop end///////////////////////////////////////////////////////////////////////////////////////

void starting_snakelenght(uint8_t snakelenght){
static uint8_t start=0;
if(start>snakelenght? : start++);
while((start>snakelenght)&&(hungry)){   
        pop();                                    //delete tail if hungry, if snake picked up food its not hungry anymore -> doesnt delete tail
        break;
       }
     hungry=true; 
    }


void collission_food(){

  if(((pos_x==pos_food_x)&&(pos_y==pos_food_y)))
  {
       generate_new_food();
       hungry=false;
  }
}


bool movement_up(){
  bool moveup = ((direction!=down)&&(analogRead(VRx)>550)&&(470<analogRead(VRx)<=530)&&!lock_move);        //doesnt allow countermovemenet
  return moveup;
}

bool movement_down(){
  bool movedown = ((direction!=up)&&(analogRead(VRx)<450)&&(470<analogRead(VRx)<=530)&&!lock_move);       //VRx, VRy are the inputs of the joystick
  return movedown;
}

bool movement_left(){
  bool moveleft = ((direction!=right)&&(470<analogRead(VRx)<530)&&(analogRead(VRy)<450)&&!lock_move);
  return moveleft;
}

bool movement_right(){
  bool moveright = ((direction!=left)&&(470<analogRead(VRx)<530)&&(analogRead(VRy)>550)&&!lock_move);
  return moveright;
}


void joystick_input(){

    if(movement_up()){
      direction=up;
      lock_move = true;       //only one input/cycle
    }
    
    if(movement_down()){
      direction=down;  
      lock_move = true;
    }
    
    if(movement_left()){
      direction=left;
      lock_move = true;
    }
    
    if(movement_right()){
      direction=right;
      lock_move = true;
    }
    
lock_move = false;

      switch(direction){
        case right  :pos_x+=8; break;
        case left   :pos_x-=8;break;
        case up     :pos_y-=8;break;
        case down   :pos_y+=8;break;
        default     :break;
      }
}



void collision_check_border(){

      if((pos_x>124)||(pos_x<4)||(pos_y<4)||(pos_y>156))
      {
        gamestatus=false;   
      }  
}


void generate_food(){
  tft.drawRect(pos_food_x, pos_food_y  , 8, 8, ST77XX_WHITE);
}


void generate_new_food(){
uint8_t  xy_positions[]={8,16,24,32,40,48,56,64,72,80,88,96,104,112,120,128,136,144};
  
  pos_food_x=xy_positions[random(0,13)]; //8-112  kind of easymode, snakepart/food has weight of 8
  pos_food_y=xy_positions[random(0,17)]; //8-144
  
tft.drawRect(pos_food_x, pos_food_y  , 8, 8, ST77XX_WHITE);
}


struct body{
    uint8_t xxx, yyy;
    struct body *next;
};


void collision_check_snake(){
  struct body* alt;

  alt=top;
  while(alt->next!=NULL)
  {
   if(((alt->xxx) == pos_x)&&
      ((alt->yyy) == pos_y)){
      gamestatus=false;  
      tft.drawRect(33, 33  , 8, 8, ST77XX_RED);
                           }
  
    alt=alt->next;
  }
}

void snake_output(){

  struct body* temp;
  if(top==NULL)
  {
    //empty  
  }
  else
  {
    temp=top;
    while(temp->next!=NULL){
         tft.fillRect(temp->xxx, temp->yyy , 8, 8, ST77XX_WHITE);
         tft.drawRect(temp->xxx, temp->yyy  , 8, 8, ST77XX_WHITE);
         temp=temp->next;
                           }
  }
}
 
void push()           //creates new snakepart
{
    struct body* temp;
    temp=(struct body*)malloc(sizeof(struct body));
   
    temp->xxx=pos_x;
    temp->yyy=pos_y;

    temp->next=NULL;
    if(top==NULL){
    top=temp;
 }
 else{
    struct body* p;
     p=top;
      while(p->next != NULL)
      {
         p=p->next;
      } 
      p->next=temp;    
    }
 }
    

void pop(){               //deletes tailpart of snake
  struct body* temp;
  
        temp=top;
        tft.fillRect(temp->xxx, temp->yyy , 8, 8, ST77XX_BLACK); 
        top=top->next;
        temp->next=NULL;
        free(temp);  
}

void joystick_axistest(){
  
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextSize(1);
   tft.print("x-axis tilt: ");
  tft.println(analogRead(VRx));  
  tft.setCursor(0, 30); 
  tft.print("y-axis tilt: ");
  tft.println(analogRead(VRy));
  delay(100);
}
  
