#ifndef FUNC_H
#define FUNC_H

#include "types.h"

void printHex(uint8_t);

inline char CharPOINTERToChar(char* c)
{
    return *c;
}

inline void printHex16(uint16_t key)
{
    printHex((key >> 8) & 0xFF);
    printHex(key & 0xFF);
}
inline void printHex32(uint32_t key)
{
    printHex((key >> 24) & 0xFF);
    printHex((key >> 16) & 0xFF);
    printHex((key >> 8) & 0xFF);
    printHex(key & 0xFF);
}

#endif