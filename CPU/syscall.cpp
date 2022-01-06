#include "syscall.h"

void printf(char*);
void printfchar(char);
PowerControl syscontrol;

inline GlobalDescriptorTable gdt;
void taskA();
void taskB();

Task tsk1(&gdt, taskA);
Task tsk2(&gdt, taskB);

SyscallHandler::SyscallHandler(InterruptManager* interruptManager, uint8_t InterruptNumber)
:InterruptHandler(InterruptNumber  + interruptManager->HardwareInterruptOffset(), interruptManager)
{
    taskManager.AddTask(&tsk1);
    taskManager.AddTask(&tsk2);
}

SyscallHandler::~SyscallHandler()
{
}

uint32_t SyscallHandler::HandleInterrupt(uint32_t esp)
{
    CPUState* cpu = (CPUState*)esp;


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
            esp = (uint32_t)taskManager.SwitchTask((int)cpu->ebx, (CPUState*)esp);
            break;

        default:
            break;
    }


    return esp;
}