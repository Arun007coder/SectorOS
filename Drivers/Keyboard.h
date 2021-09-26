#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../Includes/types.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"
#include "Driver.h"

class KeyboardEventHandler
{
public:
    KeyboardEventHandler();

    virtual void onKeyDown(char);
    virtual void onKeyUp(char);
};

class KeyboardDriver : public InterruptHandler, public Driver // Driver for keyboard
{
    port8BIT DataPort;
    port8BIT CommandPort;
    void CLI();

    KeyboardEventHandler* handler;
public:
    KeyboardDriver(InterruptManager* manager, KeyboardEventHandler *handler);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    char* KeycodeToASCII(uint8_t Keycode);
    char* key_buffer[256]; // Max number keystroke to save in the buffer is 256 Chars
    void clear_key_buffer(); // To clear the keystrokes in the keyboard buffer
    virtual void activate();
};

#endif