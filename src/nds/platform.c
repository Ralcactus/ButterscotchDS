#include "common.h"
#include "platformdefs.h"
#include "runner_mouse.h"
#include "gettime.h"

#include <nds.h>

static Runner* g_runner = NULL;

static int32_t g_width = 256;
static int32_t g_height = 192;

bool platformInit(int32_t reqW, int32_t reqH, const char* title, bool headless){
    (void)reqW;
    (void)reqH;
    (void)title;
    (void)headless;

    g_width = 256;
    g_height = 192;

    return true;
}


void platformExit(void){

}


void platformInitFunctions(Runner* runner){
    g_runner = runner;
    runner->setCursor = NULL;
    runner->currentCursor = GML_CR_DEFAULT;
}


bool platformGetWindowSize(int32_t* outW, int32_t* outH){
    if (outW)
        *outW = g_width;

    if (outH)
        *outH = g_height;

    return true;
}


bool platformGetScaledWindowSize(int32_t* outW, int32_t* outH){
    return platformGetWindowSize(outW, outH);
}


void platformSetWindowSize(int32_t width, int32_t height){
    if (width > 0)
        g_width = width;

    if (height > 0)
        g_height = height;
}


void platformSetWindowTitle(const char* title){
    (void)title;
}


void platformGetMousePos(double* xPos, double* yPos){
    if (xPos)
        *xPos = 0.0;

    if (yPos)
        *yPos = 0.0;
}


void platformSwapBuffers(void) {
}


void* platformGetProcAddress(const char* name){
    (void)name;

    return NULL;
}


typedef struct {
    u32 mask;
    int32_t gmlKey;
} keymap;

static const keymap keymapvar[] = {
    { KEY_A, 'Z' }, //CONFIRM
    { KEY_B, 'X' }, //BACK
    { KEY_X | KEY_Y, 'C' }, //MENU
    { KEY_LEFT, VK_LEFT  }, //LEFT
    { KEY_RIGHT, VK_RIGHT }, //RIGHT
    { KEY_UP, VK_UP    }, //UP
    { KEY_DOWN, VK_DOWN  }, //DOWN
};

bool platformHandleEvents(void) {
    scanKeys();
    u32 pressed  = keysDown();
    u32 released = keysUp();

    for (size_t i = 0; i < sizeof(keymapvar) / sizeof(keymapvar[0]); i++){
        if (pressed & keymapvar[i].mask)
            RunnerKeyboard_onKeyDown(g_runner->keyboard, keymapvar[i].gmlKey);

        if (released & keymapvar[i].mask)
            RunnerKeyboard_onKeyUp(g_runner->keyboard, keymapvar[i].gmlKey);
    }
    
    return false;
}


void platformSleepUntil(uint64_t targetTime){
    while (nowNanos() < targetTime){
        swiWaitForVBlank();
    }
}