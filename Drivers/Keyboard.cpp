#include "Keyboard.h"

static bool isCTRLed = false;
void BootSound();
bool canNewLine = true;

void StartWEBServer(uint16_t port);

void PrintPrompt();

bool isUSRChanged = false;

extern void *allocated;
extern uint32_t *memupper;

size_t heap = 10 * 1024 * 1024;

void printf(char *);
void printfchar(char st);
void printHex(uint8_t key);
void PrintDate();
char* INTTOCHARPOINT(int num);
void beep();
void ColourPrint(int type);
bool txtcolor;
bool isESPChanged = false;
inline GlobalDescriptorTable gdt;
void taskA();
void taskB();
const void* mb;
void PrintHDD();
void detect_cpu();
void PrintSATA();
void PVGA();
void PrintPartitions();

PowerControl power;

CustomShell::CustomShell()
{
	printf("Event handler init failed\n");
}

CustomShell::~CustomShell()
{
}

void CustomShell::Shell()
{
	
}

void CustomShell::clearBuffer()
{
}

void CustomShell::OnKeyDown(uint8_t key)
{
}

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
:InterruptHandler(0x21, manager),
DataPort(0x60),
CommandPort(0x64)
{
	shell = 0;
	clear_key_buffer();
	isTxtMode = false;

	for(int i = 0; i < 30; i++)
	{
		SP[i] = "\0";
	}

	char *Shell_Prompt[14] = {"%", "u", "@", "%", "h", "[", "%", "d" , "}" , ";", "~", "#", ">", " "};
	for (int i = 0; i < 14; i++)
	{
		SP[SPIndex] = Shell_Prompt[i];
		SPIndex++;
	}
}

KeyboardDriver::~KeyboardDriver()
{
}

void KeyboardDriver::activate()
{
	while (CommandPort.ReadFromPort() & 0x1)
		DataPort.ReadFromPort();
	CommandPort.WriteToPort(0xAE); // Activates the interrupts
	CommandPort.WriteToPort(0x20); // Gets current state
	uint8_t status = (DataPort.ReadFromPort() | 1) & ~0x10;
	CommandPort.WriteToPort(0x60); // set state
	DataPort.WriteToPort(status);

	DataPort.WriteToPort(0xF4);
}

