#include "JD1618.h"

void printf(char *);
void printfchar(char st);

struct CPU1618::Memory mem;
CPU1618::CPU cpu;

CPU1618::CPU1618()
{
    cpu.I = 0;
}

CPU1618::~CPU1618()
{
}

void CPU1618::Memory::InitializeMEM()
{
    for (uint32_t i = 0; i < MAX_MEM; i++)
    {
        RAM[i] = 0;
    }
}

void CPU1618::CPU::ResetCPU(Memory &mem)
{
    PC = 0xFFFC;
    SP = 0x0100;
    C = Z = I = D = B = V = N = E = 0;
    A = 0;
    X = 0;
    Y = 0;
    Q = 0;
    K = 0;

    mem.InitializeMEM();
}

uint8_t CPU1618::CPU::FetchByte(uint32_t &ClockCycles, Memory &mem)
{
    uint8_t Result = mem[PC];
    PC++;
    ClockCycles--;

    return Result;
}

uint8_t CPU1618::CPU::ReadByte(uint32_t &ClockCycles, uint8_t Address, Memory &mem)
{
    uint8_t Result = mem[Address];
    ClockCycles--;

    return Result;
}

void CPU1618::CPU::ExecuteFromMemory(Memory &mem)
{
    bool isFaulted = true;
    bool isInterruptsenabled = false;

    uint32_t ClockCycles = 1;
    uint8_t Interrupt = 0;
    printf("Register PC = 0x");
    printHex16(PC);
    printf("\n");

    while (ClockCycles > 0)
    {
        if (I == 0)
        {
            isInterruptsenabled = false;
        }
        else
        {
            isInterruptsenabled = true;
        }
        ClockCycles = 1;
        uint8_t INSTRUCTION = FetchByte(ClockCycles, mem);
        // ClockCycles = 0;
        switch (INSTRUCTION)
        {
        case INS_MV:
        case INS_ADD:
        case INS_SUB:
        {
            ClockCycles = 5;
        }
        break;
        case INS_JMP:
        case INS_INT:
        case INS_JZ:
        case INS_JNZ:
        case INS_JE:
        case INS_JNE:
        case INS_INC:
        case INS_DEC:
        case INS_CMP:
        {
            ClockCycles = 4;
        }
        break;
        case INS_STI:
        case INS_CLI:
        case INS_NDE:
        case INS_SDE:
        case INS_HLT:
        {
            ClockCycles = 3;
        }
        break;
        default:
        {
            ClockCycles = 0;
        }
        break;
        }
        char *insname = (char*)NULL;
        uint8_t ADDRESS = 0;
        uint8_t REG = 0;

        switch (INSTRUCTION)
        {
        case INS_MV:
        {
            insname = (char *)"MOV";
            uint8_t destination = FetchByte(ClockCycles, mem);
            uint8_t source = FetchByte(ClockCycles, mem);

            if (destination == 0x01)
            {
                if (source == 0x02)
                {
                    A = X;
                    X = 0;
                }
                else if (source == 0x03)
                {
                    A = Y;
                    Y = 0;
                }
                else if (source == 0x04)
                {
                    A = Q;
                    Q = 0;
                }
                else if (source == 0x05)
                {
                    A = K;
                    K = 0;
                }
                else
                {
                    if (OF == 0x00)
                        A = mem.RAM[source];
                    else
                        A = mem.RAM[source * OF];
                }
            }
            else if (destination == 0x02)
            {
                if (source == 0x01)
                {
                    X = A;
                    A = 0;
                }
                else if (source == 0x03)
                {
                    X = Y;
                    Y = 0;
                }
                else if (source == 0x04)
                {
                    X = Q;
                    Q = 0;
                }
                else if (source == 0x05)
                {
                    X = K;
                    K = 0;
                }
                else
                {
                    if (OF == 0x00)
                        X = mem.RAM[source];
                    else
                        X = mem.RAM[source * OF];
                }
            }
            else if (destination == 0x03)
            {
                if (source == 0x01)
                {
                    Y = A;
                    A = 0;
                }
                else if (source == 0x02)
                {
                    Y = X;
                    X = 0;
                }
                else if (source == 0x04)
                {
                    Y = Q;
                    Q = 0;
                }
                else if (source == 0x05)
                {
                    Y = K;
                    K = 0;
                }
                else
                {
                    if (OF == 0x00)
                        Y = mem.RAM[source];
                    else
                        Y = mem.RAM[source * OF];
                }
            }
            else if (destination == 0x04)
            {
                if (source == 0x01)
                {
                    Q = A;
                    A = 0;
                }
                else if (source == 0x02)
                {
                    Q = X;
                    X = 0;
                }
                else if (source == 0x03)
                {
                    Q = Y;
                    Q = 0;
                }
                else if (source == 0x05)
                {
                    Q = K;
                    K = 0;
                }
                else
                {
                    if (OF == 0x00)
                        Q = mem.RAM[source];
                    else
                        Q = mem.RAM[source * OF];
                }
            }
            else if (destination == 0x06)
            {
                if (source == 0x01)
                {
                    OF = A;
                    A = 0;
                }
                else if (source == 0x02)
                {
                    OF = X;
                    X = 0;
                }
                else if (source == 0x03)
                {
                    OF = Y;
                    Y = 0;
                }
                else if (source == 0x04)
                {
                    OF = Q;
                    Q = 0;
                }
                else
                {
                    OF = mem.RAM[source];
                }
            }
            else
            {
                if (source == 0x01)
                {
                    if (OF == 0x00)
                        mem.RAM[destination] = A;
                    else
                        mem.RAM[destination * OF] = A;
                }
                else if (source == 0x02)
                {
                    if (OF == 0x00)
                        mem.RAM[destination] = X;
                    else
                        mem.RAM[destination * OF] = X;
                }
                else if (source == 0x03)
                {
                    if (OF == 0x00)
                        mem.RAM[destination] = Y;
                    else
                        mem.RAM[destination * OF] = Y;
                }
                else if (source == 0x04)
                {
                    if (OF == 0x00)
                        mem.RAM[destination] = Q;
                    else
                        mem.RAM[destination * OF] = Q;
                }
                else
                {
                    mem.RAM[destination] = mem.RAM[source];
                }
            }
            ClockCycles--;
        }
        break;
        case INS_JMP:
        {
            insname = (char *)"JMP";
            ADDRESS = FetchByte(ClockCycles, mem);
            if (OF == 0x00)
                PC = ADDRESS;
            else
                PC = ADDRESS * OF;
            ClockCycles--;
        }
        break;
        case INS_JZ:
        {
            insname = (char *)"JZ";
            ADDRESS = FetchByte(ClockCycles, mem);
            if (Z == 1)
            {
                if (OF == 0x00)
                    PC = ADDRESS;
                else
                    PC = ADDRESS * OF;
            }
            ClockCycles--;
        }
        break;
        case INS_JNZ:
        {
            insname = (char *)"JNZ";
            ADDRESS = FetchByte(ClockCycles, mem);
            if (Z == 0)
            {
                if (OF == 0x00)
                    PC = ADDRESS;
                else
                    PC = ADDRESS * OF;
            }
            ClockCycles--;
        }
        break;
        case INS_JE:
        {
            insname = (char *)"JE";
            ADDRESS = FetchByte(ClockCycles, mem);
            if (E == 1)
            {
                if (OF == 0x00)
                    PC = ADDRESS;
                else
                    PC = ADDRESS * OF;
            }
            ClockCycles--;
        }
        break;
        case INS_JNE:
        {
            insname = (char *)"JNE";
            ADDRESS = FetchByte(ClockCycles, mem);
            if (E == 0)
            {
                if (OF == 0x00)
                    PC = ADDRESS;
                else
                    PC = ADDRESS * OF;
            }
            ClockCycles--;
        }
        break;
        case INS_DEC:
        {
            insname = (char *)"DEC";
            REG = FetchByte(ClockCycles, mem);
            if (REG == 0x01)
            {
                A--;
                Z = (A == 0);
                N = (A & 0B10000000) > 0;
            }
            else if (REG == 0x02)
            {
                X--;
                Z = (X == 0);
                N = (X & 0B10000000) > 0;
            }
            else if (REG == 0x03)
            {
                Y--;
                Z = (Y == 0);
                N = (Y & 0B10000000) > 0;
            }
            else if (REG == 0x04)
            {
                Q--;
                Z = (Q == 0);
                N = (Q & 0B10000000) > 0;
            }
            ClockCycles--;
        }
        break;
        case INS_INC:
        {
            insname = (char *)"INC";
            REG = FetchByte(ClockCycles, mem);
            if (REG == 0x01)
                A++;
            else if (REG == 0x02)
                X++;
            else if (REG == 0x03)
                Y++;
            else if (REG == 0x04)
                Q++;
            ClockCycles--;
        }
        break;
        case INS_ADD:
        {
            insname = (char *)"ADD";
            uint8_t Dest = FetchByte(ClockCycles, mem);
            uint8_t Sour = FetchByte(ClockCycles, mem);

            if (Dest == 0x01)
            {
                if (Sour == 0x02)
                    A = X + A;
                else if (Sour == 0x03)
                    A = Y + A;
            }
            else if (Dest == 0x02)
            {
                if (Sour == 0x01)
                    X = A + X;
                else if (Sour == 0x03)
                    X = Y + X;
            }
            else if (Dest == 0x03)
            {
                if (Sour == 0x01)
                    Y = A + Y;
                else if (Sour == 0x02)
                    Y = X + Y;
            }
            ClockCycles--;
        }
        break;
        case INS_SUB:
        {
            insname = (char *)"SUB";
            uint8_t D = FetchByte(ClockCycles, mem);
            uint8_t S = FetchByte(ClockCycles, mem);

            if (D == 0x01)
            {
                if (S == 0x02)
                {
                    A = X - A;
                    Z = (A == 0);
                    N = (A & 0B10000000) > 0;
                }
                else if (S == 0x03)
                {
                    A = Y - A;
                    Z = (A == 0);
                    N = (A & 0B10000000) > 0;
                }
            }
            else if (D == 0x02)
            {
                if (S == 0x01)
                {
                    X = A - X;
                    Z = (X == 0);
                    N = (X & 0B10000000) > 0;
                }
                else if (S == 0x03)
                {
                    X = Y - X;
                    Z = (X == 0);
                    N = (X & 0B10000000) > 0;
                }
            }
            else if (D == 0x03)
            {
                if (S == 0x01)
                {
                    Y = A - Y;
                    Z = (Y == 0);
                    N = (Y & 0B10000000) > 0;
                }
                else if (S == 0x02)
                {
                    Y = X - Y;
                    Z = (Y == 0);
                    N = (Y & 0B10000000) > 0;
                }
            }
            ClockCycles--;
        }
        break;
        case INS_CMP:
        {
            insname = (char *)"CMP";
            uint8_t SO = FetchByte(ClockCycles, mem);
            uint8_t DE = FetchByte(ClockCycles, mem);

            if (SO == 0x01 & DE == 0x02)
            {
                E = (A == X);
            }
            else if (SO == 0x01 & DE == 0x03)
            {
                E = (A == Y);
            }
            else if (SO == 0x02 & DE == 0x01)
            {
                E = (X == A);
            }
            else if (SO == 0x02 & DE == 0x03)
            {
                E = (X == Y);
            }
            else if (SO == 0x03 & DE == 0x01)
            {
                E = (Y == A);
            }
            else if (SO == 0x03 & DE == 0x02)
            {
                E = (Y == X);
            }
            else if (DE == 0x01)
            {
                E = (A == SO);
            }
            else if (SO == 0x01)
            {
                E = (A == DE);
            }
            else if (DE == 0x02)
            {
                E = (X == SO);
            }
            else if (SO == 0x02)
            {
                E = (X == DE);
            }
            else if (DE == 0x03)
            {
                E = (Y == SO);
            }
            else if (SO == 0x03)
            {
                E = (Y == DE);
            }
            else
            {
                E = 10;
            }
            ClockCycles--;
        }
        break;
        case 0x00:
        {
            printf("The instruction in memory address 0x");
            printHex16(PC);
            printf(" cannot be executed because it is zero\n");
            insname = (char *)"ZOP";
            // isFaulted = true;
        }
        break;
        case INS_INT:
        {
            Interrupt = FetchByte(ClockCycles, mem);
            if (isInterruptsenabled)
            {
                insname = (char *)"INT";
                switch (Interrupt)
                {
                case INT_PC:
                {
                    isFaulted = true;
                    printfchar((char)Y);
                }
                break;
                case INT_SRP:
                {
                    printf("status registers :\n");
                    printf("C = "); printf((char*)(C ? "1" : "0"));printf("\n");
                    printf("E = "); printf((char*)(E ? "1" : "0"));printf("\n");
                    printf("Z = "); printf((char*)(Z ? "1" : "0"));printf("\n");
                    printf("I = "); printf((char*)(I ? "1" : "0"));printf("\n");
                    printf("D = "); printf((char*)(D ? "1" : "0"));printf("\n");
                    printf("B = "); printf((char*)(B ? "1" : "0"));printf("\n");
                    printf("V = "); printf((char*)(V ? "1" : "0"));printf("\n");
                    printf("N = "); printf((char*)(N ? "1" : "0"));printf("\n");
                }
                break;
                case INT_CLS:
                {
                    printf("\5");
                }
                break;
                }
            }
        }
        break;
        case INS_CLI:
        {
            insname = (char *)"CLI";
            I = 0;
        }
        break;
        case INS_STI:
        {
            insname = (char *)"STI";
            I = 1;
        }
        break;
        case INS_NOP:
        {
            insname = (char *)"NOP";
            // isFaulted = true;
        }
        break;
        case INS_HLT:
        {
            isFaulted = true;
            ResetCPU(mem);
            asm("hlt");
        }
        break;
        case INS_SDE:
        {
            insname = (char *)"SDE";
            isFaulted = false;
        }
        break;
        case INS_NDE:
        {
            insname = (char *)"NDE";
            isFaulted = true;
        }
        default:
        {
            printf("The instruction 0x");
            printHex(INSTRUCTION);
            printf(" is not implemented.\n");
            insname = (char *)"IOC";
        }
        break;
        }
        if (!isFaulted)
        {
            if (insname != "INT")
            {
                printf("The instruction is 0x");
                printHex(INSTRUCTION);
                printf(" | ");
                printf(insname);
                printf(" \n");
            }
            else
            {
                printf("The instruction is 0x");
                printHex(INSTRUCTION);

                printf(" | ");
                printf(insname);
                printf(" ");
                printHex(Interrupt);
                printf("h \n");
            }
            printf("Register A = 0x"); printHex(A); printf("\n");
            printf("Register X = 0x"); printHex(X); printf("\n");
            printf("Register Y = 0x"); printHex(Y); printf("\n");
            printf("Register Q = 0x"); printHex(Q); printf("\n");
            printf("Offset Register = 0x");
            printHex(OF);
            printf("\n");
            printf("Register PC = 0x");
            printHex16(PC);
            printf("\n");
        }
    }
}

void CPU1618::System::SystemReset(Memory &mem, CPU &cpu)
{
    cpu.ResetCPU(mem);
}

void CPU1618::System::LoadPRG(uint8_t* PRG, uint32_t SIZE)
{
    if (PRG && SIZE != 2)
    {
        uint32_t at = 0;
        const uint16_t LoadAddress = PRG[at++] | (PRG[at++] << 8);
        cpu.PC = LoadAddress;
        for(uint16_t i = LoadAddress; i < LoadAddress + SIZE - 2; i++)
        {
            mem[i] = PRG[at++];
        }
    }
}

void CPU1618::System::Start()
{
    cpu.ResetCPU(mem);
}

void CPU1618::System::StartExecution()
{
    cpu.ExecuteFromMemory(mem);
}