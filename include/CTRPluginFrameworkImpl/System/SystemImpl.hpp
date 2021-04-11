#ifndef CTRPLUGINFRAMEWORKIMPL_SYSTEM_SYSTEMIMPL_HPP
#define CTRPLUGINFRAMEWORKIMPL_SYSTEM_SYSTEMIMPL_HPP

#include "3ds.h"
#include "types.h"

namespace CTRPluginFramework
{
    class System
    {
    public:

        static Result Initialize(void);
        static bool   WantsToSleep(void);
        static void   ReadyToSleep(void);
        static void   WakeUpFromSleep(void);
        static bool   Status(void);

        static u8         Language;
        static bool       IsNew3DS;
        static u32        CFWVersion, RosalinaHotkey, AptStatus;

    };
}

#endif
