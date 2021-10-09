#include "pci.h"

void printf(char* str);
void printHex(uint8_t num);

PCI::PCI()
:DataPort(0xCFC),
CommandPort(0xCF8)
{

}

PCI::~PCI()
{
}

PCIDeviceDescriptor::PCIDeviceDescriptor()
{
}

PCIDeviceDescriptor::~PCIDeviceDescriptor()
{
}

uint32_t PCI::ReadPCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t RegisterOffset)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (RegisterOffset & 0xFC);
    CommandPort.WriteToPort(id);
    uint32_t result = DataPort.ReadFromPort();
    return result >> (8* (RegisterOffset % 4));
}

void PCI::WritePCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t RegisterOffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (RegisterOffset & 0xFC);
    CommandPort.WriteToPort(id);
    DataPort.WriteToPort(value);
}

bool PCI::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return ReadPCI(bus, device, 0, 0x0E) & (1 << 7);
}

void PCI::SelectDrivers(DriverManager* drivermanager)
{
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunction = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function = 0; function < numFunction; function++)
            {
                PCIDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);

                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    break;

                printf("PCI BUS ");
                printHex(bus & 0xFF);

                printf(", DEVICE ");
                printHex(device & 0xFF);

                printf(", FUNCTION ");
                printHex(function & 0xFF);

                printf(" = VENDOR ");
                printHex((dev.vendor_id & 0xFF00)>> 8);
                printHex(dev.vendor_id & 0xFF);

                printf(", DEVICE ");
                printHex((dev.device_id & 0xFF00)>> 8);
                printHex(dev.device_id & 0xFF);
                printf("\n");


            }
        }
    }
}

PCIDeviceDescriptor PCI::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PCIDeviceDescriptor result;

    result.bus = bus;
    result.device = device;
    result.function = function;

    result.vendor_id = ReadPCI(bus, device, function, 0x00);
    result.device_id = ReadPCI(bus, device, function, 0x02);

    result.class_id = ReadPCI(bus, device, function, 0x0B);
    result.subclas_id = ReadPCI(bus, device, function, 0x0A);
    result.interface_id = ReadPCI(bus, device, function, 0x09);

    result.revision = ReadPCI(bus, device, function, 0x08);
    result.interrupt = ReadPCI(bus, device, function, 0x3C);
}