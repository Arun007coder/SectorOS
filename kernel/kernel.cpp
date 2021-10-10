#include "../Includes/types.h"
#include "gdt.h"
#include"../CPU/Interrupts.h"
#include "../Drivers/IOPorts.h"
#include "../Drivers/Keyboard.h"
#include "../Drivers/Mouse.h"
#include "../Drivers/Driver.h"
#include "../Drivers/RTC.h"
#include "../Hardcom/pci.h"
#include "../Debug/Debug.h"

static uint8_t cursory;
static uint8_t cursorx;
static bool useMouse = true;
bool isused;
bool isTxtMode;
debugfunctions deb;
static uint16_t* VideoMemory = (uint16_t*)0xb8000;

// Can Support up to number 100
char* INTTOCHARPOINT(int num)
{
    char* res;
    switch (num)
    {
        case 0:res = "00"; break;
        case 1:res = "01"; break;
        case 2:res = "02"; break;
        case 3:res = "03";break;
        case 4:res = "04";break;
        case 5:res = "05";break;
        case 6:res = "06";break;
        case 7:res = "07";break;
        case 8:res = "08";break;
        case 9:res  =  "09";break;
        case 10:res  =  "10"; break;
        case 11:res = "11"; break;
        case 12:res = "12"; break;
        case 13:res = "13"; break;
        case 14:res = "14"; break;
        case 15:res = "15"; break;
        case 16:res = "16"; break;
        case 17:res = "17"; break;
        case 18:res = "18"; break;
        case 19:res = "19"; break;
        case 20:res = "20"; break;
        case 21:res = "21"; break;
        case 22:res = "22"; break;
        case 23:res = "23"; break;
        case 24:res = "24"; break;
        case 25:res = "25"; break;
        case 26:res = "26"; break;
        case 27:res = "26"; break;
        case 28:res = "28"; break;
        case 29:res = "29"; break;
        case 30:res = "30"; break;
        case 31:res = "31"; break;
        case 32:res = "32"; break;
        case 33:res = "33"; break;
        case 34:res = "34"; break;
        case 35:res = "35"; break;
        case 36:res = "36"; break;
        case 37:res = "37"; break;
        case 38:res = "38"; break;
        case 39:res = "39"; break;
        case 40:res = "40"; break;
        case 41:res = "41"; break;
        case 42:res = "42"; break;
        case 43:res = "43"; break;
        case 44:res = "44"; break;
        case 45:res = "45"; break;
        case 46:res = "46"; break;
        case 47:res = "47"; break;
        case 48:res = "48"; break;
        case 49:res = "49"; break;
        case 50:res = "50"; break;
        case 51:res = "51"; break;
        case 52:res = "52"; break;
        case 53:res = "53"; break;
        case 54:res = "54"; break;
        case 55:res = "55"; break;
        case 56:res = "56"; break;
        case 57:res = "57"; break;
        case 58:res = "58"; break;
        case 59:res = "59"; break;
        case 60:res = "60"; break;
        case 61:res = "61"; break;
        case 62:res = "62"; break;
        case 63:res = "63"; break;
        case 64:res = "64"; break;
        case 65:res = "65"; break;
        case 66:res = "66"; break;
        case 67:res = "67"; break;
        case 68:res = "68"; break;
        case 69:res = "69"; break;
        case 70:res = "70"; break;
        case 71:res = "71"; break;
        case 72:res = "72"; break;
        case 73:res = "73"; break;
        case 74:res = "74"; break;
        case 75:res = "75"; break;
        case 76:res = "76"; break;
        case 77:res = "77"; break;
        case 78:res = "78"; break;
        case 79:res = "79"; break;
        case 80:res = "80"; break;
        case 81:res = "81"; break;
        case 82:res = "82"; break;
        case 83:res = "83"; break;
        case 84:res = "84"; break;
        case 85:res = "85"; break;
        case 86:res = "86"; break;
        case 87:res = "87"; break;
        case 88:res = "88"; break;
        case 89:res = "89"; break;
        case 90:res = "90"; break;
        case 91:res = "91"; break;
        case 92:res = "92"; break;
        case 93:res = "93"; break;
        case 94:res = "94"; break;
        case 95:res = "95"; break;
        case 96:res = "96"; break;
        case 97:res = "97"; break;
        case 98:res = "98"; break;
        case 99:res = "99"; break;
        case 100:res = "100"; break;
        case 2021:res = "2021"; break;

        default: res = "NaN"; break;
    }

    return res;

}

