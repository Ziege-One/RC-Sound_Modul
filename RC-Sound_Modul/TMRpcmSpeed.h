/*Library by TMRh20 2012-2014

Contributors:

  ftp27 (GitHub) - setVolume(); function and code
  muessigb (GitHub) - metatata (ID3) support

*/

#ifndef TMRpcm_h   // if x.h hasn't been included yet...
#define TMRpcm_h   //   #define this so the compiler knows it has been included

#include <Arduino.h>
#include "pcmConfig.h"
#include "pcmRF.h"
#if !defined (SDFAT)
	#include <SD.h>
#else
	#include <SdFat.h>
#endif

#if defined (ENABLE_RF)
	class RF24;
#endif

class TMRpcm
{
 public:
 	//TMRpcm();
 	//*** General Playback Functions and Vars ***
	void play(const char* filename);
	void stopPlayback();
	void volume(char vol);
	void setVolume(char vol);
	void disable();
	void pause();
	void quality(boolean q);
	void loop(boolean set);
	byte speakerPin;
	boolean isPlaying();
    uint8_t CSPin;				
	void speed(int sampleRate);			// Speed MOD

	//*** Public vars used by RF library also ***
	boolean wavInfo(const char* filename);
	boolean rfPlaying;
	unsigned int SAMPLE_RATE;
	unsigned int orgsamplerate;		// Speed MOD

	//*** Advanced usage Vars ***
	byte listInfo(const char* filename, unsigned char *tagData, byte infoNum);
	byte id3Info(const char* filename, unsigned char *tagData, byte infoNum);
	byte getInfo(const char* filename, unsigned char* tagData, byte infoNum);


	#if !defined (ENABLE_MULTI)//Normal Mode
		void play(const char* filename, unsigned long seekPoint);

	//*** MULTI MODE **
	#else
		void quality(boolean q, boolean q2);
		void play(const char* filename, boolean which);
		void play(const char* filename, unsigned long seekPoint, boolean which);
		boolean isPlaying(boolean which);
		void stopPlayback(boolean which);
		void volume(char upDown,boolean which);
		void setVolume(char vol, boolean which);
		void loop(boolean set, boolean which);
	#endif
	#if defined (MODE2)
			byte speakerPin2;
	#endif

	//*** Recording WAV files ***
	void createWavTemplate(const char* filename,unsigned int sampleRate);
	void finalizeWavTemplate(const char* filename);
	#if defined (ENABLE_RECORDING)
		void startRecording(const char* fileName, unsigned int SAMPLE_RATE, byte pin);
		void startRecording(const char* fileName, unsigned int SAMPLE_RATE, byte pin, byte passThrough);
		void stopRecording(const char* fileName);
	#endif

 private:

 	void setPin();
	void timerSt();			
	unsigned long fPosition();
	unsigned int resolution;	
	byte lastSpeakPin;
	byte metaInfo(boolean infoType, const char* filename, unsigned char* tagData, byte whichInfo);
	boolean seek(unsigned long pos);
	boolean ifOpen();

	#if !defined (SDFAT)
		boolean searchMainTags(File xFile,const  char *datStr);
	#else
		unsigned long searchMainTags(SdFile xFile,const  char *datStr);
	#endif

	#if defined (ENABLE_MULTI)
		void ramp(boolean wBuff);
	#endif

	#if defined (MODE2)
		void setPins();
	#endif
};

#endif
