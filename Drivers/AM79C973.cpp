#include "AM79C973.h"

RawDataHandler::RawDataHandler(AM79C973 *backend)
{
    this->Backend = backend;
    backend->setHandler(this);
}

RawDataHandler::~RawDataHandler()
{
    Backend->setHandler(0);
}

bool RawDataHandler::OnRawDataReceived(uint8_t *buffer, uint32_t size)
{
    return false;
}

void RawDataHandler::Send(uint8_t *buffer, uint32_t size)
{
    Backend->send(buffer, size);
}

void printf(char *);
void printHex(uint8_t);

AM79C973::AM79C973(PCIDeviceDescriptor *dev, InterruptManager *interrupts)
    : Driver(),
      InterruptHandler(dev->interrupt + interrupts->HardwareInterruptOffset(), interrupts),
      MACAddress0Port(dev->portBase),
      MACAddress2Port(dev->portBase + 0x02),
      MACAddress4Port(dev->portBase + 0x04),
      registerDataPort(dev->portBase + 0x10),
      registerAddressPort(dev->portBase + 0x12),
      resetPort(dev->portBase + 0x14),
      busControlRegisterDataPort(dev->portBase + 0x16)
{
    this->handler = 0;
    currentSendBuffer = 0;
    currentRecvBuffer = 0;

    uint64_t MAC0 = MACAddress0Port.ReadFromPort() % 256;
    uint64_t MAC1 = MACAddress0Port.ReadFromPort() / 256;
    uint64_t MAC2 = MACAddress2Port.ReadFromPort() % 256;
    uint64_t MAC3 = MACAddress2Port.ReadFromPort() / 256;
    uint64_t MAC4 = MACAddress4Port.ReadFromPort() % 256;
    uint64_t MAC5 = MACAddress4Port.ReadFromPort() / 256;

    uint64_t MAC = MAC5 << 40 | MAC4 << 32 | MAC3 << 24 | MAC2 << 16 | MAC1 << 8 | MAC0;

    registerAddressPort.WriteToPort(20);
    busControlRegisterDataPort.WriteToPort(0x102);

    registerAddressPort.WriteToPort(0);
    registerDataPort.WriteToPort(0x04);

    initBlock.mode = 0x0000; // promiscuous mode = false
    initBlock.reserved1 = 0;
    initBlock.numSendBuffers = 3;
    initBlock.reserved2 = 0;
    initBlock.numRecvBuffers = 3;
    initBlock.physicalAddress = MAC;
    initBlock.reserved3 = 0;
    initBlock.logicalAddress = 0;

    sendBufferDescr = (BufferDescriptor *)((((uint32_t)&sendBufferDescrMemory[0]) + 15) & ~((uint32_t)0xF));
    initBlock.sendBufferDescrAddress = (uint32_t)sendBufferDescr;
    recvBufferDescr = (BufferDescriptor *)((((uint32_t)&recvBufferDescrMemory[0]) + 15) & ~((uint32_t)0xF));
    initBlock.recvBufferDescrAddress = (uint32_t)recvBufferDescr;

    for (uint8_t i = 0; i < 8; i++)
    {
        sendBufferDescr[i].address = (((uint32_t)&sendBuffers[i]) + 15) & ~(uint32_t)0xF;
        sendBufferDescr[i].flags = 0x7FF | 0xF000;
        sendBufferDescr[i].flags2 = 0;
        sendBufferDescr[i].avail = 0;

        recvBufferDescr[i].address = (((uint32_t)&recvBuffers[i]) + 15) & ~(uint32_t)0xF;
        recvBufferDescr[i].flags = 0xF7FF | 0x80000000;
        recvBufferDescr[i].flags2 = 0;
        sendBufferDescr[i].avail = 0;
    }

    registerAddressPort.WriteToPort(1);
    registerDataPort.WriteToPort((uint32_t)(&initBlock) & 0xFFFF);
    registerAddressPort.WriteToPort(2);
    registerDataPort.WriteToPort(((uint32_t)(&initBlock) >> 16) & 0xFFFF);
}

AM79C973::~AM79C973()
{
}

int AM79C973::UniquedriverID()
{
    return 38576;
}

