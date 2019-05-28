#include <SD.h>                // need to include the SD library
#define SD_ChipSelectPin 10    // using digital pin 4 on arduino nano 328, can use other pins
#include <TMRpcm.h>            // also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

//#define DEBUG       // Serial output on
//#define Serial_in   // Serial input on (change throttle with keyboard)

unsigned long time = 0;
int numLoop = 0;
int throttle = 0;
int prevThrottle = 0;
int currThrottle = 0;
int playingSound = 0;
bool bPlayStart = true, bPlayStop = false, bFinished = false, bPlayNormal = false, bPlaySound1 = false, bPlaySound2 = false, bPlaySound3 = false, bPlaySound4 = false, bPlayDiesel = false;
int iPlayShutdownDelay = 15;   // Number of seconds to wait before shutdown sound is played.

//Pinout
int rc_pin = 2;     // Throttle RC PPM channel
int rc_diesel = 3;  // Diesel on = pin to ground
int rc_sound1 = 4;  // Sound l on = pin to ground
int rc_sound2 = 5;  // Sound 2 on = pin to ground
int rc_sound3 = 6;  // Sound 3 on = pin to ground
int rc_sound4 = 7;  // Sound 4 on = pin to ground
unsigned long duration = 1500;
unsigned long timer1;

// Setup
void setup() {

  pinMode(rc_diesel, INPUT_PULLUP);
  pinMode(rc_sound1, INPUT_PULLUP);
  pinMode(rc_sound2, INPUT_PULLUP);
  pinMode(rc_sound3, INPUT_PULLUP);
  pinMode(rc_sound4, INPUT_PULLUP);  

  tmrpcm.speakerPin = 9; //5,6,11 or 46 on Mega, 9 on Uno, Nano, etc
  Serial.begin(115200);   
  if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized
#ifdef DEBUG        // Serial output on     
    Serial.println("SD fail");
#endif     
    return;   // don't do anything more if not
  }
  else 
  {
#ifdef DEBUG        // Serial output on     
    Serial.println("SD ok");
#endif     
  }

  //PLay init file to tell sound version
  //Serial.println("beginsound.wav");
  //tmrpcm.play("beginsound.wav");
  //while(tmrpcm.isPlaying()){
  delay(2000);
#ifdef DEBUG        // Serial output on 
  Serial.println("setup end");
#endif  
}

