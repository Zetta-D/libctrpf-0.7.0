#include "CTRPluginFramework/System/Lock.hpp"

namespace CTRPluginFramework
{
    Lock::Lock(LightLock &llock) : _type(1), _llock(llock)
    {
        LightLock_Lock(llock);
    }

    Lock::Lock(RecursiveLock &rlock) : _type(2), _rlock(rlock)
    {
        RecursiveLock_Lock(rlock);
    }

    Lock::Lock(Mutex &mutex) : _type(3), _mutex(mutex)
    {
        this->_mutex->Lock();
    }

    Lock Lock::~Lock(void)
    {
        switch (this->_type)
        {
            case 1:
                LightLock_Unlock(this->_llock);
                break;
            case 2:
                RecursiveLock_Unlock(this->_rlock);
                break;
            case 3:
                this->_mutex->Unlock();
                break;
        }
    }
}
