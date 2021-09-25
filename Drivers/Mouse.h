#ifndef __MOUSE_H
#define __MOUSE_H

#include "../Includes/types.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"

class MouseDriver : public InterruptHandler // Driver for mouse
{
    port8BIT DataPort;
    port8BIT CommandPort;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    int8_t x, y;

public:
    MouseDriver(InterruptManager* manager);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);

};

#endif