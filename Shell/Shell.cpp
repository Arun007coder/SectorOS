// Copyright (C) 2021 Arun007coder
// 
// This file is part of SectorOS.
// 
// SectorOS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// SectorOS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with SectorOS.  If not, see <http://www.gnu.org/licenses/>.

#include "Shell.h"

CShell::CShell()
{
}

CShell::~CShell()
{
}

void printf(char *);
void printHex(uint8_t);
void printfchar(char st);

static int CharBufferIndex = 0;

void CShell::clearBuffer()
{
    for (int i = 0; i < 256; i++)
    {
        CharBuffer[i] = 0x00;
    }
    CharBufferIndex = 0;
}

void CShell::OnKeyDown(uint8_t key)
{
    if (key < 0x80 & key != 0x3A & key != 0x2A & key != 0x2A & key != 0x36 & key != 0x3A & key != 0x0E & key != 0x38 & key != 0x1D)
    {
        CharBuffer[CharBufferIndex] = KeyCodeToChar(key);
        CharBufferIndex++;
    }
    else if (key == 0x3E)
    {
        clearBuffer();
        CharBufferIndex = 0;
    }
}

void CShell::Shell()
{
    printf("\n");
    if (CharBuffer[0x00] == 'e' & CharBuffer[0x01] == 'c' & CharBuffer[0x02] == 'h' & CharBuffer[0x03] == 'o')
    {
        if (CharBuffer[0x04] == '\0')
        {
            printf("cannot print null character");
        }
        else if (CharBuffer[0x05] == '$' && CharBuffer[0x06] == 'S' && CharBuffer[0x07] == 'P')
        {
            for (int i = 0; i < SPIndex; i++)
            {
                printf(SP[i]);
            }
        }
        else
        {
            for (uint8_t i = 0x05; CharBuffer[i] != '\n'; i++)
            {
                printfchar(CharBuffer[i]);
            }
        }
    }
    else
    {
        for (int i = 0x00; i < CharBufferIndex; i++)
        {
            printHex(i);
            printf(" : ");
            printfchar(CharBuffer[i]);
            printf("\n");
        }
    }
    printf("\n");
    clearBuffer();
    asm("int $0x80"
        :
        : "a"(1), "b"("root@secos:~#> "));
}