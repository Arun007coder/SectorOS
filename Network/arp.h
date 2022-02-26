#ifndef __ARP_H__
#define __ARP_H__

#include "../Include/types.h"
#include "EtherFrame.h"

struct AddressResolutionProtocolMessage
{
    uint16_t hardwareType;
    uint16_t protocol;
    uint8_t hardwareAddressSize;
    uint8_t protocolAddressSize;
    uint16_t command;

    uint64_t srcMAC : 48;
    uint32_t srcIP;
    uint64_t dstMAC : 48;
    uint32_t dstIP;

} __attribute__((packed));

class AddressResolutionProtocol : public EtherFrameHandler
{

    uint32_t IPcache[128];
    uint64_t MACcache[128];
    int numCacheEntries;

public:
    AddressResolutionProtocol(EtherFrameProvider *backend);
    ~AddressResolutionProtocol();

    bool OnEtherFrameReceived(uint8_t *etherframePayload, uint32_t size);

    void RequestMACAddress(uint32_t IP_BE);
    uint64_t GetMACFromCache(uint32_t IP_BE);
    uint64_t Resolve(uint32_t IP_BE);
    void BroadcastMACAddress(uint32_t IP_BE);
};

#endif