#include <windows.h>
#include <fstream>
#include "text_process.h"

DWORD originalFuncAddr;
DWORD returnAddress;

void WriteToFile(const char* text, bool addAtSign)
{
    std::ofstream file("output.txt", std::ios::app);
    if (file.is_open())
    {
        if (addAtSign)
            file << "@";
        file << text << std::endl;
        file.close();
    }
}

void __declspec(naked) HookFunction_dumptext()
{
    __asm
    {
        // ����Ĵ���
        pushad
        pushfd

        // ��ȡesp+4����ָ��
        mov eax, [esp + 0x28]  

        // ���ebp�Ƿ�Ϊ0
        test ebp, ebp
        jnz non_zero_ebp

        // ebpΪ0�����
        push 1  // addAtSign = true
        push eax
        call WriteToFile
        add esp, 8  // �����ջ
        jmp end_write

        non_zero_ebp :
        // ebp��Ϊ0�����
        push 0  // addAtSign = false
            push eax
            call WriteToFile
            add esp, 8  // �����ջ

            end_write :
            // �ָ��Ĵ���
            popfd
            popad

        // ִ��ԭʼָ��
        sub esp, 8
        push ebx
        push ebp

        

        // ����ԭ����
        jmp dword ptr[returnAddress]
    }
}

void InstallHook_dumptext()
{
    DWORD oldProtect;

    DWORD baseAddress = (DWORD)GetModuleHandle(L"resident.dll");

    originalFuncAddr = baseAddress+0x4B7F0;  // resident.dll��ַ + 0x4B7F0
    returnAddress = baseAddress+0x4B7F5;  // resident.dll��ַ + 0x4B7F0

    // �޸��ڴ汣��
    VirtualProtect((LPVOID)originalFuncAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    // д����תָ��
    *(BYTE*)originalFuncAddr = 0xE9;  // JMP
    *(DWORD*)(originalFuncAddr + 1) = (DWORD)HookFunction_dumptext - originalFuncAddr - 5;

    // �ָ��ڴ汣��
    VirtualProtect((LPVOID)originalFuncAddr, 5, oldProtect, &oldProtect);
}