#ifndef __PUBLIC_VAR_H
#define __PUBLIC_VAR_H

#define BreakPNT Breakpoint()
#define STOP    \
    asm("cli"); \
    asm("hlt")

// Unique Driver ID
#define UDID_AM79C973 38576
#define UDID_KEYBOARD 64699
#define UDID_MOUSE 79154

#include "../kernel/MultiTask.h"

inline char *OS_NAME = "SectorOS";

inline char *KERNEL_NAME = "SectorOS";
inline char *KERNEL_VERSION = "V3.5.5";
inline char *KERNEL_BUILD = "Build: 2022-03-28";
inline char *KERNEL_ARCH = "x86";

inline char *SHELL_NAME = "SOSH";
inline char *SHELL_VER = "V1.5.0";

// START Environment variables
inline char *SP[30];    // Shell Prompt
inline int SPIndex = 0; // Shell Prompt Index
// END Environment variables

// START Kernel variables
inline bool IsShellDisabled = false; // To disable the shell
inline bool isShift = false;         // To know if the shift key is pressed
inline bool GlobalBool1 = false;     // Global bool 1
inline uint8_t INITS[513];           // Init file contents
inline uint32_t ESP;                 // ESP
inline uint32_t OLDESP;              // EBP
// END Kernel variables

// START Common Functions

inline char KeyCodeToChar(uint8_t keyCode)
{
    char result;
    if (keyCode < 0x80)
    {
        switch (keyCode)
        {
        case 0xFA:
            break;
        case 0x45:
        case 0xC5:
            break;
        case 0x29:
            if (!isShift)
                result = '`';
            else
                result = '~';
            break;
        case 0x02:
            if (!isShift)
                result = '1';
            else
                result = '!';
            break;
        case 0x03:
            if (!isShift)
                result = '2';
            else
                result = '@';
            break;
        case 0x04:
            if (!isShift)
                result = '3';
            else
                result = '#';
            break;
        case 0x05:
            if (!isShift)
                result = '4';
            else
                result = '$';
            break;
        case 0x06:
            if (!isShift)
                result = '5';
            else
                result = '%';
            break;
        case 0x07:
            if (!isShift)
                result = '6';
            else
                result = '^';
            break;
        case 0x08:
            if (!isShift)
                result = '7';
            else
                result = '&';
            break;
        case 0x09:
            if (!isShift)
                result = '8';
            else
                result = '*';
            break;
        case 0x0A:
            if (!isShift)
                result = '9';
            else
                result = '(';
            break;
        case 0x0B:
            if (!isShift)
                result = '0';
            else
                result = ')';
            break;
        case 0x0C:
            if (!isShift)
                result = '-';
            else
                result = '_';
            break;
        case 0x0D:
            if (!isShift)
                result = '=';
            else
                result = '+';
            break;

        case 0x10:
            if (!isShift)
                result = 'q';
            else
                result = 'Q';
            break;
        case 0x11:
            if (!isShift)
                result = 'w';
            else
                result = 'W';
            break;
        case 0x12:
            if (!isShift)
                result = 'e';
            else
                result = 'E';
            break;
        case 0x13:
            if (!isShift)
                result = 'r';
            else
                result = 'R';
            break;
        case 0x14:
            if (!isShift)
                result = 't';
            else
                result = 'T';
            break;
        case 0x15:
            if (!isShift)
                result = 'y';
            else
                result = 'Y';
            break;
        case 0x16:
            if (!isShift)
                result = 'u';
            else
                result = 'U';
            break;
        case 0x17:
            if (!isShift)
                result = 'i';
            else
                result = 'I';
            break;
        case 0x18:
            if (!isShift)
                result = 'o';
            else
                result = 'O';
            break;
        case 0x19:
            if (!isShift)
                result = 'p';
            else
                result = 'P';
            break;
        case 0x1A:
            if (!isShift)
                result = '[';
            else
                result = '{';
            break;
        case 0x1B:
            if (!isShift)
                result = ']';
            else
                result = '}';
            break;
        case 0x1C:
            if (!isShift)
                result = '\n';
            else
                result = '\n';
            break;

        case 0x1E:
            if (!isShift)
                result = 'a';
            else
                result = 'A';
            break;
        case 0x1F:
            if (!isShift)
                result = 's';
            else
                result = 'S';
            break;
        case 0x20:
            if (!isShift)
                result = 'd';
            else
                result = 'D';
            break;
        case 0x21:
            if (!isShift)
                result = 'f';
            else
                result = 'F';
            break;
        case 0x22:
            if (!isShift)
                result = 'g';
            else
                result = 'G';
            break;
        case 0x23:
            if (!isShift)
                result = 'h';
            else
                result = 'H';
            break;
        case 0x24:
            if (!isShift)
                result = 'j';
            else
                result = 'J';
            break;
        case 0x25:
            if (!isShift)
                result = 'k';
            else
                result = 'K';
            break;
        case 0x26:
            if (!isShift)
                result = 'l';
            else
                result = 'L';
            break;
        case 0x27:
            if (!isShift)
                result = ';';
            else
                result = ':';
            break;

        case 0x2C:
            if (!isShift)
                result = 'z';
            else
                result = 'Z';
            break;
        case 0x2D:
            if (!isShift)
                result = 'x';
            else
                result = 'X';
            break;
        case 0x2E:
            if (!isShift)
                result = 'c';
            else
                result = 'C';
            break;
        case 0x2F:
            if (!isShift)
                result = 'v';
            else
                result = 'V';
            break;
        case 0x30:
            if (!isShift)
                result = 'v';
            else
                result = 'B';
            break;
        case 0x31:
            if (!isShift)
                result = 'n';
            else
                result = 'N';
            break;
        case 0x32:
            if (!isShift)
                result = 'm';
            else
                result = 'M';
            break;
        case 0x33:
            if (!isShift)
                result = ',';
            else
                result = '<';
            break;
        case 0x34:
            if (!isShift)
                result = '.';
            else
                result = '>';
            break;
        case 0x35:
            if (!isShift)
                result = '/';
            else
                result = '?';
            break;
        case 0x39:
            if (!isShift)
                result = ' ';
            else
                result = ' ';
            break;
        case 0x2B:
            if (!isShift)
                result = '\\';
            else
                result = '|';
            break;
        }
    }
    return result;
}

