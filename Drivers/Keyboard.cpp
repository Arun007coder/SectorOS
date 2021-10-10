#include "Keyboard.h"

static int keybufferpoint = 0;
static bool isShift = false;
static bool isCTRLed = false;
void printf(char*);
void printfchar(char st);
void printHex(uint8_t key);
void PrintDate();
char* INTTOCHARPOINT(int num);
void ColourPrint(int type);
bool txtcolor;

PowerControl power;

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
:InterruptHandler(0x21, manager),
DataPort(0x60),
CommandPort(0x64)
{
    isTxtMode = false;
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
    if(key < 0x80 & key != 0x3A & key != 0x2A & key != 0x2A & key != 0x36 & key != 0x3A & key != 0x0E & key != 0x38 & key != 0x1D ){
        key_buffer[keybufferpoint] = KeycodeToASCII(key);
        keybufferpoint++;
    }
    switch (key)
    {
        case 0xFA: break;
        case 0x45: case 0xC5: case 0x61: clear_key_buffer(); break;
        //case 0x1D: case 0x38: break; // It is used by Vmware workstation player to lose focus
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
        case 0x1C: if(!isTxtMode)
                        CommandInterpreter();
                   else
                        printf("\n");
        break; //Enter
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
        case 0x2A: isShift = true; break;
        case 0x2C:if (!isShift) printf("z"); else printf("Z"); break;
        case 0x2D:if (!isShift) printf("x"); else printf("X"); break;
        case 0x2E:
            if(isTxtMode){

                if(!isCTRLed)
                    if (!isShift) printf("c"); else printf("C"); 
                else
                    returnHScreen();
            }
            else
            {
                if (!isShift) printf("c"); else printf("C");
            }
        break;
        case 0x2F:if (!isShift) printf("v"); else printf("V"); break;
        case 0x30:if (!isShift) printf("b"); else printf("B"); break;
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

        case 0x1D:
            isCTRLed = true;
        break;
        case 0xE0:
            isCTRLed = false;
        break;
        case 0x9D: //LeftCTRL return code
            isCTRLed = false;
        break;
        case 0x0E:
            if(!isTxtMode)
                printf("\3");
            else
                printf("\f"); 
            if(keybufferpoint != 0)
                keybufferpoint--; 
            break;

        case 0x39: printf(" "); break;

        
        case 0x48 : 
            if(isTxtMode)
                printf("\7");
        break;

        case 0x50 : 
            if(isTxtMode)
                printf("\2"); 
        break;

        case 0x4B : 
            if(isTxtMode)
                printf("\4");
            else
                printf("\4"); 
                if(keybufferpoint != 0)
                    keybufferpoint--;
        break;

        case 0x4D : 
            if(isTxtMode)
                printf("\6");
            else
                printf("\6"); 
                if(keybufferpoint != 0)
                    keybufferpoint++;
         break;
            
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

void KeyboardDriver::CommandInterpreter()
{
    printf("\n");
    if(key_buffer[0] == "e" & key_buffer[1] == "c" & key_buffer[2] == "h" & key_buffer[3] == "o")
    {
        if (key_buffer[4] == "\0")
        {
            printf("cannot print null character");
        }
        else
        {
            for (int i = 5; key_buffer[i] != "\n"; i++)
            {
                printf(key_buffer[i]);
            }
        }
    }
    else if (key_buffer[0] == "h" & key_buffer[1] == "e" & key_buffer[2] == "l" & key_buffer[3] == "p")
    {
        if(key_buffer[5] == "s" & key_buffer[6] == "d")
        {
            printf("sd <options> : \nh : to halt the computer\nr : To reboot he computer\nsv : If running in virtualbox. then shutdown");
        }
        else if(key_buffer[5] == "1")
            printf("Help page 1:\necho <message> : to print the message in the console \nhelp : to show this message \nclear : to clear the screen \nsd <options> : controls the power of the computer ");
        else if(key_buffer[5] == "2")
            printf("Help page 2:\nadd1 <num1> <num2> :To add 2 numbers.This command only supports 1 digit number\nsub1 <num1> <num2> :to subtract 2 numbers.This command only supports 1 digit number\ntxt : To enter the text mode. You cannot save files\nmul1 <num1> <num2> : To multiply 2 numbers. ");
        else
            printf("Help page 1:\necho <message> : to print the message in the console \nhelp : to show this message \nclear : to clear the screen \nsd <options> : controls the power of the computer ");
    }
    else if (key_buffer[0] == "c" & key_buffer[1] == "l" & key_buffer[2] == "e" & key_buffer[3] == "a" & key_buffer[4] == "r")
    {
        printf("\5");
        printf("SectorOS Monolithic kernel ");PrintDate();printf("                               Type: Shell\n");
    }
    else if (key_buffer[0] == "s" & key_buffer[1] == "d")
    {
        if(key_buffer[3] == "h")
        {
            power.halt();
        }
        else if(key_buffer[3] == "s" & key_buffer[4] == "v")
        {
            power.StopVirtualBox();
        }
        else if(key_buffer[3] == "r")
        {
            power.reboot();
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
        printf(INTTOCHARPOINT(resint));
        
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
        printf(INTTOCHARPOINT(resint));
    }
    else if(key_buffer[0] == "m" & key_buffer[1] == "u" & key_buffer[2] == "l" & key_buffer[3] == "1" )
    {
        char* arg1;
        char* arg2;
        arg1 = key_buffer[5];
        arg2 = key_buffer[7];

        int res;

        int x = arg1[0] - '0';
        int y = arg2[0] - '0';

        res = x * y;

        printf(INTTOCHARPOINT(res));
    }
    /*
    else if(key_buffer[0] == "m" & key_buffer[1] == "e" & key_buffer[2] == "m")
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

        printf(INTTOCHARPOINT(total));
    }*/
    else if(key_buffer[0] == "t" & key_buffer[1] == "x" & key_buffer[2] == "t")
    {
        printf("Entering Text editing mode. Please wait....");
        for(int i = 999999999; i != 0; i--);
        printf("\5");
        /*
        if(txtcolor){
            ColourPrint(1);
            txtcolor = false;
        }
        else
        {
            ColourPrint(0);
            txtcolor = true;
        }
        */


        
        RTC rtclock;
        rtclock.read_rtc();
        printf("welcome to SectorOS text mode ");PrintDate(); printf("                             Type: Text ");printf("This is experimental. you cannot save the documents . To return to CLI press LCTRL+C");
        isTxtMode = true;
    }
    /*
    else if(key_buffer[0] == "o" & key_buffer[1] == "u" & key_buffer[2] == "t" & key_buffer[3] == "p" & key_buffer[4] == "o" & key_buffer[5] == "r" & key_buffer[6] == "t")
    {
        char* arg1 = key_buffer[10];
        uint16_t portno = *arg1 - '0';
        port8BIT dp(portno);
        port8BIT cp(portno);
        if(key_buffer[8] == "c")
        {
            printHex(cp.ReadFromPort());
        }
        else if (key_buffer[8] == "d")
        {
            printHex(dp.ReadFromPort());
        }
        else{
            printf("debug");
        }
    }
    */
    /*
    else if(key_buffer[0] == "i" & key_buffer[1] == "n" & key_buffer[2] == "p" & key_buffer[3] == "o" & key_buffer[4] == "r" & key_buffer[5] == "t")
    {
        char* arg2 = key_buffer[11];
        uint16_t portno = *arg2 - '0';
        port8BIT dp(portno);
        port8BIT cp(portno);
        char* arg1 = key_buffer[9];
        uint8_t data = *arg1 - '0';
        if(key_buffer[7] == "c")
        {
            cp.WriteToPort(data);
            printf("data writen to dataport");
        }
        else if(key_buffer[7] == "d")
        {
            dp.WriteToPort(data);
            printf("data written to dataport");
        }
    }
    */
    else
    {
        printf("Unknown Command. Type help in console to get all the commands");
    }
    if(!isTxtMode){
        printf("\n");
        printf("$: ");
    }
    clear_key_buffer();
}

void KeyboardDriver::returnHScreen()
{
    clear_key_buffer();
    printf("\5");
    //ColourPrint(1);
    //ColourPrint(0);
    //printf("Welcome to SectorOS Monolithic kernel                               Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");

    printf("SectorOS Monolithic kernel ");
    PrintDate();
    printf("                               Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");
    printf("Run help to get the list of commands which is implemented \n \n");

    printf("$: ");
    isTxtMode = false;
}

