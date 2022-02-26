#ifndef __ETHERFRAME_H
#define __ETHERFRAME_H

#include "../Include/types.h"
#include "../Drivers/AM79C973.h"

struct EtherFrameHeader
{
    uint64_t dstMAC_BE : 48;
    uint64_t srcMAC_BE : 48;
    uint16_t etherType_BE;
} __attribute__((packed));

typedef uint32_t EtherFrameFooter;

class EtherFrameProvider;

class EtherFrameHandler
{
protected:
    EtherFrameProvider *backend;
    uint16_t etherType_BE;

public:
    EtherFrameHandler(EtherFrameProvider* backend, uint16_t etherType);
    ~EtherFrameHandler();
    virtual bool OnEtherFrameReceived(uint8_t *etherframepayload, uint32_t length);
    void Send(uint64_t dstMAC_BE, uint8_t *data, uint32_t size);
    uint32_t GetIPAddress();
};

class EtherFrameProvider : public RawDataHandler
{
    friend class EtherFrameHandler;
protected:
    EtherFrameHandler *handlers[65535];
public:
    EtherFrameProvider(AM79C973 *backend);
    ~EtherFrameProvider();
    void Send(uint64_t DSTMACADDR_BE, uint16_t ETHERTYPE_BE, uint8_t *data, uint32_t length);
    virtual bool OnRawDataReceived(uint8_t *data, uint32_t length);

    uint64_t GetMACAddress();
    uint32_t GetIPAddress();
};

#endif