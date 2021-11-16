#ifndef __HDD_ATA_H
#define __HDD_ATA_H

#include "IOPorts.h"
#include "../Includes/types.h"


class AdvancedTechnologyAttachment
{
protected:
    port8BIT DataPort; // To send data to the drive
    port8BIT ErrorPort; // To read error information
    port8BIT SectorCountPort; // To send the number of sectors to read/write
    port8BIT LBALowPort; // To send the low byte of the LBA
    port8BIT LBAMidPort; // To send the middle byte of the LBA
    port8BIT LBAHighPort; // To send the high byte of the LBA
    port8BIT DevicePort; // To send which device to use
    port8BIT CommandPort; // To send the command
    port8BIT ControlPort; // To send the control information
    bool Master; // Is this the master or slave drive?

    uint16_t BytesPerSector; // The number of bytes per sector

public:
    AdvancedTechnologyAttachment(uint16_t PortBase, bool master);
    ~AdvancedTechnologyAttachment();

    void identify();

    void Read28(uint32_t Sector, int count = 512);                                   // To read data from the drive
    void Write28(uint32_t Sector, uint8_t* Data, uint32_t SectorCount);  // To write data to the drive
    void Flush();                                                   // To flush the cache of the drive


};


#endif