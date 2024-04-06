
#ifndef MYCONTROLLER_H
#define MYCONTROLLER_H
#include <iostream>
#include <Windows.h>
#include <Xinput.h>

bool GetControllerState(XINPUT_STATE*);
bool sendCommand(unsigned char);
bool controllerStateChange(XINPUT_STATE, int*);
unsigned char commandEncoder(int, int);

#endif // !MYCONTROLLER_H

