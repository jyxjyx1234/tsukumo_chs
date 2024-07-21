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

    // �޸��ַ���
    modifiedLf.lfCharSet = GB2312_CHARSET; // ����������Ҫ���ַ���

    // ����ԭʼ��������ʹ���޸ĺ�� LOGFONTA
    return TrueCreateFontIndirectA(&modifiedLf);
}

void hook_createfontindirectA_main() {

    DetourTransactionBegin();

    DetourUpdateThread(GetCurrentThread());

    DetourAttach(&(PVOID&)TrueCreateFontIndirectA, HookedCreateFontIndirectA);

    DetourTransactionCommit();
}