void loop() {

  if (numLoop == 10000) {
  
  if (not digitalRead(rc_sound1))
  {
    if (!bPlaySound1)
    {
      bPlaySound1 = true; 
      tmrpcm.play("sound1.wav");
#ifdef DEBUG        // Serial output on      
      Serial.println("sound1.wav");
#endif   
      tmrpcm.loop(0);
      while (tmrpcm.isPlaying()) {}
      tmrpcm.disable();  // disables the timer on output pin and stops the music
    }
  }
  else  
  {
    bPlaySound1 = false;
  }

  if (not digitalRead(rc_sound2))
  {
    if (!bPlaySound1)
    {
      bPlaySound2 = true; 
      tmrpcm.play("sound2.wav");
#ifdef DEBUG        // Serial output on      
      Serial.println("sound2.wav");
#endif      
      tmrpcm.loop(0);
      while (tmrpcm.isPlaying()) {}
      tmrpcm.disable();  // disables the timer on output pin and stops the music
    }
  }
  else  
  {
    bPlaySound2 = false;
  }

  if (not digitalRead(rc_sound3))
  {
    if (!bPlaySound3)
    {
      bPlaySound1 = true; 
      tmrpcm.play("sound3.wav");
#ifdef DEBUG        // Serial output on      
      Serial.println("sound3.wav");
#endif      
      tmrpcm.loop(0);
      while (tmrpcm.isPlaying()) {}
      tmrpcm.disable();  // disables the timer on output pin and stops the music
    }
  }
  else  
  {
    bPlaySound3 = false;
  }

  if (not digitalRead(rc_sound4))
  {
    if (!bPlaySound4)
    {
      bPlaySound1 = true; 
      tmrpcm.play("sound4.wav");
#ifdef DEBUG        // Serial output on      
      Serial.println("sound4.wav");
#endif      
      tmrpcm.loop(0);   
      while (tmrpcm.isPlaying()) {}
      tmrpcm.disable();  // disables the timer on output pin and stops the music
    }
  }
  else  
  {
    bPlaySound4 = false;
  }    

  if (not digitalRead(rc_diesel))
  {
    if (!bPlayDiesel)
    {
      bPlayDiesel = true;
      bPlayStart = false;
    }  
  }
  else
  {
    bPlayDiesel = false;
    currThrottle = 0;
    bPlayNormal = false;
  }

  }
  ++numLoop;
  if (numLoop == 30000) {
    
#ifndef Serial_in         // Serial input off (change throttle with keyboard)   
    noInterrupts();
    duration = pulseIn(rc_pin, HIGH);      //Read throttel value
    interrupts();
#endif    
    
  if (digitalRead(rc_diesel))
    {
      duration = 1500; 
    }
  
#ifdef Serial_in        // Serial input on (change throttle with keyboard)
if(Serial.available()) {                          //Send commands over serial to play
      switch(Serial.read()){;  
        case '0': duration = 950; break; 
        case '1': duration = 1050; break;
        case '2': duration = 1150; break;
        case '3': duration = 1250; break;
        case '4': duration = 1450; break;
        case '5': duration = 1500; break;
        case '6': duration = 1550; break;
        case '7': duration = 1750; break;
        case '8': duration = 1850; break;        
        case '9': duration = 1950; break;         
        case ',': duration = 2050; break;   
      }}
#endif

    numLoop = 0;
#ifdef DEBUG        // Serial output on    
    Serial.print("Pulse in : ");
    Serial.print(duration);
    Serial.print(" | currThrottle : ");
    Serial.print(currThrottle);
    Serial.print(" | Playing : ");
#endif     
    /* Code to handle statup and */
    if ( ! bPlayStart ) {
      currThrottle = 1; // Play statup sound
      bPlayStart = true;
    }

    if ( (millis() - timer1) > (iPlayShutdownDelay * 1000) && currThrottle == 2 ) {
      currThrottle = 0;
      bPlayNormal = false;
    }

    /* Max Reverse */
    if (duration <= 990 && bPlayNormal) 
    {
      currThrottle = 7;
    };
    if ((duration > 990) && (duration <= 1080) && bPlayNormal) 
    {
      currThrottle = 6;
    };
    if ((duration > 1080) && (duration <= 1170) && bPlayNormal) 
    {
      currThrottle = 5;
    };
    if ((duration > 1170) && (duration <= 1260) && bPlayNormal) 
    {
      currThrottle = 4;
    };
    if ((duration > 1260) && (duration <= 1480)) 
    {
      if (bPlayNormal)
      {
      currThrottle = 3;
      }
      else
      {
      bPlayStart = false;
      }
    };

    /* Throttle stick center - Play idle sound */
    if ((duration > 1480) && (duration <= 1520) && bPlayNormal) 
    {
      currThrottle = 2;
    };

    /* Forard throttle */
    if ((duration > 1520) && (duration <= 1680)) 
    {
      if (bPlayNormal)
      {
      currThrottle = 3;
      }
      else
      {
      bPlayStart = false;
      }
    };
    if ((duration > 1680) && (duration <= 1770) && bPlayNormal) 
    {
      currThrottle = 4;
    };
    if ((duration > 1770) && (duration <= 1860) && bPlayNormal) 
    {
      currThrottle = 5;
    };
    if ((duration > 1860) && (duration <= 1950) && bPlayNormal) 
    {
      currThrottle = 6;
    };
    /* Max Forward */
    if ((duration > 1950) && bPlayNormal) {
      currThrottle = 7;
    };

    //If currThrottle != prevThrottle or no playing set start playing new file
    if ((currThrottle != prevThrottle) or (tmrpcm.isPlaying() == 0)) {
      timer1 = millis();

      tmrpcm.loop(1);

      if ((currThrottle == 0) && (prevThrottle > 0)) {
        tmrpcm.play("shut.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("shut.wav");
#endif 
        tmrpcm.loop(0);
        while (tmrpcm.isPlaying()) {}
        tmrpcm.disable();  // disables the timer on output pin and stops the music
        prevThrottle = currThrottle;
        bPlayNormal = false;
      }
      if (currThrottle == 1) {
        if (currThrottle > prevThrottle) {
          tmrpcm.play("start.wav");
#ifdef DEBUG        // Serial output on          
          Serial.println("start.wav");
#endif           
          tmrpcm.loop(0);
          while (tmrpcm.isPlaying()) {}
        }
        bPlayNormal = true;
        playingSound = 1;
        prevThrottle = currThrottle;
      }
      if (currThrottle == 2) {
        tmrpcm.play("1.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("1.wav");
#endif       
        prevThrottle = currThrottle;
        playingSound = 1;
      }
      if (currThrottle == 3) {
        tmrpcm.play("2.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("2.wav");
#endif    
        prevThrottle = currThrottle;
        playingSound = 2;
      }
      if (currThrottle == 4) {
        tmrpcm.play("3.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("3.wav");
#endif     
        prevThrottle = currThrottle;
        playingSound = 3;
      }
      if (currThrottle == 5) {
        tmrpcm.play("4.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("4.wav");
#endif       
        prevThrottle = currThrottle;
        playingSound = 4;
      }
      if (currThrottle == 6) {
        tmrpcm.play("5.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("5.wav");
#endif     
        prevThrottle = currThrottle;
        playingSound = 5;
      }
      if (currThrottle == 7) {
        tmrpcm.play("6.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("6.wav");
#endif       
        prevThrottle = currThrottle;
        playingSound = 6;
      }
    }
#ifdef DEBUG        // Serial output on      
    Serial.println(tmrpcm.isPlaying());
#endif       
  }
}
