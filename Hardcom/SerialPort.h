#ifndef __SERIALPORT_H
#define __SERIALPORT_H

#include "../Include/types.h"
#include "../Drivers/IOPorts.h"


class SerialPort
{
public:
    static int INITSerial();
    SerialPort();
    ~SerialPort();
    void logToSerialPort(char* msg);
    int isSerialEmpty();
    int serial_received();
    char read_serial();
};

#endif