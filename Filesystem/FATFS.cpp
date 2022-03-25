#include "FATFS.h"

char *INTTOCHARPOINT(int num);
Task* tasks[256];
int numTasks;
void* EPoints[256];
int ENTINDEX = 0;

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

void printf(char*);
void printfchar(char);
void printHex(uint8_t Key);

void Entry();

char *PATH;
char *lst_dir;
char *lst_dir2;
bool IsDir;
bool IsProgDir;
int PATHIndex = 0;

void Entry()
{
    printf("EXECUTION FAILED");
}

void Program::entrypoint()
{
    printf("EXECUTION FAILED");
}

void ReadBiosBlock(AdvancedTechnologyAttachment *hd, uint32_t partitionOffset)
{
    BiosParameterBlock32 bpb;
    hd->Read28(partitionOffset, (uint8_t *)&bpb, sizeof(BiosParameterBlock32));

    printf("sectors per cluster: ");
    printHex(bpb.sectorsPerCluster);
    printf("\n");

    uint32_t fatStart = partitionOffset + bpb.reservedSectors;
    uint32_t fatSize = bpb.tableSize;

    uint32_t dataStart = fatStart + fatSize * bpb.fatCopies;

    uint32_t rootStart = dataStart + bpb.sectorsPerCluster * (bpb.rootCluster - 2);

    DirectoryEntryFat32 dirent[16];
    hd->Read28(rootStart, (uint8_t *)&dirent[0], 16 * sizeof(DirectoryEntryFat32));

    for (int i = 0; i < 16; i++)
    {
        if (dirent[i].name[0] == 0x00)
            break;

        if ((dirent[i].attributes & 0x0F) == 0x0F)
            continue;

        char *foo = "        \n";
        for (int j = 0; j < 8; j++)
            foo[j] = dirent[i].name[j];
        printf(foo);

        if ((dirent[i].attributes & 0x10) == 0x10) // directory
        {
            IsDir = true;
            if(dirent[i].name[0] == 'P' && dirent[i].name[1] == 'R' && dirent[i].name[2] == 'O' && dirent[i].name[3] == 'G' && dirent[i].name[4] == 'S')
            {
                IsProgDir = true;
                printf("\n");
                printf("prog dir found\n");
                printf("\n");
                continue;
            }
            else
            {
                IsProgDir = false;
                continue;
            }
        }

        uint32_t firstFileCluster = ((uint32_t)dirent[i].firstClusterHi) << 16 | ((uint32_t)dirent[i].firstClusterLow);

        int32_t SIZE = dirent[i].size;
        int32_t nextFileCluster = firstFileCluster;
        uint8_t buffer[513];
        uint8_t fatbuffer[513];

        while (SIZE > 0)
        {
            uint32_t fileSector = dataStart + bpb.sectorsPerCluster * (nextFileCluster - 2);
            int sectorOffset = 0;
            for (; SIZE > 0; SIZE -= 512)
            {
                /*
                if (dirent[i].ext[0] == 'C' && dirent[i].ext[1] == 'O' && dirent[i].ext[2] == 'M')
                {
                    hd->Read28(fileSector + sectorOffset, (uint8_t *)Entry, 512);
                    printf("executing ");
                    printf((char *)dirent[i].name);
                    printf(".");
                    printf((char *)dirent[i].ext);
                    printf("\n");
                    Entry();
                }
                else
                */
                if (dirent[i].name[1] == 'I' && dirent[i].name[2] == 'N' && dirent[i].name[3] == 'I' && dirent[4].name[3] == 'T')
                {
                    hd->Read28(fileSector + sectorOffset, INITS, 512);
                    INITS[SIZE > 512 ? 512 : SIZE] = '\0';
                    printf("INIT: ");
                    printf((char*)INITS);
                    asm("cli");
                    asm("hlt");
                }
                else
                {
                    hd->Read28(fileSector + sectorOffset, buffer, 512);
                    buffer[SIZE > 512 ? 512 : SIZE] = '\0';
                    printf((char *)dirent[i].name);
                    printf(" :\n");
                    printf((char *)buffer);
                }

                if (++sectorOffset > bpb.sectorsPerCluster)
                    break;
            }

            uint32_t fatSectorForCurrentCluster = nextFileCluster / (512 / sizeof(uint32_t));
            hd->Read28(fatStart + fatSectorForCurrentCluster, fatbuffer, 512);
            uint32_t fatOffsetInSectorForCurrentCluster = nextFileCluster % (512 / sizeof(uint32_t));
            nextFileCluster = ((uint32_t *)&fatbuffer)[fatOffsetInSectorForCurrentCluster] & 0x0FFFFFFF;
        }
    }
}

