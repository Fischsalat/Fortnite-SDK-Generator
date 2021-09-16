#pragma once
#include "Generator.h"


void Generator::ProcessPackages(const fs::path& sdkPath)
{
	std::vector<UEObject> packageObjects;
	UEObjectStore::GetAllPackages(packageObjects);

	for (auto packageObj : packageObjects)
	{
		Package pack(packageObj);

		pack.Process(packageObj);
	}
}

void Generator::PrintFileHeader(std::ofstream& stream, const Generator::FileType& ft) const
{
	stream << std::format("#pragma once\n\n//Fortnite {}\n\n", Settings::GetGameVersion());

	stream << "#ifdef _MSC_VER\n\t#pragma pack(push, 0x8)\n#endif\n\n";

	if (ft == FileType::Function && Settings::ShouldUsePrecompiledHeaders())
		stream << "#include \"pch.h\"\n#include \"../pch.h\"\n\n";
	else if (ft == FileType::Function)
		stream << "#include \"../SDK.h\"\n\n";

	if (Settings::ShouldUseNamespaceForSDK)
		stream << "namespace " << Settings::GetSDKNamespace() << "\n{\n";

	if (ft == FileType::Function && Settings::ShouldUseNamespaceForParams())
		stream << "namespace " << Settings::GetParamNamespace() << "\n{\n";	
}

void Generator::PrintFileEnding(std::ofstream& stream, const Generator::FileType& ft) const
{
	if (ft == FileType::Function && Settings::ShouldUseNamespaceForParams())
		stream << "\n\n}\n";

	if (Settings::ShouldUseNamespaceForSDK())
		stream << "\n}\n\n";

	stream << "#ifdef _MSC_VER\n\t#pragma pack(pop)\n#endif\n";
}

void Generator::GenerateStructsFile(std::ofstream& stream, const std::vector<Package::Struct>& structs, const std::vector<Package::Enum>& enums) const
{
	PrintFileHeader(stream, FileType::Struct);

	for (auto enumClass : enums)
	{
		stream << std::format("//{}\nenum class {} : {}\n", enumClass.fullName, enumClass.name, enumClass.underlayingType);
		stream << "{\n";

		for (int i = 0; i < enumClass.members.size(); i++)
		{
			if (i != enumClass.members.size() - 1)
				std::cout << std::format("\t{:{}} = {},", enumClass.members[i], 30);
			else
				std::cout << std::format("\t{:{}} = {}", enumClass.members[i], 30);
		}
		stream << "};\n\n\n";
	}

	stream << "\n\n";

	for (auto scriptStruct : structs)
	{
		stream << std::format("//{}\n", scriptStruct.fullName);

		if (scriptStruct.inheritedSize == 0)
			stream << std::format("//0x{:04X}", scriptStruct.structSize);
		else
			stream << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})", scriptStruct.structSize - scriptStruct.inheritedSize, scriptStruct.structSize, scriptStruct.inheritedSize);

		stream << scriptStruct.cppFullName << "\n{\n";

		for (auto member : scriptStruct.members)
		{
			stream << std::format("\t{:{}}{:{}}//{}({})", member.type, 50, member.name += ";", 100, member.offset, member.size);
		}
		
		stream << "};\n\n\n";
	}

	PrintFileEnding(stream, FileType::Struct);
}

void Generator::GenerateParameterFile(std::ofstream& stream, const std::vector<Package::Function>& parameters) const
{
	PrintFileHeader(stream, FileType::Parameter);

	for (auto parm : parameters)
	{

		stream << std::format("//{}\n", parm.fullName);

		if (parm.selfAsStruct.inheritedSize == 0)
			stream << std::format("//0x{:04X}", parm.selfAsStruct.structSize);
		else
			stream << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})", parm.selfAsStruct.structSize - parm.selfAsStruct.inheritedSize, parm.selfAsStruct.structSize, parm.selfAsStruct.inheritedSize);

		stream << parm.paramName << "\n{\n";

		for (auto member : parm.selfAsStruct.members)
		{
			stream << std::format("\t{:{}}{:{}}//{}({})", member.type, 50, member.name += ";", 100, member.offset, member.size);
		}

		stream << "};\n\n\n";
	}

	PrintFileEnding(stream, FileType::Parameter);
}

void Generator::GenerateFunctionFile(std::ofstream& stream, const std::vector<Package::Function>& functions) const
{
	PrintFileHeader(stream, FileType::Function);

	for (auto func : functions)
	{
		stream << std::format("//{}\n\n{}\n", func.fullName, func.cppFullName);
		stream << "{\n";
		stream << std::format("\tstatic auto fn = UObject::FindObject<UFunction>(\"{}\");\n\n", func.fullName);
		
		if (Settings::ShouldUseNamespaceForParams())
			stream << Settings::GetParamNamespace() << "::";

		stream << func.paramName; //finish writing this please
	}

	PrintFileEnding(stream, FileType::Function);
}