int KeyboardDriver::UniquedriverID()
{
	return UDID_KEYBOARD;
}

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
	OLDESP = esp;
	uint8_t key = DataPort.ReadFromPort(); // The variable where a single keystroke is stored
	if (shell != 0 && IsShellDisabled)
	{
		shell->OnKeyDown(key);
	}

	if(key < 0x80 & key != 0x3A & key != 0x2A & key != 0x2A & key != 0x36 & key != 0x3A & key != 0x0E & key != 0x38 & key != 0x1D ){
		if(!IsShellDisabled)
		{
			if(!isTxtMode)
			{
				key_buffer[key_buffer_index] = KeycodeToASCII(key);
				key_buffer_index++;
			}
		}
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
		case 0x1C: 
					if(!IsShellDisabled)
					{
						if(!isTxtMode)
							CommandInterpreter();
						else
							printf("\n");
					}
					else
					{
						if (shell != 0)
							shell->Shell();
					}
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
		case 0x2B:if (!isShift) printf("\\"); else printf("|"); break;
		case 0x2A: isShift = true; break;
		case 0x2C:if (!isShift) printf("z"); else printf("Z"); break;
		case 0x2D:if (!isShift) printf("x"); else printf("X"); break;
		case 0x2E:
			if(isTxtMode){

				if(!isCTRLed)
					if (!isShift) printf("c"); else printf("C"); 
				else
				{
					returnHScreen();
				}
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
			if(key_buffer_index != 0)
				key_buffer_index--; 
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
				if(key_buffer_index != 0)
					key_buffer_index--;
		break;

		case 0x3E :
			for (int i = 0; i < key_buffer_index; i++)
			{
				printf(key_buffer[i]);
			}
		break;


		case 0x4D : 
			if(isTxtMode)
				printf("\6");
			else
				printf("\6"); 
				if(key_buffer_index != 0)
					key_buffer_index++;
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



void KeyboardDriver::clear_key_buffer()
{
	if(!IsShellDisabled)
	{
		char* zeros = "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000\a";
		for (int i = 0; zeros[i] != '\a'; i++)
		{
			key_buffer[i] = "\0";
		}
		key_buffer_index = 0;
	}
	else
	{
		if(shell != 0)
			shell->clearBuffer();
	}
}

// TODO: Move the Command Interpreter to a new class
void KeyboardDriver::CommandInterpreter() // SOSH v1.0.3 [SectorOS SHell]. 11 Commands 
{
	if (!IsShellDisabled)
	{
		char* COMNAME;
		serialport.logToSerialPort("Command Interpreter Initialised");
		printf("\n");
		if(key_buffer[0] == "e" & key_buffer[1] == "c" & key_buffer[2] == "h" & key_buffer[3] == "o")
		{
			COMNAME = "echo";
			if (key_buffer[4] == "\0")
			{
				printf("cannot print null character");
			}
			else if (key_buffer[5] == "$" && key_buffer[6] == "S" && key_buffer[7] == "P")
			{
				for (int i = 0; i < SPIndex; i++)
				{
					printf(SP[i]);
				}
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
			COMNAME = "help";
			if(key_buffer[5] == "s" & key_buffer[6] == "d")
			{
				printf("sd <options> : \nh : to halt the computer\nr : To reboot he computer\nsv : If running in virtualbox. then shutdown");
			}
			else if (key_buffer[5] == "s" && key_buffer[6] == "y" && key_buffer[7] == "s" && key_buffer[8] == "i" && key_buffer[9] == "n" && key_buffer[10] == "f" && key_buffer[11] == "o")
			{
				printf("sysinfo [options] : \n-C : To get CPU information\n-M : To get Memory Info\n-A : To get the Kernel architecture\n-K : To get kernel information\n-O : To get OS name\n-B : To get kernel build date\n-D : To identify a ATA drive\n-V : To get Kernel Version");
			}
			else if(key_buffer[5] == "1")
				printf("Help page 1:\necho <message> : to print the message in the console \nhelp : to show this message \nclear : to clear the screen \nsd <options> : controls the power of the computer ");
			else if(key_buffer[5] == "2")
				printf("Help page 2:\nadd1 <num1> <num2> :To add 2 numbers.This command only supports 1 digit number\nsub1 <num1> <num2> :to subtract 2 numbers.This command only supports 1 digit number\ntxt : To enter the text mode. You cannot save files\nmul1 <num1> <num2> : To multiply 2 numbers.");
			else if (key_buffer[5] == "3")
				printf("Help page 3:\nspi : To print the data in serial port 0x3F8.\nspo : To write data to serial port 0x3F8.\nsysinfo [option] : To get info about system.\nvga : To use experimental vga graphics.");
			else if (key_buffer[5] == "4")
				printf("Help page 4:\nlspt: To list partitions in a drive.\ntsk:to change instance.[EXPERIMENTAL]\nexport [var]=[value] : To change value of a ENV var in shell.\nbmus : To play the boot music.");
			else if (key_buffer[5] == "5")
				printf("Help page 5:\nwserver <port> : To start a web server on the port.\nnet <options> : To get network information.\nbeep : To make a beep sound.\nls : To list files in the 1st slave drive.\nexec <filename> <extension> : To execute the given file from the 1st slave hard drive.");
			else
				printf("Help page 1:\necho <message> : to print the message in the console \nhelp : to show this message \nclear : to clear the screen \nsd <options> : controls the power of the computer ");
		}
		else if (key_buffer[0] == "c" & key_buffer[1] == "l" & key_buffer[2] == "e" & key_buffer[3] == "a" & key_buffer[4] == "r")
		{
			COMNAME = "clear";
			printf("\5");
				
			printf("SectorOS ");
			printf(KERNEL_VERSION);
			printf("               ");
			PrintDate();
			printf("                            Type: Shell ");
			canNewLine = false;
		}
		else if (key_buffer[0] == "s" & key_buffer[1] == "d")
		{
			COMNAME = "sd";
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
			COMNAME = "add1";
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
			COMNAME = "sub1";
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
			COMNAME = "mul1";
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
		else if(key_buffer[0] == "t" & key_buffer[1] == "x" & key_buffer[2] == "t")
		{                                                                
			COMNAME = "txt";
			serialport.logToSerialPort("\ntxt mode starting...");
			printf("Entering Text editing mode. Please wait....");
			for(int i = 999999999; i != 0; i--);
			printf("\5");
			
			RTC rtclock;
			rtclock.read_rtc();
			printf("welcome to SectorOS text mode ");PrintDate(); printf("                             Type: Text ");printf("This is experimental. you cannot save the documents . To return to CLI press LCTRL+C");
			isTxtMode = true;
			serialport.logToSerialPort("\ntxt mode initialised");
		}
		else if(key_buffer[0] == "s" && key_buffer[1] == "p" && key_buffer[2] == "o")
		{
			for (int i = 4; key_buffer[i] != "\n"; i++)
			{
				serialport.logToSerialPort(key_buffer[i]);
			}
		}
		else if(key_buffer[0] == "s" && key_buffer[1] == "p" && key_buffer[2] == "i")
		{
			printfchar(serialport.read_serial());
		}
		else if(key_buffer[0] == "s" && key_buffer[1] == "y" && key_buffer[2] == "s" && key_buffer[3] == "i" && key_buffer[4] == "n" && key_buffer[5] == "f" && key_buffer[6] == "o")
		{
			COMNAME = "sysinfo";
			if(key_buffer[8] == "-" & key_buffer[9] == "C")
			{
				detect_cpu();
			}
			else if (key_buffer[8] == "-" & key_buffer[9] == "A")
			{
				printf(KERNEL_ARCH);
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "V")
			{
				printf(KERNEL_VERSION);
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "B")
			{
				printf(KERNEL_BUILD);
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "H")
			{
				printf("sysinfo [options] : \n-C : To get CPU information\n-A : To get the Kernel architecture\n-K : To get kernel information\n-O : To get OS name\n-B : To get kernel build date\n-V : To get Kernel Version\n-D : To identify a ATA drive\n-H : To print this message");
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "O")
			{
				printf(OS_NAME);
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "D")
			{
				PrintSATA();
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "M")
			{
				printf("heap: 0x");
				printHex32(heap);
				printf("\nallocated: 0x");
				printHex32((size_t)allocated);
				printf("\n");
				printf("Memory on system: 0x");
				printHex32((*memupper) * 1024);
			}
			else if (key_buffer[8] == "-" && key_buffer[9] == "K")
			{
				printf("SectorOS Kernel "); printf(KERNEL_VERSION); printf(" "); printf(KERNEL_BUILD);
			}
			else
			{
				printf("SectorOS Kernel "); printf(KERNEL_VERSION); printf(" "); printf(KERNEL_BUILD);
			}
		}
		else if (key_buffer[0] == "v" && key_buffer[1] == "g" && key_buffer[2] == "a")
		{
			COMNAME = "vga";
			PVGA();
		}
		else if (key_buffer[0] == "l" && key_buffer[1] == "s" && key_buffer[2] == "p" && key_buffer[3] == "t")
		{
			COMNAME = "lspt";
			PrintPartitions();
		}
		else if (key_buffer[0] == "b" && key_buffer[1] == "m" && key_buffer[2] == "u" && key_buffer[3] == "s")
		{
			COMNAME = "bmus";
			BootSound();
		}
		else if (key_buffer[0] == "w" && key_buffer[1] == "s" && key_buffer[2] == "e" && key_buffer[3] == "r" && key_buffer[4] == "v" && key_buffer[5] == "e" && key_buffer[6] == "r" )
		{
			COMNAME = "wserver";
			StartWEBServer(1234);
		}
		else if (key_buffer[0] == "n" && key_buffer[1] == "e" && key_buffer[2] == "t")
		{
			COMNAME = "net";
			if (DriverManager::ActiveDriverManager->drivers[2] != 0)
			{
				AM79C973* eth0 = (AM79C973*)DriverManager::ActiveDriverManager->GetDriver(UDID_AM79C973);
				if(key_buffer[4] == "-" && key_buffer[5] == "i")
				{
					printf("IP: ");
					printHex32(eth0->GetIPAddress());
				
				}
				else if (key_buffer[4] == "-" && key_buffer[5] == "m")
				{
					printf("Mac Address: ");
					uint64_t MACADDR = eth0->GetMACAddress();
					printHex((MACADDR >> 40) & 0xFF); printf(":");
					printHex((MACADDR >> 32) & 0xFF); printf(":");
					printHex((MACADDR >> 24) & 0xFF); printf(":");
					printHex((MACADDR >> 16) & 0xFF); printf(":");
					printHex((MACADDR >> 8) & 0xFF);  printf(":");
					printHex(MACADDR & 0xFF);
				}
				else if (key_buffer[4] == "-" && key_buffer[5] == "h")
				{
					printf("net [options] : \n-i : To get IP address\n-m : To get mac address\n-h : To print this message");
				}
				else
				{
					printf("Mac Address: ");
					uint64_t MACADDR = eth0->GetMACAddress();
					printHex((MACADDR >> 40) & 0xFF); printf(":");
					printHex((MACADDR >> 32) & 0xFF); printf(":");
					printHex((MACADDR >> 24) & 0xFF); printf(":");
					printHex((MACADDR >> 16) & 0xFF); printf(":");
					printHex((MACADDR >> 8) & 0xFF);  printf(":");
					printHex(MACADDR & 0xFF);
				}
			}
			else
			{
				printf("Driver for ethernet card not found\n");
			}
		}
		else if(key_buffer[0] == "t" && key_buffer[1] == "s" && key_buffer[2] == "k")
		{
			char* arg1 = key_buffer[4]; 
			int tsknum = arg1[0] - '0';
			asm("int $0x80" : : "a" (0x04), "b" (tsknum));
		}
		else if (key_buffer[0] == "s" && key_buffer[1] == "e" && key_buffer[2] == "t")
		{
			COMNAME = "set";
			if (key_buffer[4] == "S" && key_buffer[5] == "P" && key_buffer[6] == "=" )
			{
				SPIndex = 0;
				for (int i = 7; key_buffer[i] != "\n"; i++)
				{
					SP[SPIndex] = key_buffer[i];
					SPIndex++;
				}
				printf("SP is set to : ");
				for (int i = 0; i < SPIndex; i++)
				{
					printf(SP[i]);
				}
			}
			else if (key_buffer[4] == "S" && key_buffer[5] == "P")
			{
				printf("SP is set to : ");
				for (int i = 0; i < SPIndex; i++)
				{
					printf(SP[i]);
				}
			}
			else
			{
				printf("Adding new variable to the environment is not supported yet");
			}
		}
		else if (key_buffer[0] == "b" && key_buffer[1] == "e" && key_buffer[2] == "e" && key_buffer[3] == "p")
		{
			beep();
		}
		else if (key_buffer[0] == "l" && key_buffer[1] == "s")
		{
			COMNAME = "ls";
			AdvancedTechnologyAttachment ata0s(0x1F0, false);
			listFiles(&ata0s);
		}
		else if (key_buffer[0] == "e" && key_buffer[1] == "x" && key_buffer[2] == "e" && key_buffer[3] == "c")
		{
			AdvancedTechnologyAttachment ata0s(0x1F0, false);
			char FNAME[8];
			char FEXT[3];
			int x = 0;
			for (int i = 5; key_buffer[i] != " " && (i - 5) != 8; i++)
			{
				FNAME[i - 5] = key_buffer[i][0];
				x++;
			}
			char *ARG1 = (char*)0x550;
			memset(ARG1, 0, sizeof(ARG1));
			strncpy(ARG1, FNAME, x);
			canNewLine = false;

			int y = 0;
			char* ARG2 = (char*)0x780;
			memset(ARG2, 0, sizeof(ARG2));
			int XP = (x + 5);
			for (int i = XP + 1; key_buffer[i] != "\n" && (i - x) != 3; i++)
			{
				FEXT[y] = key_buffer[i][0];
				y++;
			}
			strncpy(ARG2, FEXT, y);
			ExecutePRG(ARG1, ARG2, &ata0s);
		}
		else
		{
			printf("Unknown Command \"");
			for (int i = 0; key_buffer[i] != "\n"; i++)
			{
				printf(key_buffer[i]);
			}
			printf("\". Type help in console to get all the commands");
		}
		if(!isTxtMode){
			if (canNewLine)
				printf("\n");
			canNewLine = true;
			PrintPrompt();
		}
		serialport.logToSerialPort("Command interpreter got a command :- ");
		serialport.logToSerialPort(COMNAME);
		serialport.logToSerialPort("\n");
		clear_key_buffer();
	}
}

void KeyboardDriver::returnHScreen()
{
	clear_key_buffer();
	printf("\5");
	//ColourPrint(1);
	//ColourPrint(0);
	//printf("Welcome to SectorOS Monolithic kernel                               Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");

	printf("SectorOS ");
	printf(KERNEL_VERSION);
	printf("               ");
	PrintDate();
	printf("                          Type: Shell\nhttps://github.com/Arun007coder/SectorOS \n");
	printf("Run help to get the list of commands which is implemented \n \n");

	PrintPrompt();
	isTxtMode = false;
}

void KeyboardDriver::ChangeEventHandler(CustomShell* sh)
{
	shell = sh;
	IsShellDisabled = true;
	printf("Event handler changed to custom shell\n");
}

void KeyboardDriver::ResetEventHandler()
{
	shell = 0;
	IsShellDisabled = false;
}