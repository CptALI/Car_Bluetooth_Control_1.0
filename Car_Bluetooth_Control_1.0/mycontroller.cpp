#include "mycontroller.h"
#include "mybluetooth.h"
#include <iostream>
#include <Windows.h>
#include <Xinput.h>

bool GetControllerState(XINPUT_STATE* stateAdress) {

    DWORD dwResult = XInputGetState(0, stateAdress);

    if (dwResult == ERROR_SUCCESS) {
        //std::cout << "Successful Controller Connection";
        return true;
    }
    else {

        ZeroMemory(stateAdress, sizeof(XINPUT_STATE));
        //std::cout << "Error in controller connection";
        return false;
    }
}

bool sendCommand(unsigned char command) {
    std::cout << "Command :" << int(command) << " was sent" << std::endl;
    std::string cmd1;
    cmd1 += (command);
    COMSend(cmd1);
    return true;
};

bool controllerStateChange(XINPUT_STATE state, int* input) {
    int initialInput[16];
    bool change = false;
    for (int i = 0; i < 16; i++) {
        initialInput[i] = input[i];
    };
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_A) != 0) {
        input[0] = 1;
    }
    else {
        input[0] = 0;
    };
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_B) != 0) {
        input[1] = 1;
    }
    else {
        input[1] = 0;
    };
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_X) != 0) {
        input[2] = 1;
    }
    else {
        input[2] = 0;
    };
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) != 0) {
        input[3] = 1;
    }
    else {
        input[3] = 0;
    };
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) != 0) {
        input[4] = 1;
    }
    else {
        input[4] = 0;
    };
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) != 0) {
        input[5] = 1;
    }
    else {
        input[5] = 0;
    };
    if (state.Gamepad.bLeftTrigger == 255) {
        input[6] = 1;
    }
    else {
        input[6] = 0;
    };
    if (state.Gamepad.bRightTrigger == 255) {
        input[7] = 1;
    }
    else {
        input[7] = 0;
    }
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) != 0) {
        input[8] = 1;
    }
    else {
        input[8] = 0;
    }
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) != 0) {
        input[9] = 1;
    }
    else {
        input[9] = 0;
    }
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) != 0) {
        input[10] = 1;
    }
    else {
        input[10] = 0;
    }
    if ((state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) != 0) {
        input[11] = 1;
    }
    else {
        input[11] = 0;
    }
    input[12] = state.Gamepad.sThumbLX;
    input[13] = state.Gamepad.sThumbLY;
    input[14] = state.Gamepad.sThumbRX;
    input[15] = state.Gamepad.sThumbRY;

    for (int i = 0; i < 12; i++) {
        if ((initialInput[i] != input[i]) && (input[i] != 0)) {
            sendCommand(commandEncoder(i,1));
            //std::cout << (int)commandEncoder(i, 1) << '\n';
            change = true;
        }
    }

    for (int i = 12; i < 16; i++) {
        if (initialInput[i] != input[i]) {
            //std::cout << '\t' << input[i] << '\n';
            //std::cout << (int)commandEncoder(i, input[i]) << '\n';
            sendCommand(commandEncoder(i,input[i]));
            change = true;
        }
    }

    return change;

}

unsigned char commandEncoder(int buttonNumber, int buttonValue) {
    unsigned char encodedValue;
    for (int i = 0; i < 12; i++) {
        if ((buttonNumber == i) && (buttonValue != 0)) {
            encodedValue = static_cast<unsigned char>(i);
            return encodedValue;
        }
    }
    if (buttonNumber == 12) {
        int j = 12 + 32 + (buttonValue / 1024);
        encodedValue = static_cast<unsigned char>(j);
        return encodedValue;
    }
    else if (buttonNumber == 13) {
        int j = 76 + 32 + (buttonValue / 1024);
        encodedValue = static_cast<unsigned char>(j);
        return encodedValue;
    }
    else if (buttonNumber == 14) {
        int j = 140 + 32 + (buttonValue / 1024);
        encodedValue = static_cast<unsigned char>(j);
        return encodedValue;
    }
    else if (buttonNumber == 15) {
        int j = 204 + 16 + (buttonValue / 2048);
        encodedValue = static_cast<unsigned char>(j);
        return encodedValue;
    }

    return 0;
}
