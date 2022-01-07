#include "../Include/types.h"
#include "gdt.h"
#include "../CPU/Interrupts.h"
#include "../Drivers/IOPorts.h"
#include "../Drivers/Keyboard.h"
#include "../Shell/Shell.h"
#include "../Include/multiboot.h"
#include "../CPU/syscall.h"
#include "../Drivers/Mouse.h"
#include "../Drivers/Driver.h"
#include "../Drivers/RTC.h"
#include "../Hardcom/pci.h"
#include "../Hardcom/SerialPort.h"
#include "../Include/Public_VAR.h"
#include "MultiTask.h"
#include "../Memory/MemoryManagement.h"
#include "../Include/Debug.h"
#include "../Drivers/HDD-ATA.h"
#include "../Drivers/VGADriver.h"
#include "../Filesystem/MSDOSPT.h"

static uint8_t cursory;
static uint8_t cursorx;
static bool useMouse = true;
bool isused;

static uint8_t x = 0, y = 0;

bool isTxtMode;
KeyboardDriver* kbrd;
DriverManager* drvMngr;
InterruptManager* intMngr;
SyscallHandler* shand;
extern const void *mb;
uint32_t mm;
inline GlobalDescriptorTable gdt;
extern TaskManager taskManager;
AdvancedTechnologyAttachment ata0m(0x1F0, true);
AdvancedTechnologyAttachment ata0s(0x1F0, false);

port8BIT port43(0x43);
port8BIT port42(0x42);
port8BIT port61(0x61);
int delay;
volatile int done;
static uint16_t *VideoMemory = (uint16_t *)0xb8000;

static void play_sound(uint32_t nFrequence)
{
    uint32_t Div;
    uint8_t tmp;

    // Set the PIT to the desired frequency
    Div = 1193180 / nFrequence;
    port43.WriteToPort(0xb6);
    port42.WriteToPort((uint8_t)(Div));
    port42.WriteToPort((uint8_t)(Div >> 8));

    // And play the sound using the PC speaker
    tmp = port61.ReadFromPort();
    if (tmp != (tmp | 3))
    {
        port61.WriteToPort(tmp | 3);
    }
}

static void nosound()
{
    uint8_t tmp = port61.ReadFromPort() & 0xFC;

    port61.WriteToPort(tmp);
}

void beep()
{
    play_sound(1000);
    for (int i = 0; i != 10000; i++)
        ;
    nosound();
    SerialPort ss;
    ss.INITSerial();
    ss.logToSerialPort("beep\n");
}

