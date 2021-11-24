#ifndef __SYSCALL_H
#define __SYSCALL_H

#include "../Includes/types.h"
#include "Interrupts.h"

class SyscallHandler : public InterruptHandler
{
public:
    SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber);
    ~SyscallHandler();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif