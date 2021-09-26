#pragma once
#include "Generator.h"

void Generator::Generate()
{
	genPath = fs::path(Settings::GetPath());

	if (!genPath.is_absolute())
	{

	}

	fs::create_directories(genPath);
	fs::create_directories(genPath / "SDK");

	std::vector<std::string> packageNames;
	
	ProcessPackages(genPath / "SDK", packageNames);

	CreateSDKHeaderFile(genPath, packageNames);
}

void Generator::ProcessPackages(const fs::path& sdkPath, std::vector<std::string>& outNames)
{
	
	std::vector<UEObject> packageObjects;
	UEObjectStore::GetAllPackages(packageObjects);

	for (auto packageObj : packageObjects)
	{
		std::string packageName = packageObj.GetName();
		std::cout << "Started processing package " << packageName << "\n";

		Package pack(packageObj);
		pack.Process();

		if (!pack.IsEmpty())
		{
			GenerateClassFile(pack.allClasses, packageName);
			GenerateStructsFile(pack.allStructs, pack.allEnums, packageName);
			GenerateParameterFile(pack.allFunctions, packageName);
			GenerateFunctionFile(pack.allFunctions, packageName);
		}

		std::cout << "Processed package " << packageName << "\n\n";
	}
}

void Generator::CreateSDKHeaderFile(const fs::path& sdkPath, const std::vector<std::string>& packageNames)
{
	std::ofstream stream(sdkPath / "SDK.hpp");
}

void Generator::SetStream(const fs::path&& sdkPath, std::ofstream& stream, FileType type, std::string packageName)
{
	switch (type)
	{
	case Generator::FileType::Parameter:
		stream.open(sdkPath / (packageName + "_parameters.hpp"));
		break;
	case Generator::FileType::Function:
		stream.open(sdkPath / (packageName + "_functions.cpp"));
		break;
	case Generator::FileType::Struct:
		stream.open(sdkPath / (packageName + "_structs.hpp"));
		break;
	case Generator::FileType::Class:
		stream.open(sdkPath / (packageName + "_classes.hpp"));
		break;
	default:
		stream.open(sdkPath / (packageName + ".hpp"));
		break;
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

	if (Settings::ShouldUseNamespaceForSDK())
		stream << "namespace " << Settings::GetSDKNamespace() << "\n{\n";

	if (ft == FileType::Parameter && Settings::ShouldUseNamespaceForParams())
		stream << "namespace " << Settings::GetParamNamespace() << "\n{\n";	
}

void Generator::PrintFileEnding(std::ofstream& stream, const Generator::FileType& ft) const
{
	if (ft == FileType::Parameter && Settings::ShouldUseNamespaceForParams())
		stream << "}\n";

	if (Settings::ShouldUseNamespaceForSDK())
		stream << "}\n\n";

	stream << "#ifdef _MSC_VER\n\t#pragma pack(pop)\n#endif\n";
}

void Generator::GenerateStructsFile(const std::vector<Package::Struct>& structs, const std::vector<Package::Enum>& enums, std::string packageName) const
{
	std::ofstream stream;

	SetStream(genPath / "SDK", stream, FileType::Struct, packageName);

	PrintFileHeader(stream, FileType::Struct);

	for (auto enumClass : enums)
	{
		stream << std::format("//{}\n{} : {}\n", enumClass.fullName, enumClass.name, enumClass.underlayingType);
		stream << "{\n";

		int count = 0;

		for (auto enmStr : enumClass.members)
		{
			
			stream << std::format("\t{:{}} = {}", enmStr, 30, count);
			++count;
			if (count != enumClass.members.size())
				stream << ",";

			stream << "\n";
		}
		stream << "};\n\n\n";
	}

	stream << "\n" << std::endl;

	for (auto scriptStruct : structs)
	{
		if (scriptStruct.cppName == "FInt32Range")
			std::cout << "pause" << std::endl;


		stream << std::format("//{}\n", scriptStruct.fullName);

		if (scriptStruct.inheritedSize == 0)
			stream << std::format("//0x{:04X}", scriptStruct.structSize);
		else
			stream << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})", scriptStruct.structSize - scriptStruct.inheritedSize, scriptStruct.structSize, scriptStruct.inheritedSize);
		
		stream << "\n" << scriptStruct.cppFullName << "\n{\n";

		for (auto member : scriptStruct.members)
		{
			stream << std::format("\t{:{}}{:{}}//{}\n", member.type, 55, member.name += ";", 75, member.comment);
		}
		
		stream << "};\n" << std::endl;
	}

	PrintFileEnding(stream, FileType::Struct);

	stream.flush();
}

void Generator::GenerateClassFile(const std::vector<Package::Class>& classes, std::string packageName) const
{
	std::ofstream stream;

	SetStream(genPath / "SDK", stream, FileType::Class, packageName);

	PrintFileHeader(stream, FileType::Class);

	for (auto clss : classes)
	{
		stream << std::format("//{}\n", clss.fullName);

		if (clss.inheritedSize == 0)
			stream << std::format("//0x{:04X}\n", clss.structSize);
		else
			stream << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})\n", clss.structSize - clss.inheritedSize, clss.structSize, clss.inheritedSize);

		stream << clss.cppFullName << "\n{\npublic:\n";

		if (predefinedMembers.find(clss.fullName) != std::end(predefinedMembers))
		{			
			for (auto member : predefinedMembers[clss.fullName])
			{
				stream << std::format("\t{:{}}{:{}}//(0x{:02X}[{:02d}]) NOT AUTO-GENERATED PROPERTY\n", member.type, 50, member.name + ";", 75, member.size, member.size);
			}
		}
		for (auto member : clss.members)
		{
			stream << std::format("\t{:{}}{:{}}//{}\n", member.type, 50, member.name += ";", 75, member.comment);
		}

		stream << "\n\tstatic UClass* StaticClass()\n\t{";
		stream << std::format("\n\t\tstatic auto ptr = UObject::FindClass(\"{}\");\n\t\treturn ptr;\n", clss.fullName);
		stream << "\t}\n";

		if (!clss.functions.empty())
		{
			stream << "\n";
			for (auto func : clss.functions)
			{
				stream << std::format("\t{} {}(", func.returnType, func.cppName);

				for (int i = 0; i < func.params.size(); i++)
				{
					stream << func.params[i].typedName;

					if (i != func.params.size() - 1)
						stream << ", ";
				}
				stream << ");\n";
			}
		}

		stream << "};\n" << std::endl;
	}


	PrintFileEnding(stream, FileType::Class);

	stream.flush();
}

void Generator::GenerateParameterFile(const std::vector<Package::Function>& parameters, std::string packageName) const
{
	std::ofstream stream;

	SetStream(genPath / "SDK", stream, FileType::Parameter, packageName);

	PrintFileHeader(stream, FileType::Parameter);

	for (auto parm : parameters)
	{

		stream << std::format("//{}\n", parm.fullName);

		if (parm.selfAsStruct.inheritedSize == 0)
			stream << std::format("//0x{:04X}\n", parm.selfAsStruct.structSize);
		else
			stream << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})\n", parm.selfAsStruct.structSize - parm.selfAsStruct.inheritedSize, parm.selfAsStruct.structSize, parm.selfAsStruct.inheritedSize);

		stream << parm.parameterStructName << "\n{\n";

		for (auto member : parm.selfAsStruct.members)
		{
			stream << std::format("\t{:{}}{:{}}//{}({})", member.type, 50, member.name += ";", 75, member.offset, member.size);
		}

		stream << "};\n" << std::endl;
	}

	PrintFileEnding(stream, FileType::Parameter);

	stream.flush();
}

void Generator::GenerateFunctionFile(const std::vector<Package::Function>& functions, std::string packageName) const
{
	std::ofstream stream;

	SetStream(genPath / "SDK", stream, FileType::Function, packageName);

	PrintFileHeader(stream, FileType::Function);

	for (auto func : functions)
	{
		stream << std::format("//{}\n//({})\n{} {}::{}(", func.fullName, func.allFlags, func.returnType, func.superName, func.cppName);

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

		stream << "\tUObject::ProcessEvent(fn, &params);\n";

		if (func.bIsNative)
		{
			"\n\tfn->functionFlags = flags;\n";
		}

		if (func.bHasReturnValue)
		{
			stream << "\n\treturn parms.ReturnValue;\n";
		}
			
		stream << "}\n" << std::endl;
	}

	PrintFileEnding(stream, FileType::Function);

	stream.flush();
}

Generator::Generator()
{
	predefinedMembers["Class CoreUObject.Object"] =
	{
		{ "void*", "Vft", 0x08 },
		{ "int32_t", "Flags", 0x04 },
		{ "int32_t", "Index", 0x04 },
		{ "class UClass*", "Class", 0x08 },
		{ "class FName", "Name", 0x08},
		{ "class UObject", "Outer", 0x08 }
	};

	predefinedMembers["Class CoreUObject.Field"] =
	{
		{ "class UField*", "Next", 0x08 },
	};

	predefinedMembers["Class CoreUObject.Enum"] =
	{
		{ "class FString", "CppType", 0x0C },
		{ "TArray<TPari<FName, int64_t>>", "Names", 0x0C},
		{ "int64_t", "CppForm", 0x08 },
		{ "uint8_t", "Pad1[0x10]", 0x10 }
	};

	predefinedMembers["Class CoreUObject.Struct"] =
	{
		{ "class UStruct*", "Super", 0x08 },
		{ "class UField*", "Children", 0x08 },
		{ "int32_t", "PropertySize", 0x04 },
		{ "int32_t", "MinAlignment", 0x04 },
		{ "uint8_t", "Pad2[0x40]", 0x40 }
	};

	predefinedMembers["Class CoreUObject.Class"] =
	{
		{ "uint8_t", "Pad3[0x88]", 0x88 },
		{ "class UObject*", "DefaultObject", 0x08 }
	};

	predefinedMembers["Class CoreUObject.Function"] =
	{
		{ "uint32_t", "FuncFlags", 0x04 },
		{ "uint16_t", "RepOffset", 0x02 },
		{ "int8_t", "NumberParams", 0x01 },
		{ "int8_t", "Pad4[0x1]", 0x01 },
		{ "int16_t", "SizeParams", 0x02 },
		{ "int16_t", "OffsetReturnValue", 0x02 },
		{ "uint8_t", "Pad5[0x1C]", 0x1C },
		{ "void*", "Func", 0x8 }
	};

	predefinedMembers["Class CoreUObject.Property"] =
	{
		{ "uint32_t", "ArrayDim", 0x04 },
		{ "uint32_t", "ElementSize", 0x04 },
		{ "int8_t", "Pad6[0x10]", 0x10 },
		{ "uint64_t", "PropertyFlags", 0x08 },
		{ "uint32_t", "OffsetInternal", 0x04 },
		{ "int8_t", "Pad7[0x04]", 0x04 },
		{ "uint8_t", "Pad8[0x1C]", 0x1C },
		{ "class UProperty*", "PropertyLinkNext", 0x8 },
		{ "int8_t", "Pad9[0x18]", 0x18 }
	};
}

fs::path Generator::genPath;
std::unordered_map<std::string, std::vector<Generator::PredefinedMember>> Generator::predefinedMembers;