// Can Support up to number 100
char *INTTOCHARPOINT(int num)
{
    char *res;
    switch (num)
    {
    case 0:
        res = "00";
        break;
    case 1:
        res = "01";
        break;
    case 2:
        res = "02";
        break;
    case 3:
        res = "03";
        break;
    case 4:
        res = "04";
        break;
    case 5:
        res = "05";
        break;
    case 6:
        res = "06";
        break;
    case 7:
        res = "07";
        break;
    case 8:
        res = "08";
        break;
    case 9:
        res = "09";
        break;
    case 10:
        res = "10";
        break;
    case 11:
        res = "11";
        break;
    case 12:
        res = "12";
        break;
    case 13:
        res = "13";
        break;
    case 14:
        res = "14";
        break;
    case 15:
        res = "15";
        break;
    case 16:
        res = "16";
        break;
    case 17:
        res = "17";
        break;
    case 18:
        res = "18";
        break;
    case 19:
        res = "19";
        break;
    case 20:
        res = "20";
        break;
    case 21:
        res = "21";
        break;
    case 22:
        res = "22";
        break;
    case 23:
        res = "23";
        break;
    case 24:
        res = "24";
        break;
    case 25:
        res = "25";
        break;
    case 26:
        res = "26";
        break;
    case 27:
        res = "26";
        break;
    case 28:
        res = "28";
        break;
    case 29:
        res = "29";
        break;
    case 30:
        res = "30";
        break;
    case 31:
        res = "31";
        break;
    case 32:
        res = "32";
        break;
    case 33:
        res = "33";
        break;
    case 34:
        res = "34";
        break;
    case 35:
        res = "35";
        break;
    case 36:
        res = "36";
        break;
    case 37:
        res = "37";
        break;
    case 38:
        res = "38";
        break;
    case 39:
        res = "39";
        break;
    case 40:
        res = "40";
        break;
    case 41:
        res = "41";
        break;
    case 42:
        res = "42";
        break;
    case 43:
        res = "43";
        break;
    case 44:
        res = "44";
        break;
    case 45:
        res = "45";
        break;
    case 46:
        res = "46";
        break;
    case 47:
        res = "47";
        break;
    case 48:
        res = "48";
        break;
    case 49:
        res = "49";
        break;
    case 50:
        res = "50";
        break;
    case 51:
        res = "51";
        break;
    case 52:
        res = "52";
        break;
    case 53:
        res = "53";
        break;
    case 54:
        res = "54";
        break;
    case 55:
        res = "55";
        break;
    case 56:
        res = "56";
        break;
    case 57:
        res = "57";
        break;
    case 58:
        res = "58";
        break;
    case 59:
        res = "59";
        break;
    case 60:
        res = "60";
        break;
    case 61:
        res = "61";
        break;
    case 62:
        res = "62";
        break;
    case 63:
        res = "63";
        break;
    case 64:
        res = "64";
        break;
    case 65:
        res = "65";
        break;
    case 66:
        res = "66";
        break;
    case 67:
        res = "67";
        break;
    case 68:
        res = "68";
        break;
    case 69:
        res = "69";
        break;
    case 70:
        res = "70";
        break;
    case 71:
        res = "71";
        break;
    case 72:
        res = "72";
        break;
    case 73:
        res = "73";
        break;
    case 74:
        res = "74";
        break;
    case 75:
        res = "75";
        break;
    case 76:
        res = "76";
        break;
    case 77:
        res = "77";
        break;
    case 78:
        res = "78";
        break;
    case 79:
        res = "79";
        break;
    case 80:
        res = "80";
        break;
    case 81:
        res = "81";
        break;
    case 82:
        res = "82";
        break;
    case 83:
        res = "83";
        break;
    case 84:
        res = "84";
        break;
    case 85:
        res = "85";
        break;
    case 86:
        res = "86";
        break;
    case 87:
        res = "87";
        break;
    case 88:
        res = "88";
        break;
    case 89:
        res = "89";
        break;
    case 90:
        res = "90";
        break;
    case 91:
        res = "91";
        break;
    case 92:
        res = "92";
        break;
    case 93:
        res = "93";
        break;
    case 94:
        res = "94";
        break;
    case 95:
        res = "95";
        break;
    case 96:
        res = "96";
        break;
    case 97:
        res = "97";
        break;
    case 98:
        res = "98";
        break;
    case 99:
        res = "99";
        break;
    case 100:
        res = "100";
        break;
    case 2021:
        res = "2021";
        break;

    case 2022:
        res = "2022";
        break;

    case 2023:
        res = "2023";
        break;

    case 2024:
        res = "2024";
        break;

    case 2025:
        res = "2025";
        break;

    case 2026:
        res = "2026";
        break;

    case 2027:
        res = "2027";
        break;

    case 2028:
        res = "2028";
        break;

    case 2029:
        res = "2029";
        break;

    case 2030:
        res = "2030";
        break;

    case 2031:
        res = "2031";
        break;

    case 2032:
        res = "2032";
        break;

    case 2033:
        res = "2033";
        break;

    case 2034:
        res = "2034";
        break;

    case 2035:
        res = "2035";
        break;

    case 2036:
        res = "2036";
        break;

    case 2037:
        res = "2037";
        break;

    case 2038:
        res = "2038";
        break;

    case 2039:
        res = "2039";
        break;

    case 2040:
        res = "2040";
        break;

    case 2041:
        res = "2041";
        break;

    case 2042:
        res = "2042";
        break;

    case 2043:
        res = "2043";
        break;

    case 2044:
        res = "2044";
        break;

    case 2045:
        res = "2045";
        break;

    case 2046:
        res = "2046";
        break;

    case 2047:
        res = "2047";
        break;

    case 2048:
        res = "2048";
        break;

    case 2049:
        res = "2049";
        break;

    case 2050:
        res = "2050";
        break;

    case 2051:
        res = "2051";
        break;

    case 2052:

    default:
        res = "NaN";
        break;
    }

    return res;
}

