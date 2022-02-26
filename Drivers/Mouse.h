#ifndef __MOUSE_H
#define __MOUSE_H

#include "../Include/types.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"
#include "Driver.h"

class MouseEventHandler
{
public:
    MouseEventHandler();
    virtual void OnActivate();
    virtual void OnMouseDown(uint8_t button);
    virtual void OnMouseUp(uint8_t button);
    virtual void OnMouseMove(int x, int y);
};

class MouseDriver : public InterruptHandler, public Driver // Driver for mouse
{
    port8BIT DataPort;
    port8BIT CommandPort;

    uint8_t buffer[3];
    uint8_t offset;
    uint8_t buttons;

    MouseEventHandler* handler;

public:
    MouseDriver(InterruptManager* manager, MouseEventHandler* handler);
    ~MouseDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    // To activate the Mouse driver
    virtual void activate();
    int UniquedriverID();
};

#endif