#ifndef __DEBUG_H
#define __DEBUG_H

inline void Breakpoint()
{
    asm("int $3");
}

#endif