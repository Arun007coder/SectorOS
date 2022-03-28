#include "MSDOSPT.h"

void printf(char*);
void printHex(uint8_t);

MasterBootRecord *MSDOSPartitionTable::MBR = 0;

void MSDOSPartitionTable::ReadPartitions(AdvancedTechnologyAttachment *hd)
{
    MasterBootRecord mbr;

    printf("MBR: ");

    hd->Read28(0, (uint8_t *)&mbr, sizeof(MasterBootRecord));
    MBR = &mbr;

    /*
    for(int i = 0x1BE; i <= 0x01FF; i++)
    {
        printfHex(((uint8_t*)&mbr)[i]);
        printf(" ");
    }
    printf("\n");
    */

    if (mbr.magicnumber != 0xAA55)
    {
        printf("illegal MBR");
        return;
    }

    for (int i = 0; i < 4; i++)
    {
        printf("Partition ");
        printHex(i & 0xFF);
        printf("\n");

        if (mbr.primaryPartition[i].partition_id == 0x00)
            continue;

        printf(" Partition ");
        printHex(i & 0xFF);

        if (mbr.primaryPartition[i].bootable == 0x80)
            printf(" bootable. Type");
        else
            printf(" not bootable. Type ");

        printHex(mbr.primaryPartition[i].partition_id);

        ReadBiosBlock(hd, mbr.primaryPartition[i].start_lba);
    }
}

MasterBootRecord* MSDOSPartitionTable::GetMBR()
{
    return MBR;
}