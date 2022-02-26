#include "pci.h"
#include "../Drivers/AM79C973.h"

void printf(char* str);
void printHex(uint8_t num);


PCI::PCI()
: DataPort(0xCFC),
  CommandPort(0xCF8)
{
}

PCI::~PCI()
{
}

uint32_t PCI::ReadPCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    CommandPort.WriteToPort(id);
    uint32_t result = DataPort.ReadFromPort();
    return result >> (8* (registeroffset % 4));
}

void PCI::WritePCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t registeroffset, uint32_t value)
{
    uint32_t id =
        0x1 << 31
        | ((bus & 0xFF) << 16)
        | ((device & 0x1F) << 11)
        | ((function & 0x07) << 8)
        | (registeroffset & 0xFC);
    CommandPort.WriteToPort(id);
    DataPort.WriteToPort(value); 
}

bool PCI::DeviceHasFunctions(uint16_t bus, uint16_t device)
{
    return ReadPCI(bus, device, 0, 0x0E) & (1<<7);
}


void printf(char* str);
void printHex(uint8_t);

void PCI::SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts)
{
    for(int bus = 0; bus < 8; bus++)
    {
        for(int device = 0; device < 32; device++)
        {
            int numFunctions = DeviceHasFunctions(bus, device) ? 8 : 1;
            for(int function = 0; function < numFunctions; function++)
            {
                PCIDeviceDescriptor dev = GetDeviceDescriptor(bus, device, function);
                
                if(dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    continue;
                
                
                for(int barNum = 0; barNum < 6; barNum++)
                {
                    BaseAddressRegister bar = GetBaseAddressRegister(bus, device, function, barNum);
                    if(bar.address && (bar.type == InputOutput))
                        dev.portBase = (uint32_t)bar.address;
                }
                
                Driver* driver = GetDriver(dev, interrupts);
                if(driver != 0)
                    driverManager->AddDriver(driver);

                
                printf("PCI BUS ");
                printHex(bus & 0xFF);
                
                printf(", DEVICE ");
                printHex(device & 0xFF);

                printf(", FUNCTION ");
                printHex(function & 0xFF);
                
                printf(" = VENDOR ");
                printHex((dev.vendor_id & 0xFF00) >> 8);
                printHex(dev.vendor_id & 0xFF);
                printf(", DEVICE ");
                printHex((dev.device_id & 0xFF00) >> 8);
                printHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }
}


BaseAddressRegister PCI::GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar)
{
    BaseAddressRegister result;
    
    
    uint32_t headertype = ReadPCI(bus, device, function, 0x0E) & 0x7F;
    int maxBARs = 6 - (4*headertype);
    if(bar >= maxBARs)
        return result;
    
    
    uint32_t bar_value = ReadPCI(bus, device, function, 0x10 + 4*bar);
    result.type = (bar_value & 0x1) ? InputOutput : MemoryMapping;
    uint32_t temp;
    
    
    
    if(result.type == MemoryMapping)
    {
        
        switch((bar_value >> 1) & 0x3)
        {
            
            case 0: // 32 Bit Mode
            case 1: // 20 Bit Mode
            case 2: // 64 Bit Mode
                break;
        }
        
    }
    else // InputOutput
    {
        result.address = (uint8_t*)(bar_value & ~0x3);
        result.prefetchable = false;
    }
    
    
    return result;
}

Driver* PCI::GetDriver(PCIDeviceDescriptor dev, InterruptManager* interrupts)
{
    Driver* driver = 0;
    switch(dev.vendor_id)
    {
        case 0x1022: // AMD
            switch(dev.device_id)
            {
                case 0x2000:
                    printf("AMD AM79C973 \n");
                    driver = (AM79C973*)MemoryManager::ActiveMemoryManager->MemAllocate(sizeof(AM79C973));
                    if (driver != 0)
                    {
                        driver = new AM79C973(&dev, interrupts);
                        printf("INSTANTIALIZED AMD AM79C973 \n");
                    }
                    else
                        printf("instantiation failed");
                    return driver;
                break;
            }
            break;

        case 0x8086: // Intel
            switch (dev.device_id)
            {
            case 0x7000:
                printf("82371SB PIIX3 ISA \n");
                break;
            }
            break;
    }
    
    
    switch(dev.class_id)
    {
        case 0x03: // graphics
            switch(dev.subclass_id)
            {
                case 0x00: // VGA
                    printf("VGA \n");
                    break;
            }
            break;
    }
    
    
    return driver;
}



PCIDeviceDescriptor PCI::GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
    PCIDeviceDescriptor result;
    
    result.bus = bus;
    result.device = device;
    result.function = function;
    
    result.vendor_id = ReadPCI(bus, device, function, 0x00);
    result.device_id = ReadPCI(bus, device, function, 0x02);

    result.class_id = ReadPCI(bus, device, function, 0x0b);
    result.subclass_id = ReadPCI(bus, device, function, 0x0a);
    result.interface_id = ReadPCI(bus, device, function, 0x09);

    result.revision = ReadPCI(bus, device, function, 0x08);
    result.interrupt = ReadPCI(bus, device, function, 0x3c);
    
    return result;
}

PCIDeviceDescriptor::PCIDeviceDescriptor()
{

}

PCIDeviceDescriptor::~PCIDeviceDescriptor()
{

}
