#ifndef __PCI_H
#define __PCI_H


#include "../Drivers/IOPorts.h"
#include "../Include/Public_VAR.h"
#include "../Include/types.h"
#include "../CPU/Interrupts.h"
#include "../Memory/MemoryManagement.h"
#include "../Drivers/Driver.h"

class PCIDeviceDescriptor
{
public:
    uint32_t portBase;
    uint32_t interrupt;

    uint16_t bus;
    uint16_t device;
    uint16_t function;

    uint16_t vendor_id;
    uint16_t device_id;

    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t interface_id;

    uint8_t revision;

    PCIDeviceDescriptor();
    ~PCIDeviceDescriptor();
};

enum BaseAddressRegisterType
{
    MemoryMapping = 0,
    InputOutput = 1
};

class BaseAddressRegister
{
public:
    bool prefetchable;
    uint8_t* address;
    uint32_t size;
    BaseAddressRegisterType type;
};

class PCI
{
    port32BIT DataPort;
    port32BIT CommandPort;

public:
    PCI();
    ~PCI();

    uint32_t ReadPCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t RegisterOffset);
    void WritePCI(uint16_t bus, uint16_t device, uint16_t function, uint32_t RegisterOffset, uint32_t value);
    bool DeviceHasFunctions(uint16_t bus, uint16_t device);

    void SelectDrivers(DriverManager* driverManager, InterruptManager* interrupts);
    Driver* GetDriver(PCIDeviceDescriptor dev, InterruptManager* interrupts);

    PCIDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
    BaseAddressRegister GetBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t bar);
};

#endif