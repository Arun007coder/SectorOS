#ifndef __DEBUG_H
#define __DEBUG_H

#include "../Includes/types.h"
#include "../Drivers/IOPorts.h"


class debugfunctions
{
public:
    static int INITSerial();
    debugfunctions();
    ~debugfunctions();
    void logToSerialPort(char* msg);
    int isSerialEmpty();
};

#endif