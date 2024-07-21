#include <Windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")
#include "hook_createfontindrectA.h"

typedef HFONT(WINAPI* CREATEFONTINDIRECTA)(CONST LOGFONTA* lplf);
CREATEFONTINDIRECTA TrueCreateFontIndirectA = CreateFontIndirectA;


HFONT WINAPI HookedCreateFontIndirectA(CONST LOGFONTA* lplf)
{
   // MessageBoxA(NULL, "HOOKED CFIA!", NULL, NULL);
    LOGFONTA modifiedLf = *lplf;

    // 修改字符集
    modifiedLf.lfCharSet = GB2312_CHARSET; // 或其他你想要的字符集

    // 调用原始函数，但使用修改后的 LOGFONTA
    return TrueCreateFontIndirectA(&modifiedLf);
}

void hook_createfontindirectA_main() {

    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)TrueCreateFontIndirectA, HookedCreateFontIndirectA);

    DetourTransactionCommit();
}