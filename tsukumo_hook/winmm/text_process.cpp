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
        // 保存寄存器
        pushad
        pushfd

        // 获取esp+4处的指针
        mov eax, [esp + 0x28]  

        // 检查ebp是否为0
        test ebp, ebp
        jnz non_zero_ebp

        // ebp为0的情况
        push 1  // addAtSign = true
        push eax
        call WriteToFile
        add esp, 8  // 清理堆栈
        jmp end_write

        non_zero_ebp :
        // ebp不为0的情况
        push 0  // addAtSign = false
            push eax
            call WriteToFile
            add esp, 8  // 清理堆栈

            end_write :
            // 恢复寄存器
            popfd
            popad

        // 执行原始指令
        sub esp, 8
        push ebx
        push ebp

        

        // 跳回原函数
        jmp dword ptr[returnAddress]
    }
}

void InstallHook_dumptext()
{
    DWORD oldProtect;

    DWORD baseAddress = (DWORD)GetModuleHandle(L"resident.dll");

    originalFuncAddr = baseAddress+0x4B7F0;  // resident.dll基址 + 0x4B7F0
    returnAddress = baseAddress+0x4B7F5;  // resident.dll基址 + 0x4B7F0

    // 修改内存保护
    VirtualProtect((LPVOID)originalFuncAddr, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    // 写入跳转指令
    *(BYTE*)originalFuncAddr = 0xE9;  // JMP
    *(DWORD*)(originalFuncAddr + 1) = (DWORD)HookFunction_dumptext - originalFuncAddr - 5;

    // 恢复内存保护
    VirtualProtect((LPVOID)originalFuncAddr, 5, oldProtect, &oldProtect);
}