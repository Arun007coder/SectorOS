#include "Mouse.h"

    MouseDriver::MouseDriver(InterruptManager* manager)
    : InterruptHandler(0x2C, manager),
    DataPort(0x60),
    CommandPort(0x64)
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        offset = 0;
        buttons = 0;
        x = 40;
        y = 12;
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        CommandPort.WriteToPort(0xA8);
        CommandPort.WriteToPort(0x20); // command 0x60 = ReadFromPort controller command byte
        uint8_t status = DataPort.ReadFromPort() | 2;
        CommandPort.WriteToPort(0x60); // command 0x60 = set controller command byte
        DataPort.WriteToPort(status);

        CommandPort.WriteToPort(0xD4);
        DataPort.WriteToPort(0xF4);
        DataPort.ReadFromPort();
    }

    MouseDriver::~MouseDriver()
    {
    }

    uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
    {
        uint8_t status = CommandPort.ReadFromPort();
        if (!(status & 0x20))
            return esp;

        buffer[offset] = DataPort.ReadFromPort();
        offset = (offset + 1) % 3;

        if(offset == 0)
        {
            if(buffer[1] != 0 || buffer[2] != 0)
            {
                static uint16_t* VideoMemory = (uint16_t*)0xb8000;
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                    | (VideoMemory[80*y+x] & 0xF000) >> 4
                                    | (VideoMemory[80*y+x] & 0x00FF);

                x += buffer[1];
                if(x >= 80) x = 79;
                if(x < 0) x = 0;
                y -= buffer[2];
                if(y >= 25) y = 24;
                if(y < 0) y = 0;

                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                    | (VideoMemory[80*y+x] & 0xF000) >> 4
                                    | (VideoMemory[80*y+x] & 0x00FF);
            }
        }
        return esp;
    }