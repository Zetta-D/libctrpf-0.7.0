#include "CTRPluginFramework/System/Hook.hpp"

namespace CTRPluginFramework
{
    Hook::Hook(void)
    {   
        this->_ctx->refcount          = 1;
        this->_ctx->flags             = 3;
        this->_ctx->targetAddress     = 0;
        this->_ctx->returnAddress     = 0;
        this->_ctx->callbackAddress   = 0;
        this->_ctx->callbackAddress2  = 0;
        this->_ctx->overwrittenInstr  = 0;
        this->_ctx->index             = 0;
    }

    Hook&   Hook::Initialize(u32 targetAddr, u32 callbackAddr)
    {
        this->_ctx->targetAddress     = targetAddr;
        this->_ctx->callbackAddress   = callbackAddr;
        this->_ctx->returnAddress     = targetAddr + 4;
    }

    Hook&   Hook::InitializeForMitm(u32 targetAddr, u32 callbackAddr)
    {
        this->_ctx->targetAddress     = targetAddr;
        this->_ctx->callbackAddress   = callbackAddr;
        this->_ctx->returnAddress     = targetAddr + 4;
        this->_ctx->flags             = 8;
    }

    Hook&   Hook::InitializeForSubWrap(u32 targetAddr, u32 beforeCallback, u32 afterCallback)
    {
        this->_ctx->targetAddress     = targetAddr;
        this->_ctx->callbackAddress   = callbackAddr;
        this->_ctx->returnAddress     = targetAddr + 4;
        this->_ctx->callbackAddress2  = afterCallback;
        this->_ctx->flags             = 16;
    }

    Hook&   Hook::SetFlags(u32 flags)
    {
        this->_ctx->flags = flags;
    }

    Hook&   SetReturnAddress(u32 returnAddr)
    {
        this->_ctx->returnAddress = returnAddr;
    }

    bool   Hook::IsEnabled(void)
    {
        return this->_ctx->refcount && this->_ctx->index != -1;
    }

    bool  IsTargetAlreadyHooked(u32 targetAddr, u32 overwrittenInstr)
    {
        if ( overwrittenInstr >> 24 == 234 )
            return (overwrittenInstr & 0xFFFFFF) >= (0x1E7FFF8 - targetAddr) >> 2 && (overwrittenInstr & 0xFFFFFF) < (0x1E80FF8 - targetAddr) >> 2;
        return false;
    }

    /*
    bool  IsInstructionPCDependant(u32 overwrittenInstr)
    {
        static _UNKNOWN forbiddenInstructions;
        u32 ( *i)(u32); 
        for ( i = (u32 ( *)(u32))&forbiddenInstructions; i != IsInstructionPCDependant; i = (u32 ( *)(u32))((char *)i + 4) )
        {
            if ( overwrittenInstr >> 16 == *(u32 *)i )
                return true;
        }
        return false;
    }
    */

    HookResult  Hook::Enable(void)
    {
        if ( !this->_ctx->refcount )
            return HookResult::InvalidContext;
        if ( Hook::IsEnabled() )
            return HookResult::Success;

        Lock::Lock(HookManager::GetLock(0));
        if ( this->_ctx->flags & 2 && this->_ctx->flags & 4 )
            return HookResult::HookParamsError;
        else if ( !Process::CheckAdress(this->_ctx->targetAddress, 3) )
            return HookResult::InvalidAddress;
        else
        {
            this->_ctx->overwrittenInstr = this->_ctx->targetAddr;
            if ( IsTargetAlreadyHooked(this->_ctx->targetAddress, this->_ctx->overwrittenInstr) )
                return HookResult::AddressAlreadyHooked;
            else if ( (this->_ctx->flags & 2 || this->_ctx->flags & 4) && IsInstructionPCDependant(this->_ctx->overwrittenInstr) )
                return HookResult::TargetInstructionCannotBeHandledAutomatically;
            else if ( this->_ctx->flags & 0x10 && this->_ctx->overwrittenInstr >> 24 != 235 )
                return HookResult::HookParamsError;
            else
                return HookManager::ApplyHook(this->_ctx);
        }
        Lock::~Lock();
        return HookResult::HookParamsError;
    }

    HookResult  Hook::Disable(void)
    {
        if ( !this->_ctx->refcount )
            return 1;
        if ( !Hook::IsEnabled() )
            return 0;
        return HookManager::DisableHook(this->_ctx);
    }

    const HookContext& Hook::GetContext(void)
    {
        return this->_ctx;
    }
}
