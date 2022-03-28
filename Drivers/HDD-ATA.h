#ifndef __HDD_ATA_H
#define __HDD_ATA_H

#include "IOPorts.h"
#include "../Include/types.h"

class AdvancedTechnologyAttachment
{
protected:
    bool master;
    port16BIT dataPort;
    port8BIT errorPort;
    port8BIT sectorCountPort;
    port8BIT lbaLowPort;
    port8BIT lbaMidPort;
    port8BIT lbaHiPort;
    port8BIT devicePort;
    port8BIT commandPort;
    port8BIT controlPort;

    uint16_t bytesPerSector;

public:
    AdvancedTechnologyAttachment(uint16_t portBase, bool master);
    ~AdvancedTechnologyAttachment();
    void Identify();
    void Read28(uint32_t sectorNum, uint8_t* data, int count = 512);
    void Write28(uint32_t sectorNum, uint8_t* data, int count);
    void Flush();
};


#endif