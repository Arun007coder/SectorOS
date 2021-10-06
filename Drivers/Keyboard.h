#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../Includes/types.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"
#include "Driver.h"

class KeyboardDriver : public InterruptHandler, public Driver // Driver for keyboard
{
    port8BIT DataPort;
    port8BIT CommandPort;
    // To interpret and execute the command
    void CommandInterpreter();

public:
    KeyboardDriver(InterruptManager* manager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    // To change the keycode to ascii to write it into string
    char* KeycodeToASCII(uint8_t Keycode);
    char* key_buffer[256]; // To Save the keystrokes in memory to use them later. Max number keystroke to save in the buffer is 256 Chars
    // To clear the keystrokes in the keyboard buffer
    void clear_key_buffer(); 
    // To activate the keyboard driver
    virtual void activate();
};

#endif