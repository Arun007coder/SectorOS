#include "Mouse.h"

MouseEventHandler::MouseEventHandler()
{
}
void MouseEventHandler::OnActivate()
{
}
void MouseEventHandler::OnMouseDown(uint8_t button)
{
}
void MouseEventHandler::OnMouseUp(uint8_t button)
{
}
void MouseEventHandler::OnMouseMove(int x, int y)
{
}

MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
: InterruptHandler(0x2C, manager),
DataPort(0x60),
CommandPort(0x64)
{
    this->handler = handler;
}
MouseDriver::~MouseDriver()
{
}
void MouseDriver::activate()
{
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
    offset = 0;
    buttons = 0;

    if(handler != 0)
        handler->OnActivate();
    
    CommandPort.WriteToPort(0xA8);
    CommandPort.WriteToPort(0x20); // command 0x60 = ReadFromPort controller command byte
    uint8_t status = DataPort.ReadFromPort() | 2;
    CommandPort.WriteToPort(0x60); // command 0x60 = set controller command byte
    DataPort.WriteToPort(status);
    CommandPort.WriteToPort(0xD4);
    DataPort.WriteToPort(0xF4);
    DataPort.ReadFromPort();
}

int MouseDriver::UniquedriverID()
{
    return 79154;
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = CommandPort.ReadFromPort();
    if (!(status & 0x20))
        return esp;
    buffer[offset] = DataPort.ReadFromPort();

    if(handler == 0)
        return esp;

    offset = (offset + 1) % 3;
    if(offset == 0)
    {
        if(buffer[1] != 0 || buffer[2] != 0)
        {
            handler->OnMouseMove(buffer[1], -buffer[2]);
        }

        for(uint8_t i = 0; i < 3; i++)
        {
            if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
            {
                if(buttons & (0x1<<i))
                    handler->OnMouseUp(i+1);
                else
                    handler->OnMouseDown(i+1);
            }
            buttons = buffer[0];
        }
    }
    return esp;
}