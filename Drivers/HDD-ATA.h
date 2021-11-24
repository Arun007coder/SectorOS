#ifndef __HDD_ATA_H
#define __HDD_ATA_H

#include "IOPorts.h"
#include "../Includes/types.h"


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
public:
    AdvancedTechnologyAttachment(bool master, uint16_t portBase);
    ~AdvancedTechnologyAttachment();
    void Identify();
    void Read28(uint32_t sectorNum, int count = 512);
    void Write28(uint32_t sectorNum, uint8_t* data, uint32_t count);
    void Flush();
};


#endif