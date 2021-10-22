#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include "../Includes/types.h"
#include "../Drivers/IOPorts.h"


class SerialPort
{
public:
    static int INITSerial();
    SerialPort();
    ~SerialPort();
    void logToSerialPort(char* msg);
    int isSerialEmpty();
};

#endif