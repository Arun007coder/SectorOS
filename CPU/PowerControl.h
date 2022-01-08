#ifndef __POWERCONTROL_H
#define __POWERCONTROL_H

#include "../Include/types.h"
#include "../Drivers/IOPorts.h"

class PowerControl
{
public:
    //To reboot the computer
    void reboot();
    //To halt the CPU
    void halt();
    //If running in virtualBox. This function will Stop virtualBox
    void StopVirtualBox();
    void shutdown(); //!: This function is not implemented yet

    //To get the current power state
    bool getPowerState();
};

#endif