#ifndef __UDP_H__
#define __UDP_H__

#include "../Include/types.h"
#include "InternetProtocolV4.h"

struct UserDatagramProtocolHeader
{
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t length;
    uint16_t checksum;
}
__attribute__((packed));

class UserDatagramProtocolSocket;
class UserDatagramProtocolProvider;

class UserDatagramProtocolHandler
{
public:
    UserDatagramProtocolHandler();
    ~UserDatagramProtocolHandler();
    virtual void HandleUserDatagramProtocolMessage(UserDatagramProtocolSocket *socket, uint8_t *data, uint16_t size);
};

class UserDatagramProtocolSocket
{
    friend class UserDatagramProtocolProvider;

protected:
    uint16_t remotePort;
    uint32_t remoteIP;
    uint16_t localPort;
    uint32_t localIP;
    UserDatagramProtocolProvider *backend;
    UserDatagramProtocolHandler *handler;
    bool listening;

public:
    UserDatagramProtocolSocket(UserDatagramProtocolProvider *backend);
    ~UserDatagramProtocolSocket();
    virtual void HandleUserDatagramProtocolMessage(uint8_t *data, uint16_t size);
    virtual void Send(uint8_t *data, uint16_t size);
    virtual void Disconnect();
};

class UserDatagramProtocolProvider : InternetProtocolHandler
{
protected:
    UserDatagramProtocolSocket *sockets[65535];
    uint16_t numSockets;
    uint16_t freePort;

public:
    UserDatagramProtocolProvider(InternetProtocolProvider *backend);
    ~UserDatagramProtocolProvider();

    virtual bool OnInternetProtocolReceived(uint32_t srcIP_BE, uint32_t dstIP_BE, uint8_t *internetprotocolPayload, uint32_t size);

    virtual UserDatagramProtocolSocket *Connect(uint32_t ip, uint16_t port);
    virtual UserDatagramProtocolSocket *Listen(uint16_t port);
    virtual void Disconnect(UserDatagramProtocolSocket *socket);
    virtual void Send(UserDatagramProtocolSocket *socket, uint8_t *data, uint16_t size);

    virtual void Bind(UserDatagramProtocolSocket *socket, UserDatagramProtocolHandler *handler);
};

#endif