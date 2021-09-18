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
			stream << std::format("\t{:{}}{:{}}//", member.type, 50, member.name += ";", 100, member.comment);
		}
		
		stream << "};\n\n\n";
	}

	PrintFileEnding(stream, FileType::Struct);
}

void Generator::GenerateClassFile(std::ofstream& stream, const std::vector<Package::Class>& classes) const
{
	PrintFileHeader(stream, FileType::Class);

	for (auto clss : classes)
	{

	}


	PrintFileEnding(stream, FileType::Class);
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

		stream << parm.parameterStructName << "\n{\n";

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
		stream << std::format("//{}\n({})\n{} {}::{}(", func.fullName, func.allFlags, func.returnType, func.superName, func.cppName);

		for (int i = 0; i < func.params.size(); i++)
		{
			if (func.params[i].nameOnly != "ReturnValue" || func.params[i].nameOnly != "FReturnValue")
			{
				stream << func.params[i].typedName;

				if (i != func.params.size() - 1)
					stream << ", ";
			}
		}
		stream << ")\n";

		stream << "{\n";
		stream << std::format("\tstatic auto fn = UObject::FindObject<UFunction>(\"{}\");\n\n", func.fullName);
		
		if (Settings::ShouldUseNamespaceForParams())
		{
			stream << "\t" << Settings::GetParamNamespace() << "::" << func.parameterStructName << " params;\n";
		}
		else
		{
			stream << "\t" << func.parameterStructName << " params;\n";
		}
			
		for (auto parm : func.params)
		{
			if (parm.nameOnly != "ReturnValue" || parm.nameOnly != "FReturnValue")
			{
				stream << std::format("\tparams.{} = {};\n", parm.nameOnly, parm.nameOnly);
			}
		}

		stream << "\n";

		if (func.bIsNative)
		{
			"\tauto flags = fn->functionFlags\nfn->functionFlags |= 0x400;\n\n";
		}

		stream << "\tUObject::ProcessEvent(fn, &params);\n\n";

		if (func.bIsNative)
		{
			"\tfn->functionFlags = flags;\n\n";
		}

		if (func.bHasReturnValue)
		{
			stream << "\treturn parms.ReturnValue;";
		}
			
		stream << "\n}";
	}

	PrintFileEnding(stream, FileType::Function);
}