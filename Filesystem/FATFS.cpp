#include "FATFS.h"

void printf(char*);
void printHex(uint8_t Key);

char *PATH;
char *lst_dir;
char *lst_dir2;
int PATHIndex = 0;

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
            int ZERO = 0;

            if (lst_dir2 == (char*)dirent[i].name)
            {
                continue;
            }
            else
            {
                if (lst_dir == (char *)dirent[i].name)
                {
                    continue;
                }
                else
                {
                    for (int x = 0; dirent[i].name[x] != ' ' && x < 8; x++)
                    {
                        PATH[PATHIndex] = dirent[i].name[x];
                        PATHIndex++;
                    }
                    PATH[PATHIndex] = '/';
                    PATHIndex++;
                    printf("\nPATH: ");
                    printf(PATH);
                    printf("\n");
                }
            }
            lst_dir2 = lst_dir;
            lst_dir = (char *)dirent[i].name;
            continue;
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
                hd->Read28(fileSector + sectorOffset, buffer, 512);

                buffer[SIZE > 512 ? 512 : SIZE] = '\0';
                printf((char *)buffer);

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