uint16_t GetAvailableMem()
{
    port8BIT port(0x70);
    port8BIT inport(0x71);

    unsigned short total;
    unsigned char lowmem, highmem;

    port.WriteToPort(0x30);
    lowmem = inport.ReadFromPort();
    port.WriteToPort(0x31);
    highmem = inport.ReadFromPort();

    total = lowmem | highmem << 8;

    return total;
}

char *hertochar(uint16_t hex2)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(hex2 >> 4) & 0xF];
    foo[1] = hex[hex2 & 0xF];
    return foo;
}

void ColourPrint(int type)
{
    uint8_t x; // crx;
    uint8_t y; // cry;
    switch (type)
    {
    case 0:
        for (x = 0; x < 80; x++)
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | ' ';
        break;

    case 1:
        if (!isused)
        {
            for (x = 0; x < 80; x++)
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) >> 4 | ' ';
            isused = true;
        }
        else
        {
            isused = false;
        }

        break;
    }
}

void printf(char *str)
{
    uint8_t curx;
    uint8_t cury;

    // SerialPort s;
    // s.logToSerialPort(str);

    // static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    for (int i = 0; str[i] != '\0'; ++i)
    {
        cursorx = x;

        switch (str[i])
        {
        case '\n':
            y++;
            x = 0;
            break;
        case '\5':
            // ColourPrint(1);
            isused = true;
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';

            x = 0;
            y = 0;
            break;
        case '\3':
            for (int i = 0; i != 2; i++)
                x = cursorx;
            if (x != SPIndex)
            {
                x--;
            }
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            break;
        case '\f':
            for (int i = 0; i != 2; i++)
                x = cursorx;
            x--;
            if (x == 0 & y != 0)
            {
                x = 0;
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
                y--;
                x = 79;
            }
            else if (y == 0)
            {
                y = 1;
            }
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            break;
        case '\7':
            if (y != 0)
                y--;
            else if (y == 0)
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
            VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | str[i];
            x++;
            break;
        }

        if (x >= 80)
        {
            y++;
            x = 0;
        }

        if (y >= 25)
        {
            // ColourPrint(1);
            isused = true;
            for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
            // ColourPrint(0);
            printf("SectorOS ");
            printf(KERNEL_VERSION);
            printf("                   ");
            RTC rtclock;
            rtclock.read_rtc();
            printf(INTTOCHARPOINT(rtclock.day));
            printf("/");
            printf(INTTOCHARPOINT(rtclock.month));
            printf("/");
            printf(INTTOCHARPOINT(rtclock.year));
            printf("                        Type: Shell ");
        }
    }
}

// Uses 10 Character Space
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
    if (!done && delay)
        delay--;
    if (delay == 0)
        done = true;
    uint8_t x = 51;
    uint8_t y = 0;
    RTC rtclock;
    rtclock.read_rtc();
    char *strH = INTTOCHARPOINT(rtclock.hour);
    char dd = ':';
    char *strM = INTTOCHARPOINT(rtclock.minute);
    char *strS = INTTOCHARPOINT(rtclock.second);

    for (int i = 0; i != 3; ++i)
    {
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | strH[i];
        x++;
    }

    x--;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | dd;
    x++;

    for (int i = 0; i != 3; ++i)
    {
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | strM[i];
        x++;
    }

    x--;
    VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ',';
    x++;

    for (int i = 0; i != 3; ++i)
    {
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | strS[i];
        x++;
    }
}

void PVGA()
{
    VideoGraphicsArray vga;
    vga.SetMode(320, 200, 8);
    for (int32_t y = 0; y < 200; y++)
        for (int32_t x = 0; x < 320; x++)
            vga.PutPixel(x, y, 0x00, 0x00, 0xA8);
}

void printfchar(char st)
{

    // static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    switch (st)
    {
    case '\n':
        y++;
        x = 0;
        break;
    default:
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | st;
        x++;
        break;
    }

    if (x >= 80)
    {
        y++;
        x = 0;
    }

    if (y >= 25)
    {
        for (y = 0; y < 25; y++)
            for (x = 0; x < 80; x++)
                VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0xFF00) | ' ';

        x = 0;
        y = 0;
    }
}

void printHex(uint8_t Key)
{
    char *foo = "00";
    char *hex = "0123456789ABCDEF";
    foo[0] = hex[(Key >> 4) & 0xF];
    foo[1] = hex[Key & 0xF];
    printf(foo);
}

void PrintPartitions()
{
    AdvancedTechnologyAttachment ata0s(0x1F0, false);
    MSDOSPartitionTable::ReadPartitions(&ata0s);
}

