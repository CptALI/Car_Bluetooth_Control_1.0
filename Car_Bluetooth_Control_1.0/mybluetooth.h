#ifndef MYBLUETOOTH_H
#define MYBLUETOOTH_H

#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <string>

#define COM "COM8"

extern HANDLE hSerial;

bool COMInit(const TCHAR*);
void COMEnd();
bool COMSend(const std::string&);
std::string COMRead();
bool BTInit();

#endif 

