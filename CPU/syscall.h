#ifndef __SYSCALL_H
#define __SYSCALL_H

#include "../Include/types.h"
#include "Interrupts.h"
#include "../Include/Public_VAR.h"
#include "PowerControl.h"
#include "../Drivers/HDD-ATA.h"
#include "../Drivers/VGADriver.h"
#include "../Memory/MemoryManagement.h"
#include "../kernel/MultiTask.h"

class SyscallHandler : public InterruptHandler
{
    TaskManager taskManager;
public:
    SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber);
    ~SyscallHandler();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif