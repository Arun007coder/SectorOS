#include "Keyboard.h"

static int keybufferpoint = 1;
char ASCII;

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

    DataPort.WriteToPort(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char*);
void printfchar(char st);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = DataPort.ReadFromPort();
    if (key < 0x80)
    {
        key_buffer[keybufferpoint] = KeycodeToASCII(key);
        keybufferpoint++;

        switch (key)
        {
            case 0xFA: break;
            case 0x45: case 0xC5: case 0x61: break;
            case 0x29: printf("`"); break;
            case 0x02: printf("1"); break;
            case 0x03: printf("2"); break;
            case 0x04: printf("3"); break;
            case 0x05: printf("4"); break;
            case 0x06: printf("5"); break;
            case 0x07: printf("6"); break;
            case 0x08: printf("7"); break;
            case 0x09: printf("8"); break;
            case 0x0A: printf("9"); break;
            case 0x0B: printf("0"); break;
            case 0x0C: printf("-"); break;
            case 0x0D: printf("="); break;

            case 0x0F: printf("   "); break;
            case 0x10: printf("q"); break;
            case 0x11: printf("w"); break;
            case 0x12: printf("e"); break;
            case 0x13: printf("r"); break;
            case 0x14: printf("t"); break;
            case 0x15: printf("y"); break;
            case 0x16: printf("u"); break;
            case 0x17: printf("i"); break;
            case 0x18: printf("o"); break;
            case 0x19: printf("p"); break;
            case 0x1A: printf("["); break;
            case 0x1B: printf("]"); break;
            case 0x1C:
                for (int i = 0; key_buffer[i] != "\n"; i++)
                {
                    printf(key_buffer[i]);
                }
                clear_key_buffer();
                
             break;
            case 0x1E: printf("a"); break;
            case 0x1F: printf("s"); break;
            case 0x20: printf("d"); break;
            case 0x21: printf("f"); break;
            case 0x22: printf("g"); break;
            case 0x23: printf("h"); break;
            case 0x24: printf("j"); break;
            case 0x25: printf("k"); break;
            case 0x26: printf("l"); break;
            case 0x27: printf(";"); break;
            case 0x28: printf("'"); break;
            case 0x2B: printf(" BKSLX "); break;

            case 0x2A: printf(" LSHIFT "); break;
            case 0x2C: printf("z"); break;
            case 0x2D: printf("x"); break;
            case 0x2E: printf("c"); break;
            case 0x2F: printf("v"); break;
            case 0x30: printf("v"); break;
            case 0x31: printf("n"); break;
            case 0x32: printf("m"); break;
            case 0x33: printf(","); break;
            case 0x34: printf("."); break;
            case 0x35: printf("/"); break;
            case 0x36: clear_key_buffer(); break;

            case 0x1D: printf(" CTRL "); break;
            case 0x5B: break;
            case 0x38: printf(" LALT "); break;
            case 0x39: printf(" "); break;

            

            default:
                char* foo = " KEYBOARD 0x00 ";
                char* hex = "0123456789ABCDEF";
                foo[12] = hex[(key >> 4) & 0x0F];
                foo[13] = hex[key & 0x0F];
                printf(foo);
            break;
        }
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
            case 0x29: result = "`"; break;
            case 0x02: result = "1"; break;
            case 0x03: result = "2"; break;
            case 0x04: result = "3"; break;
            case 0x05: result = "4"; break;
            case 0x06: result = "5"; break;
            case 0x07: result = "6"; break;
            case 0x08: result = "7"; break;
            case 0x09: result = "8"; break;
            case 0x0A: result = "9"; break;
            case 0x0B: result = "0"; break;
            case 0x0C: result = "-"; break;
            case 0x0D: result = "="; break;

            //?case 0x0F: printf("   "); break;
            case 0x10: result = "q"; break;
            case 0x11: result = "w"; break;
            case 0x12: result = "e"; break;
            case 0x13: result = "r"; break;
            case 0x14: result = "t"; break;
            case 0x15: result = "y"; break;
            case 0x16: result = "u"; break;
            case 0x17: result = "i"; break;
            case 0x18: result = "o"; break;
            case 0x19: result = "p"; break;
            case 0x1A: result = "["; break;
            case 0x1B: result = "]"; break;
            case 0x1C: result = "\n"; break;

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
            //?case 0x28: result = ""'; break;

            case 0x2C: result = "z"; break;
            case 0x2D: result = "x"; break;
            case 0x2E: result = "c"; break;
            case 0x2F: result = "v"; break;
            case 0x30: result = "v"; break;
            case 0x31: result = "n"; break;
            case 0x32: result = "m"; break;
            case 0x33: result = ","; break;
            case 0x34: result = "."; break;
            case 0x35: result = "/"; break;
            case 0x39: result = " "; break;
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