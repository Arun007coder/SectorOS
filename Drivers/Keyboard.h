#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../Includes/types.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"

class KeyboardDriver : public InterruptHandler
{
    port8BIT DataPort;
    port8BIT CommandPort;
public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    char KeycodeToASCII(uint8_t Keycode);
    char key_buffer[256];
    void clear_key_buffer();
};

#endif