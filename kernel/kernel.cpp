#include "../Includes/types.h"
#include "gdt.h"
#include"../CPU/Interrupts.h"
#include "../Drivers/Keyboard.h"
#include "../Drivers/Mouse.h"

static uint8_t cursory;
static uint8_t cursorx;

void printf(char* str)
{
    static uint8_t x=0, y=0;
    uint8_t curx;
    uint8_t cury;

    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        cursorx = x;

        switch(str[i])
        {
            case '\n':
                y++;
                x = 0;
            break;
            case '\5':
                for(y = 0; y < 25; y++)
                    for(x = 0; x < 80; x++)
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';

                x = 0;
                y = 0;
            break;
            case '\3':
                for(x = cursorx - 0x02; x == cursorx +2 ; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | 'a';
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
            break;
        }
        

        if (x >= 80)
        {
            y++;
            x = 0;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';

            x = 0;
            y = 0;
        }
        
    }
}

void printfchar(char st)
{
    static uint8_t x=0, y=0;

    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

        switch(st)
        {
            case '\n':
                y++;
                x = 0;
            break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | st;
                x++;
            break;
        }
        

        if (x >= 80)
        {
            y++;
            x = 0;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';

            x = 0;
            y = 0;
        }
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{
    printf("Welcome to SectorOS Monolithic kernel \nInterrupt descriptor table loaded \n");

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);
    interrupts.Activate();

    KeyboardDriver KeyboardDriver(&interrupts);
    MouseDriver MouseDriver(&interrupts);

    while(1);
}