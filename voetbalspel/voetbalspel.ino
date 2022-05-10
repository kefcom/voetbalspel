//---------------------------------------------
//                voetbalspel
//---------------------------------------------
#include "Keyboard.h"
#include <WS2812FX.h>
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#include "FastLED.h"

#define LED_COUNT     16
#define LED_PIN1      9
#define LED_PIN2      10
#define START_HEALTH  16
#define SCORE_LOW     1
#define SCORE_MID     2
#define SCORE_TOP     3

//CRGB leds1[LED_COUNT];

WS2812FX ws2812fx1 = WS2812FX(LED_COUNT, LED_PIN1, NEO_GRB + NEO_KHZ800);
WS2812FX ws2812fx2 = WS2812FX(LED_COUNT, LED_PIN2, NEO_GRB + NEO_KHZ800);

int gameMode = 0; //0: 2PL, 1:1PL, 2:CAMELRACE

unsigned long PL1MILLIS;
unsigned long PL2MILLIS;
unsigned long HOSTMILLIS;
int debounceTimeMs = 500;

bool PL1SCORED;
bool PL2SCORED;

int PL1HEALTH;
int PL2HEALTH;





void setup() {
  //set pins:
  pinMode(2, INPUT_PULLUP); // LOW PL1
  pinMode(3, INPUT_PULLUP); // LOW PL2
  pinMode(4, INPUT_PULLUP); // MID PL1
  pinMode(5, INPUT_PULLUP); // MID PL2
  pinMode(6, INPUT_PULLUP); // TOP PL1
  pinMode(7, INPUT_PULLUP); // TOP PL2
  pinMode(8, INPUT_PULLUP); // NXT RND
  //pinMode(D9, OUTPUT); // PL1 LED
  //pinMode(D10, OUTPUT); // PL2 LED
  pinMode(11, INPUT_PULLUP); // RESET
  pinMode(12, INPUT_PULLUP); // SELECT

  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  
  //START LEDSTRIPS:
  // FastLED.addLeds<NEOPIXEL, LED_PIN1>(leds1, LED_COUNT);
  // for(int i = 0; i<LED_COUNT;i++)
  // {
  //   leds1[i] = CRGB::Red;
  // }
  // FastLED.show();




   ws2812fx1.init();
   ws2812fx1.setBrightness(255);
   ws2812fx1.setSpeed(1000);
   ws2812fx1.setMode(43);
   ws2812fx1.setColor(0x00FF00);

  ws2812fx2.init();
  ws2812fx2.setBrightness(255);
  ws2812fx2.setSpeed(1000);
  ws2812fx2.setMode(43);
  ws2812fx2.setColor(0x00FF00);

  ws2812fx1.start();
  ws2812fx2.start();


  //random stuff
  Serial.begin(9600);
  Serial.println("Voetbalspel");
  PL1MILLIS = millis();
  PL2MILLIS = millis();
  HOSTMILLIS = millis();

  PL1SCORED = false;
  PL2SCORED = false;
  
  PL1HEALTH = START_HEALTH;
  PL2HEALTH = START_HEALTH;

  Keyboard.begin();
}

void resetGameMode0()
{
        PL1SCORED = false;
        PL2SCORED = false;
        PL1HEALTH = START_HEALTH;
        PL2HEALTH = START_HEALTH;
        //move segments out of view
        ws2812fx1.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx1.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx2.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx2.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
        //set animation
        ws2812fx1.setSegment(0, 0, LED_COUNT, 14, RED, 1000, false);
        ws2812fx2.setSegment(0, 0, LED_COUNT, 14, BLUE, 1000, false);        
}
void resetGameMode1()
{
        PL1SCORED = false;
        PL2SCORED = false;
        PL1HEALTH = 0;
        PL2HEALTH = 0;
        //move segments out of view
        ws2812fx1.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx1.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx2.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx2.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
        //set animation
        ws2812fx1.setSegment(0, 0, LED_COUNT, 14, GREEN, 1000, false);
        ws2812fx2.setSegment(0, 0, LED_COUNT, 14, GREEN, 1000, false);        
}

