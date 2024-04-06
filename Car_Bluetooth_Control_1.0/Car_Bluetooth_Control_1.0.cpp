
#include <iostream>
#include <Windows.h>
#include <Xinput.h>
#include <tchar.h>
#include <string>
#include "mycontroller.h"
#include "mybluetooth.h"

#define BIT(a) (1UL << (a))
#pragma comment(lib, "Xinput.lib")



int main()
{
    XINPUT_STATE state;
    const TCHAR* portName = _T(COM);
    int input[16];
    for (int i = 0; i < 16; i++) {
        input[i] = 0;
    }
    if (COMInit(portName)) {
        std::cout << "COM port opened successfully!\n";

        std::cout << "Is the bluetooth initiated?(Y/N)";
        char bt_init = getchar();
        if ((bt_init == 'N') || (bt_init == 'n')) {
            if (BTInit()) {
                //sendCommand('0');
                //Sleep(1000);
                //sendCommand('1');
                while (true) {
                    if (GetControllerState(&state) && controllerStateChange(state, &input[0])) {
                        //Sleep(50);
                        std::cout<<"Press detected and command was sent"<<std::endl;
                    }
                }
            }
            else {
                std::cout << "Failed Init";
            }
        }
        else {
            //sendCommand('0');
            //Sleep(1000);
            //sendCommand('1');
            while (true) {
                if (GetControllerState(&state) && controllerStateChange(state, &input[0])) {
                    //Sleep(50);
                    std::cout<<"Press detected and command was sent"<<std::endl;
                }
            }
        }
    }


    return 0;
}

