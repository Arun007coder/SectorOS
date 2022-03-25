#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "../Include/types.h"
#include "../Include/func.h"
#include "AM79C973.h"
#include "IOPorts.h"
#include "../CPU/Interrupts.h"
#include "Driver.h"
#include "RTC.h"
#include "../CPU/PowerControl.h"
#include "../Hardcom/SerialPort.h"
#include "VGADriver.h"
#include "../Include/Public_VAR.h"
#include "../Include/multiboot.h"
#include "HDD-ATA.h"
#include "../Filesystem/FATFS.h"

class CustomShell
{
public:
    CustomShell();
    ~CustomShell();
    virtual void Shell();
    virtual void clearBuffer();
    virtual void OnKeyDown(uint8_t key);
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
    KeyboardDriver(InterruptManager *manager);
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    CustomShell *shell;
    TaskManager taskManager;
    uint32_t esp1;
    uint32_t esp2;
    char *key_buffer[256];    // To Save the keystrokes in memory to use them later. Max number keystroke to save in the buffer is 256 Chars
    int key_buffer_index = 0; // The index of the key_buffer
    void ChangeEventHandler(CustomShell* shell);
    void ResetEventHandler();

    // To clear the keystrokes in the keyboard buffer
    void clear_key_buffer(); 
    // To activate the keyboard driver
    virtual void activate();
    int UniquedriverID();
    // To print text like when booting up 
    void returnHScreen();

    //void debugport(uint16_t portno, uint8_t writedat, int porttype, int action);

    bool isTxtMode;
};

#endif