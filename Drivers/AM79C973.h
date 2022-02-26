#ifndef __AM79C973_H__
#define __AM79C973_H__

#include "../Hardcom/pci.h"
#include "Driver.h"
#include "../Include/types.h"
#include "../CPU/Interrupts.h"
#include "IOPorts.h"

class AM79C973;

class RawDataHandler
{
protected:
    AM79C973 *Backend;
public:
    RawDataHandler(AM79C973 *backend);
    ~RawDataHandler();
    virtual bool OnRawDataReceived(uint8_t *data, uint32_t length);
    void Send(uint8_t *data, uint32_t length);
};

class AM79C973 : public Driver, public InterruptHandler
{
    struct InitializationBlock
    {
        uint16_t mode;
        unsigned reserved1 : 4;
        unsigned numSendBuffers : 4;
        unsigned reserved2 : 4;
        unsigned numRecvBuffers : 4;
        uint64_t physicalAddress : 48;
        uint16_t reserved3;
        uint64_t logicalAddress;
        uint32_t recvBufferDescrAddress;
        uint32_t sendBufferDescrAddress;
    } __attribute__((packed));

    struct BufferDescriptor
    {
        uint32_t address;
        uint32_t flags;
        uint32_t flags2;
        uint32_t avail;
    } __attribute__((packed));

    port16BIT MACAddress0Port;
    port16BIT MACAddress2Port;
    port16BIT MACAddress4Port;
    port16BIT registerDataPort;
    port16BIT registerAddressPort;
    port16BIT resetPort;
    port16BIT busControlRegisterDataPort;

    InitializationBlock initBlock;

    BufferDescriptor *sendBufferDescr;
    uint8_t sendBufferDescrMemory[2048 + 15];
    uint8_t sendBuffers[2 * 1024 + 15][8];
    uint8_t currentSendBuffer;

    BufferDescriptor *recvBufferDescr;
    uint8_t recvBufferDescrMemory[2048 + 15];
    uint8_t recvBuffers[2 * 1024 + 15][8];
    uint8_t currentRecvBuffer;

    RawDataHandler* handler;

public:
    AM79C973(PCIDeviceDescriptor *dev, InterruptManager *interrupts);
    ~AM79C973();

    void send(uint8_t* buffer, int count);
    void receive();

    void activate();
    int UniquedriverID();
    int reset();
    uint32_t HandleInterrupt(uint32_t esp);

    void setHandler(RawDataHandler* handler);

    uint64_t GetMACAddress();

    void SetIPAddress(uint32_t);
    uint32_t GetIPAddress();
};

#endif