void ColourPrint(int type)
{
    uint8_t x; //crx;
    uint8_t y; //cry;
    switch (type)
    {
        case 0:
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4 | ' ';
        break;

        case 1:
                if (!isused){
                    for(x = 0; x < 80; x++)
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) >> 4 | ' ';
                        isused = true;
                }
                else
                {
                    isused = false;
                }
                
        break;

    }
}

void printf(char* str)
{
    static uint8_t x=0, y=0;
    uint8_t curx;
    uint8_t cury;

    //static uint16_t* VideoMemory = (uint16_t*)0xb8000;

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
                //ColourPrint(1);
                isused = true;
                for(y = 0; y < 25; y++)
                    for(x = 0; x < 80; x++)
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';

                x = 0;
                y = 0;
            break;
            case '\3':
                for(int i = 0; i != 2; i++)
                    x = cursorx;
                    if (x != 3){
                        x--;
                    }
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            break;
            case '\f':
                for(int i = 0; i != 2; i++)
                    x = cursorx;
                    x--;
                    if(x == 0 & y != 0)
                    {
                        x = 0;
                        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
                        y--;
                        x = 79;
                    }
                    else if(y == 0)
                    {
                        y = 1;
                    }
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            break;
            case '\7':
                if(y != 0)
                    y--;
                else if(y == 0)
                {
                    y = 1;
                }
            break;

            case '\4':
                    x--;
            break;

            case '\2':
                if (y != 25)
                    y++;
            break;

            case '\6':
                if (x != 80)
                    x++;
            break;

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
            //ColourPrint(1);
            isused = true;
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
            //ColourPrint(0);
            printf("SectorOS Monolithic kernel       ");
            RTC rtclock;
            rtclock.read_rtc();
            printf(INTTOCHARPOINT(rtclock.day));
            printf("/");
            printf(INTTOCHARPOINT(rtclock.month));
            printf("/");
            printf(INTTOCHARPOINT(rtclock.year));
            printf("                         Type: Shell ");
        }
        
    }
}

//Uses 10 Character Space
void PrintDate()
{
    RTC rtclock;
    rtclock.read_rtc();
    printf(INTTOCHARPOINT(rtclock.day));
    printf("/");
    printf(INTTOCHARPOINT(rtclock.month));
    printf("/");
    printf(INTTOCHARPOINT(rtclock.year));
}

void printTime()
{
    uint8_t x = 51;
    uint8_t y = 0;
    RTC rtclock;
    rtclock.read_rtc();
    char* strH = INTTOCHARPOINT(rtclock.hour);
    char dd = ':';
    char* strM = INTTOCHARPOINT(rtclock.minute);
    char* strS = INTTOCHARPOINT(rtclock.second);


    for(int i = 0; i != 3; ++i)
    {
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | strH[i];
        x++;
    }

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | dd;
        x++;

    for(int i = 0; i != 3; ++i)
    {
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | strM[i];
        x++;
    }

        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ',';
        x++;

    for(int i = 0; i != 3; ++i)
    {
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | strS[i];
        x++;
    }

}

void printfchar(char st)
{
    static uint8_t x=0, y=0;

    //static uint16_t* VideoMemory = (uint16_t*)0xb8000;

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
    deb.logToSerialPort("kernel started");
    ColourPrint(0);
    printTime();

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt);

    DriverManager drvmgr;

    printf("\nSYSMSG: Initializing Hardwares [Stage 1]...");
    KeyboardDriver KeyboardDriver(&interrupts);
    

    drvmgr.AddDriver(&KeyboardDriver);
    

    MouseToConsole msmgr;
    MouseDriver MouseDriver(&interrupts, &msmgr);
    if (useMouse = true)
    {
        drvmgr.AddDriver(&MouseDriver);
    }
    else
    {
        printf("\nSYSMSG: Cannot initialize mouse driver. This driver is disabled by default.");
    }

    PCI PCICONT;
    PCICONT.SelectDrivers(&drvmgr);

    printf("\nSYSMSG: Initializing Hardwares [Stage 2]...");
    drvmgr.activateall();

    printf("\nSYSMSG: Initializing Hardwares [Stage 3]...\n");
    interrupts.Activate();

    printf("\5");

    printf("Welcome to SectorOS Monolithic kernel ");PrintDate();printf("                    Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");

    printf("Run help to get the list of commands which is implemented \n \n");

    printf("$: ");
    while(1);
}