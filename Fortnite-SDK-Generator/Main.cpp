/*---------------------------------------------------------------------------------------------------*/
/* Basically full credits to KN4CK3R and Guittr14, I used their versions of this generator as a base */
/*---------------------------------------------------------------------------------------------------*/
// + Epic for the UE4 stuff
#pragma once
#include "Global.h"
#include "CoreFunctions.h"
#include "ClassWrapper.h"
#include "ObjectStore.h"
#include "Package.h"


std::string GetWorld()
{
	return "World";
}


DWORD WINAPI Main(LPVOID lpParam)
{
	AllocConsole();
	FILE* p;
	freopen_s(&p, "CONOUT$", "w", stdout);
	freopen_s(&p, "CONIN$", "r", stdin);

	UEObjectStore::Initialize();

	std::cout << "search" << std::endl;
	
	std::cout << std::format("Hello{:{}X}", 0xFF, 50) << "\n\n\n";


	
	for (auto obj : UEObjectStore())
	{

		if (obj.IsA(UEEnum::StaticClass()))
		{
			Package pack(obj.GetPackage());

			Package::Enum myEnum = pack.GenerateEnumClass(UEEnum(obj.GetUObject()));

			std::cout << std::format("//{}\n{} : {}\n", myEnum.fullName, myEnum.name, myEnum.underlayingType);
			std::cout << "{\n";

			for (int i = 0; i < myEnum.members.size(); i++)
			{
				std::cout << std::format("\t{:{}}= {},", myEnum.members[i], 45, i) << std::endl;
			}
			std::cout << "};\n\n";
		}
		

		/*
		if (obj.GetFullName() == "Class Engine.ExponentialHeightFogComponent")
		{
			UEClass casted = obj.Cast<UEClass>();

			std::cout << "//" << obj.GetFullName() << std::endl;
			std::cout << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})", casted.GetStructSize() - casted.GetSuper().GetStructSize(), casted.GetStructSize(), casted.GetSuper().GetStructSize()) << std::endl;

			if (casted.GetSuper().IsValid())
				std::cout << std::format("class {} : {}", casted.GetUniqueName(), casted.GetSuper().GetCppName()) << std::endl;
			else
				std::cout << std::format("class {}", casted.GetUniqueName()) << std::endl;

			std::cout << "{" << std::endl;


			for (UEProperty prop = casted.GetChildren().Cast<UEProperty>(); prop.IsValid(); prop = prop.GetNext().Cast<UEProperty>())
			{
				std::cout << std::format("\t{:{}}\t\t{:{}}\t\t//0x{:04X}(0x{:04X})", prop.GetPropertyType().second, 35, prop.GetName(), 33, prop.GetOffset(), prop.GetElementSize()) << std::endl;
				//std::cout << std::format("\t{:{}}{}", "Hello", 35, "World!") << std::endl;
			}

			//std::cout << std::format("Hello{:{}X}", 0xFF, 50) << "\n\n\n";

			std::cout << "};" << std::endl;
			break;
		}
		*/
	}
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