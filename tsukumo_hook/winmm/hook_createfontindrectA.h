#ifndef hook_cfiA_h
#define hook_cfiA_h

#include <Windows.h>
#include "detours.h"
#pragma comment(lib, "detours.lib")

void hook_createfontindirectA_main();

#endif // !hook_cfiA_h