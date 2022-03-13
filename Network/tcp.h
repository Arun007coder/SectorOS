#ifndef __TCP_H
#define __TCP_H

#include "../Memory/MemoryManagement.h"
#include "../Include/types.h"
#include "InternetProtocolV4.h"

enum TransmissionControlProtocolSocketState
{
    CLOSED,
    LISTEN,
    SYN_SENT,
    SYN_RECEIVED,
    
    ESTABLISHED,
    
    FIN_WAIT1,
    FIN_WAIT2,
    CLOSING,
    TIME_WAIT,
    
    CLOSE_WAIT
    //LAST_ACK
};

enum TransmissionControlProtocolFlag
{
    FIN = 1,
    SYN = 2,
    RST = 4,
    PSH = 8,
    ACK = 16,
    URG = 32,
    ECE = 64,
    CWR = 128,
    NS = 256
};


struct TransmissionControlProtocolHeader
{
    uint16_t srcPort;
    uint16_t dstPort;
    uint32_t sequenceNumber;
    uint32_t acknowledgementNumber;
    
    uint8_t reserved : 4;
    uint8_t headerSize32 : 4;
    uint8_t flags;
    
    uint16_t windowSize;
    uint16_t checksum;
    uint16_t urgentPtr;
    
    uint32_t options;
} __attribute__((packed));


struct TransmissionControlProtocolPseudoHeader
{
    uint32_t srcIP;
    uint32_t dstIP;
    uint16_t protocol;
    uint16_t totalLength;
} __attribute__((packed));


class TransmissionControlProtocolSocket;
class TransmissionControlProtocolProvider;



class TransmissionControlProtocolHandler
{
public:
    TransmissionControlProtocolHandler();
    ~TransmissionControlProtocolHandler();
    virtual void TEST();
    virtual void TEST2();
    virtual bool HandleTransmissionControlProtocolMessage(TransmissionControlProtocolSocket *socket, uint8_t *data, uint16_t size);
};



class TransmissionControlProtocolSocket
{
friend class TransmissionControlProtocolProvider;
protected:
    uint16_t remotePort;
    uint32_t remoteIP;
    uint16_t localPort;
    uint32_t localIP;
    uint32_t sequenceNumber;
    uint32_t acknowledgementNumber;
    TransmissionControlProtocolProvider* backend;
    TransmissionControlProtocolHandler* handler;
    
    TransmissionControlProtocolSocketState state;
public:
    TransmissionControlProtocolSocket(TransmissionControlProtocolProvider* backend);
    ~TransmissionControlProtocolSocket();
    virtual bool HandleTransmissionControlProtocolMessage(uint8_t *data, uint16_t size);
    virtual void Send(uint8_t* data, uint16_t size);
    virtual void Disconnect();
};

class TransmissionControlProtocolProvider : InternetProtocolHandler
{
protected:
    TransmissionControlProtocolSocket* sockets[65535];
    uint16_t numSockets;
    uint16_t freePort;
    
public:
    TransmissionControlProtocolProvider(InternetProtocolProvider* backend);
    ~TransmissionControlProtocolProvider();
    
    virtual bool OnInternetProtocolReceived(uint32_t srcIP_BE, uint32_t dstIP_BE, uint8_t* internetprotocolPayload, uint32_t size);
    virtual TransmissionControlProtocolSocket* Connect(uint32_t ip, uint16_t port);
    virtual void Disconnect(TransmissionControlProtocolSocket* socket);
    virtual void Send(TransmissionControlProtocolSocket* socket, uint8_t* data, uint16_t size, uint16_t flags = 0);
    virtual TransmissionControlProtocolSocket* Listen(uint16_t port);
    virtual void Bind(TransmissionControlProtocolSocket* socket, TransmissionControlProtocolHandler* handler);
};

#endif