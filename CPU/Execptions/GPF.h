#ifndef EXCEPTIONS_GPF_H
#define EXCEPTIONS_GPF_H

#include "../../Include/types.h"
#include "../../Include/Public_VAR.h"
#include "../Interrupts.h"
#include "../PowerControl.h"
#include "../../Include/func.h"

class EXCEPTIONS_GPF : public InterruptHandler
{
    PowerControl Power;
public:
    EXCEPTIONS_GPF(InterruptManager* interruptManager);
    ~EXCEPTIONS_GPF();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif