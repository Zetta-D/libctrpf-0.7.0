#include "CTRPluginFramework/System/Clock.hpp"

namespace CTRPluginFramework
{
    Time   GetCurrentTime(void)
    {
        return Ticks(svcGetSystemTick());
    }

    Clock::Clock(void)
    {
        this->_startTime = GetCurrentTime();
    }
    
    Clock::Clock(Time time)
    {
        this->_startTime = time;
    }

    Time    GetElapsedTime(void) const
    {
        Time ct = GetCurrentTime();
        return Ticks(ct.AsTicks() - this->_startTime.AsTicks());
    }

    bool    Clock::HasTimePassed(Time time) const
    {
        return GetElapsedTime().AsTicks() >= time.AsTicks();
    }

    Time    Clock::Restart(void)
    {
        s64 res = this->_startTime.AsTicks() - GetCurrentTime();
        Time t = Ticks(res);
        this->_startTime = res;
        return t;
    }
}
