#ifndef __PORT_H

#define __PORT_H
#include "../Include/types.h"

    class port
    {
    protected:
        uint16_t portnumber;
        port(uint16_t portnumber);
        ~port();
    };

    class port8BIT : public port
    {
    public:
        port8BIT(uint16_t portnumber);
        ~port8BIT();
        virtual void WriteToPort(uint8_t data);
        virtual uint8_t ReadFromPort();
    };

    class port8BITSlow : public port8BIT
    {
    public:
        port8BITSlow(uint16_t portnumber);
        ~port8BITSlow();
        virtual void WriteToPort(uint8_t data);
    };

    class port16BIT : public port
    {
    public:
        port16BIT(uint16_t portnumber);
        ~port16BIT();
        virtual void WriteToPort(uint16_t data);
        virtual uint16_t ReadFromPort();
    };

    class port32BIT : public port
    {
    public:
        port32BIT(uint16_t portnumber);
        ~port32BIT();
        virtual void WriteToPort(uint32_t data);
        virtual uint32_t ReadFromPort();
    };
#endif