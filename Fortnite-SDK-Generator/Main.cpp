#pragma once
#include <Windows.h>
#include <iostream>
#include "Global.h"
#include "CoreFunctions.h"

/*
* ToDo:
* 
* iterator for ObjectStore
* 
* Generator
*/


DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	GObjects = reinterpret_cast<FUObjectArray*>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::GObjects); //1.2


	return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, Main, hModule, 0, 0);
	}

	return TRUE;
}