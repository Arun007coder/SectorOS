#include "Keyboard.h"

static int keybufferpoint = 1;
char ASCII;
static bool isShift = false;
void printf(char*);
void printfchar(char st);

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
:InterruptHandler(0x21, manager),
DataPort(0x60),
CommandPort(0x64)
{
    while(CommandPort.ReadFromPort() & 0x1)
        DataPort.ReadFromPort();
    CommandPort.WriteToPort(0xAE); // Activates the interrupts
    CommandPort.WriteToPort(0x20); // Gets current state
    uint8_t status = (DataPort.ReadFromPort() | 1) & ~0x10;
    CommandPort.WriteToPort(0x60); // set state
    DataPort.WriteToPort(status);

    clear_key_buffer();
    DataPort.WriteToPort(0xF4);
    printf("$: ");
    clear_key_buffer();
}

KeyboardDriver::~KeyboardDriver()
{
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = DataPort.ReadFromPort();
    if(key < 0x80){
        key_buffer[keybufferpoint] = KeycodeToASCII(key);
        keybufferpoint++;
    }

    switch (key)
    {
        case 0xFA: break;
        case 0x45: case 0xC5: case 0x61: clear_key_buffer(); break;
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
        case 0x0F: printf("   "); break;
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
        case 0x1C: CLI();break; //Enter
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
        case 0x28:if (!isShift) printf("'"); else printf("'"); break;
        case 0x2B:if (!isShift) printf("\ "); else printf("|"); break;
        case 0x2A: isShift = true; break;
        case 0x2C:if (!isShift) printf("z"); else printf("Z"); break;
        case 0x2D:if (!isShift) printf("x"); else printf("X"); break;
        case 0x2E:if (!isShift) printf("c"); else printf("C"); break;
        case 0x2F:if (!isShift) printf("v"); else printf("V"); break;
        case 0x30:if (!isShift) printf("v"); else printf("B"); break;
        case 0x31:if (!isShift) printf("n"); else printf("N"); break;
        case 0x32:if (!isShift) printf("m"); else printf("M"); break;
        case 0x33:if (!isShift) printf(","); else printf("<"); break;
        case 0x34:if (!isShift) printf("."); else printf(">"); break;
        case 0x35:if (!isShift) printf("/"); else printf("?"); break;
        case 0x36: isShift = true; break;
        case 0xAA: case 0xB6: isShift = false; break;
        case 0x3A: if(isShift){
            isShift = false;
        }
        else{
            isShift = true;
        }
        break;

        case 0x1D: printf(" CTRL "); break;
        case 0x5B: break;
        case 0x38: clear_key_buffer(); break;
        case 0x39: printf(" "); break;
        case 0x0E: printf("\3"); break;

            
        default:
            if (key < 0x80){
                char* foo = " KEYBOARD 0x00 ";
                char* hex = "0123456789ABCDEF";
                foo[12] = hex[(key >> 4) & 0x0F];
                foo[13] = hex[key & 0x0F];
                printf(foo);
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
            case 0x29:if (!isShift) result = "`"; else result = "~"; break;
            case 0x02:if (!isShift) result = "1"; else result = "!"; break;
            case 0x03:if (!isShift) result = "2"; else result = "@"; break;
            case 0x04:if (!isShift) result = "3"; else result = "#"; break;
            case 0x05:if (!isShift) result = "4"; else result = "$"; break;
            case 0x06:if (!isShift) result = "5"; else result = "%"; break;
            case 0x07:if (!isShift) result = "6"; else result = "^"; break;
            case 0x08:if (!isShift) result = "7"; else result = "&"; break;
            case 0x09:if (!isShift) result = "8"; else result = "*"; break;
            case 0x0A:if (!isShift) result = "9"; else result = "("; break;
            case 0x0B:if (!isShift) result = "0"; else result = ")"; break;
            case 0x0C:if (!isShift) result = "-"; else result = "_"; break;
            case 0x0D:if (!isShift) result = "="; else result = "+"; break;

            case 0x10:if (!isShift) result = "q"; else result = "Q"; break;
            case 0x11:if (!isShift) result = "w"; else result = "W"; break;
            case 0x12:if (!isShift) result = "e"; else result = "E"; break;
            case 0x13:if (!isShift) result = "r"; else result = "R"; break;
            case 0x14:if (!isShift) result = "t"; else result = "T"; break;
            case 0x15:if (!isShift) result = "y"; else result = "Y"; break;
            case 0x16:if (!isShift) result = "u"; else result = "U"; break;
            case 0x17:if (!isShift) result = "i"; else result = "I"; break;
            case 0x18:if (!isShift) result = "o"; else result = "O"; break;
            case 0x19:if (!isShift) result = "p"; else result = "P"; break;
            case 0x1A:if (!isShift) result = "["; else result = "{"; break;
            case 0x1B:if (!isShift) result = "]"; else result = "}"; break;
            case 0x1C:if (!isShift) result = "\n"; else result = "\n"; break;

            case 0x1E:if (!isShift) result = "a"; else result = "A"; break;
            case 0x1F:if (!isShift) result = "s"; else result = "S"; break;
            case 0x20:if (!isShift) result = "d"; else result = "D"; break;
            case 0x21:if (!isShift) result = "f"; else result = "F"; break;
            case 0x22:if (!isShift) result = "g"; else result = "G"; break;
            case 0x23:if (!isShift) result = "h"; else result = "H"; break;
            case 0x24:if (!isShift) result = "j"; else result = "J"; break;
            case 0x25:if (!isShift) result = "k"; else result = "K"; break;
            case 0x26:if (!isShift) result = "l"; else result = "L"; break;
            case 0x27:if (!isShift) result = ";"; else result = ":"; break;

            case 0x2C:if (!isShift) result = "z"; else result = "Z"; break;
            case 0x2D:if (!isShift) result = "x"; else result = "X"; break;
            case 0x2E:if (!isShift) result = "c"; else result = "C"; break;
            case 0x2F:if (!isShift) result = "v"; else result = "V"; break;
            case 0x30:if (!isShift) result = "v"; else result = "B"; break;
            case 0x31:if (!isShift) result = "n"; else result = "N"; break;
            case 0x32:if (!isShift) result = "m"; else result = "M"; break;
            case 0x33:if (!isShift) result = ","; else result = "<"; break;
            case 0x34:if (!isShift) result = "."; else result = ">"; break;
            case 0x35:if (!isShift) result = "/"; else result = "?"; break;
            case 0x39:if (!isShift) result = " "; else result = " "; break;
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

void KeyboardDriver::CLI()
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
        if(key_buffer[5] == "\0")
        {
            printf("Help:\necho <message> : to print the message in the console \nhelp : to show this message \nclear : to clear the screen \nsd <options> : controls the power of the computer");
        }
        else if(key_buffer[5] == "s" & key_buffer[6] == "d")
        {
            printf("sd <options> : \nh to halt the computer");
        }
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
    else
    {
        printf("Unknown Command. Type help in console to get all the commands");
    }
    printf("\n");
    printf("$: ");
    clear_key_buffer();
}