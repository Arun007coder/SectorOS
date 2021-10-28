#include "VGADriver.h"

VideoGraphicsArray::VideoGraphicsArray() : 
    MiscPort(0x3c2),
    CRTCIndexPort(0x3d4),
    CRTCDataPort(0x3d5),
    SequencerIndexPort(0x3c4),
    SequencerDataPort(0x3c5),
    GraphicsControllerIndexPort(0x3ce),
    GraphicsControllerDataPort(0x3cf),
    AttributeControllerIndexPort(0x3c0),
    AttributeControllerReadPort(0x3c1),
    AttributeControllerWritePort(0x3c0),
    AttributeControllerResetPort(0x3da)
{
}

VideoGraphicsArray::~VideoGraphicsArray()
{
}



void VideoGraphicsArray::WriteToRegisters(uint8_t* registers)
{
    //  misc
    MiscPort.WriteToPort(*(registers++));

    // Sequencer
    for(uint8_t i = 0; i < 5; i++)
    {
        SequencerIndexPort.WriteToPort(i);
        SequencerDataPort.WriteToPort(*(registers++));
    }

    // cathode ray tube controller
    CRTCIndexPort.WriteToPort(0x03);
    CRTCDataPort.WriteToPort(CRTCDataPort.ReadFromPort() | 0x80);
    CRTCIndexPort.WriteToPort(0x11);
    CRTCDataPort.WriteToPort(CRTCDataPort.ReadFromPort() & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for(uint8_t i = 0; i < 25; i++)
    {
        CRTCIndexPort.WriteToPort(i);
        CRTCDataPort.WriteToPort(*(registers++));
    }

    // graphics controller
    for(uint8_t i = 0; i < 9; i++)
    {
        GraphicsControllerIndexPort.WriteToPort(i);
        GraphicsControllerDataPort.WriteToPort(*(registers++));
    }

    // Attribute controller
    for(uint8_t i = 0; i < 21; i++)
    {
        AttributeControllerResetPort.ReadFromPort();
        AttributeControllerIndexPort.WriteToPort(i);
        AttributeControllerWritePort.WriteToPort(*(registers++));
    }

    AttributeControllerResetPort.ReadFromPort();
    AttributeControllerIndexPort.WriteToPort(0x20);

}

bool VideoGraphicsArray::isModeSupported(uint32_t width, uint32_t height, uint32_t colordepth)
{
    return width == 320 && height == 200 && colordepth == 8;
}

bool VideoGraphicsArray::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    if(!isModeSupported(width, height, colordepth))
        return false;

    unsigned char g_320x200x256[] =
    {
        /* MISC */
            0x63,
        /* SEQ */
            0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
            0xFF,
        /* GC */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
        /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00
    };

    WriteToRegisters(g_320x200x256);
    return true;
}

uint8_t* VideoGraphicsArray::GetVGAFrameBuffer()
{
    GraphicsControllerIndexPort.WriteToPort(0x06);
    uint8_t segmentNumber = GraphicsControllerDataPort.ReadFromPort() & (3<<2);
    switch(segmentNumber)
    {
        default:
        case 0<<2: return (uint8_t*)0x00000;
        case 1<<2: return (uint8_t*)0xA0000;
        case 2<<2: return (uint8_t*)0xB0000;
        case 3<<2: return (uint8_t*)0xB8000;
    }
}

void VideoGraphicsArray::PutPixel(uint32_t x, uint32_t y,  uint8_t colorIndex)
{
    uint8_t* pixelAddress = GetVGAFrameBuffer() + 320*y + x;
    *pixelAddress = colorIndex;
}

uint8_t VideoGraphicsArray::GetColorIndex(uint8_t Redr, uint8_t Green, uint8_t Blue)
{
    if(Redr == 0x00, Green == 0x00, Blue == 0xA8)
        return 0x01;
    return 0x00;
}

void VideoGraphicsArray::PutPixelOnScreen(uint32_t x, uint32_t y,  uint8_t r, uint8_t g, uint8_t b)
{
    PutPixel(x,y, GetColorIndex(r,g,b));
}