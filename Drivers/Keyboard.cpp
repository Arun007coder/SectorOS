#include "Keyboard.h"

static int keybufferpoint = 0;
static bool isShift = false;
void printf(char*);
void printfchar(char st);
void printHex(uint8_t key);
void printint(int num);

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
:InterruptHandler(0x21, manager),
DataPort(0x60),
CommandPort(0x64)
{
}

KeyboardDriver::~KeyboardDriver()
{
}

void KeyboardDriver::activate()
{
    while(CommandPort.ReadFromPort() & 0x1)
        DataPort.ReadFromPort();
    CommandPort.WriteToPort(0xAE); // Activates the interrupts
    CommandPort.WriteToPort(0x20); // Gets current state
    uint8_t status = (DataPort.ReadFromPort() | 1) & ~0x10;
    CommandPort.WriteToPort(0x60); // set state
    DataPort.WriteToPort(status);

    DataPort.WriteToPort(0xF4);
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = DataPort.ReadFromPort(); // The variable where a single keystroke is stored
    if(key < 0x80 & key != 0x2A & key != 0x36 & key != 0x3A & key != 0x0E & key != 0x38 & key != 0x1D ){
        key_buffer[keybufferpoint] = KeycodeToASCII(key);
        keybufferpoint++;
    }
    switch (key)
    {
        case 0xFA: break;
        case 0x45: case 0xC5: case 0x61: clear_key_buffer(); break;
        case 0x1D: case 0x38: break; // It is used by Vmware workstation player to lose focus
        case 0x29:if (!isShift) printf("`"); else printf("~"); break;
        case 0x02:if (!isShift) printf("1"); else printf("!"); break;
        case 0x03:if (!isShift) printf("2"); else printf("@"); break;
        case 0x04:if (!isShift) printf("3"); else printf("#"); break;
        case 0x05:if (!isShift) printf("4"); else printf("$"); break;
        case 0x06:if (!isShift) printf("5"); else printf("%"); break;
        case 0x07:if (!isShift) printf("6"); else printf("^"); break;
        case 0x08:if (!isShift) printf("7"); else printf("&"); break;
        case 0x09:if (!isShift) printf("8"); else printf("*"); break;
        case 0x0A:if (!isShift) printf("9"); else printf("("); break;
        case 0x0B:if (!isShift) printf("0"); else printf(")"); break;
        case 0x0C:if (!isShift) printf("-"); else printf("_"); break;
        case 0x0D:if (!isShift) printf("="); else printf("+"); break;
        //case 0x0F: printf("   "); break;
        case 0x10:if (!isShift) printf("q"); else printf("Q"); break;
        case 0x11:if (!isShift) printf("w"); else printf("W"); break;
        case 0x12:if (!isShift) printf("e"); else printf("E"); break;
        case 0x13:if (!isShift) printf("r"); else printf("R"); break;
        case 0x14:if (!isShift) printf("t"); else printf("T"); break;
        case 0x15:if (!isShift) printf("y"); else printf("Y"); break;
        case 0x16:if (!isShift) printf("u"); else printf("U"); break;
        case 0x17:if (!isShift) printf("i"); else printf("I"); break;
        case 0x18:if (!isShift) printf("o"); else printf("O"); break;
        case 0x19:if (!isShift) printf("p"); else printf("P"); break;
        case 0x1A:if (!isShift) printf("["); else printf("{"); break;
        case 0x1B:if (!isShift) printf("]"); else printf("}"); break;
        case 0x1C: CommandInterpreter(); break; //Enter
        case 0x1E:if (!isShift) printf("a"); else printf("A"); break;
        case 0x1F:if (!isShift) printf("s"); else printf("S"); break;
        case 0x20:if (!isShift) printf("d"); else printf("D"); break;
        case 0x21:if (!isShift) printf("f"); else printf("F"); break;
        case 0x22:if (!isShift) printf("g"); else printf("G"); break;
        case 0x23:if (!isShift) printf("h"); else printf("H"); break;
        case 0x24:if (!isShift) printf("j"); else printf("J"); break;
        case 0x25:if (!isShift) printf("k"); else printf("K"); break;
        case 0x26:if (!isShift) printf("l"); else printf("L"); break;
        case 0x27:if (!isShift) printf(";"); else printf(":"); break;
        case 0x28:if (!isShift) printf("E"); else printf("e"); break;
        //case 0x2B:if (!isShift) printf("\ "); else printf("|"); break; 
        case 0x2A: isShift = false; break;
        case 0x2C:if (!isShift) printf("z"); else printf("Z"); break;
        case 0x2D:if (!isShift) printf("x"); else printf("X"); break;
        case 0x2E:if (!isShift) printf("c"); else printf("C"); break;
        case 0x2F:if (!isShift) printf("v"); else printf("V"); break;
        case 0x30:if (!isShift) printf("b"); else printf("B"); break;
        case 0x31:if (!isShift) printf("n"); else printf("N"); break;
        case 0x32:if (!isShift) printf("m"); else printf("M"); break;
        case 0x33:if (!isShift) printf(","); else printf("<"); break;
        case 0x34:if (!isShift) printf("."); else printf(">"); break;
        case 0x35:if (!isShift) printf("/"); else printf("?"); break;
        case 0x36: isShift = false; break;
        case 0xAA: case 0xB6: isShift = false; break;
        case 0x3A: if(isShift){
            isShift = false;
        }
        else{
            isShift = false;
        }
        break; //Capital letters are blocked because it crashes the os

        case 0x0E: printf("\3"); break;

        case 0x39: printf(" "); break;
            
        default: // To tell that a unmapped key is pressed on the keyboard
            if (key < 0x80){
                printf("KEYBOARD 0x");
                printHex(key);
            }
        break;
    }
    

    return esp;
}

