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


DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	std::cout << "search" << std::endl;
	
	std::cout << std::format("Hello{:{}X}", 0xFF, 50) << "\n\n\n";

	Generator::Generate();
	
	std::unordered_map<int32, std::vector<int32>> map;

	for (auto obj : UEObjectStore())
	{
		if (!obj.IsValid())
		{
			continue;
		}

		if (!obj.IsA(UEPackage::StaticClass()))
		{
			if(obj.IsA(UEClass::StaticClass()) || obj.IsA(UEEnum::StaticClass()) || obj.IsA(UEStruct::StaticClass()))
			{
				map[obj.GetPackage().GetInernalIndex()].push_back(obj.GetInernalIndex());
			}
		}
	}

	std::cout << "Package count: " << map.size() << std::endl;
	Sleep(3000);

	int32 index = UEObjectStore::FindClass("Package FortniteGame").GetInernalIndex();

	std::cout << "FortniteGame: " << map[index].size() << std::endl;

	Sleep(4000);

	for (auto pair : map)
	{
		std::cout << "Package: " << UEObjectStore::StaticGetByIndex(pair.first).GetFullName() << "\nMemberCount: " << pair.second.size() << "\n\n";
	}


	UEObjectStore::GetAllPackages(map);

	std::cout << map.size() << std::endl;
	
	
	/*
	{
		std::cout << "\n\nStrting measurment now" << std::endl;
		auto now = std::chrono::high_resolution_clock::now();

		for (int i = 0; i < 5000; i++)
		{
			std::cout << std::format("Hallo {}\n", i);
		}

		auto after = std::chrono::high_resolution_clock::now();
		auto something = std::chrono::duration_cast<std::chrono::milliseconds>(after - now);
		std::cout << "Execution took: " << something.count() / 1000 << "s (" << something.count() << "ms)" << std::endl;

	}
	*/

	/*
	int ddlSoup = 342;
	int count = 0;
	const double percent = static_cast<double>(ddlSoup) / 100;

	for (int i = 0; i < 342; i++)
	{
		std::cout << std::format("I: {}\nSinglePercent: {}\nPercent: {:.0f}\n", i, percent, i / percent);
	}
	*/

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