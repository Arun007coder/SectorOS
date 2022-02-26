#ifndef __ICMP_H__
#define __ICMP_H__

#include "../Include/types.h"
#include "InternetProtocolV4.h"

struct InternetControlMessageProtocolMessage
{
    uint8_t type;
    uint8_t code;

    uint16_t checksum;
    uint32_t data;

} __attribute__((packed));

class InternetControlMessageProtocol : InternetProtocolHandler
{
public:
    InternetControlMessageProtocol(InternetProtocolProvider *backend);
    ~InternetControlMessageProtocol();

    bool OnInternetProtocolReceived(uint32_t srcIP_BE, uint32_t dstIP_BE,
                                    uint8_t *internetprotocolPayload, uint32_t size);
    void RequestEchoReply(uint32_t ip_be);
};

#endif