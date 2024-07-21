#include <Windows.h>
#include <Shlwapi.h>
#include <fstream>
#include <iostream>
#include "HOOK_main.h"
#include "hook_createfontindrectA.h"
#include "text_process.h"
#include "hook_LoadLib.h"
#include "readconfig.h"

void CreateConsole()
{
	// �����µĿ���̨
	if (AllocConsole())
	{
		FILE* fp;
		freopen_s(&fp, "CONOUT$", "w", stdout);
		setlocale(LC_CTYPE, "zh-CH");
		// ���ÿ���̨����ҳΪUTF-8
		//_setmode(_fileno(stdout), _O_U16TEXT);
		SetConsoleOutputCP(0);
	}
}

void HOOK_main() {
	rr::RConfig config;
	config.ReadConfig("hook.ini");
	if (config.ReadInt("GLOBAL", "DEBUG", 0) == 1) {
		CreateConsole();
	}

	if (config.ReadInt("GLOBAL", "CHANGECHARSET", 0) == 1) {
		hook_createfontindirectA_main();
	}

	if (config.ReadInt("TEXTPROCESS", "ENABLE", 0) == 1) {
		HMODULE a = GetModuleHandleA("resident.dll");
		if (config.ReadInt("TEXTPROCESS", "MODE", 0) == 1){
			if (a) {
				InstallHook_dumptext();
			}
			else {
				HOOK_LL_main();
			}
		}		
		if (config.ReadInt("TEXTPROCESS", "MODE", 0) == 2) {
			if (a) {
				
			}
			else {
				HOOK_LL_main();
			}
		}
	}
}