inline char *KeycodeToASCII(uint8_t Keycode)
{
    char *result;
    if (Keycode < 0x80)
    {
        switch (Keycode)
        {
        case 0xFA:
            break;
        case 0x45:
        case 0xC5:
            break;
        case 0x29:
            if (!isShift)
                result = "`";
            else
                result = "~";
            break;
        case 0x02:
            if (!isShift)
                result = "1";
            else
                result = "!";
            break;
        case 0x03:
            if (!isShift)
                result = "2";
            else
                result = "@";
            break;
        case 0x04:
            if (!isShift)
                result = "3";
            else
                result = "#";
            break;
        case 0x05:
            if (!isShift)
                result = "4";
            else
                result = "$";
            break;
        case 0x06:
            if (!isShift)
                result = "5";
            else
                result = "%";
            break;
        case 0x07:
            if (!isShift)
                result = "6";
            else
                result = "^";
            break;
        case 0x08:
            if (!isShift)
                result = "7";
            else
                result = "&";
            break;
        case 0x09:
            if (!isShift)
                result = "8";
            else
                result = "*";
            break;
        case 0x0A:
            if (!isShift)
                result = "9";
            else
                result = "(";
            break;
        case 0x0B:
            if (!isShift)
                result = "0";
            else
                result = ")";
            break;
        case 0x0C:
            if (!isShift)
                result = "-";
            else
                result = "_";
            break;
        case 0x0D:
            if (!isShift)
                result = "=";
            else
                result = "+";
            break;

        case 0x10:
            if (!isShift)
                result = "q";
            else
                result = "Q";
            break;
        case 0x11:
            if (!isShift)
                result = "w";
            else
                result = "W";
            break;
        case 0x12:
            if (!isShift)
                result = "e";
            else
                result = "E";
            break;
        case 0x13:
            if (!isShift)
                result = "r";
            else
                result = "R";
            break;
        case 0x14:
            if (!isShift)
                result = "t";
            else
                result = "T";
            break;
        case 0x15:
            if (!isShift)
                result = "y";
            else
                result = "Y";
            break;
        case 0x16:
            if (!isShift)
                result = "u";
            else
                result = "U";
            break;
        case 0x17:
            if (!isShift)
                result = "i";
            else
                result = "I";
            break;
        case 0x18:
            if (!isShift)
                result = "o";
            else
                result = "O";
            break;
        case 0x19:
            if (!isShift)
                result = "p";
            else
                result = "P";
            break;
        case 0x1A:
            if (!isShift)
                result = "[";
            else
                result = "{";
            break;
        case 0x1B:
            if (!isShift)
                result = "]";
            else
                result = "}";
            break;
        case 0x1C:
            if (!isShift)
                result = "\n";
            else
                result = "\n";
            break;

        case 0x1E:
            if (!isShift)
                result = "a";
            else
                result = "A";
            break;
        case 0x1F:
            if (!isShift)
                result = "s";
            else
                result = "S";
            break;
        case 0x20:
            if (!isShift)
                result = "d";
            else
                result = "D";
            break;
        case 0x21:
            if (!isShift)
                result = "f";
            else
                result = "F";
            break;
        case 0x22:
            if (!isShift)
                result = "g";
            else
                result = "G";
            break;
        case 0x23:
            if (!isShift)
                result = "h";
            else
                result = "H";
            break;
        case 0x24:
            if (!isShift)
                result = "j";
            else
                result = "J";
            break;
        case 0x25:
            if (!isShift)
                result = "k";
            else
                result = "K";
            break;
        case 0x26:
            if (!isShift)
                result = "l";
            else
                result = "L";
            break;
        case 0x27:
            if (!isShift)
                result = ";";
            else
                result = ":";
            break;

        case 0x2C:
            if (!isShift)
                result = "z";
            else
                result = "Z";
            break;
        case 0x2D:
            if (!isShift)
                result = "x";
            else
                result = "X";
            break;
        case 0x2E:
            if (!isShift)
                result = "c";
            else
                result = "C";
            break;
        case 0x2F:
            if (!isShift)
                result = "v";
            else
                result = "V";
            break;
        case 0x30:
            if (!isShift)
                result = "b";
            else
                result = "B";
            break;
        case 0x31:
            if (!isShift)
                result = "n";
            else
                result = "N";
            break;
        case 0x32:
            if (!isShift)
                result = "m";
            else
                result = "M";
            break;
        case 0x33:
            if (!isShift)
                result = ",";
            else
                result = "<";
            break;
        case 0x34:
            if (!isShift)
                result = ".";
            else
                result = ">";
            break;
        case 0x35:
            if (!isShift)
                result = "/";
            else
                result = "?";
            break;
        case 0x39:
            if (!isShift)
                result = " ";
            else
                result = " ";
            break;
        case 0x2B:
            if (!isShift)
                result = "\\";
            else
                result = "|";
            break;
        }
    }
    return result;
}

inline int strcmp(unsigned char str1[], unsigned char str2[])
{
    int i = 0;

    if (str1[0] == '\0' && str2[0] == '\0')
        return 0;

    for (i = 0; str1[i] != '\0'; i++)
    {
        if (str1[i] != str2[i])
            return 1;
    }

    if (str2[i] != '\0')
        return 1;

    return 0;
}

// END Common Functions

#endif