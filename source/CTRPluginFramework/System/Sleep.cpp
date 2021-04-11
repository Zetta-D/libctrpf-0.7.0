#include "CTRPluginFramework/System/Sleep.hpp"
#include "CTRPluginFramework/System/Time.hpp"
#include "3ds.h"

namespace CTRPluginFramework
{
    void   Sleep(Time sleepTime)
    {
        if(sleepTime > Time::Zero)
        {
            svcSleepThread(1000 * sleepTime.AsMicroseconds());
        }
    }
}
