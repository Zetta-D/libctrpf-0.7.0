#include "CTRPluginFramework/System/Process.hpp"
#include "CTRPluginFramework/SystemImpl/ProcessImpl.hpp"
#include "CTRPluginFramework/Graphics/OSDImpl.hpp"

namespace CTRPluginFramework
{
    Handle Process::GetHandle(void)
    {
        return ProcessImpl::ProcessHandle;
    }

    u32 Process::GetProcessID(void)
    {
        return ProcessImpl::ProcessId;
    }

    u64 Process::GetTitleID(void)
    {
        return ProcessImpl::TitleId;
    }

    void Process::GetTitleID(std::string &output)
    {
        char *s = new char[0x10u];
        sprintf(s, "%016llX", ProcessImpl::TitleId);
        output = "";
        for(int i = 0; i < 0x10; i++)
            output += s[i];
        delete s;
    }

    void    Process::GetName(std::string &output)
    {
        output = "";
        for(int i = 0; i < 8; i++)
            output += (char)ProcessImpl::CodeSet[80+i];
    }

    u16    Process::GetVersion(void)
    {
        u64 titleId;
        AM_TitleEntry titleInfo;
        if ( AM_GetTitleInfo(MEDIATYPE_GAME_CARD, 1, &titleId, &titleInfo) < 0 )
            return titleInfo.version;
        return 0;
    }
    
    u32    Process::GetTextSize(void)
    {
        return ((ProcessImpl::CodeSet[71] << 24) | (ProcessImpl::CodeSet[70] << 16) | (ProcessImpl::CodeSet[69] << 8) | ProcessImpl::CodeSet[68]) << 12;
    }

    u32    Process::GetRoDataSize(void)
    {
        return ((ProcessImpl::CodeSet[75] << 24) | (ProcessImpl::CodeSet[74] << 16) | (ProcessImpl::CodeSet[73] << 8) | ProcessImpl::CodeSet[72]) << 12;
    }

    u32    Process::GetRwDataSize(void)
    {
        return ((ProcessImpl::CodeSet[79] << 24) | (ProcessImpl::CodeSet[78] << 16) | (ProcessImpl::CodeSet[77] << 8) | ProcessImpl::CodeSet[76]) << 12;
    }

    bool     Process::IsPaused(void)
    {
        return ProcessImpl::IsPaused != 0;
    }

    void    Process::Pause(void)
    {
        ProcessImpl::Pause(0);
    }

    void    Process::Play(const u32 frames)
    {
        if(OnPauseResume)
            OSDImpl::ResumeFrame(frames);
        else
            ProcessImpl::Play(0);
    }

    bool      Process::Patch(u32 addr, void *patch, u32 length, void *original)
    {
        return ProcessImpl::PatchProcess(addr, patch, length, original);
    }

    bool      Process::Patch(u32 addr, u32 patch, void *original)
    {
        return ProcessImpl::PatchProcess(addr, patch, std::piecewise_construct, original);
    }

    bool      Process::ProtectMemory(u32  addr, u32 size, int perm)
    {
        svcControlProcess(ProcessImpl::ProcessHandle, 1, 0, 0);
        return 1;
    }

    bool      Process::ProtectRegion(u32 addr, int perm)
    {
        PageInfo pageInfo;
        MemInfo memInfo;
        if ( svcQueryProcessMemory(&memInfo, &pageInfo, ProcessImpl::ProcessHandle, addr) < 0 )
            return false;
        if ( memInfo.base_addr > addr || base_addr.base_addr < addr )
            return false;
        return Process::ProtectMemory(&memInfo, memInfo.size, perm);
    }

    void    Process::ProtectRegionInRange(u32 startAddress, u32 endAddress, int perm)
    {
        ;
    }

    bool      Process::CopyMemory(void *dst, const void *src, u32 size)
    {
        if(!ProcessImpl::IsValidAddress(&dst) || !!ProcessImpl::IsValidAddress(&src))
        return false;
        svcFlushProcessDataCache(ProcessImpl::ProcessHandle, src, size);
        svcInvalidateProcessDataCache(ProcessImpl::ProcessHandle, dst, size);
        memcpy((void *)dst, (void *)src, (size_t)size);
        svcFlushProcessDataCache(ProcessImpl::ProcessHandle, dst, size);
        return true;
    }

    bool      Process::CheckAddress(u32 address, u32 perm)
    {
        return ProcessImpl::IsValidAddress(address, perm);
    }

    bool      Process::CheckRegion(u32 address, u32& size, u32 perm)
    {
        ProcessImpl::GetMemRegion(address, perm);
        if ( address == ProcessImpl::InvalidRegion[0] )
            return ProcessImpl::InvalidRegion[1] != size;
        return true;
    }

    u32    Process::GetFreeMemoryRegion(u32 size, u32 startAddress = 0x100000)
    {
        return  ProcessImpl::GetFreeMemoryRegion(size, startAddress);
    }