void loop() {

  //check button presses:
  // PL1 LOW
  if(digitalRead(2) == 0 && (millis() - PL1MILLIS) > debounceTimeMs)
  {
    PL1MILLIS = millis();
    PL1LOW();
  }
  // PL1 MID
  if(digitalRead(4) == 0 && (millis() - PL1MILLIS) > debounceTimeMs)
  {
    PL1MILLIS = millis();
    PL1MID();
  }  
  // PL1 TOP
  if(digitalRead(6) == 0 && (millis() - PL1MILLIS) > debounceTimeMs)
  {
    PL1MILLIS = millis();
    PL1TOP();
  }

  // PL2 LOW
  if(digitalRead(3) == 0 && (millis() - PL2MILLIS) > debounceTimeMs)
  {
    PL2MILLIS = millis();
    PL2LOW();
  }
  // PL2 MID
  if(digitalRead(5) == 0 && (millis() - PL2MILLIS) > debounceTimeMs)
  {
    PL2MILLIS = millis();
    PL2MID();
  }  
  // PL2 TOP
  if(digitalRead(7) == 0 && (millis() - PL2MILLIS) > debounceTimeMs)
  {
    PL2MILLIS = millis();
    PL2TOP();
  }  

  //SELECT GAME MODE
  if(digitalRead(12) == 0 && (millis() - HOSTMILLIS) > 500)
  {
    HOSTMILLIS = millis();
    Serial.println("GAME MODE PRESSED");
    switch (gameMode)
    {
      case 0:
        //init for game mode 1
        gameMode = 1;
        resetGameMode1();
      break;
      case 1:
        //init for game mode 2
        gameMode = 2;
      break;
      case 2:
        //init for game mode 0
        gameMode = 0;
        resetGameMode0();
      break;            
    }
  }

  // RESET BUTTON
  if(digitalRead(11) == 0 && (millis() - HOSTMILLIS) > 500)
  {
    HOSTMILLIS = millis();
    Serial.println("RESET BUTTON PRESSED");
    switch (gameMode)
    {
      case 0:
        resetGameMode0();
      break;
      case 1:
        resetGameMode1();
      break;
      case 2:
        
      break;      
    }
  }

  // NEXT ROUND BUTTON
  if(digitalRead(8) == 0 && (millis() - HOSTMILLIS) > 5000)
  {
    HOSTMILLIS = millis();
    Serial.println("NEXT ROUND BTN PRESSED");
    switch (gameMode)
    {
      case 0:
        PL1SCORED = false;
        PL2SCORED = false;
        //move segments out of view
        ws2812fx1.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx1.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx2.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
        ws2812fx2.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
        //set animation
        ws2812fx1.setSegment(0, 0, LED_COUNT, 14, RED, 1000, false);
        ws2812fx2.setSegment(0, 0, LED_COUNT, 14, BLUE, 1000, false);
        break;
      case 1:

      break;
      case 2:
      Keyboard.write(KEY_F11);
      break;
    }
  }






    //------------------------------------------------------------------------------------------------------------------
  //                                                    Service Area
  //------------------------------------------------------------------------------------------------------------------
    //service ledstrips:
    ws2812fx1.service();
    ws2812fx2.service();
    //show game mode led
    switch (gameMode)
    {
      case 0:
        analogWrite(A0, 255);
        analogWrite(A1, 0);
        analogWrite(A2, 0);
        break;
      case 1:
        analogWrite(A0, 0);
        analogWrite(A1, 255);
        analogWrite(A2, 0);
        break;
      case 2:
        analogWrite(A0, 0);
        analogWrite(A1, 0);
        analogWrite(A2, 255);
        break;            
    }
  //------------------------------------------------------------------------------------------------------------------
  //                                                   END OF Service Area
  //------------------------------------------------------------------------------------------------------------------
}//end of void loop

void PL1setLedsMode0(int theScore)
{
  int ledsHealth2 = map(PL2HEALTH,0,START_HEALTH,0,LED_COUNT);
  int ledsWin = map(theScore,0,START_HEALTH,0,LED_COUNT);
  int ledsOFF = LED_COUNT - (ledsHealth2 + ledsWin);

  ws2812fx1.setSegment(0, 0, ledsHealth2-1, FX_MODE_STATIC, BLUE, 1000, false);
  ws2812fx1.setSegment(1, ledsHealth2, (ledsHealth2 + ledsWin)-1, 1, RED, 1000, false);
  ws2812fx1.setSegment(2, ledsHealth2+ledsWin, LED_COUNT, 0, BLACK, 1000, false);
}
void PL2setLedsMode0(int theScore)
{
  int ledsHealth1 = map(PL1HEALTH,0,START_HEALTH,0,LED_COUNT);
  int ledsWin = map(theScore,0,START_HEALTH,0,LED_COUNT);
  int ledsOFF = LED_COUNT - (ledsHealth1 + ledsWin);

  ws2812fx2.setSegment(0, 0, ledsHealth1-1, FX_MODE_STATIC, RED, 1000, false);
  ws2812fx2.setSegment(1, ledsHealth1, (ledsHealth1 + ledsWin)-1, 1, BLUE, 1000, false);
  ws2812fx2.setSegment(2, ledsHealth1+ledsWin, LED_COUNT, 0, BLACK, 1000, false);
}
void PL1setLedsMode1()
{
  int ledsOn = map(PL1HEALTH,0,START_HEALTH,0,LED_COUNT);
  ws2812fx1.setSegment(0, 0, ledsOn-1, 0, RED, 1000, false);
  ws2812fx1.setSegment(1, ledsOn, LED_COUNT, 0, BLACK, 1000, false);
}
void PL2setLedsMode1()
{
  int ledsOn = map(PL2HEALTH,0,START_HEALTH,0,LED_COUNT);
  ws2812fx2.setSegment(0, 0, ledsOn-1, 0, BLUE, 1000, false);
  ws2812fx2.setSegment(1, ledsOn, LED_COUNT, 0, BLACK, 1000, false);
}
void PL2WIN()
{
  //WINNER PL2
  PL1SCORED = true;
  PL2SCORED = true;
  //move segments out of view
  ws2812fx1.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
  ws2812fx1.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
  ws2812fx2.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
  ws2812fx2.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
  //set animation
  ws2812fx2.setSegment(0, 0, LED_COUNT, 16, BLUE, 1000, false);
  ws2812fx1.setSegment(0, 0, LED_COUNT, 0, BLACK, 1000, false);
}
void PL1WIN()
{
  //WINNER PL1
  PL1SCORED = true;
  PL2SCORED = true;
  //move segments out of view
  ws2812fx1.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
  ws2812fx1.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
  ws2812fx2.setSegment(1, LED_COUNT+1, 1, FX_MODE_STATIC, BLACK, 1000, false);
  ws2812fx2.setSegment(2, LED_COUNT+2, 1, FX_MODE_STATIC, BLACK, 1000, false);
  //set animation
  ws2812fx1.setSegment(0, 0, LED_COUNT, 16, RED, 1000, false);
  ws2812fx2.setSegment(0, 0, LED_COUNT, 0, BLACK, 1000, false);
}

void PL1LOW()
{
  Serial.println("PL1 LOW pressed");
  switch (gameMode)
  {
    case 0:
      if(PL1SCORED == false)
      {
        //VALID SHOT, 1 POINT, deduct from PL2
        PL2HEALTH = PL2HEALTH - SCORE_LOW;
        if(PL2HEALTH <= 0)
        {
          PL1WIN();          
        }else{
          PL1setLedsMode0(SCORE_LOW);
          PL1SCORED = true;
        }
      }
    break;
    case 1:
    if(PL1HEALTH < START_HEALTH && PL2HEALTH < START_HEALTH)
    {
      PL1HEALTH = PL1HEALTH + SCORE_LOW;
      if(PL1HEALTH >= START_HEALTH)
      {
        PL1WIN();
      }else{
        PL1setLedsMode1();
      }
    }
    break;
    case 2:
    Keyboard.write('c');
    break;        
  }
}
void PL2LOW()
{
  Serial.println("PL2 LOW pressed");
  switch (gameMode)
  {
    case 0:
      if(PL2SCORED == false)
      {
        //VALID SHOT, 1 POINT, deduct from PL1
        PL1HEALTH = PL1HEALTH - SCORE_LOW;
        if(PL1HEALTH <= 0)
        {
          PL2WIN();
        }else{        
        PL2setLedsMode0(SCORE_LOW);
        PL2SCORED = true;
        }
      }
    break;
    case 1:
    if(PL2HEALTH < START_HEALTH && PL1HEALTH < START_HEALTH)
    {
      PL2HEALTH = PL2HEALTH + SCORE_LOW;
      if(PL2HEALTH >= START_HEALTH)
      {
        PL2WIN();
      }else{
        PL2setLedsMode1();
      }
    }
    break;
    case 2:
    Keyboard.write('f');
    break;        
  }  
}
void PL1MID()
{
  Serial.println("PL1 MID pressed");
  switch (gameMode)
  {
    case 0:
     if(PL1SCORED == false)
      {
        //VALID SHOT, 1 POINT, deduct from PL2
        PL2HEALTH = PL2HEALTH - SCORE_MID;
        if(PL2HEALTH <= 0)
        {
          PL1WIN();
        }else{
        PL1setLedsMode0(SCORE_MID);
        PL1SCORED = true;
        }
      }    
    break;
    case 1:
    if(PL1HEALTH < START_HEALTH && PL2HEALTH < START_HEALTH)
    {
      PL1HEALTH = PL1HEALTH + SCORE_MID;
      if(PL1HEALTH >= START_HEALTH)
      {
        PL1WIN();
      }else{
        PL1setLedsMode1();
      }
    }
    break;
    case 2:
    Keyboard.write('b');
    break;        
  }  
}
void PL2MID()
{
  Serial.println("PL2 MID pressed");
  switch (gameMode)
  {
    case 0:
      if(PL2SCORED == false)
      {
        //VALID SHOT, 1 POINT, deduct from PL1
        PL1HEALTH = PL1HEALTH - SCORE_MID;
        if(PL1HEALTH <= 0)
        {
          PL2WIN();
        }else{
        PL2setLedsMode0(SCORE_MID);
        PL2SCORED = true;
        }
      }
    break;
    case 1:
    if(PL2HEALTH < START_HEALTH && PL1HEALTH < START_HEALTH)
    {
      PL2HEALTH = PL2HEALTH + SCORE_MID;
      if(PL2HEALTH >= START_HEALTH)
      {
        PL2WIN();
      }else{
        PL2setLedsMode1();
      }    
    }
    break;
    case 2:
    Keyboard.write('e');
    break;        
  }  
}
void PL1TOP()
{
  Serial.println(millis());
  Serial.println("PL1 TOP pressed");
  switch (gameMode)
  {
    case 0:
         if(PL1SCORED == false)
      {
        //VALID SHOT, 1 POINT, deduct from PL2
        PL2HEALTH = PL2HEALTH - SCORE_TOP;
        if(PL2HEALTH <= 0)
        {
         PL1WIN();
        }else{
          PL1setLedsMode0(SCORE_TOP);
          PL1SCORED = true;
        }
      }   
    break;
    case 1:
    if(PL1HEALTH < START_HEALTH && PL2HEALTH < START_HEALTH)
    {
      PL1HEALTH = PL1HEALTH + SCORE_TOP;
      if(PL1HEALTH >= START_HEALTH)
      {
        PL1WIN();
      }else{
        PL1setLedsMode1();
      }
    }
    break;
    case 2:
    Keyboard.write('x');
    break;        
  }  
}
void PL2TOP()
{
  Serial.println("PL2 TOP pressed");
  switch (gameMode)
  {
    case 0:
      if(PL2SCORED == false)
      {
        //VALID SHOT, 1 POINT, deduct from PL1
        PL1HEALTH = PL1HEALTH - SCORE_TOP;
        if(PL1HEALTH <= 0)
        {
          PL2WIN();
        }else{
        PL2setLedsMode0(SCORE_TOP);
        PL2SCORED = true;
        }
      }
    break;
    case 1:
        if(PL2HEALTH < START_HEALTH && PL1HEALTH < START_HEALTH)
    {
      PL2HEALTH = PL2HEALTH + SCORE_TOP;
      if(PL2HEALTH >= START_HEALTH)
      {
        PL2WIN();
      }else{
        PL2setLedsMode1();
      }
    }
    break;
    case 2:
    Keyboard.write('d');
    break;        
  }  
}
