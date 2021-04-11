#include "CTRPluginFramework/System/Thread.hpp"

namespace   CTRPluginFramework
{
    ThreadEx::ThreadEx(ThreadFunc entrypoint, u32 stackSize, u32 priority, int affinity)
    {
        this->priority = priority;
        this->affinity = affinity;
        this->_state   = IDLE;
        this->_thread  = __createThread(entrypoint, stackSize, 0);
    }

    ThreadEx::~ThreadEx(void)
    {
        if (this->_thread)
        {
            free(this->_thread);
            this->_thread = nullptr;
        }
    }

    Result ThreadEx::Start(void *arg)
    {
        Result res;
        if (!this->_thread)
            return -1;
        if ( this->_state == RUNNING )
            return 0;
        this->_thread->arg = arg;
        res = __startThread(this->_thread, this->priority, this->affinity);
        if ( res >= 0 )
            this->_state = RUNNING;
        return res;
    }

    Result ThreadEx::Join(bool releaseResources)
    {
        Result res = threadJoin(this->_thread, -1);
        if(releaseResources)
        {
            threadFree(this->_thread);
            this->_thread = nullptr;
        }
        this->_state = FINISHED;
        res;
    }

    Handle  ThreadEx::GetHandle(void)
    {
        if(this->_thread)
            return this->_thread->handle;
        return 0;
    }

    u32  ThreadEx::GetStatus(void)
    {
        return this->_state;
    }

    void  ThreadEx::Yield(void)
    {
        return svcSleepThread(0);
    }
}