    bool   Process::Write64(u32 address, u64 value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            *(u64*)address = value;
            return true;
        }
        return false
    }

    bool   Process::Write32(u32 address, u32 value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            *(u32*)address = value;
            return true;
        }
        return false
    }

    bool   Process::Write16(u32 address, u16 value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            *(u16*)address = value;
            return true;
        }
        return false
    }

    bool   Process::Write8(u32 address, u8 value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            *(u8*)address = value;
            return true;
        }
        return false
    }

    bool   Process::WriteFloat(u32 address, float value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            *(float*)address = value;
            return true;
        }
        return false
    }

    bool   Process::WriteDouble(u32 address, double value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            *(double*)address = value;
            return true;
        }
        return false
    }

    bool   Process::Read64(u32 address, u64 &value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            value = *(u64*)address;
            return true;
        }
        return false
    }

    bool   Process::Read32(u32 address, u32 &value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            value = *(u32*)address;
            return true;
        }
        return false
    }

    bool   Process::Read16(u32 address, u16 &value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            value = *(u16*)address;
            return true;
        }
        return false
    }

    bool   Process::Read8(u32 address, u8 &value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            value = *(u8*)address;
            return true;
        }
        return false
    }

    bool   Process::ReadFloat(u32 address, float &value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            value = *(float*)address;
            return true;
        }
        return false
    }

    bool   Process::ReadDouble(u32 address, double &value)
    {
        if(ProcessImpl::CheckAddress(address))
        {
            value = *(double*)address;
            return true;
        }
        return false
    }

    // This is not the original function
    bool    Process::ReadString(u32 address, std::string &output, u32 size, StringFormat format)
    {
        if ( !Process::CheckAddress(address, 1))
            return false;

        output = "";

        if(format == StringFormat::Utf8)
        {
            u8 data[size];
            for(size_t i = 0; i < size; i++)
                Process::Read8(address + i, data[i]);

            for(size_t i = 0; i < size; i++)
                output += *(char*)(data+i);
            return true;
        }
        else if(format == StringFormat::Utf16)
        {
            u16 data[size];
            for(size_t i = 0; i < size; i++)
                Process::Read16(address + i*2, data[i]);

            const u16* str16 = reinterpret_cast<const u16*>(data);
            u8 *str = new u8[size*2];

            if(utf16_to_utf8(str, str16, size) =! -1)
            {
                for(size_t i = 0; i < size*2; i++)
                    output += *(char*)(str+i);
                return true;
            }
        }
        else
        {
            u32 data[size];
            for(size_t i = 0; i < size; i++)
                Process::Read32(address + i*4, data[i]);

            const u32* str32 = reinterpret_cast<const u32*>(data);
            u8 *str = new u8[size*2];

            if(utf32_to_utf8(str, str32, size) =! -1)
            {
                for(size_t i = 0; i < size*4; i++)
                    output += *(char*)(str+i);
                return true;
            }
        }

        return false;
    }

    // This is not the original function
    bool    ConvertString(u32 address, const char* input, size_t size, StringFormat format)
    {
        const u8* str8 = reinterpret_cast<const u8*>(input);

        if(format == StringFormat::Utf16)
        {
            u16 *str = new u16[size];
            u16 *addr = (u16*)(address);
            if(utf8_to_utf16(str, str8, size) != -1)
            {
                for(size_t i = 0; i < size; i++)
                    *(addr+i) = *(str+i);
                return true;
            }
        }
        else
        {
            u32 *str = new u32[size];
            u32 *addr = (u32*)(address);
            if(utf8_to_utf32(str, str8, size) != -1)
            {
                for(size_t i = 0; i < size; i++)
                    *(addr+i) = *(str+i);
                return true;
            }
        }
        return false;
    }

    // This is not the original function
    bool   Process::WriteString(u32 address, const std::string &input, StringFormat outFmt = StringFormat::Utf8)
    {
        if ( !Process::CheckAddress(address, 3) || input.empty() )
            return false;

        u32 offset = 0;
        bool res = false;

        if (outFmt == StringFormat::Utf8)
        {
            for ( std::string::iterator it = input.begin(); it != input.end(); ++it)
            {
                *(char*)(address + offset) = *it;
                offset++;
                ++it;
            }
            *(char*)(address + offset) = 0;
            res = true;
        }
        else if (outFmt == StringFormat::Utf16)
        {
            res = ConvertString(address, input.c_str(), input.size(), StringFormat::Utf16);
        }
        else
        {
            res = ConvertString(address, input.c_str(), input.size(), StringFormat::Utf32);
        }

        return res;
    }

    void   Process::ReturnToHomeMenu(void);
    {
        APT_PrepareToCloseApplication(1);
        APT_CloseApplication(0, 0, 0);
        ProcessImpl::UnlockGameThreads();
        svcExitProcess();
    }
}
