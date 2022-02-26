#include "GPF.h"

void printf(char *);
void printHex(uint8_t);

/*

GENERAL PROTECTION FAULT

A General Protection Fault may occur for various reasons. The most common are:

* Segment error (privilege, type, limit, read/write rights).
* Executing a privileged instruction while CPL != 0.
* Writing a 1 in a reserved register field or writing invalid value combinations (e.g. CR0 with PE=0 and PG=1).
* Referencing or accessing a null-descriptor.
* The saved instruction pointer points to the instruction which caused the exception.

Source: https://wiki.osdev.org/General_Protection_Fault

*/

EXCEPTIONS_GPF::EXCEPTIONS_GPF(InterruptManager* interruptManager)
    : InterruptHandler(0x0D, interruptManager)
{
}

EXCEPTIONS_GPF::~EXCEPTIONS_GPF()
{
}

uint32_t EXCEPTIONS_GPF::HandleInterrupt(uint32_t esp)
{
    CPUState* CSTATE = (CPUState*)esp;
    printf("\5");
    printf("A Kernel Panic Occured. HALTING...                 \nSYSMSG_EXCEPTION:  General Protection Fault\n\n");
    printf("An GPF exception has occured in the system during execution which cannot fix itself.The general protection fault is called when a function is improperly executed which leads to memory corruption. Restarting may solve the problem. If this happens again, Report it on github.com/Arun007coder/SectorOS/issues \n\n");
    printf("Extended stack pointer = 0x");
    printHex32(esp);
    printf("\nExtended instruction pointer  = 0x");
    printHex32(CSTATE->eip);
    printf("\nCode segment selector = 0x");
    printHex32(CSTATE->cs);
    printf("\nError code = 0x");
    printHex32(CSTATE->error);
    printf("\n");
    printf("Registers:\n");
    printf("EAX = 0x");
    printHex32(CSTATE->eax);
    printf("\nEBX = 0x");
    printHex32(CSTATE->ebx);
    printf("\nECX = 0x");
    printHex32(CSTATE->ecx);
    printf("\nEDX = 0x");
    printHex32(CSTATE->edx);
    printf("\nESI = 0x");
    printHex32(CSTATE->esi);
    printf("\nEDI = 0x");
    printHex32(CSTATE->edi);
    printf("\nEBP = 0x");
    printHex32(CSTATE->ebp);
    Power.halt();
    return esp;
}