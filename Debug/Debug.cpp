#include "Debug.h"

uint16_t PORT = 0x3f8;

uint16_t PORT5 = PORT + 5;

port16BIT sendport(PORT5);

int debugfunctions::INITSerial()
{
    port16BIT port1(PORT + 1);
    port16BIT port3(PORT + 3);
    port16BIT port0(PORT + 0);
    port16BIT port2(PORT + 2);
    port16BIT port4(PORT + 4);
    port1.WriteToPort(0x00);
    port3.WriteToPort(0x80);    
    port0.WriteToPort(0x03);    
    port1.WriteToPort(0x00);
    port2.WriteToPort(0xC7);
    port4.WriteToPort(0xC7);
    port4.WriteToPort(0x0B); 
    port4.WriteToPort(0x1E);
    port0.WriteToPort(0x02);
    port0.WriteToPort(0xAE);
    
    if(port0.ReadFromPort() != 0xAE) {
        return 1;
    }

    port4.WriteToPort(0x0F);
    return 0;
}

debugfunctions::debugfunctions()
{
    INITSerial();
}

debugfunctions::~debugfunctions()
{
}

int debugfunctions::isSerialEmpty()
{
    return sendport.ReadFromPort() & 0x20;
}

void debugfunctions::logToSerialPort(char* msg)
{
   while (isSerialEmpty() == 0);

   port16BIT datport(PORT5);
 
   for(int i = 0; msg[i] != '\0'; i++)
   {
       datport.WriteToPort(0x61);
   }
}