void PrintMEM(const void *multiboot_structure)
{
    uint32_t *memupper = (uint32_t *)(((size_t)multiboot_structure) + 8);
    size_t heap = 10 * 1024 * 1024;
    MemoryManager memoryManager(heap, (*memupper) * 1024 - heap - 10 * 1024);

    printf("heap: 0x");
    printHex((heap >> 24) & 0xFF);
    printHex((heap >> 16) & 0xFF);
    printHex((heap >> 8) & 0xFF);
    printHex((heap)&0xFF);

    void *allocated = memoryManager.MemAllocate(1024);
    printf("\nallocated: 0x");
    printHex(((size_t)allocated >> 24) & 0xFF);
    printHex(((size_t)allocated >> 16) & 0xFF);
    printHex(((size_t)allocated >> 8) & 0xFF);
    printHex(((size_t)allocated) & 0xFF);
    printf("\n");
}

#define cpuid(in, a, b, c, d) __asm__("cpuid"                              \
                                      : "=a"(a), "=b"(b), "=c"(c), "=d"(d) \
                                      : "a"(in));

int do_intel(void)
{
    printf("CPU Manufacture - Genuineintel\n");
    printf("Intel Specific Features:\n");
    unsigned long eax, ebx, ecx, edx, max_eax, signature, unused;
    int model, family, type, brand;
    int extended_family = -1;
    cpuid(1, eax, ebx, unused, unused);
    model = (eax >> 4) & 0xf;
    family = (eax >> 8) & 0xf;
    type = (eax >> 12) & 0x3;
    brand = ebx & 0xff;

    printf("Type - ");
    switch (type)
    {
    case 0:
        printf("Original OEM");
        break;
    case 1:
        printf("Overdrive");
        break;
    case 2:
        printf("Dual-capable");
        break;
    case 3:
        printf("Reserved");
        break;
    }
    printf("\n");

    printf("Family - ");
    switch (family)
    {
    case 3:
        printf("i386");
        break;
    case 4:
        printf("i486");
        break;
    case 5:
        printf("Pentium");
        break;
    case 6:
        printf("Pentium Pro");
        break;
    case 15:
        printf("Pentium 4");
    }
    printf("\n");

    printf("Model - ");
    switch (family)
    {
    case 3:
        break;
    case 4:
        switch (model)
        {
        case 0:
        case 1:
            printf("DX");
            break;
        case 2:
            printf("SX");
            break;
        case 3:
            printf("487/DX2");
            break;
        case 4:
            printf("SL");
            break;
        case 5:
            printf("SX2");
            break;
        case 7:
            printf("Write-back enhanced DX2");
            break;
        case 8:
            printf("DX4");
            break;
        }
        break;
    case 5:
        switch (model)
        {
        case 1:
            printf("60/66");
            break;
        case 2:
            printf("75-200");
            break;
        case 3:
            printf("for 486 system");
            break;
        case 4:
            printf("MMX");
            break;
        }
        break;
    case 6:
        switch (model)
        {
        case 1:
            printf("Pentium Pro");
            break;
        case 3:
            printf("Pentium II Model 3");
            break;
        case 5:
            printf("Pentium II Model 5/Xeon/Celeron");
            break;
        case 6:
            printf("Celeron");
            break;
        case 7:
            printf("Pentium III/Pentium III Xeon - external L2 cache");
            break;
        case 8:
            printf("Pentium III/Pentium III Xeon - internal L2 cache");
            break;
        }
        break;
    case 15:
        break;
        printf("\n");
    }

    return 0;
}

void PrintSATA()
{
    printf("\nS-ATA primary master: ");
    AdvancedTechnologyAttachment ata0m(0x1F0, true);
    ata0m.Identify();

    printf("\nS-ATA primary slave: ");
    AdvancedTechnologyAttachment ata0s(0x1F0, false);
    ata0s.Identify();
    printf("\n");

    printf("\nS-ATA secondary master: ");
    AdvancedTechnologyAttachment ata1m(0x170, true);
    ata1m.Identify();

    printf("\nS-ATA secondary slave: ");
    AdvancedTechnologyAttachment ata1s(0x170, false);
    ata1s.Identify();
    printf("\n");
}

