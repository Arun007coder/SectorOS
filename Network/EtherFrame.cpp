#include "EtherFrame.h"

EtherFrameHandler::EtherFrameHandler(EtherFrameProvider *backend, uint16_t etherType)
{
    this->etherType_BE = ((etherType & 0x00FF) << 8) | ((etherType & 0xFF00) >> 8);
    this->backend = backend;
    backend->handlers[etherType_BE] = this;
}

EtherFrameHandler::~EtherFrameHandler()
{
    if (backend->handlers[etherType_BE] == this)
        backend->handlers[etherType_BE] = 0;
}

bool EtherFrameHandler::OnEtherFrameReceived(uint8_t *etherframePayload, uint32_t size)
{
    return false;
}

void EtherFrameHandler::Send(uint64_t dstMAC_BE, uint8_t *data, uint32_t size)
{
    backend->Send(dstMAC_BE, etherType_BE, data, size);
}

uint32_t EtherFrameHandler::GetIPAddress()
{
    return backend->GetIPAddress();
}

EtherFrameProvider::EtherFrameProvider(AM79C973 *backend)
    : RawDataHandler(backend)
{
    for (uint32_t i = 0; i < 65535; i++)
        handlers[i] = 0;
}

EtherFrameProvider::~EtherFrameProvider()
{
}

bool EtherFrameProvider::OnRawDataReceived(uint8_t *buffer, uint32_t size)
{
    if (size < sizeof(EtherFrameHeader))
        return false;

    EtherFrameHeader *frame = (EtherFrameHeader *)buffer;
    bool sendBack = false;

    if (frame->dstMAC_BE == 0xFFFFFFFFFFFF || frame->dstMAC_BE == Backend->GetMACAddress())
    {
        if (handlers[frame->etherType_BE] != 0)
            sendBack = handlers[frame->etherType_BE]->OnEtherFrameReceived(
                buffer + sizeof(EtherFrameHeader), size - sizeof(EtherFrameHeader));
    }

    if (sendBack)
    {
        frame->dstMAC_BE = frame->srcMAC_BE;
        frame->srcMAC_BE = Backend->GetMACAddress();
    }

    return sendBack;
}

void EtherFrameProvider::Send(uint64_t dstMAC_BE, uint16_t etherType_BE, uint8_t *buffer, uint32_t size)
{
    uint8_t *buffer2 = (uint8_t *)MemoryManager::ActiveMemoryManager->MemAllocate(sizeof(EtherFrameHeader) + size);
    EtherFrameHeader *frame = (EtherFrameHeader *)buffer2;

    frame->dstMAC_BE = dstMAC_BE;
    frame->srcMAC_BE = Backend->GetMACAddress();
    frame->etherType_BE = etherType_BE;

    uint8_t *src = buffer;
    uint8_t *dst = buffer2 + sizeof(EtherFrameHeader);
    for (uint32_t i = 0; i < size; i++)
        dst[i] = src[i];

    Backend->send(buffer2, size + sizeof(EtherFrameHeader));

    MemoryManager::ActiveMemoryManager->MemFree(buffer2);
}

uint32_t EtherFrameProvider::GetIPAddress()
{
    return Backend->GetIPAddress();
}

uint64_t EtherFrameProvider::GetMACAddress()
{
    return Backend->GetMACAddress();
}