#include "MemoryManagement.h"

MemoryManager* MemoryManager::ActiveMemoryManager = 0;

MemoryManager::MemoryManager(size_t size, size_t start)
{
    ActiveMemoryManager = this;

    if(size < sizeof(MemoryManager))
    {
        first = 0;
    }
    else
    {
        first = (MemoryChunk*)start;
        first->allocated = false;
        first->prev = 0;
        first->next = 0;
        first->size = size - sizeof(MemoryChunk);
    }
}

MemoryManager::~MemoryManager()
{
    if(ActiveMemoryManager == this)
    {
        ActiveMemoryManager = 0;
    }
}

void * MemoryManager::MemReallocate(void *ptr, size_t size)
{
    if(!ptr)
    {
        return MemAllocate(size);
    }

    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
    if(chunk->size >= size)
    {
        return ptr;
    }

    void* newPtr = MemAllocate(size);
    memcpy(newPtr, ptr, chunk->size);
    MemFree(ptr);
    return newPtr;

    return 0;
}

void MemoryManager::MemFree(void* ptr)
{
    MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
    chunk->allocated = false;

    if(chunk->prev != 0 && chunk->prev->allocated == false)
    {
        chunk->prev->size += chunk->size + sizeof(MemoryChunk);
        chunk->prev->next = chunk->next;
        if(chunk->next != 0)
        {
            chunk->next->prev = chunk->prev;
        }

        chunk = chunk->prev;
    }
    else if(chunk->next != 0 && chunk->next->allocated == false)
    {
        chunk->next->size += chunk->size + sizeof(MemoryChunk);
        chunk->next->prev = chunk->prev;
        if(chunk->prev != 0)
        {
            chunk->prev->next = chunk->next;
        }
    }
    else
    {
        chunk->prev = 0;
        chunk->next = first;
        first->prev = chunk;
        first = chunk;
    }
    {
    }
}

void* MemoryManager::MemAllocate(size_t size) // Malloc
{
    MemoryChunk *result = 0;

    for(MemoryChunk* chunk = first; chunk != 0 && result == 0; chunk = chunk->next)
    {
        if(chunk->size > size && !chunk->allocated)
        {
            result = chunk;
            break;
        }
    }

    if(result == 0)
    {
        return 0;
    }

    if(result->size > size + sizeof(MemoryChunk) + 1)
    {
        MemoryChunk* newChunk = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
        newChunk->allocated = false;
        newChunk->prev = result;
        newChunk->next = result->next;
        newChunk->size = result->size - sizeof(MemoryChunk) - size;

        if(result->next != 0)
        {
            result->next->prev = newChunk;
        }

        result->next = newChunk;
        result->size = size;
    }

    result->allocated = true;
    return (void*)(((size_t)result) + sizeof(MemoryChunk));
}



void* operator new(unsigned size)
{
    if(MemoryManager::ActiveMemoryManager == 0)
    {
        return 0;
    }
    return MemoryManager::ActiveMemoryManager->MemAllocate(size);
}

void* operator new[](unsigned size)
{
    if(MemoryManager::ActiveMemoryManager == 0)
    {
        return 0;
    }
    return MemoryManager::ActiveMemoryManager->MemAllocate(size);
}

void* operator new(unsigned size, void* ptr)
{
    return ptr;
}

void* operator new[](unsigned size, void* ptr)
{
    return ptr;
}

void operator delete(void* ptr)
{
    if(MemoryManager::ActiveMemoryManager != 0)
        MemoryManager::ActiveMemoryManager->MemFree(ptr);
}

void operator delete[](void* ptr)
{
    if(MemoryManager::ActiveMemoryManager != 0)
        MemoryManager::ActiveMemoryManager->MemFree(ptr);
}