#include "MultiTask.h"

char *INTTOCHARPOINT(int num);
void printf(char *str);
void printHex(uint8_t num);

TaskManager *TaskManager::ActiveManager = 0;

Task::Task(GlobalDescriptorTable *gdt, void entrypoint(), uint8_t* name, int id)
{
    cpustate = (CPUState *)(stack + 4096 - sizeof(CPUState));

    printf("Task ");
    printf((char*)name);
    printf(" created with ID ");
    printHex(id & 0xFF);
    printf("\n");

    TaskName = name;
    TaskID = id;

    cpustate -> eax = 0;
    cpustate -> ebx = 0;
    cpustate -> ecx = 0;
    cpustate -> edx = 0;

    cpustate -> esi = 0;
    cpustate -> edi = 0;
    cpustate -> ebp = 0;

    /*
    cpustate -> gs = 0;
    cpustate -> fs = 0;
    cpustate -> es = 0;
    cpustate -> ds = 0;
    */

    // cpustate -> error = 0;    

    // cpustate -> esp = ;
    cpustate -> eip = (uint32_t)entrypoint;
    cpustate -> cs = gdt->CodeSegmentSelector();
    // cpustate -> ss = ;
    cpustate -> eflags = 0x202;
}

Task::~Task()
{
}


TaskManager::TaskManager()
{
    numTasks = 0;
    currentTask = -1;
}

TaskManager::~TaskManager()
{
}

bool TaskManager::AddTask(Task* task)
{
    printf("Adding task ");
    printf((char*)task->TaskName);
    printf(" with ID ");
    printHex(task->TaskID);
    printf("\n");
    if(numTasks >= 256)
    {
        return false;
    }
    tasks[numTasks++] = task;
    return true;
}

CPUState* TaskManager::Schedule(CPUState* cpustate)
{
    if(numTasks <= 0)
        return cpustate;

    if(currentTask >= 0)
        tasks[currentTask]->cpustate = cpustate;

    if(++currentTask >= numTasks)
        currentTask %= numTasks;
    return tasks[currentTask]->cpustate;
}

CPUState* TaskManager::SwitchTask(int tasknum, CPUState* CSTATE)
{
    if(tasknum <= numTasks)
    {
        currentTask = tasknum;
        printf("TaskManager::SwitchTask: Task switched to ");
        printf(INTTOCHARPOINT(tasks[tasknum]->TaskID));
        printf(".\n");
        return tasks[tasknum]->cpustate;
    }
    else
    {
        printf("TaskManager::SwitchTask: Task number out of range\n");
        return CSTATE;
    }
}

void TaskManager::listTasks()
{
    printf("NumTasks: ");
    printf(INTTOCHARPOINT(numTasks));
    printf("\n");
    printf("Tasks Registered: \n");
    for(int i = 0; i != numTasks; i++)
    {
        printf("Task ");
        printf(INTTOCHARPOINT(tasks[i]->TaskID));
        printf(" : ");
        printf((char*)tasks[i]->TaskName);
        printf("\n");
    }
}

void TaskManager::MakeDefault()
{
    ActiveManager = this;
}