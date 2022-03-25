#ifndef __VFS_H
#define __VFS_H

#include "../Include/types.h"

#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STRING "/"
#define PATH_UP ".."
#define PATH_CURRENT "."

typedef char cstring;

struct VFSNode
{
    char name[8];
    char ext[3];
    uint8_t attributes;
    uint8_t reserved;
    uint8_t cTimeTenth;
    uint16_t cTime;
    uint16_t cDate;
    uint16_t aTime;
    uint32_t FileSector;
    uint32_t FileSize;
};

struct VFSDirectory
{
    char name[8];
    char ext[3];
    uint8_t attributes;
    uint8_t cTimeTenth;
    uint16_t cTime;
    uint16_t cDate;
    uint16_t aTime;
    VFSNode files[256];
};

VFSNode* VFS_Open(cstring path);

#endif