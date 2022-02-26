#include "ICMP.h"

InternetControlMessageProtocol::InternetControlMessageProtocol(InternetProtocolProvider *backend)
    : InternetProtocolHandler(backend, 0x01)
{
}

InternetControlMessageProtocol::~InternetControlMessageProtocol()
{
}

void printf(char *);
void printHex(uint8_t);
bool InternetControlMessageProtocol::OnInternetProtocolReceived(uint32_t srcIP_BE, uint32_t dstIP_BE, uint8_t *internetprotocolPayload, uint32_t size)
{
    if (size < sizeof(InternetControlMessageProtocolMessage))
        return false;

    InternetControlMessageProtocolMessage *msg = (InternetControlMessageProtocolMessage *)internetprotocolPayload;

    switch (msg->type)
    {

    case 0:
        printf("ping response from ");
        printHex(srcIP_BE & 0xFF);
        printf(".");
        printHex((srcIP_BE >> 8) & 0xFF);
        printf(".");
        printHex((srcIP_BE >> 16) & 0xFF);
        printf(".");
        printHex((srcIP_BE >> 24) & 0xFF);
        printf("\n");
        break;

    case 8:
        msg->type = 0;
        msg->checksum = 0;
        msg->checksum = InternetProtocolProvider::Checksum((uint16_t *)msg, sizeof(InternetControlMessageProtocolMessage));
        return true;
    }

    return false;
}

void InternetControlMessageProtocol::RequestEchoReply(uint32_t ip_be)
{
    InternetControlMessageProtocolMessage icmp;
    icmp.type = 8;
    icmp.code = 0;
    icmp.data = 0x3713;
    icmp.checksum = 0;
    icmp.checksum = InternetProtocolProvider::Checksum((uint16_t *)&icmp, sizeof(InternetControlMessageProtocolMessage));

    InternetProtocolHandler::Send(ip_be, (uint8_t *)&icmp, sizeof(InternetControlMessageProtocolMessage));
}