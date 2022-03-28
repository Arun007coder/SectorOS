
#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H

    #include "../kernel/gdt.h"
    #include "../Include/types.h"
    #include "../Drivers/IOPorts.h"
    #include "../CPU/PowerControl.h"
    #include "../kernel/MultiTask.h"

    class InterruptManager;

    class InterruptHandler
    {
    protected:
        uint8_t interrupt;
        InterruptManager* interruptManager;

        InterruptHandler(uint8_t interrupt, InterruptManager* interruptManager);
        ~InterruptHandler();

    public:
        virtual uint32_t HandleInterrupt(uint32_t esp);
        void wait(int msec);
    };

class InterruptManager
{
    friend class InterruptHandler;
protected:
    InterruptHandler* handlers[256];
    TaskManager *taskManager;

    struct GateDescriptor
    {
        uint16_t handlerAddressLowBits;
        uint16_t gdt_codeSegmentSelector;
        uint8_t reserved;
        uint8_t access;
        uint16_t handlerAddressHighBits;
    } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                uint16_t size;
                uint32_t base;
            } __attribute__((packed));

            uint16_t hardwareInterruptOffset;
            //static InterruptManager* ActiveInterruptManager;
            static void SetInterruptDescriptorTableEntry(uint8_t interrupt,
                uint16_t codeSegmentSelectorOffset, void (*handler)(),
                uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType);


            static void InterruptIgnore();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
            static void HandleInterruptRequest0x02();
            static void HandleInterruptRequest0x03();
            static void HandleInterruptRequest0x04();
            static void HandleInterruptRequest0x05();
            static void HandleInterruptRequest0x06();
            static void HandleInterruptRequest0x07();
            static void HandleInterruptRequest0x08();
            static void HandleInterruptRequest0x09();
            static void HandleInterruptRequest0x0A();
            static void HandleInterruptRequest0x0B();
            static void HandleInterruptRequest0x0C();
            static void HandleInterruptRequest0x0D();
            static void HandleInterruptRequest0x0E();
            static void HandleInterruptRequest0x0F();
            static void HandleInterruptRequest0x31();

            static void HandleInterruptRequest0x80();

            static void HandleException0x00();
            static void HandleException0x01();
            static void HandleException0x02();
            static void HandleException0x03();
            static void HandleException0x04();
            static void HandleException0x05();
            static void HandleException0x06();
            static void HandleException0x07();
            static void HandleException0x08();
            static void HandleException0x09();
            static void HandleException0x0A();
            static void HandleException0x0B();
            static void HandleException0x0C();
            static void HandleException0x0D();
            static void HandleException0x0E();
            static void HandleException0x0F();
            static void HandleException0x10();
            static void HandleException0x11();
            static void HandleException0x12();
            static void HandleException0x13();

            static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);

            port8BITSlow programmableInterruptControllerMasterCommandPort;
            port8BITSlow programmableInterruptControllerMasterDataPort;
            port8BITSlow programmableInterruptControllerSlaveCommandPort;
            port8BITSlow programmableInterruptControllerSlaveDataPort;

            bool isTaskAllowed;

        public:
            static InterruptManager *ActiveInterruptManager;
            InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable);
            ~InterruptManager();
            uint16_t HardwareInterruptOffset();
            uint32_t DoHandleInterrupt(uint8_t interrupt, uint32_t esp);
            void Activate();
            void Deactivate();
    };
#endif 