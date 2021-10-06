/*---------------------------------------------------------------------------------------------------*/
/* Basically full credits to KN4CK3R and Guittr14, I used their versions of this generator as a base */
/*---------------------------------------------------------------------------------------------------*/
// + Epic for the UE4 stuff
#pragma once
#include "Global.h"
#include "ObjectStore.h"
#include "Package.h"
#include "Generator.h"

//To do:
/*
* Basic.hpp and Basic.cpp files
*
*/

template<typename T, typename ... Args>
inline void DbgMeasureTime(T* func, Args&& ... params)
{
	auto now = std::chrono::high_resolution_clock::now();

	func(params...);
	
	auto after = std::chrono::high_resolution_clock::now();
	auto something = std::chrono::duration_cast<std::chrono::milliseconds>(after - now);
	std::cout << "Execution took: " << something.count() / 1000 << "s (" << something.count() << "ms)" << std::endl;
}

DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	std::cout << "search" << std::endl;
	
	std::cout << std::format("Hello{:{}X}", 0xFF, 50) << "\n\n\n";



	//Generator::Generate();
	


	std::cout << "nfi" << std::endl;
	
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