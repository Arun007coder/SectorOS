#include "syscall.h"

void printf(char*);
VideoGraphicsArray vga;
void printfchar(char);
PowerControl syscontrol;
AdvancedTechnologyAttachment hdd1s(0x1F0, false);

inline GlobalDescriptorTable gdt;

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
    TaskManager *TMGR = TaskManager::ActiveManager;

    switch(cpu->eax)
    {
        case 1: // sys_printf
            printf((char*)cpu->ebx);
            break;

        case 2: // sys_printc
            printfchar((char)cpu->ebx);
            break;

        case 3: // sys_reboot
            syscontrol.reboot();
            break;

        case 4: // sys_cng_task
            esp = (uint32_t)TMGR->SwitchTask((int)cpu->ebx, (CPUState*)esp);
            break;

        case 5: // sys_writehdd
            hdd1s.Write28((uint32_t)cpu->ebx, (uint8_t*)cpu->ecx, (int)cpu->edx);
            break;

        case 6: // sys_readhdd
            hdd1s.Read28((uint32_t)cpu->ebx, (uint8_t*)cpu->ecx);
            break;
            
        default:
            break;
    }


    return esp;
}