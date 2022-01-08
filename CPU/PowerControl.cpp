#include "PowerControl.h"

extern uint32_t shutdown();

void PowerControl::reboot()
{
    uint8_t good = 0x02;
    port8BIT pt(0x64);
    while (good & 0x02)
        good = pt.ReadFromPort();
    pt.WriteToPort(0xFE);
}

void PowerControl::halt()
{
    __asm__ volatile("hlt");
}

void PowerControl::StopVirtualBox()
{
    port32BIT pt(0x4004);
    pt.WriteToPort(0x3400);
}

bool PowerControl::getPowerState()
{
    port8BIT pt(0xB0);
    return pt.ReadFromPort() & 0x01;
}

void PowerControl::shutdown()
{
    shutdown();
}