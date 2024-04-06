#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

#include "mybluetooth.h"

HANDLE hSerial;

bool COMInit(const TCHAR* portName) {
    hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error: Unable to open COM port\n";
        return false;
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: Unable to get COM port state\n";
        CloseHandle(hSerial);
        return false;
    }
    dcbSerialParams.BaudRate = CBR_38400;  // Set your desired baud rate here
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = 0; // Set stop bits to 0
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error: Unable to set COM port state\n";
        CloseHandle(hSerial);
        return false;
    }

    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error: Unable to set COM port timeouts\n";
        CloseHandle(hSerial);
        return false;
    }

    return true;
}

void COMEnd() {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

bool COMSend(const std::string& data) {
    DWORD bytesWritten;
    if (!WriteFile(hSerial, data.c_str(), data.length(), &bytesWritten, NULL)) {
        std::cerr << "Error: Unable to write to COM port\n";
        return false;
    }
    return true;
}

std::string COMRead() {
    const int BUFFER_SIZE = 128;
    char buffer[BUFFER_SIZE];
    DWORD bytesRead;
    if (!ReadFile(hSerial, buffer, BUFFER_SIZE, &bytesRead, NULL)) {
        std::cerr << "Error: Unable to read from COM port\n";
        return "";
    }
    return std::string(buffer, bytesRead);
}

bool BTInit() {
    std::string command1 = "AT\r\n";
    std::string command2 = "AT+INQ\r\n";
    std::string command3 = "AT+CONN1\r\n";
    std::string answer1;
    std::string answer2;
    //std::string answer2_sub;
    std::string answer3;
    //std::string answer3_sub;
    bool condition1 = false;
    bool condition2 = false;
    bool condition3 = false;

    //////////////////////////////////////////////////////////////////////////
    Sleep(50);
    if (COMSend(command1)) {
        std::cout << "AT Checking!\n";
    }
    else {
        std::cout << "Failed to send data to master\n";
        return false;
    }
    Sleep(50);
    answer1 = COMRead();
    if (!answer1.empty()) {
        std::cout << "Received Response\n" << answer1;
    }
    else {
        std::cout << "Master module is not responding\n";
        return false;
    }
    if (answer1.find("OK") != std::string::npos) {
        condition1 = true;
    }
    else {
        std::cout << "There is an error in the first initiation command\n";
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////
    Sleep(50);
    if (COMSend(command2)) {
        std::cout << "AT Inquiring slave devices!\n";
    }
    else {
        std::cout << "Failed to send data to master\n";
        return false;
    }
    Sleep(50);
    answer2 = COMRead();
    if (!answer2.empty()) {
        std::cout << "Received Response\n" << answer2;
    }
    else {
        std::cout << "Master module is not responding\n";
        return false;
    }
    Sleep(500);
    if (answer2.find("1") != std::string::npos) {
        condition2 = true;
    }
    else {
        std::cout << "There is an error in the seccond initiation command\n";
        return false;
    }
    /////////////////////////////////////////////////////////////////////////////////////////
    Sleep(50);
    if (COMSend(command3)) {
        std::cout << "AT connecting to the device number1 !\n";
    }
    else {
        std::cout << "Failed to send data to master\n";
        return false;
    }
    Sleep(700);
    answer3 = COMRead();
    if (!answer3.empty()) {
        std::cout << "Received Response\n" << answer3;
    }
    else {
        std::cout << "Master module is not responding\n";
        return false;
    }
    if ((answer3.find("CONNECTED") != std::string::npos) || (answer3.find("Connected") != std::string::npos)) {
        condition3 = true;
    }
    else {
        std::cout << "There is an error in the third initiation command\n";
        return false;
    }
    std::cout << "BT SUCCESSFULLY INITIATED\n";
    Sleep(2000);
    return condition1 && condition2 && condition3;

}