void AM79C973::activate()
{
    registerAddressPort.WriteToPort(0);
    registerDataPort.WriteToPort(0x41);

    registerAddressPort.WriteToPort(4);
    uint32_t temp = registerDataPort.ReadFromPort();
    registerAddressPort.WriteToPort(4);
    registerDataPort.WriteToPort(temp | 0xC00);

    registerAddressPort.WriteToPort(0);
    registerDataPort.WriteToPort(0x42);

    printf("AM79C973: Activated\n");
}

int AM79C973::reset()
{
    resetPort.ReadFromPort();
    resetPort.WriteToPort(0);
    return 10;
}

uint32_t AM79C973::HandleInterrupt(uint32_t esp)
{
    registerAddressPort.WriteToPort(0);
    uint32_t temp = registerDataPort.ReadFromPort();

    if ((temp & 0x8000) == 0x8000)
        printf("AMD am79c973 ERROR\n");
    if ((temp & 0x2000) == 0x2000)
        printf("AMD am79c973 COLLISION ERROR\n");
    if ((temp & 0x1000) == 0x1000)
        printf("AMD am79c973 MISSED FRAME\n");
    if ((temp & 0x0800) == 0x0800)
        printf("AMD am79c973 MEMORY ERROR\n");
    if ((temp & 0x0400) == 0x0400)
        receive();
    if ((temp & 0x0200) == 0x0200)
        printf(" SENT");

    registerAddressPort.WriteToPort(0);
    registerDataPort.WriteToPort(temp);

    if ((temp & 0x0100) == 0x0100)
        printf("AMD am79c973 INIT DONE\n");

    return esp;
}

void AM79C973::send(uint8_t *buffer, int size)
{
    int sendDescriptor = currentSendBuffer;
    currentSendBuffer = (currentSendBuffer + 1) % 8;

    if (size > 1518)
        size = 1518;

    for (uint8_t *src = buffer + size - 1,
                 *dst = (uint8_t *)(sendBufferDescr[sendDescriptor].address + size - 1);
        src >= buffer; src--, dst--)
        *dst = *src;

    printf("\nSEND: ");
    for (int i = 14 + 20; i < (size > 64 ? 64 : size); i++)
    {
        printHex(buffer[i]);
        printf(" ");
    }

    sendBufferDescr[sendDescriptor].avail = 0;
    sendBufferDescr[sendDescriptor].flags2 = 0;
    sendBufferDescr[sendDescriptor].flags = 0x8300F000 | ((uint16_t)((-size) & 0xFFF));
    registerAddressPort.WriteToPort(0);
    registerDataPort.WriteToPort(0x48);
}

void AM79C973::receive()
{
    printf("\nRECV: ");

    for (; (recvBufferDescr[currentRecvBuffer].flags & 0x80000000) == 0;
        currentRecvBuffer = (currentRecvBuffer + 1) % 8)
    {
        if (!(recvBufferDescr[currentRecvBuffer].flags & 0x40000000) && (recvBufferDescr[currentRecvBuffer].flags & 0x03000000) == 0x03000000)

        {
            uint32_t size = recvBufferDescr[currentRecvBuffer].flags & 0xFFF;
            if (size > 64)
                size -= 4;

            uint8_t *buffer = (uint8_t *)(recvBufferDescr[currentRecvBuffer].address);

            for (int i = 14 + 20; i < (size > 64 ? 64 : size); i++)
            {
                printHex(buffer[i]);
                printf(" ");
            }

            if (handler != 0)
                if (handler->OnRawDataReceived(buffer, size))
                    send(buffer, size);
        }

        recvBufferDescr[currentRecvBuffer].flags2 = 0;
        recvBufferDescr[currentRecvBuffer].flags = 0x8000F7FF;
    }
}

void AM79C973::setHandler(RawDataHandler *handler)
{
    this->handler = handler;
}

uint64_t AM79C973::GetMACAddress()
{
    return initBlock.physicalAddress;
}

void AM79C973::SetIPAddress(uint32_t ip)
{
    initBlock.logicalAddress = ip;
}

uint32_t AM79C973::GetIPAddress()
{
    return initBlock.logicalAddress;
}