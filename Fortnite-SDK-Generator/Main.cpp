/*---------------------------------------------------------------------------------------------------*/
/* Basically full credits to KN4CK3R and Guittr14, I used their versions of this generator as a base */
/*---------------------------------------------------------------------------------------------------*/
// + Epic for the UE4 stuff
#pragma once
#include "Global.h"
#include "ObjectStore.h"
#include "Package.h"
#include "Generator.h"

//Knows bugs:
/*
* Too much classess/structs in the FortniteGame package
*
*/

DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	UEObjectStore::Initialize();

	std::cout << "search" << std::endl;
	
	std::cout << std::format("Hello{:{}X}", 0xFF, 50) << "\n\n\n";

	/*Generator sdkGen;
	sdkGen.Generate();*/

	std::vector<UEObject> outPackages;

	UEObjectStore::GetAllPackages(outPackages);

	std::cout << "Num: " << outPackages.size() << std::endl;

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


/*
* Package:
* std::unordered_map<std::vector<std::string>* classMemberNames, std::string className>;
* 
* or rather instead of name strings just the comparison index...
*/