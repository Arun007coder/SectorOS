#ifndef EXCEPTIONS_IOC_H
#define EXCEPTIONS_IOC_H

#include "../../Include/types.h"
#include "../../Include/Public_VAR.h"
#include "../Interrupts.h"
#include "../PowerControl.h"
#include "../../Include/func.h"

class EXCEPTIONS_IOC : public InterruptHandler
{
    PowerControl Power;
public:
    EXCEPTIONS_IOC(InterruptManager* interruptManager);
    ~EXCEPTIONS_IOC();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif