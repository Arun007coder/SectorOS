#include "DFault.h"

void printf(char *);
void printHex(uint8_t);

/*
DOUBLE FAULT

A Double Fault occurs when an exception is unhandled or when an exception occurs while the CPU is trying to call an exception handler. Normally, two exception at the same time are handled one after another, but in some cases that is not possible. For example, if a page fault occurs, but the exception handler is located in a not-present page, two page faults would occur and neither can be handled. A double fault would occur.
A double fault will always generate an error code with a value of zero.

The saved instruction pointer is undefined. A double fault cannot be recovered. The faulting process must be terminated.

In several starting hobby OSes, a double fault is also quite often a misdiagnosed IRQ0 in the cases where the PIC hasn't been reprogrammed yet.

Source: https://wiki.osdev.org/Double_Fault
*/

EXECPTION_DOUBLE_FAULT::EXECPTION_DOUBLE_FAULT(InterruptManager* interruptManager)
    : InterruptHandler(0x08, interruptManager)
{

}

EXECPTION_DOUBLE_FAULT::~EXECPTION_DOUBLE_FAULT()
{

}

uint32_t EXECPTION_DOUBLE_FAULT::HandleInterrupt(uint32_t esp)
{
    CPUState* CSTATE = (CPUState*)esp;
    printf("\5");
    printf("A Kernel Panic Occured. HALTING...                 \nSYSMSG_EXCEPTION:  Double Fault\n\n");
    printf("A double fault has occured in the system during execution. Double fault occures when the os cannot handle the previous exeption or an exeption occured when cpu tried to call the execption handler. Restarting may solve the problem. If this happens again, Report it on github.com/Arun007coder/SectorOS/issues \n\n");
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