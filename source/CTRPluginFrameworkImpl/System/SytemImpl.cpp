#include "CTRPluginFrameworkImpl/System/SystemImpl.hpp"

namespace CTRPluginFramework
{
    LightEvent g_sleepEvent;
    LanguageId SystemImpl::Language;
    bool SystemImpl::IsNew3DS;
    u32 SystemImpl::CFWVersion, SystemImpl::RosalinaHotkey, SystemImpl::AptStatus;

    Result SystemImpl::Initialize(void)
    {
        s64 info;
        LightEvent_Init(&g_sleepEvent, RESET_STICKY);
        IsNew3DS = svcGetSystemInfo(&info, 0x10001, 1) >= 0;
        if ( svcGetSystemInfo(&info, 0x10000, 0) >= 0 )
        {
            CFWVersion = (u32)info;
            if (svcGetSystemInfo(&info, 0x10000, 0x101) >= 0)
                RosalinaHotkey = (u32)info;
        }
        else
            CFWVersion = 0;
        return CFGU_GetSystemLanguage(&Language);
    }

    bool    SystemImpl::WantsToSleep(void)
    {
        return (AptStatus & 0x40) != 0;
    }

    void    SystemImpl::ReadyToSleep(void)
    {
        if ( AptStatus & 0x40 )
        {
            AptStatus &= 0xFFFFFFBF;
            LightEvent_Clear(&g_sleepEvent);
            AptStatus |= 0x80;
        }
    }

    void    SystemImpl::WakeUpFromSleep(void)
    {
        AptStatus &= 0xFFFFFFBF;
        AptStatus &= 0xFFFFFF7F;
        LightEvent_Signal(&g_sleepEvent);
    }

    bool    SystemImpl::Status(void)
    {
        if (AptStatus & 8)
            return true;
        if (AptStatus & 0x80)
            LightEvent_Wait(&g_sleepEvent);
        return false;
    }
}
