#ifndef __VGADRIVER_H
#define __VGADRIVER_H

#include "../Includes/types.h"
#include "IOPorts.h"
#include "Driver.h"

//! This driver is deprecated because it will cause damage to the monitor, I guess
class VideoGraphicsArray
{
protected:
    port8BIT MiscPort;
    port8BIT CRTCIndexPort;
    port8BIT CRTCDataPort;
    port8BIT SequencerIndexPort;
    port8BIT SequencerDataPort;
    port8BIT GraphicsControllerIndexPort;
    port8BIT GraphicsControllerDataPort;
    port8BIT AttributeControllerIndexPort;
    port8BIT AttributeControllerReadPort;
    port8BIT AttributeControllerWritePort;
    port8BIT AttributeControllerResetPort;


protected:
    void WriteToRegisters(uint8_t* registers);
    uint8_t* GetVGAFrameBuffer();
    virtual void PutPixel(uint32_t X, uint32_t Y, uint8_t ColorIndex);
    virtual uint8_t GetColorIndex(uint8_t Red, uint8_t Green, uint8_t Blue);

public:
    VideoGraphicsArray();
    ~VideoGraphicsArray();

public:
    virtual bool SetMode(uint32_t ScreenWidth, uint32_t ScreenHeight, uint32_t ColorDepth);
    virtual bool isModeSupported(uint32_t ScreenWidth, uint32_t ScreenHeight, uint32_t ColorDepth);
    virtual void PutPixelOnScreen(uint32_t X, uint32_t Y, uint8_t Red, uint8_t Green, uint8_t Blue);
};




#endif