void listFiles(AdvancedTechnologyAttachment *HDD)
{
    MasterBootRecord MBR;
    HDD->Read28(0, (uint8_t *)&MBR, sizeof(MasterBootRecord));
    printf("\nMBR signature: ");
    printHex(MBR.signature);
    printf("\n");
    BiosParameterBlock32 bpb32;
    HDD->Read28(MBR.primaryPartition[1].start_lba, (uint8_t *)&bpb32, sizeof(BiosParameterBlock32));
    uint32_t fatStart = MBR.primaryPartition[1].start_lba + bpb32.reservedSectors;
    uint32_t fatSize = bpb32.tableSize;

    uint32_t dataStart = fatStart + fatSize * bpb32.fatCopies;

    uint32_t rootStart = dataStart + bpb32.sectorsPerCluster * (bpb32.rootCluster - 2);

    DirectoryEntryFat32 Dirent[16];
    HDD->Read28(rootStart, (uint8_t *)&Dirent[0], 16 * sizeof(DirectoryEntryFat32));

    printf("Files found: \n");
    for (int x = 0; x < 16; x++)
    {
        printf((char *)Dirent[x].name);
    }
}

// COM files are basic executable
// ROM files are executables used by the JD1618 emulator
bool ExecutePRG(char name[8], AdvancedTechnologyAttachment* HDD)
{
    MasterBootRecord MBR;
    HDD->Read28(0, (uint8_t *)&MBR, sizeof(MasterBootRecord));
    printf("\nMBR signature: ");
    printHex(MBR.signature);
    printf("\n");
    BiosParameterBlock32 bpb32;
    HDD->Read28(MBR.primaryPartition[1].start_lba, (uint8_t *)&bpb32, sizeof(BiosParameterBlock32));
    uint32_t fatStart = MBR.primaryPartition[1].start_lba + bpb32.reservedSectors;
    uint32_t fatSize = bpb32.tableSize;

    uint32_t dataStart = fatStart + fatSize * bpb32.fatCopies;

    uint32_t rootStart = dataStart + bpb32.sectorsPerCluster * (bpb32.rootCluster - 2);

    DirectoryEntryFat32 Dirent[16];
    HDD->Read28(rootStart, (uint8_t *)&Dirent[0], 16 * sizeof(DirectoryEntryFat32));

    printf("Files found: \n");
    for (int x = 0; x < 16; x++)
    {
        printf((char *)Dirent[x].name);
    }

    for (int i = 0; i < 16; i++)
    {
        uint32_t firstFileCluster = ((uint32_t)Dirent[i].firstClusterHi) << 16 | ((uint32_t)Dirent[i].firstClusterLow);

        int32_t SIZE = Dirent[i].size;
        int32_t nextFileCluster = firstFileCluster;
        uint8_t buffer[513];
        uint8_t fatbuffer[513];

        while (SIZE > 0)
        {
            uint32_t fileSector = dataStart + bpb32.sectorsPerCluster * (nextFileCluster - 2);
            int sectorOffset = 0;
            for (; SIZE > 0; SIZE -= 512)
            {
                if(Dirent[i].ext[0] == 'C' && Dirent[i].ext[1] == 'O' && Dirent[i].ext[2] == 'M')
                {
                    Program* prg = (Program*)MemoryManager::ActiveMemoryManager->MemAllocate(Dirent[i].size);
                    HDD->Read28(fileSector + sectorOffset, (uint8_t *)Entry, Dirent[i].size);
                    printf("executing ");
                    printf((char *)Dirent[i].name);
                    printf(".");
                    printf((char *)Dirent[i].ext);
                    printf("\n");
                    Entry();
                }
                else if (Dirent[i].ext[0] == 'R' && Dirent[i].ext[1] == 'O' && Dirent[i].ext[2] == 'M')
                {
                    uint8_t *rom;
                    HDD->Read28(fileSector + sectorOffset, rom, 512);
                    CPU1618::System sys;
                    sys.LoadPRG(rom, SIZE);
                    sys.StartExecution();
                }
            }
        }
    }
    printf("\n");
    return false;
}