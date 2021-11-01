#ifndef __MEMORYMANAGEMENT_H
#define __MEMORYMANAGEMENT_H

#include "../Includes/types.h"

struct MemoryChunk
{
    MemoryChunk* next;
    MemoryChunk* prev;
    size_t size;
    bool allocated;
};

class MemoryManager
{
protected:
    MemoryChunk* first;
public:
    MemoryManager(size_t size, size_t first);
    ~MemoryManager();
    static MemoryManager* ActiveMemoryManager;
    void* MemAllocate(size_t size);
    void MemFree(void* ptr);
};

void* operator new(unsigned size);
void* operator new[](unsigned size);

void* operator new(unsigned size, void* ptr);
void* operator new[](unsigned size, void* ptr);

void operator delete(void* ptr);
void operator delete[](void* ptr);


#endif