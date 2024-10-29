#ifndef PC_SPEAKER_DRIVER_H
#define PC_SPEAKER_DRIVER_H

#include <stdint.h>

class PcSpeakerDriver {
public:
    PcSpeakerDriver();
    ~PcSpeakerDriver();

    void playSound(uint32_t frequency);
    void stopSound();

private:
    void enableSpeaker();
    void disableSpeaker();
};

#endif // PC_SPEAKER_DRIVER_H