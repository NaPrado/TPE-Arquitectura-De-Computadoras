#include<sounds.h>
#include<libasm.h>
#include<libc.h>
#define SYS_PLAY_SOUND 12
#define SYS_STOP_SOUND 13


void playSound(uint32_t frequency){
    sys_call(SYS_PLAY_SOUND, frequency, 0, 0, 0);
}
void stopSound(void){
    sys_call(SYS_STOP_SOUND, 0, 0, 0, 0);
}
void playSoundForTicks(uint32_t nFrequence, uint32_t ticks){
    playSound(nFrequence);
    sleep(ticks);
    stopSound();
}