/*
   RC-Sound_Modul
   Ziege-One
   v2.0
 
 Arduino pro Mini 5v/16mHz w/ Atmega 328

 
 ///// pinout ////
 D0: 
 D1: 
 D2: RC in Throttle
 D3: Engine on = pin to ground
 D4: Sound l on = pin to ground
 D5: Sound 2 on = pin to ground
 D6: Sound 3 on = pin to ground
 D7: Sound 4 on = pin to ground
 D8: 
 D9: Sound out 
 D10: CS   SD-Card 
 D11: MOSI SD-Card
 D12: MISO SD-Card
 D13: SCK  SD-Card + LED
 
 A0: 
 A1: 
 A2: 
 A3: 
 A4: 
 A5: 
 
 */
 
// ======== RC-Sound_Modul =======================================

#include <SD.h>                // need to include the SD library
#define SD_ChipSelectPin 10    // using digital pin 10 on arduino nano 328, can use other pins
#include "TMRpcmSpeed.h"       // also need to include this library...
#include <SPI.h>

TMRpcm tmrpcm;   // create an object for use in this sketch

//#define DEBUG       // Serial output on
//#define Serial_in   // Serial input on (change throttle with keyboard)

unsigned long time = 0;
int numLoop = 0;
int prevThrottle = 0;
int currThrottle = 0;
int playingSound = 0;
bool bPlayStart = true; 
bool bPlayNormal = false; 
bool bPlaySound1 = false; 
bool bPlaySound2 = false; 
bool bPlaySound3 = false; 
bool bPlaySound4 = false; 
bool bPlayEngine = false;
int shutdowndelay = 15;               // Number of seconds to wait before shutdown sound is played.
boolean shutdowndelay_file = false;   // is there a delay_.txt file 
unsigned int sampleSpeed = 8000;      // Loop samplerate
unsigned int sampleSpeed_max = 32000; // Loop samplerate max
boolean sampleSpeed_max_file = false; // is there a Speed_.txt file

//Pinout
int rc_pin = 2;     // Throttle RC PPM channel
int rc_engine = 3;  // Engine on = pin to ground
int rc_sound1 = 4;  // Sound l on = pin to ground
int rc_sound2 = 5;  // Sound 2 on = pin to ground
int rc_sound3 = 6;  // Sound 3 on = pin to ground
int rc_sound4 = 7;  // Sound 4 on = pin to ground
unsigned long duration = 1500;
unsigned long timer1;

// Setup
void setup() {

  pinMode(rc_engine, INPUT_PULLUP);
  pinMode(rc_sound1, INPUT_PULLUP);
  pinMode(rc_sound2, INPUT_PULLUP);
  pinMode(rc_sound3, INPUT_PULLUP);
  pinMode(rc_sound4, INPUT_PULLUP);  

  tmrpcm.speakerPin = 9; // Sound out PIN
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

  // check for config files
  if(SD.exists("/Delay.txt")) shutdowndelay_file = true;
  if(SD.exists("/Speed.txt")) sampleSpeed_max_file = true;

  if(shutdowndelay_file) shutdowndelay = readIntFromFile("Delay.txt");
  else shutdowndelay = 0; // Default off    
  
  if(sampleSpeed_max_file) sampleSpeed_max = readIntFromFile("Speed.txt");
  else  sampleSpeed_max = 32000; // Default max
  
#ifdef DEBUG        // Serial output on 
  Serial.print("Shutdowndelay : ");
  Serial.println(shutdowndelay);  
  Serial.print("SampleSpeed : ");
  Serial.println(sampleSpeed_max);
  Serial.println("setup end");
#endif    

  tmrpcm.volume(1);
}

void loop() {

  if (numLoop == 10000) {
  
  if (not digitalRead(rc_sound1))
  {
    if (!bPlaySound1)
    {
      bPlaySound1 = true; 
      tmrpcm.play("sound1.wav");
      tmrpcm.speed(tmrpcm.orgsamplerate);
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
      tmrpcm.speed(tmrpcm.orgsamplerate);
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
      tmrpcm.speed(tmrpcm.orgsamplerate);
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
      tmrpcm.speed(tmrpcm.orgsamplerate);
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

  if (not digitalRead(rc_engine))
  {
    if (!bPlayEngine)
    {
      bPlayEngine = true;
      bPlayStart = false;
    }  
  }
  else
  {
    bPlayEngine = false;
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
    
  if (digitalRead(rc_engine))
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
    Serial.print(" | SampleSpeed : ");
    Serial.print(sampleSpeed);
    Serial.print(" | currThrottle : ");
    Serial.print(currThrottle);
    Serial.print(" | Playing : ");
#endif     
    // Code to handle statup and //
    if ( ! bPlayStart ) {
      currThrottle = 1; // Play statup sound
      bPlayStart = true;
    }
    // Shutdowndelay 0 = off
    if ( (millis() - timer1) > (shutdowndelay * 1000) && currThrottle == 2 && (shutdowndelay > 0)) {
      currThrottle = 0;
      bPlayNormal = false;
    }

    /* Reverse throttle */
    if (duration <= 1480) 
    {
      if (bPlayNormal)
      {
      currThrottle = 3;
      sampleSpeed = map(duration,1480,1000,tmrpcm.orgsamplerate,sampleSpeed_max);
      tmrpcm.speed(sampleSpeed);
      }
      else
      {
      bPlayStart = false;
      }
    };

    /* Throttle stick center - Play sound orignal samplerate  */
    if ((duration > 1480) && (duration <= 1520) && bPlayNormal) 
    {
      currThrottle = 2;
    };

    /* Forard throttle */
    if (duration > 1520) 
    {
      if (bPlayNormal)
      {
      currThrottle = 3;
      sampleSpeed = map(duration,1520,2000,tmrpcm.orgsamplerate,sampleSpeed_max);
      tmrpcm.speed(sampleSpeed);
      }
      else
      {
      bPlayStart = false;
      }
    };

    if ((currThrottle != prevThrottle) or (tmrpcm.isPlaying() == 0)) {
      timer1 = millis();

      tmrpcm.loop(1);

      if ((currThrottle == 0) && (prevThrottle > 0)) {
        tmrpcm.play("shut.wav");
        tmrpcm.speed(tmrpcm.orgsamplerate);
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
          tmrpcm.speed(tmrpcm.orgsamplerate);
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
      if (((currThrottle == 2) && (prevThrottle == 1)) || ((currThrottle == 3) && (prevThrottle == 1))) {
        tmrpcm.play("loop.wav");
#ifdef DEBUG        // Serial output on        
        Serial.println("loop.wav");
#endif      
        prevThrottle = currThrottle;
        playingSound = 1;
      }
      if (currThrottle == 2) { 
        prevThrottle = currThrottle;
        playingSound = 1;
      }
      if (currThrottle == 3) { 
        prevThrottle = currThrottle;
        playingSound = 2;
      }
    }
#ifdef DEBUG        // Serial output on      
    Serial.println(tmrpcm.isPlaying());
#endif       
  }
}

// read Int from textfile via SD-card
int readIntFromFile(const char filename[])
{
  uint8_t pos=0;
  uint8_t BUFSIZE=20;
  char buffer[BUFSIZE]; 
  
  File dataFile = SD.open (filename, FILE_READ);
  if(!dataFile) return 0;
  while ((buffer[pos] = dataFile.read()) >= 0 && pos < (BUFSIZE-1)) pos++;
  dataFile.close();
  return (atoi(buffer));
}
