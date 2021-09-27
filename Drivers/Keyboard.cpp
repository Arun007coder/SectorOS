#include "Keyboard.h"

static int keybufferpoint = 0;
static bool isShift = false;
void printf(char*);
void printfchar(char st);
void printHex(uint8_t key);

KeyboardEventHandler::KeyboardEventHandler()
{
}

void KeyboardEventHandler::onKeyDown(char c)
{
    char* foo = " ";
    foo[0] = c;
    printf(foo);
}

void KeyboardEventHandler::onKeyUp(char)
{

}

KeyboardDriver::KeyboardDriver(InterruptManager* manager , KeyboardEventHandler *handler)
:InterruptHandler(0x21, manager),
DataPort(0x60),
CommandPort(0x64)
{
    this->handler = handler;
    clear_key_buffer();
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
    uint8_t key = DataPort.ReadFromPort();
    if(key < 0x80 & key != 0x2A & key != 0x36 & key != 0x3A & key != 0x0E ){
        key_buffer[keybufferpoint] = KeycodeToASCII(key);
        keybufferpoint++;
    }
    if(handler == 0)
        return esp;

    switch (key)
    {
        case 0xFA: break;
        case 0x45: case 0xC5: case 0x61: clear_key_buffer(); break;
        case 0x29:if (!isShift) handler->onKeyDown('`'); else handler->onKeyDown('~'); break;
        case 0x02:if (!isShift) handler->onKeyDown('1'); else handler->onKeyDown('!'); break;
        case 0x03:if (!isShift) handler->onKeyDown('2'); else handler->onKeyDown('@'); break;
        case 0x04:if (!isShift) handler->onKeyDown('3'); else handler->onKeyDown('#'); break;
        case 0x05:if (!isShift) handler->onKeyDown('4'); else handler->onKeyDown('$'); break;
        case 0x06:if (!isShift) handler->onKeyDown('5'); else handler->onKeyDown('%'); break;
        case 0x07:if (!isShift) handler->onKeyDown('6'); else handler->onKeyDown('^'); break;
        case 0x08:if (!isShift) handler->onKeyDown('7'); else handler->onKeyDown('&'); break;
        case 0x09:if (!isShift) handler->onKeyDown('8'); else handler->onKeyDown('*'); break;
        case 0x0A:if (!isShift) handler->onKeyDown('9'); else handler->onKeyDown('('); break;
        case 0x0B:if (!isShift) handler->onKeyDown('0'); else handler->onKeyDown(')'); break;
        case 0x0C:if (!isShift) handler->onKeyDown('-'); else handler->onKeyDown('_'); break;
        case 0x0D:if (!isShift) handler->onKeyDown('='); else handler->onKeyDown('+'); break;
        //case 0x0F: handler->onKeyDown('   '); break;
        case 0x10:if (!isShift) handler->onKeyDown('q'); else handler->onKeyDown('Q'); break;
        case 0x11:if (!isShift) handler->onKeyDown('w'); else handler->onKeyDown('W'); break;
        case 0x12:if (!isShift) handler->onKeyDown('e'); else handler->onKeyDown('E'); break;
        case 0x13:if (!isShift) handler->onKeyDown('r'); else handler->onKeyDown('R'); break;
        case 0x14:if (!isShift) handler->onKeyDown('t'); else handler->onKeyDown('T'); break;
        case 0x15:if (!isShift) handler->onKeyDown('y'); else handler->onKeyDown('Y'); break;
        case 0x16:if (!isShift) handler->onKeyDown('u'); else handler->onKeyDown('U'); break;
        case 0x17:if (!isShift) handler->onKeyDown('i'); else handler->onKeyDown('I'); break;
        case 0x18:if (!isShift) handler->onKeyDown('o'); else handler->onKeyDown('O'); break;
        case 0x19:if (!isShift) handler->onKeyDown('p'); else handler->onKeyDown('P'); break;
        case 0x1A:if (!isShift) handler->onKeyDown('['); else handler->onKeyDown('{'); break;
        case 0x1B:if (!isShift) handler->onKeyDown(']'); else handler->onKeyDown('}'); break;
        case 0x1C: CLI(); break; //Enter
        case 0x1E:if (!isShift) handler->onKeyDown('a'); else handler->onKeyDown('A'); break;
        case 0x1F:if (!isShift) handler->onKeyDown('s'); else handler->onKeyDown('S'); break;
        case 0x20:if (!isShift) handler->onKeyDown('d'); else handler->onKeyDown('D'); break;
        case 0x21:if (!isShift) handler->onKeyDown('f'); else handler->onKeyDown('F'); break;
        case 0x22:if (!isShift) handler->onKeyDown('g'); else handler->onKeyDown('G'); break;
        case 0x23:if (!isShift) handler->onKeyDown('h'); else handler->onKeyDown('H'); break;
        case 0x24:if (!isShift) handler->onKeyDown('j'); else handler->onKeyDown('J'); break;
        case 0x25:if (!isShift) handler->onKeyDown('k'); else handler->onKeyDown('K'); break;
        case 0x26:if (!isShift) handler->onKeyDown('l'); else handler->onKeyDown('L'); break;
        case 0x27:if (!isShift) handler->onKeyDown(';'); else handler->onKeyDown(':'); break;
        case 0x28:if (!isShift) handler->onKeyDown('E'); else handler->onKeyDown('e'); break;
        //case 0x2B:if (!isShift) handler->onKeyDown('\ '); else handler->onKeyDown('|'); break; 
        case 0x2A: isShift = false; break;
        case 0x2C:if (!isShift) handler->onKeyDown('z'); else handler->onKeyDown('Z'); break;
        case 0x2D:if (!isShift) handler->onKeyDown('x'); else handler->onKeyDown('X'); break;
        case 0x2E:if (!isShift) handler->onKeyDown('c'); else handler->onKeyDown('C'); break;
        case 0x2F:if (!isShift) handler->onKeyDown('v'); else handler->onKeyDown('V'); break;
        case 0x30:if (!isShift) handler->onKeyDown('v'); else handler->onKeyDown('B'); break;
        case 0x31:if (!isShift) handler->onKeyDown('n'); else handler->onKeyDown('N'); break;
        case 0x32:if (!isShift) handler->onKeyDown('m'); else handler->onKeyDown('M'); break;
        case 0x33:if (!isShift) handler->onKeyDown(','); else handler->onKeyDown('<'); break;
        case 0x34:if (!isShift) handler->onKeyDown('.'); else handler->onKeyDown('>'); break;
        case 0x35:if (!isShift) handler->onKeyDown('/'); else handler->onKeyDown('?'); break;
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

        case 0x39: handler->onKeyDown(' '); break;
            
        default:
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
            case 0x29:if (!isShift) result = "`"; break;
            case 0x02:if (!isShift) result = "1"; break;
            case 0x03:if (!isShift) result = "2"; break;
            case 0x04:if (!isShift) result = "3"; break;
            case 0x05:if (!isShift) result = "4"; break;
            case 0x06:if (!isShift) result = "5"; break;
            case 0x07:if (!isShift) result = "6"; break;
            case 0x08:if (!isShift) result = "7"; break;
            case 0x09:if (!isShift) result = "8"; break;
            case 0x0A:if (!isShift) result = "9"; break;
            case 0x0B:if (!isShift) result = "0"; break;
            case 0x0C:if (!isShift) result = "-"; break;
            case 0x0D:if (!isShift) result = "="; break;

            case 0x10:if (!isShift) result = "q";  break;
            case 0x11:if (!isShift) result = "w";  break;
            case 0x12:if (!isShift) result = "e";  break;
            case 0x13:if (!isShift) result = "r";  break;
            case 0x14:if (!isShift) result = "t";  break;
            case 0x15:if (!isShift) result = "y";  break;
            case 0x16:if (!isShift) result = "u";  break;
            case 0x17:if (!isShift) result = "i";  break;
            case 0x18:if (!isShift) result = "o";  break;
            case 0x19:if (!isShift) result = "p";  break;
            case 0x1A:if (!isShift) result = "[";  break;
            case 0x1B:if (!isShift) result = "]";  break;
            case 0x1C:if (!isShift) result = "\n"; break;

            case 0x1E:if (!isShift) result = "a"; break;
            case 0x1F:if (!isShift) result = "s"; break;
            case 0x20:if (!isShift) result = "d"; break;
            case 0x21:if (!isShift) result = "f"; break;
            case 0x22:if (!isShift) result = "g"; break;
            case 0x23:if (!isShift) result = "h"; break;
            case 0x24:if (!isShift) result = "j"; break;
            case 0x25:if (!isShift) result = "k"; break;
            case 0x26:if (!isShift) result = "l"; break;
            case 0x27:if (!isShift) result = ";"; break;

            case 0x2C:if (!isShift) result = "z"; break;
            case 0x2D:if (!isShift) result = "x"; break;
            case 0x2E:if (!isShift) result = "c"; break;
            case 0x2F:if (!isShift) result = "v"; break;
            case 0x30:if (!isShift) result = "v"; break;
            case 0x31:if (!isShift) result = "n"; break;
            case 0x32:if (!isShift) result = "m"; break;
            case 0x33:if (!isShift) result = ","; break;
            case 0x34:if (!isShift) result = "."; break;
            case 0x35:if (!isShift) result = "/"; break;
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