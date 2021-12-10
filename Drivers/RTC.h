#ifndef __RTC_H
#define __RTC_H

#define CURRENT_YEAR    2021

#include "IOPorts.h"
#include "../CPU/Interrupts.h"
#include "../Include/types.h"

    class RTC
    {
        port8BIT CMOS;
        port8BIT CMOSDATA;
        int century_register = 0x00;
    public:
        RTC();
        ~RTC();
        uint32_t get_update_in_progress_flag();
        uint8_t get_RTC_register(int reg);
        void read_rtc();
        uint8_t second;
        uint8_t minute;
        uint8_t hour;
        uint8_t day;
        uint8_t month;
        uint32_t year;
    };

#endif