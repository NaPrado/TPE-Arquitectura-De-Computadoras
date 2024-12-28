#include<sounds.h>
#include<libasm.h>
#include<libc.h>
#define SYS_PLAY_SOUND 12
#define SYSNUM_SET_MUSIC 13
#define SYSNUM_PAUSE_MUSIC 14
#define BUFFER_SIZE 20

void playSound(uint32_t frequency, uint32_t ticks){
    sys_call(SYS_PLAY_SOUND, frequency, ticks, 0, 0);
}
void setBackgroundMusic(Sound * m,uint32_t length){
    sys_call(SYSNUM_SET_MUSIC, m, length, 0, 0);
}
void pauseResumeMusic(uint32_t pause){
    sys_call(SYSNUM_PAUSE_MUSIC, pause, 0, 0, 0);
}
