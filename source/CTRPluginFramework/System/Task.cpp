#include "CTRPluginFramework/System/Task.hpp"

namespace   CTRPluginFramework
{
    Task::Task(TaskFunc func, void *arg, s32 affinity)
    {
        context->func = func;
        context->arg = arg;
        context->affinity = affinity;
        LightEvent_Init(context->event);
    }

    Task::Task(const Task& task)
    {
        context = task.context;
    }

    Task::Task(Task&& task)
    {
        context = task.context;
    }

    int    Task::Start(void) const
    {
        if(context == nullptr || (context->flags & 3))
            return -1;

        LightEvent_Clear(context->event);
        Scheduler::Schedule(this);
    }

    int    Task::Start(void *arg) const
    {
        if(context == nullptr || (context->flags & 3))
            return -1;

        context->arg = arg;

        LightEvent_Clear(context->event);
        Scheduler::Schedule(this);
    }

    s32    Wait(void) const
    {
        if(context == nullptr)
            return -1;

        if(context->flags != 4 && context->flags)
            LightEvent_Wait(context->event);

        return context->result;
    }

    u32    Task::Status(void) const
    {
        if(context == nullptr)
            return -1;
        return context->flags;
    }
}
