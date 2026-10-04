#include <nds.h>
#include <stdio.h>
#include "runner_keyboard.h"

bool init = false;

void InitNDSInput(){
}

//Uses keyboard because controller refuses to work for me
void HandleNDSInput(RunnerKeyboardState* keyboard){
    if (!init)
        InitNDSInput();

    scanKeys();
    u32 keys = keysHeld();

    keyboard->keyDown['Z'] = (keys & KEY_A) != 0; //CONFIRM
    keyboard->keyDown['X'] = (keys & KEY_B) != 0; //BACK
    keyboard->keyDown['C'] = (keys & (KEY_X | KEY_Y)) != 0; //MENU

    //dir keys
    keyboard->keyDown[VK_LEFT] = (keys & KEY_LEFT) != 0; //LEFT
    keyboard->keyDown[VK_RIGHT] = (keys & KEY_RIGHT) != 0; //RIGHT
    keyboard->keyDown[VK_UP] = (keys & KEY_UP) != 0; //UP
    keyboard->keyDown[VK_DOWN] = (keys & KEY_DOWN) != 0; //DOWN
}