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
}

DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	UEObjectStore::Initialize();

	std::cout << "search" << std::endl;
	
	int32 count = 0;
	for (auto obj : UEObjectStore())
	{
		if (IsAA<UE_ArrayProperty>(obj))
		{
			if (obj.GetFullName().find("Default__") != NPOS)
				continue;
			std::cout << obj.GetFullName() << std::endl;
			std::cout << UE_ArrayProperty(obj.object).GetTypeStr() << std::endl;
			count++;
		}
	} 
	std::cout << count << std::endl;
	std::cout << "nfi" << std::endl;

	/*
	UEObjectStore::Initialize();

	std::ofstream stream("PrefixedNames.txt");

	for (auto object : UEObjectStore())
	{
		stream << object.GetPrefixedName() << "\n";
	}
	std::cout << "done" << std::endl;
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