#ifndef __MULTITASK_H
#define __MULTITASK_H

#include "../Include/types.h"
#include "../kernel/gdt.h"
#include "../Include/Public_VAR.h"

struct CPUState
{
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;
    /*
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    */
    uint32_t error;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;        
} __attribute__((packed));

class Task
{
friend class TaskManager;
private:
    uint8_t stack[4096]; // 4 KiB
    CPUState* cpustate;
    uint8_t* TaskName;
    int TaskID;

public:
    Task(GlobalDescriptorTable *gdt, void entrypoint(), uint8_t* name, int id);
    ~Task();
};
class TaskManager
{
private:
    Task* tasks[256];
    int currentTask;
    int numTasks;

public:
    TaskManager();
    ~TaskManager();
    uint32_t TID;
    static TaskManager *ActiveManager;
    bool AddTask(Task *task);
    CPUState* SwitchTask(int tasknum, CPUState* CSTATE);
    CPUState* Schedule(CPUState* cpustate);
    void listTasks();
    void MakeDefault();
};

#endif