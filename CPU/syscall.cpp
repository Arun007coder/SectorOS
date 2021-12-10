#include "syscall.h"

void printf(char*);
PowerControl syscontrol;

SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber)
:InterruptHandler(InterruptNumber  + interruptManager->HardwareInterruptOffset(), interruptManager)
{
}

SyscallHandler::~SyscallHandler()
{
}

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState* cpu = (CPUState*)esp;


    switch(cpu->eax)
    {
        case 1: // sys_print
            printf((char*)cpu->ebx);
            break;

        case 2: // sys_reboot
            syscontrol.reboot();
            break;

        default:
            break;
    }


    return esp;
}