#include <Adafruit_CircuitPlayground.h>


#define K_START 1
#define K_COUNTDOWN 2
#define K_ALARM_SETUP 3
#define K_ALARM 4
#define K_EXPLOSION_SETUP 5
#define K_EXPLOSION 6
#define K_WAIT_FOR_RESTART 7

#define K_DEBUG 20

uint16_t state = K_START;

uint16_t pixelNumber=0;
int countDown = 10;
uint16_t theTone = 500;
uint16_t theToneInit = 500;

uint16_t red = 0;
uint16_t green = 0;
uint16_t blue = 255;

uint16_t tDelayInit = 1000;
uint16_t dInit = 20;
uint16_t toneBump = 100;

uint16_t isOn = true;  // used for blinking lights
uint16_t isOnCount=0;

void setup() {
  // put your setup code here, to run once:
  //Serial.begin(9600);
  CircuitPlayground.begin();
  CircuitPlayground.setBrightness(100);
  CircuitPlayground.clearPixels();
  CircuitPlayground.redLED(false);

}

void loop() {

  uint16_t d = dInit;
  uint16_t tDelay = tDelayInit;
 
  switch ( state ) {

   
    case K_START:
      resetGlobals();
      CircuitPlayground.setBrightness(20);
      CircuitPlayground.clearPixels();
        
      state = K_COUNTDOWN;
    break;
    
    case K_COUNTDOWN:
      
      tDelay -= countDownSoundTick( theTone ); // plays tones and returns how long of a delay this caused - we subtract from total delay
      theTone += toneBump; // have each successive tones increase in frequency
      if (theTone > 2000 ) theTone = theToneInit; // don't go above a certain frequency ; start over again if we reach this

      red =0;      green = 0;      blue = 255;
      drawTail( pixelNumber);
      pixelNumber++;
      if (pixelNumber > 9 ) pixelNumber=0;
      
      countDown--;
      if ( countDown == 0 ) {
        state = K_ALARM_SETUP;
      }
  
    break;

    case K_ALARM_SETUP:
        theTone = theToneInit;
        toneBump = 25;
        tDelayInit = d * 4;
        CircuitPlayground.setBrightness(255);
            
        state = K_ALARM;
    break;
    
    case K_ALARM:

      tDelay -= countDownSoundTick( theTone );
      theTone += toneBump;
      if (theTone > 2000 ) theTone = theToneInit;

      red = 255;      green = 0;      blue = 0;
      drawTail( pixelNumber);
      pixelNumber++;
      if (pixelNumber > 9 ) pixelNumber=0;

      countDown++;
      if ( countDown > 100 )
      {
          state = K_EXPLOSION_SETUP;
      }
    break;

    case K_EXPLOSION_SETUP:
      tDelay = 10;
      countDown = 0;
      isOn = true;
      isOnCount = 0;
      CircuitPlayground.setBrightness(255);
      state = K_EXPLOSION;
    break;
    
    case K_EXPLOSION:

      randomSound(10);
      flipLights( 10 );
      tDelay=10;
      countDown++;

      if ( countDown > 200 ) {
        CircuitPlayground.clearPixels();
        tDelay = 10000;
        state = K_WAIT_FOR_RESTART;
      }
    break;

    case K_WAIT_FOR_RESTART:
      tDelay=1000;
    break;
    
  }


  delay( tDelay );
  
}

void randomSound( uint16_t duration ) {

 CircuitPlayground.playTone( random(50,100) ,  duration );
   
}

void flipLights( uint16_t flashMS) {

  uint16_t red,green,blue;
  red = 255;
  green = 150;
  blue = 150;
  
  if ( isOn ) {

    if ( isOnCount < flashMS/2 ) 
    {
      red = 255;
      green = 0;
      blue = 0;
    }
    
    for ( uint16_t p = 0; p <= 9 ; p++ ) {
      CircuitPlayground.setPixelColor(p, red, green, blue);
    }

    isOnCount++;
    if (isOnCount > flashMS ) {
      isOn = ! isOn;
      isOnCount=0;
    }
    
  }
  else
  {
      CircuitPlayground.clearPixels();
      isOnCount++;
      if ( isOnCount > flashMS )
      {
        isOnCount=0;
        isOn = !isOn;
        
      }
    
  }
  
}

uint16_t  countDownSoundTick( uint16_t theTone ) {
  uint16_t d = dInit;
  uint16_t td = 0;
  
  CircuitPlayground.playTone( theTone,   d ); delay(d); td += d;
  CircuitPlayground.playTone( theTone/4, d ); delay(d); td += d;
  CircuitPlayground.playTone( theTone,   d ); delay(d); td += d;
  CircuitPlayground.playTone( theTone/2, d ); delay(d); td += d;

  return (td);
  
}

 
void drawTail( uint16_t p )
{

  CircuitPlayground.setPixelColor(p, red, green, blue);
  
  for ( uint16_t i = 1; i < 9; i++)
  {
    if ( p == 0 ) {
      p = 9;
    } else {
      p--;
    }

    // dim each successive light (so it looks like a tail trailing off)
    red = red * 0.6;
    green = green * 0.6;
    blue = blue * 0.6;
    
    CircuitPlayground.setPixelColor( p, red, green, blue);
  
  }

}


void resetGlobals()
{
  dInit = 20;
  countDown = 10;
  tDelayInit = 1000;

  red = 0;
  green = 0;
  blue = 255;
  theTone = theToneInit;
  pixelNumber = 0;
  toneBump = 100;
  
}

