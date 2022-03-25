#ifndef __MSDOSPT_H
#define __MSDOSPT_H

#include "../Drivers/HDD-ATA.h"
#include "../Include/types.h"
#include "FATFS.h"

struct PartitionTableEntry
{
    uint8_t bootable;
    uint8_t start_head;
    uint8_t start_sector : 6;
    uint16_t start_cylinder : 10;
    uint8_t partition_id;
    uint8_t end_head;
    uint8_t end_sector : 6;
    uint16_t end_cylinder : 10;
    
    uint32_t start_lba;
    uint32_t length;
} __attribute__((packed));

struct MasterBootRecord
{
    uint8_t bootloader[440];
    uint32_t signature;
    uint16_t unused;
    
    PartitionTableEntry primaryPartition[4];
    
    uint16_t magicnumber;
} __attribute__((packed));


class MSDOSPartitionTable
{
private:
    static MasterBootRecord *MBR;
public:
    static void ReadPartitions(AdvancedTechnologyAttachment *hd);
    MasterBootRecord* GetMBR();
};

#endif