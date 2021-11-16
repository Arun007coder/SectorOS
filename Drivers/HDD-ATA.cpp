#include "HDD-ATA.h"

void printf(char* );

AdvancedTechnologyAttachment::AdvancedTechnologyAttachment(uint16_t PortBase, bool master)
:DataPort(PortBase),
ErrorPort(PortBase + 1),
SectorCountPort(PortBase + 2),
LBALowPort(PortBase + 3),
LBAMidPort(PortBase + 4),
LBAHighPort(PortBase + 5),
DevicePort(PortBase + 6),
CommandPort(PortBase + 7),
ControlPort(PortBase + 0x206)
{
    this->Master = master;
    BytesPerSector = 512;

}

AdvancedTechnologyAttachment::~AdvancedTechnologyAttachment()
{

}

void AdvancedTechnologyAttachment::identify()
{
    DevicePort.WriteToPort(Master ? 0xA0 : 0xB0);
    ControlPort.WriteToPort(0);

    DevicePort.WriteToPort(0xA0);
    uint8_t status = CommandPort.ReadFromPort();
    if(status == 0xFF)
        return;


    DevicePort.WriteToPort(Master ? 0xA0 : 0xB0);
    SectorCountPort.WriteToPort(0);
    LBALowPort.WriteToPort(0);
    LBAMidPort.WriteToPort(0);
    LBAHighPort.WriteToPort(0);
    CommandPort.WriteToPort(0xEC); // identify command


    status = CommandPort.ReadFromPort();
    if(status == 0x00)
        return;

    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = CommandPort.ReadFromPort();

    if(status & 0x01)
    {
        printf("ERROR");
        return;
    }

    for(int i = 0; i < 256; i++)
    {
        uint16_t data = DataPort.ReadFromPort();
        char *text = "  \0";
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

    DevicePort.WriteToPort( (Master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24) );
    ErrorPort.WriteToPort(0);
    SectorCountPort.WriteToPort(1);
    LBALowPort.WriteToPort(  sectorNum & 0x000000FF );
    LBAMidPort.WriteToPort( (sectorNum & 0x0000FF00) >> 8);
    LBALowPort.WriteToPort( (sectorNum & 0x00FF0000) >> 16 );
    CommandPort.WriteToPort(0x20);

    uint8_t status = CommandPort.ReadFromPort();
    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = CommandPort.ReadFromPort();

    if(status & 0x01)
    {
        printf("ERROR");
        return;
    }


    printf("Reading ATA Drive: ");

    for(int i = 0; i < count; i += 2)
    {
        uint16_t wdata = DataPort.ReadFromPort();

        char *text = "  \0";
        text[0] = wdata & 0xFF;

        if(i+1 < count)
            text[1] = (wdata >> 8) & 0xFF;
        else
            text[1] = '\0';

        printf(text);
    }    

    for(int i = count + (count%2); i < 512; i += 2)
        DataPort.ReadFromPort();
}

void AdvancedTechnologyAttachment::Write28(uint32_t sectorNum, uint8_t* data, uint32_t count)
{
    if(sectorNum > 0x0FFFFFFF)
        return;
    if(count > 512)
        return;


    DevicePort.WriteToPort( (Master ? 0xE0 : 0xF0) | ((sectorNum & 0x0F000000) >> 24) );
    ErrorPort.WriteToPort(0);
    SectorCountPort.WriteToPort(1);
    LBALowPort.WriteToPort(  sectorNum & 0x000000FF );
    LBAMidPort.WriteToPort( (sectorNum & 0x0000FF00) >> 8);
    LBALowPort.WriteToPort( (sectorNum & 0x00FF0000) >> 16 );
    CommandPort.WriteToPort(0x30);


    printf("Writing to ATA Drive: ");

    for(int i = 0; i < count; i += 2)
    {
        uint16_t wdata = data[i];
        if(i+1 < count)
            wdata |= ((uint16_t)data[i+1]) << 8;
        DataPort.WriteToPort(wdata);

        char *text = "  \0";
        text[0] = (wdata >> 8) & 0xFF;
        text[1] = wdata & 0xFF;
        printf(text);
    }

    for(int i = count + (count%2); i < 512; i += 2)
        DataPort.WriteToPort(0x0000);

}

void AdvancedTechnologyAttachment::Flush()
{
    DevicePort.WriteToPort( Master ? 0xE0 : 0xF0 );
    CommandPort.WriteToPort(0xE7);

    uint8_t status = CommandPort.ReadFromPort();
    if(status == 0x00)
        return;

    while(((status & 0x80) == 0x80)
       && ((status & 0x01) != 0x01))
        status = CommandPort.ReadFromPort();

    if(status & 0x01)
    {
        printf("ERROR");
        return;
    }
}