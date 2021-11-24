#include "HDD-ATA.h"

void printf(char* );
void printHex(uint8_t );

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(bool master, uint16_t portBase)
:   dataPort(portBase),
    errorPort(portBase + 0x1),
    sectorCountPort(portBase + 0x2),
    lbaLowPort(portBase + 0x3),
    lbaMidPort(portBase + 0x4),
    lbaHiPort(portBase + 0x5),
    devicePort(portBase + 0x6),
    commandPort(portBase + 0x7),
    controlPort(portBase + 0x206)
{
    this->master = master;
}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment()
{
}

void AdvancedTechnologyAttachment::Identify()
{
    devicePort.WriteToPort(master ? 0xA0 : 0xB0);
    controlPort.WriteToPort(0);

    devicePort.WriteToPort(0xA0);
    uint8_t status = commandPort.ReadFromPort();
    if(status == 0xFF)
        return;


    devicePort.WriteToPort(master ? 0xA0 : 0xB0);
    sectorCountPort.WriteToPort(0);
    lbaLowPort.WriteToPort(0);
    lbaMidPort.WriteToPort(0);
    lbaHiPort.WriteToPort(0);
    commandPort.WriteToPort(0xEC); // identify command


    status = commandPort.ReadFromPort();
    if(status == 0x00)
        return;

    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = commandPort.ReadFromPort();

    if(status & 0x01)
    {
        printf("ERROR");
        return;
    }

    for(int i = 0; i < 35; i++)
    {
        
        uint16_t data = dataPort.ReadFromPort();
        char *text = "\0\0";
        text[0] = (data >> 8) & 0xFF;
        text[1] = data & 0xFF;
        printf(text);
    }
    printf("\n");
}

void AdvancedTechnologyAttachment::Read28(uint32_t sectorNum, int count)
{
    if(sectorNum > 0x0FFFFFFF)
        return;

    devicePort.WriteToPort( (master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24) );
    errorPort.WriteToPort(0);
    sectorCountPort.WriteToPort(1);
    lbaLowPort.WriteToPort(  sectorNum & 0x000000FF );
    lbaMidPort.WriteToPort( (sectorNum & 0x0000FF00) >> 8);
    lbaLowPort.WriteToPort( (sectorNum & 0x00FF0000) >> 16 );
    commandPort.WriteToPort(0x20);

    uint8_t status = commandPort.ReadFromPort();
    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = commandPort.ReadFromPort();

    if(status & 0x01)
    {
        printf("ERROR");
        return;
    }


    printf("Reading ATA Drive: ");

    for(int i = 0; i < count; i += 2)
    {
        uint16_t wdata = dataPort.ReadFromPort();

        char *text = "  \0";
        text[0] = wdata & 0xFF;

        if(i+1 < count)
            text[1] = (wdata >> 8) & 0xFF;
        else
            text[1] = '\0';

        printf(text);
    }    

    for(int i = count + (count%2); i < 512; i += 2)
        dataPort.ReadFromPort();
}

void AdvancedTechnologyAttachment::Write28(uint32_t sectorNum, uint8_t* data, uint32_t count)
{
    if(sectorNum > 0x0FFFFFFF)
        return;
    if(count > 512)
        return;


    devicePort.WriteToPort( (master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24) );
    errorPort.WriteToPort(0);
    sectorCountPort.WriteToPort(1);
    lbaLowPort.WriteToPort(  sectorNum & 0x000000FF );
    lbaMidPort.WriteToPort( (sectorNum & 0x0000FF00) >> 8);
    lbaLowPort.WriteToPort( (sectorNum & 0x00FF0000) >> 16 );
    commandPort.WriteToPort(0x30);


    printf("Writing to ATA Drive: ");

    for(int i = 0; i < count; i += 2)
    {
        uint16_t wdata = data[i];
        if(i+1 < count)
            wdata |= ((uint16_t)data[i+1]) << 8;
        dataPort.WriteToPort(wdata);

        char *text = "  \0";
        text[0] = (wdata >> 8) & 0xFF;
        text[1] = wdata & 0xFF;
        printf(text);
    }

    for(int i = count + (count%2); i < 512; i += 2)
        dataPort.WriteToPort(0x0000);

}

void AdvancedTechnologyAttachment::Flush()
{
    devicePort.WriteToPort( master ? 0xE0 : 0xF0 );
    commandPort.WriteToPort(0xE7);

    uint8_t status = commandPort.ReadFromPort();
    if(status == 0x00)
        return;

    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = commandPort.ReadFromPort();

    if(status & 0x01)
    {
        printf("ERROR");
        return;
    }
}