#ifndef PC_SPEAKER_DRIVER_H
#define PC_SPEAKER_DRIVER_H

#include <stdint.h>

typedef struct Sound{
    uint32_t freccuency;
    uint32_t ticks;
}Sound;


void actulizeAudio();
void addSoundBuff(Sound s);
void changeBackgroundMusic(Sound * m, uint32_t length);
void pauseBackgroundMusic();
void resumeBackgroundMusic();
void muteUnMuteAudio();
void cleanSoundBuff();

#endif // PC_SPEAKER_DRIVER_H