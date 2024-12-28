#include <pcSpeakerDriver.h>
#include <stdint.h>
#include <libasm.h>
#define SOUND_BUFFER_SIZE 128

 //Play sound using built-in speaker
void playSound(uint32_t nFrequence) {
 	uint32_t Div;
 	uint8_t tmp;
	if(nFrequence==0){
		stopSound();
		return;
	}
	//Set the PIT to the desired frequency
 	Div = 1193180 / nFrequence;
 	outb(0x43, 0xb6);
 	outb(0x42, (uint8_t) (Div) );
 	outb(0x42, (uint8_t) (Div >> 8));

    //And play the sound using the PC speaker
 	tmp = inb(0x61);
  	if (tmp != (tmp | 3)) {
 		outb(0x61, tmp | 3);
 	}
 }

 //make it shut up
void stopSound() {
 	uint8_t tmp = inb(0x61) & 0xFC;

 	outb(0x61, tmp);
 }

//////////////////////////////////////////////////////////////////////
//							Sound Driver							//
//////////////////////////////////////////////////////////////////////

// static vars
static Sound soundBuffer[128];
static uint32_t soundBufferIndexRead=0;
static uint32_t soundBufferIndexWrite=0;

static uint32_t soundBufferSize=0;

static Sound * musicBuffer;
static uint32_t musicBufferLength=0;
static uint32_t musicBufferIndexRead;
static uint32_t musicTicks=0;
static uint32_t musicForConsume=0;
static uint32_t pauseMusic=0;

static uint8_t muteFlag=0;

// functions
uint32_t doSound(){	
	if (muteFlag) stopSound();
	else playSound(soundBuffer[soundBufferIndexRead].freccuency);
	
	if (!soundBuffer[soundBufferIndexRead].ticks--){  //por cada tick se disminuye y si llega a cero se lee el siguiente
		soundBufferIndexRead++;
		soundBufferSize--;
	}
}

playMusic(){
	if (muteFlag) stopSound();
	else playSound(musicBuffer[musicBufferIndexRead].freccuency);

	if (musicBuffer[musicBufferIndexRead].ticks==musicTicks++){
		musicTicks=0;
		musicBufferIndexRead++;
		musicBufferIndexRead%=musicBufferLength;
	}
}


void actulizeAudio(){
	if (soundBufferSize){
		doSound();
	}else if (musicForConsume && !pauseMusic){
		playMusic();
	}else{
		stopSound();
	}
}

void addSoundBuff(Sound s){
	uint32_t aux=(soundBufferIndexWrite+1)%SOUND_BUFFER_SIZE;
	if (soundBufferSize<SOUND_BUFFER_SIZE){
		soundBuffer[soundBufferIndexWrite].freccuency=s.freccuency;
		soundBuffer[soundBufferIndexWrite].ticks=s.ticks;
		soundBufferIndexWrite++;
		soundBufferIndexWrite%=SOUND_BUFFER_SIZE;
		soundBufferSize++;
	}
}

void changeBackgroundMusic(Sound * m, uint32_t length){
	musicBuffer=m;
	musicBufferLength=length;
	musicBufferIndexRead=0;
	musicForConsume=1;
}

void pauseBackgroundMusic(){
	pauseMusic=1;
}
void resumeBackgroundMusic(){
	pauseMusic=0;
}
void muteUnMuteAudio(){
	muteFlag= (!muteFlag);
}

void cleanSoundBuff(){
	soundBufferIndexRead=0;
	soundBufferIndexWrite=0;
	soundBufferSize=0;
}