int detect_cpu(void)
{
    unsigned long ebx, unused;
    cpuid(0, unused, ebx, unused, unused);
    switch (ebx)
    {
    case 0x756e6547:
        do_intel();
        break;
    case 0x68747541:
        // do_amd();
        break;
    default:
        printf("Unknown x86 CPU Detected\n");
        break;
    }
    return 0;
}

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y;

public:
    MouseToConsole()
    {
    }

    virtual void OnActivate()
    {
        uint16_t *VideoMemory = (uint16_t *)0xb8000;
        x = 40;
        y = 12;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }

    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t *VideoMemory = (uint16_t *)0xb8000;
        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);

        x += xoffset;
        if (x >= 80)
            x = 79;
        if (x < 0)
            x = 0;
        y += yoffset;
        if (y >= 25)
            y = 24;
        if (y < 0)
            y = 0;

        VideoMemory[80 * y + x] = (VideoMemory[80 * y + x] & 0x0F00) << 4 | (VideoMemory[80 * y + x] & 0xF000) >> 4 | (VideoMemory[80 * y + x] & 0x00FF);
    }
};

void taskB()
{
    printf("SectorOS TASK2\nPlese restart the os manually\n");
    while (1);
}

void taskA()
{
    printf("SectorOS TASK1\nPlese restart the os manually\n");
    while (1);
}

void SPOMEMLOC(SerialPort sp)
{
    uint16_t value = GetAvailableMem();
    uint8_t partA = static_cast<uint8_t>((value & 0xFF00) >> 8);
    uint8_t partB = static_cast<uint8_t>(value & 0x00FF);
    sp.logToSerialPort("Current memory location: ");
    sp.logToSerialPort(hertochar(partA));
    sp.logToSerialPort(" ");
    sp.logToSerialPort(hertochar(partB));
}

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor *i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}

extern "C" void kernelMain(const void *multiboot_structure, uint32_t multiboot_m)
{
    TaskManager taskManager;
    uint32_t mm = multiboot_m;
    printf("Initializing SectorOS Kernel ");
    printf(KERNEL_VERSION);
    printf(" ");
    printf(KERNEL_BUILD);
    printf("....\n");
    SerialPort sp;
    sp.INITSerial();

    SPOMEMLOC(sp);
    sp.logToSerialPort("\nkernel started");

    GlobalDescriptorTable gdt;

    InterruptManager interrupts(0x20, &gdt, &taskManager);
    intMngr = &interrupts;
    SyscallHandler syscalls(&interrupts, 0x80);
    shand = &syscalls;
    DriverManager drvmgr;
    drvMngr = &drvmgr;
    printf("\nSYSMSG: Initializing Hardwares [Stage 1]...\n");
    CShell sh;

    KeyboardDriver hexboardDriver(&interrupts, &sh);
    kbrd = &hexboardDriver;
    sp.logToSerialPort("\nHardware initialising stage 1 finished");
    drvmgr.AddDriver(&hexboardDriver);

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
    PCICONT.SelectDrivers(&drvmgr, &interrupts);
    printf("\nSYSMSG: Initializing Hardwares [Stage 2]...");
    drvmgr.activateall();

    sp.logToSerialPort("\nHardware initialising stage 2 finished");
    sp.logToSerialPort("\nDriverManager started");
    printf("\nSYSMSG: Initializing Hardwares [Stage 3]...\n");

    printf("Allocating Memory....\n");
    PrintMEM(multiboot_structure);
    PrintPartitions();
    sp.logToSerialPort("\nHardware initialising stage 3 finished");
    detect_cpu();

    printf("\5");
    ColourPrint(0);
    interrupts.Activate();
    sp.logToSerialPort("\nInterrupt manager started");

    printf("Welcome to SectorOS ");
    printf(KERNEL_VERSION);
    printf("          ");
    PrintDate();
    printf("                    Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");

    printf("Initializing ");
    printf(SHELL_NAME);
    printf(" ");
    printf(SHELL_VER);
    printf("\n\n");

    printf("Welcome to SectorOS Shell\nRun help to get the list of commands which is implemented \n \n");
    sp.logToSerialPort("\nKernel initialization surcessful.\nGiving execution access to the kernel.\nAwaiting user input...");

    SPIndex = 15;

    asm("int $0x80" :: "a"(1), "b"("root@secos:~#> ")); // Used syscall to print this prompt
    SPOMEMLOC(sp);

    while (1);
}