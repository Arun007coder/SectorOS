#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../Include/types.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"
#include "Driver.h"
#include "RTC.h"
#include "../CPU/PowerControl.h"
#include "../Hardcom/SerialPort.h"
#include "VGADriver.h"
#include "../Include/Public_VAR.h"
#include "../Hardcom/pci.h"
#include "../Include/multiboot.h"
#include "HDD-ATA.h"

class CustomShell
{
public:
    CustomShell();
    ~CustomShell();
    virtual void Shell();
    virtual void clearBuffer();
    char* CharBuffer[256];
    int CharBufferIndex;

};

class KeyboardDriver : public InterruptHandler, public Driver // Driver for keyboard
{
    port8BIT DataPort;
    port8BIT CommandPort;

    // SOSH v1.0.3 [SectorOS SHell]
    // To interpret and execute the command
    void CommandInterpreter();
    SerialPort serialport;

public:
    KeyboardDriver(InterruptManager* manager, CustomShell* cshell);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    // To change the keycode to ascii to write it into string
    TaskManager taskManager;
    uint32_t esp1;
    CustomShell* shell;
    uint32_t esp2;
    char* KeycodeToASCII(uint8_t Keycode);
    // To clear the keystrokes in the keyboard buffer
    void clear_key_buffer(); 
    // To activate the keyboard driver
    virtual void activate();
    // To print text like when booting up 
    void returnHScreen();

    //void debugport(uint16_t portno, uint8_t writedat, int porttype, int action);

    bool isTxtMode;
};

#endif