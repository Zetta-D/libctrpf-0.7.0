#include "3ds.h"
#include "plgldr.h"
#include "csvc.h"
#include <string.h>

static Handle   plgLdrHandle;
static Handle   plgLdrArbiter;
static s32*     plgEvent;
static s32*     plgReply;
static int      plgLdrRefCount;
static bool     isN3DS;
static OnPlgLdrEventCb_t    onPlgLdrEventCb = NULL;

static inline u32 IPC_MakeHeader(u16 command_id, unsigned normal_params, unsigned translate_params)
{
    return ((u32) command_id << 16) | (((u32) normal_params & 0x3F) << 6) | (((u32) translate_params & 0x3F) << 0);
}

static inline u32 IPC_Desc_Buffer(size_t size, IPC_BufferRights rights)
{
    return (size << 4) | 0x8 | rights;
}

static inline void* getThreadLocalStorage(void)
{
    void* ret;
    __asm__ ("mrc p15, 0, %[data], c13, c0, 3" : [data] "=r" (ret));
    return ret;
}

static inline u32* getThreadCommandBuffer(void)
{
    return (u32*)((u8*)getThreadLocalStorage() + 0x80);
}

static inline s32 __ldrex(s32* addr)
{
    s32 val;
    __asm__ __volatile__("ldrex %[val], %[addr]" : [val] "=r" (val) : [addr] "Q" (*addr));
    return val;
}

static inline bool __strex(s32* addr, s32 val)
{
    bool res;
    __asm__ __volatile__("strex %[res], %[val], %[addr]" : [res] "=&r" (res) : [val] "r" (val), [addr] "Q" (*addr));
    return res;
}

static Result   PLGLDR__GetArbiter(void)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(9, 0, 0);
    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
        plgLdrArbiter = cmdbuf[3];
    }
    return res;
}

Result  plgLdrInit(void)
{
    s64    out = 0;
    Result res = 0;

    svcGetSystemInfo(&out, 0x10000, 0x201);
    isN3DS = out != 0;
    if (AtomicPostIncrement(&plgLdrRefCount) == 0)
        res = svcConnectToPort(&plgLdrHandle, "plg:ldr");

    if (R_SUCCEEDED(res)
       && R_SUCCEEDED((res = PLGLDR__GetArbiter())))
    {
        PluginHeader *header = (PluginHeader *)0x07000000;

        plgEvent = header->plgldrEvent;
        plgReply = header->plgldrReply;
    }
    else
        plgLdrExit();

    return res;
}

void    plgLdrExit(void)
{
    if (AtomicDecrement(&plgLdrRefCount))
        return;

    if (plgLdrHandle)
        svcCloseHandle(plgLdrHandle);
    if (plgLdrArbiter)
        svcCloseHandle(plgLdrArbiter);
    plgLdrHandle = plgLdrArbiter = 0;
}

Result  PLGLDR__IsPluginLoaderEnabled(bool *isEnabled)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(2, 0, 0);
    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
        *isEnabled = cmdbuf[2];
    }
    return res;
}

Result  PLGLDR__SetPluginLoaderState(bool enabled)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(3, 1, 0);
    cmdbuf[1] = (u32)enabled;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    return res;
}

Result  PLGLDR__SetPluginLoadParameters(PluginLoadParameters *parameters)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(4, 2, 4);
    cmdbuf[1] = (u32)parameters->noFlash;
    cmdbuf[2] = parameters->lowTitleId;
    cmdbuf[3] = IPC_Desc_Buffer(256, IPC_BUFFER_R);
    cmdbuf[4] = (u32)parameters->path;
    cmdbuf[5] = IPC_Desc_Buffer(32 * sizeof(u32), IPC_BUFFER_R);
    cmdbuf[6] = (u32)parameters->config;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    return res;
}

void     Flash(u32 color);

Result  PLGLDR__DisplayMenu(PluginMenu *menu)
{
    Result res = 0;

    u32 nbItems = menu->nbItems;
    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(5, 1, 8);
    cmdbuf[1] = nbItems;
    cmdbuf[2] = IPC_Desc_Buffer(nbItems, IPC_BUFFER_RW);
    cmdbuf[3] = (u32)menu->states;
    cmdbuf[4] = IPC_Desc_Buffer(MAX_BUFFER, IPC_BUFFER_R);
    cmdbuf[5] = (u32)menu->title;
    cmdbuf[6] = IPC_Desc_Buffer(MAX_BUFFER * nbItems, IPC_BUFFER_R);
    cmdbuf[7] = (u32)menu->items;
    cmdbuf[8] = IPC_Desc_Buffer(MAX_BUFFER * nbItems, IPC_BUFFER_R);
    cmdbuf[9] = (u32)menu->hints;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    else Flash(0xFF0000);

    return res;
}

