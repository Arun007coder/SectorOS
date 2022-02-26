#include "IOC.h"

void printf(char *);
void printHex(uint8_t);

/*

INVALID OPCODE

The Invalid Opcode exception occurs when the processor tries to execute an invalid or undefined opcode, or an instruction with invalid prefixes. It also occurs in other cases, such as:

* The instruction length exceeds 15 bytes, but this only occurs with redundant prefixes.
* The instruction tries to access a non-existent control register (for example, mov cr6, eax).
* The UD instruction is executed.

The saved instruction pointer points to the instruction which caused the exception.

*/

EXCEPTIONS_IOC::EXCEPTIONS_IOC(InterruptManager* interruptManager)
    : InterruptHandler(0x06, interruptManager)
{
}

EXCEPTIONS_IOC::~EXCEPTIONS_IOC()
{
}

uint32_t EXCEPTIONS_IOC::HandleInterrupt(uint32_t esp)
{
    CPUState* CSTATE = (CPUState*)esp;
    printf("\5");
    printf("A Kernel Panic Occured. HALTING...                 \nSYSMSG_EXCEPTION:  INVALID OPCODE\n\n");
    printf("This is an invalid opcode exception which is caused by an invalid opcode or an instruction with invalid prefixes. Restarting may solve the problem. If this happens again, Report it on github.com/Arun007coder/SectorOS/issues \n\n");
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