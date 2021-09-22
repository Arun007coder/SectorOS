#include "IOPorts.h"

port::port(uint16_t portnumber)
{
    this->portnumber = portnumber;
};

port::~port()
{
}

port8BIT::port8BIT(uint16_t portnumber)
:port(portnumber)
{

}

port8BIT::~port8BIT()
{
}

void port8BIT::WriteToPort(uint8_t data)
{
    __asm__ volatile("outb %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint8_t port8BIT::ReadFromPort()
{
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

port8BITSlow::port8BITSlow(uint16_t portnumber)
:port8BIT(portnumber)
{
}

port8BITSlow::~port8BITSlow()
{
}

void port8BITSlow::WriteToPort(uint8_t data)
{
    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (portnumber));
}

port16BIT::port16BIT(uint16_t portnumber)
:port(portnumber)
{

}

port16BIT::~port16BIT()
{
}

void port16BIT::WriteToPort(uint16_t data)
{
    __asm__ volatile("outw %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint16_t port16BIT::ReadFromPort()
{
    uint16_t result;
    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}

port32BIT::port32BIT(uint16_t portnumber)
:port(portnumber)
{

}

port32BIT::~port32BIT()
{
}

void port32BIT::WriteToPort(uint32_t data)
{
    __asm__ volatile("outl %0, %1" : : "a" (data), "Nd" (portnumber));
}

uint32_t port32BIT::ReadFromPort()
{
    uint32_t result;
    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (portnumber));
    return result;
}