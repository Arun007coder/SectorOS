#include "SerialPort.h"

void printf(char*);
void printfchar(char);

uint16_t PORT = 0x3f8;

uint16_t PORT5 = PORT + 5;

port8BIT sendport(PORT5);

int SerialPort::INITSerial()
{
    printf("Serial port COM1 initialising...\n");
    port8BIT port1(PORT + 1);
    port8BIT port3(PORT + 3);
    port8BIT port0(PORT + 0);
    port8BIT port2(PORT + 2);
    port8BIT port4(PORT + 4);
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
        printf("bad serial port");
        return 1;
    }

    port4.WriteToPort(0x0F);
    return 0;
}

SerialPort::SerialPort()
{
    INITSerial();
}

SerialPort::~SerialPort()
{
}

int SerialPort::isSerialEmpty()
{
    return sendport.ReadFromPort() & 0x20;
}

void SerialPort::logToSerialPort(char* msg)
{
   while (isSerialEmpty() == 0);

   port8BIT datport(PORT);
 
    //printf("wrote "); printf("'"); printf(msg);printf("'");printf(" to serial port COM1\n");
    for(int i = 0; msg[i] != '\0'; i++)
    {
        //printfchar(msg[i]);
        datport.WriteToPort(msg[i]);
    }
}