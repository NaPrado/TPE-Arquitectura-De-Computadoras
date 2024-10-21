#include <keyboardDriver.h>

#define CAPSLOCK 0x3A
#define LSHIFT 0x36
#define RSHIFT 0x2A
#define LCTRL 0x1D
#define RCTRL 0x9D
#define LALT 0x38
#define RALT 0xB8
#define LSHIFT_RELEASE 0xAA
#define RSHIFT_RELEASE 0xB6
#define LCTRL_RELEASE 0x9D
#define RCTRL_RELEASE 0x9D
#define LALT_RELEASE 0xB8
#define RALT_RELEASE 0xB8
#define CAPSLOCK_RELEASE 0xBA

static char charBuffer[16]={0};
static char charsAtBuffer=0;
static int charBufferIndex=0;
static char getterIndex=0;
static char presedBuffer[0xD9]={0};
static char capslock=0;
static char capslockRealized=0;
static char shift=0;
static char ctrl=0;
static char alt=0;

char hasNextKey(){
    return charsAtBuffer!=0;
}
char nextKey(){
    if (!hasNextKey()){
        return 0;
    }
    char ret=charBuffer[getterIndex];
    getterIndex++;
    getterIndex=getterIndex%16;
    charsAtBuffer--;
    return ret;
}

char getCapslock(){
    return capslock;
}
char getShiftPressed(){
    return shift;
}
char getCtrlPressed(){
    return ctrl;
}
char getAltPressed(){
    return alt;
}

void keyboard_handler(){
    char key = getKey();
    char aux=key;
    if(key==CAPSLOCK && !capslockRealized){
        capslock=!capslock;
        return;
    }else if (key==CAPSLOCK_RELEASE){
        capslockRealized=0;
        return;
    }else if (key==LSHIFT || key==RSHIFT){
        shift=1;
        return;
    }else if (key==LSHIFT_RELEASE || key==RSHIFT_RELEASE){
        shift=0;
        return;
    }else if (key==LCTRL || key==RCTRL){
        ctrl=1;
        return;
    }else if (key==LCTRL_RELEASE || key==RCTRL_RELEASE){
        ctrl=0;
        return;
    }else if (key==LALT || key==RALT){
        alt=1;
        return;
    }else if (key==LALT_RELEASE || key==RALT_RELEASE){
        alt=0;
        return;
    }else if (!(aux>>7)){
        if (charsAtBuffer<=15){
            charBuffer[charBufferIndex]=key;
            charBufferIndex++;
            charBufferIndex=charBufferIndex%16;
            charsAtBuffer++;
        }
        presedBuffer[key]=1;
    }else{
        presedBuffer[key-0x80]=0;
    }   
}