char* KeyboardDriver::KeycodeToASCII(uint8_t Keycode)
{
    char* result;
    if (Keycode < 0x80)
    {
         switch (Keycode)
        {
            case 0xFA: break;
            case 0x45: case 0xC5: break;
            case 0x29:
            result = "`"; break;
            case 0x02:
            result = "1"; break;
            case 0x03:
            result = "2"; break;
            case 0x04:
            result = "3"; break;
            case 0x05:
            result = "4"; break;
            case 0x06:
            result = "5"; break;
            case 0x07:
            result = "6"; break;
            case 0x08:
            result = "7"; break;
            case 0x09:
            result = "8"; break;
            case 0x0A:
            result = "9"; break;
            case 0x0B:
            result = "0"; break;
            case 0x0C:
            result = "-"; break;
            case 0x0D:
            result = "="; break;

            case 0x10:
            result = "q";  break;
            case 0x11:
            result = "w";  break;
            case 0x12:
            result = "e";  break;
            case 0x13:
            result = "r";  break;
            case 0x14:
            result = "t";  break;
            case 0x15:
            result = "y";  break;
            case 0x16:
            result = "u";  break;
            case 0x17:
            result = "i";  break;
            case 0x18:
            result = "o";  break;
            case 0x19:
            result = "p";  break;
            case 0x1A:
            result = "[";  break;
            case 0x1B:
            result = "]";  break;
            case 0x1C:
            result = "\n"; break;

            case 0x1E: result = "a"; break;
            case 0x1F: result = "s"; break;
            case 0x20: result = "d"; break;
            case 0x21: result = "f"; break;
            case 0x22: result = "g"; break;
            case 0x23: result = "h"; break;
            case 0x24: result = "j"; break;
            case 0x25: result = "k"; break;
            case 0x26: result = "l"; break;
            case 0x27: result = ";"; break;

            case 0x2C:result = "z"; break;
            case 0x2D:result = "x"; break;
            case 0x2E:result = "c"; break;
            case 0x2F:result = "v"; break;
            case 0x30:result = "b"; break;
            case 0x31:result = "n"; break;
            case 0x32:result = "m"; break;
            case 0x33:result = ","; break;
            case 0x34:result = "."; break;
            case 0x35:result = "/"; break;
            case 0x39:result = " "; break;
        }
    }
    return result;
}

void KeyboardDriver::clear_key_buffer()
{
    char* zeros = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\a";
    for (int i = 0; zeros[i] != '\a'; i++)
    {
        key_buffer[i] = "\0";
    }
    keybufferpoint = 0;
    
}

void KeyboardDriver::CommandInterpreter()
{
    printf("\n");
    if(key_buffer[0] == "e" & key_buffer[1] == "c" & key_buffer[2] == "h" & key_buffer[3] == "o")
    {
        for (int i = 5; key_buffer[i] != "\n"; i++)
        {
            printf(key_buffer[i]);
        }
    }
    else if (key_buffer[0] == "h" & key_buffer[1] == "e" & key_buffer[2] == "l" & key_buffer[3] == "p")
    {
        if(key_buffer[5] == "s" & key_buffer[6] == "d")
        {
            printf("sd <options> : \nh to halt the computer");
        }
        else
            printf("Help:\necho <message> : to print the message in the console \nhelp : to show this message \nclear : to clear the screen \nsd <options> : controls the power of the computer \nadd1 <num1> <num2> :To add 2 numbers.This command only supports 1 digit number\nsub1 <num1> <num2> :to subtract 2 numbers.This command only supports 1 digit number");

    }
    else if (key_buffer[0] == "c" & key_buffer[1] == "l" & key_buffer[2] == "e" & key_buffer[3] == "a" & key_buffer[4] == "r")
    {
        printf("\5");
        printf("screen cleared");
    }
    else if (key_buffer[0] == "s" & key_buffer[1] == "d")
    {
        if(key_buffer[3] == "h")
        {
            printf("Halting the computer...");
            __asm__ volatile("hlt");
        }
    }
    else if(key_buffer[0] == "a" & key_buffer[1] == "d" & key_buffer[2] == "d" & key_buffer[3] == "1" )
    {
        char* arg1;
        char *arg2;
        arg1 = key_buffer[5];
        arg2 = key_buffer[7];
        int resint;
        int x = arg1[0] - '0';
        int y = arg2[0] - '0';
        resint = x + y;
        printint(resint);
        
    }
    else if(key_buffer[0] == "s" && key_buffer[1] == "u" & key_buffer[2] == "b" & key_buffer[3] == "1")
    {
        char* arg1;
        char *arg2;
        arg1 = key_buffer[5];
        arg2 = key_buffer[7];
        int resint;
        int x = arg1[0] - '0';
        int y = arg2[0] - '0';
        resint = x - y;
        printint(resint);
    }
    else if(key_buffer[0] == "m" & key_buffer[1] == "e" & key_buffer[2] == "m")
    {
        printf("function not implemented");
        /*
        port8BIT port(0x70);
        port8BIT inport(0x71);

        unsigned short total;
        unsigned char lowmem, highmem;
    
        port.WriteToPort(0x30);
        lowmem = inport.ReadFromPort();
        port.WriteToPort(0x31);
        highmem = inport.ReadFromPort();
    
        total = lowmem | highmem << 8;

        printHex(total);
        */
    }
    else
    {
        printf("Unknown Command. Type help in console to get all the commands");
    }
    printf("\n");
    printf("$: ");
    clear_key_buffer();
}