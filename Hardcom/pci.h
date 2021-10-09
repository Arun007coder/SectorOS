#ifndef __PCI_H
#define __PCI_H

#include "../Drivers/IOPorts.h"
#include "../Includes/types.h"
#include "../CPU/Interrupts.h"
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
    uint8_t subclas_id;
    uint8_t interface_id;

    uint8_t revision;

    PCIDeviceDescriptor();
    ~PCIDeviceDescriptor();
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

    void SelectDrivers(DriverManager* drivermanager);

    PCIDeviceDescriptor GetDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
};

#endif