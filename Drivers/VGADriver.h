#ifndef __VGADRIVER_H
#define __VGADRIVER_H

#include "../Include/types.h"
#include "IOPorts.h"
#include "Driver.h"

class VideoGraphicsArray
{
protected:
    port8BIT miscPort;
    port8BIT crtcIndexPort;
    port8BIT crtcDataPort;
    port8BIT sequencerIndexPort;
    port8BIT sequencerDataPort;
    port8BIT graphicsControllerIndexPort;
    port8BIT graphicsControllerDataPort;
    port8BIT attributeControllerIndexPort;
    port8BIT attributeControllerReadPort;
    port8BIT attributeControllerWritePort;
    port8BIT attributeControllerResetPort;
    void WriteRegisters(uint8_t* registers);
    uint8_t* GetFrameBufferSegment();
    virtual uint8_t GetColorIndex(uint8_t r, uint8_t g, uint8_t b);
public:
    VideoGraphicsArray();
    ~VideoGraphicsArray();
    virtual bool SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth);
    virtual bool SetMode(uint32_t width, uint32_t height, uint32_t colordepth);
    virtual void PutPixel(uint32_t x, uint32_t y,  uint8_t r, uint8_t g, uint8_t b);
    virtual void PutPixel(uint32_t x, uint32_t y, uint8_t colorIndex);
};




#endif