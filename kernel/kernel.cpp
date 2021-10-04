#include "../Includes/types.h"
#include "gdt.h"
#include"../CPU/Interrupts.h"
#include "../Drivers/Keyboard.h"
#include "../Drivers/Mouse.h"
#include "../Drivers/Driver.h"

static uint8_t cursory;
static uint8_t cursorx;
static bool useMouse = false;

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

void printint(int num)
{
    switch (num)
    {
    case 0:printf("0"); break;
    case 1:printf("1"); break;
    case 2:printf("2"); break;
    case 3:printf("3"); break;
    case 4:printf("4"); break;
    case 5:printf("5"); break;
    case 6:printf("6"); break;
    case 7:printf("7"); break;
    case 8:printf("8"); break;
    case 9:printf("9"); break;

    case 10:printf("10"); break;
    case 11:printf("11"); break;
    case 12:printf("12"); break;
    case 13:printf("13"); break;
    case 14:printf("14"); break;
    case 15:printf("15"); break;
    case 16:printf("16"); break;
    case 17:printf("17"); break;
    case 18:printf("18"); break;
    case 19:printf("19"); break;
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

void printHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;
public:

    MouseToConsole()
    {
    }

    /*
    virtual void OnMouseUp(uint8_t button)
    {
        if (button == 0x01)
            printf("you clicked");
        else if (button == 0x03)
            printf("hhh");
    }
    */

    virtual void OnActivate()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);        
    }

    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        x += xoffset;
        if(x >= 80) x = 79;
        if(x < 0) x = 0;
        y += yoffset;
        if(y >= 25) y = 24;
        if(y < 0) y = 0;

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);
    }

};


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
    printf("Welcome to SectorOS Monolithic kernel \nhttps://github.com/Arun007coder/SectorOS \n");

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);

    uint32_t magic = 0x1BADB002;

    DriverManager drvmgr;

    printf("\nSYSMSG: Initializing Hardwares [Stage 1]...");
    KeyboardDriver KeyboardDriver(&interrupts);
    drvmgr.AddDriver(&KeyboardDriver);
    
    if (useMouse)
    {
        MouseToConsole msmgr;
        MouseDriver MouseDriver(&interrupts, &msmgr);
        drvmgr.AddDriver(&MouseDriver); 
    }
    else
        printf("\nSYSMSG: Cannot initialize mouse driver. This driver is disabled by default.");

    printf("\nSYSMSG: Initializing Hardwares [Stage 2]...");
    drvmgr.activateall();

    printf("\nSYSMSG: Initializing Hardwares [Stage 3]...\n");
    interrupts.Activate();

    printf("Run help to get the list of commands which is implemented \n \n");

    printf("$: ");
    while(1);
}