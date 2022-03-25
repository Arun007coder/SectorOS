#ifndef JD1618_H
#define JD1618_H

#include "../Drivers/HDD-ATA.h"
#include "../Include/types.h"
#include "../Include/func.h"

class CPU1618
{
public:
    CPU1618();
    ~CPU1618();

    struct Memory // Main Memory where the instructions used by the CPU to execute
    {
        static constexpr uint32_t MAX_MEM = 1024 * 64; // Maximum memory is 64 KB
        uint8_t RAM[MAX_MEM];                          // The memory array used
        void InitializeMEM();                          // To remove all Data from the RAM | Clears the ram

        uint8_t operator[](uint32_t Address) const // An operator to read 1 byte from memory
        {
            return RAM[Address];
        }

        uint8_t &operator[](uint32_t Address) // An operator to write 1 byte to memory
        {
            return RAM[Address];
        }
    };

    struct CPU // The central processor unit of the computer
    {
        // The main CPU Registers
        uint16_t PC; // Program counter of the CPU
        uint16_t SP; // Stack pointer of the CPU

        // Index CPU Registers
        uint8_t A;  // Accumalator register
        uint8_t X;  // Index X register
        uint8_t Y;  // Index Y register
        uint8_t Q;  // Can be used as loop counter
        uint8_t K;  // Can only be used as a key buffer
        uint8_t OF; // Memory Offset register

        // CPU Status register
        uint8_t C : 1; // Carry flag
        uint8_t E : 1; // Equal flag
        uint8_t Z : 1; // Zero flag
        uint8_t I : 1; // Interrupt flag
        uint8_t D : 1; // Decimal flag
        uint8_t B : 1; // Break flag
        uint8_t V : 1; // Overflow flag
        uint8_t N : 1; // Negative flag

        void ResetCPU(Memory &mem); // To reset the cpu registers and clear the memory

        uint8_t FetchByte(uint32_t &ClockCycles, Memory &mem);                 // To fetch a byte of data from the memory with a cost of a clock cycle
        uint8_t ReadByte(uint32_t &ClockCycles, uint8_t Address, Memory &mem); // To fetch a byte of data from the memory without increasing the program counter   // To fetch a byte of data from the FLP

        /*### START INSTRUCTION SET ###*/
        static constexpr uint8_t // 4 Instructions and 18 opcodes
            // Move Instruction starts from 0x30
            INS_MV = 0x30, // CC 4 3 byte

            // Jump instructions starts from 0x40
            INS_JMP = 0x40, // Jump to a memory location CC 3 2 byte
            INS_JZ = 0x41,  // Jump to a memory location if the zero flag is set
            INS_JNZ = 0x42, // Jump to a memory location if the zero flag is not set
            INS_JE = 0x43,  // Jump to a memory location if the equal flag is set
            INS_JNE = 0x44, // Jump to a memory location if the equal flag is not set

            // Arthematic instruction starts from 0x50
            INS_DEC = 0x50, // To decrement a certain register CC 3 2 byte
            INS_INC = 0x51, // To increment a certain register CC 3 2 byte
            INS_ADD = 0x52, // To add a value from a register to another CC 4 3 byte
            INS_SUB = 0x53, // To sub a value from a register to another CC 4 3 byte
            INS_CMP = 0x54, // To compare a value from a register to another CC 3 3 byte

            // 0x60 is used by the program loader for changing memory location when writing to memory
            // Interrupt based instructions starts from 0x61
            INS_INT = 0x61, // To use interrupts in the cpu
            INS_CLI = 0x62, // To clear interrupt flag
            INS_STI = 0x63, // To set interrupt flag

            // Miscellaneous instructions
            INS_NOP = 0x64, // No Opcodes
            INS_HLT = 0x65, // Halts the processor
            // Debugging mode is used for debugging purposes. In this mode the instructions and it's opcode along with the register data and program counter [memory address] will be printed on to the console
            INS_SDE = 0x67, // Starts debugging
            INS_NDE = 0x66; // Stops debugging

        /*### END INSTRUCTION SET ###*/

        /*### START INTERRUPT REQUEST TABLE ###*/
        static constexpr uint8_t // 4 Interrupt requests
            INT_PC = 0x10,       // Print character which is stored in Y register
            INT_SRP = 0x11,      // Print status register to the console
            INT_GKP = 0x12,      // Get the keyboard input from the console and store it in the K register
            INT_CLS = 0x13;      // To clear the console screen

        /*### END INTERRUPT REQUEST TABLE ###*/

        void ExecuteFromMemory(Memory &mem);
    };

    struct System
    {
        CPU cpu;
        Memory mem;
        void Start();
        void StartExecution();
        void SystemReset(Memory &mem, CPU &cpu);                           // To perform full system reset
        void LoadPRG(uint8_t * Program, uint32_t SIZE);            // To load the program into the memory
    };
};

#endif