#ifndef __EXECPTION_DOUBLE_FAULT_H
#define __EXECPTION_DOUBLE_FAULT_H

#include "../../Include/types.h"
#include "../../Include/Public_VAR.h"
#include "../Interrupts.h"
#include "../PowerControl.h"
#include "../../Include/func.h"

class EXECPTION_DOUBLE_FAULT : public InterruptHandler
{
    PowerControl Power;
public:
    EXECPTION_DOUBLE_FAULT(InterruptManager* interruptManager);
    ~EXECPTION_DOUBLE_FAULT();
    virtual uint32_t HandleInterrupt(uint32_t esp);
};

#endif