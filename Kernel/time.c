#include <time.h>
#include <times.h>
#include <libasm.h>
#include <pcSpeakerDriver.h>

static unsigned long ticks = 0;


//simil Scheduler
void timer_handler() {
	ticks++;
    actulizeAudio();
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

unsigned long getTicks(){
    return ticks;
}

void sleep(int ticksToWait) {
    unsigned long start = ticks;
    while (ticks - start < ticksToWait){
        _hlt();
    };
}

static time t;

time * getTime(){
    t.day = getSystemDayOfMonth();
    t.month = getSystemMonth();
    t.year = getSystemYear();
    t.hour =getSystemHour(); 
    t.min = getSystemMin();
    t.sec = getSystemSec();
    return &t;
}