Result  PLGLDR__DisplayMessage(const char *title, const char *body)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(6, 0, 4);
    cmdbuf[1] = IPC_Desc_Buffer(strlen(title), IPC_BUFFER_R);
    cmdbuf[2] = (u32)title;
    cmdbuf[3] = IPC_Desc_Buffer(strlen(body), IPC_BUFFER_R);
    cmdbuf[4] = (u32)body;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    return res;
}

Result  PLGLDR__DisplayErrMessage(const char *title, const char *body, u32 error)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(7, 1, 4);
    cmdbuf[1] = error;
    cmdbuf[2] = IPC_Desc_Buffer(strlen(title), IPC_BUFFER_R);
    cmdbuf[3] = (u32)title;
    cmdbuf[4] = IPC_Desc_Buffer(strlen(body), IPC_BUFFER_R);
    cmdbuf[5] = (u32)body;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    return res;
}

Result  PLGLDR__GetVersion(u32 *version)
{
    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(8, 0, 0);

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        if (cmdbuf[0] != IPC_MakeHeader(8, 2, 0))
            return 0xD900182F;

        res = cmdbuf[1];
        if (version)
            *version = cmdbuf[2];
    }
    return res;
}

Result  PLGLDR__GetPluginPath(char *path)
{
    if (path == NULL)
        return MAKERESULT(28, 7, 254, 1014);

    Result res = 0;

    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(10, 0, 2);
    cmdbuf[1] = IPC_Desc_Buffer(255, IPC_BUFFER_RW);
    cmdbuf[2] = (u32)path;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    return res;
}

Result  PLGLDR__GetMenuOpenBlock(u32 *menu)
{
    u32 *cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(11, 0, 0);

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
        *menu = cmdbuf[2] | (1 << 31);
    }
    return res;
}

Result  PLGLDR__SetSwapSettings(char* swapPath, void* encFunc, void* decFunc, void* args)
{
    Result res = 0;

    const char* trueSwapPath;
    if (swapPath) trueSwapPath = swapPath;
    else trueSwapPath = "\0";

    u32 buf[4] = { 0 };
    u32* trueArgs;
    if (args) trueArgs = args;
    else trueArgs = buf;

    u32* cmdbuf = getThreadCommandBuffer();

    cmdbuf[0] = IPC_MakeHeader(12, 2, 4);
    cmdbuf[1] = (encFunc) ? svcConvertVAToPA(encFunc, false) | (1 << 31) : 0;
    cmdbuf[2] = (decFunc) ? svcConvertVAToPA(decFunc, false) | (1 << 31) : 0;
    cmdbuf[3] = IPC_Desc_Buffer(4 * sizeof(u32), IPC_BUFFER_R);
    cmdbuf[4] = (u32)trueArgs;
    cmdbuf[5] = IPC_Desc_Buffer(strlen(trueSwapPath) + 1, IPC_BUFFER_R);
    cmdbuf[6] = (u32)trueSwapPath;

    if (R_SUCCEEDED((res = svcSendSyncRequest(plgLdrHandle))))
    {
        res = cmdbuf[1];
    }
    return res;
}

void    PLGLDR__SetEventCallback(OnPlgLdrEventCb_t cb)
{
    onPlgLdrEventCb = cb;
}

static s32      __ldrex__(s32 *addr)
{
    s32 val;
    do
        val = __ldrex(addr);
    while (__strex(addr, val));

    return val;
}

static void     __strex__(s32 *addr, s32 val)
{
    do
        __ldrex(addr);
    while (__strex(addr, val));
}

void    PLGLDR__Status(void) { }

s32     PLGLDR__FetchEvent()
{
  return __ldrex__(plgEvent);
}

void    PLGLDR__Reply(s32 value)
{
    __strex__(plgReply, 0);
    if ( value <= 2 )
        return __strex__(plgEvent, 0);
    svcArbitrateAddress(plgLdrArbiter, plgReply, 0, 1, 0, 0);
    if ( value == 3 )
    {
        __strex__(plgEvent, 0xFFFFFFFF);
        svcArbitrateAddress(plgLdrArbiter, plgEvent, 1, 0, 0, 0);
    }
    else if ( value == 4 )
    {
        svcExitThread();
    }
}
