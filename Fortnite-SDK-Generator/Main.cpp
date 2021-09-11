/*---------------------------------------------------------------------------------------------------*/
/* Basically full credits to KN4CK3R and Guittr14, I used their versions of this generator as a base */
/*---------------------------------------------------------------------------------------------------*/
// + Epic for the UE4 stuff
#pragma once
#include "Global.h"
#include "CoreFunctions.h"
#include "ClassWrapper.h"
#include "ObjectStore.h"
#include "dbg.h"


template<typename T>
bool IsAA(UEObject obj)
{
	if (obj.GetClass().IsValid())
	{
		for (UEClass clss = obj.GetClass(); clss.IsValid(); clss = clss.GetSuper().Cast<UEClass>())
		{
			if (clss == T::StaticClass() && clss.GetFullName().find("Default__") == NPOS)
				return true;
		}
		return false;
	}
	return false;
}

std::string GetWorld()
{
	return "World!";
}

DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	UEObjectStore::Initialize();

	std::cout << "search" << std::endl;
	

	std::cout << std::format("Hello {}", GetWorld()) << std::endl;

	/*
	int32 count = 0;+
	for (auto obj : UEObjectStore())
	{
		if (IsAA<UEClass>(obj))
		{
			int32 classIndex = obj.GetComparisonIndex();

		}
	} 
	std::cout << "nfi" << std::endl;
	*/
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


/*
* Package:
* std::unordered_map<std::vector<std::string>* classMemberNames, std::string className>;
* 
* or rather instead of name strings just the comparison index...
*/