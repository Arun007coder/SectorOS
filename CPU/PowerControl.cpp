#include "PowerControl.h"


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