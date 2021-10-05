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
	GenerateBasicFile(genPath);

	CreateSDKHeaderFile(genPath, packageNames);
}

void Generator::ProcessPackages(const fs::path& sdkPath, std::vector<std::string>& outNames)
{
	
	std::unordered_map<int32, std::vector<int32>> packageObjects;
	UEObjectStore::GetAllPackages(packageObjects);

	std::cout << std::format("Generation started...\nPackage-Count: 0x{:X}\n\n", packageObjects.size());

	const double percent = double(packageObjects.size()) / 100;
	int32 packageCount = 0;

	for (auto pair : packageObjects)
	{
		UEObject packageObj = UEObjectStore::StaticGetByIndex(pair.first);

		std::string packageName = packageObj.GetName();
		std::cout << std::format("Started processing package {}\n", packageName);

		Package pack(packageObj);
		pack.Process(pair.second);

		if (!pack.IsEmpty())
		{
			GenerateClassFile(pack.allClasses, packageName);
			GenerateStructsFile(pack.allStructs, pack.allEnums, packageName);
			GenerateParameterFile(pack.allFunctions, packageName);
			GenerateFunctionFile(pack.allFunctions, packageName);
			outNames.push_back(packageName);
		}

		std::cout << std::format("Processed package {} ({} out of {} [{:.0f}%])\n\n", packageName, packageCount, packageObjects.size(), packageCount / percent);
		packageCount++;
	}
}

void Generator::CreateSDKHeaderFile(const fs::path& sdkPath, const std::vector<std::string>& packageNames)
{
	std::ofstream stream(sdkPath / "SDK.hpp");

	stream << "#pragma once\n";

	for (auto defaultLib : Settings::GetIncludes().first)
	{
		stream << std::format("#include <{}>\n", defaultLib);
	}

	stream << "\n";

	for (auto ownHeader : Settings::GetIncludes().second)
	{
		stream << std::format("#include \"{}\"\n", ownHeader);
	}

	stream << "\n";

	if (Settings::ShouldIncludeUnrealTypedefs())
	{
		stream << "typedef __int8 int8;\n";
		stream << "typedef __int16 int16;\n";
		stream << "typedef __int32 int32;\n";
		stream << "typedef __int64 int64;\n\n";

		stream << "typedef unsigned __int8 uint8;\n";
		stream << "typedef unsigned __int16 uint16;\n";
		stream << "typedef unsigned __int32 uint32;\n";
		stream << "typedef unsigned __int64 uint64;\n\n";
	}

	stream << "\n";
	stream << "#include \"SDK/Basic.hpp\"\n";

	for (auto fileName : packageNames)
	{
		stream << std::format("#include \"SDK/{}_structs.hpp\"\n", fileName);
		stream << std::format("#include \"SDK/{}_classes.hpp\"\n", fileName);
		stream << std::format("#include \"SDK/{}_parameters.hpp\"\n", fileName);
	}
}

void Generator::SetStream(const fs::path&& sdkPath, std::ofstream& stream, FileType type, std::string packageName)
{
	stream.close();

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
	case Generator::FileType::OtherHeader:
		stream.open(sdkPath / (packageName + ".hpp"));
		break;
	case Generator::FileType::OtherSource:
		stream.open(sdkPath / (packageName + ".cpp"));
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
	{
		stream << "#include \"pch.h\"\n#include \"../pch.h\"\n\n";
	}
	else if (ft == FileType::Function)
	{
		stream << "#include \"../SDK.h\"\n\n";
	}

	if (Settings::ShouldUseNamespaceForSDK())
	{
		stream << "namespace " << Settings::GetSDKNamespace() << "\n{\n";
	}
	if (ft == FileType::Parameter && Settings::ShouldUseNamespaceForParams())
	{
		stream << "namespace " << Settings::GetParamNamespace() << "\n{\n";
	}
		
}

void Generator::PrintFileEnding(std::ofstream& stream, const Generator::FileType& ft) const
{
	if (ft == FileType::Parameter && Settings::ShouldUseNamespaceForParams())
		stream << "}\n";

	if (Settings::ShouldUseNamespaceForSDK())
		stream << "}\n\n";

	stream << "#ifdef _MSC_VER\n\t#pragma pack(pop)\n#endif\n";

	stream.flush();
	stream.close();
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

	stream << "\n\n";

	for (auto scriptStruct : structs)
	{
		stream << std::format("//{}\n", scriptStruct.fullName);

		if (scriptStruct.inheritedSize == 0)
			stream << std::format("//0x{:04X}", scriptStruct.structSize);
		else
			stream << std::format("//0x{:04X} (0x{:04X} - 0x{:04X})", scriptStruct.structSize - scriptStruct.inheritedSize, scriptStruct.structSize, scriptStruct.inheritedSize);
		
		stream << "\n" << scriptStruct.cppFullName << "\n{\n";

		for (auto member : scriptStruct.members)
		{
			if (member.bIsBitField)
			{
				member.name += " : " + std::to_string(member.bitFieldSize);
				stream << std::format("\t{:{}}{:{}}//{}\n", member.type, 50, member.name += ";", 75, member.comment);
			}
			else
			{
				stream << std::format("\t{:{}}{:{}}//{}\n", member.type, 50, member.name += ";", 75, member.comment);
			}
		}
		
		stream << "};\n\n";
	}

	PrintFileEnding(stream, FileType::Struct);
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
			if (member.bIsBitField)
			{
				member.name += " : " + std::to_string(member.bitFieldSize);
				stream << std::format("\t{:{}}{:{}}//{}\n", member.type, 50, member.name += ";", 75, member.comment);
			}
			else
			{
				stream << std::format("\t{:{}}{:{}}//{}\n", member.type, 50, member.name += ";", 75, member.comment);
			}
		}

		stream << "\n\tstatic UClass* StaticClass()\n\t{";
		
		if (Settings::ShouldUseStrings())
		{
			stream << "\n\t\tstatic auto ptr = UObject::FindClass";
			if (Settings::ShouldXorStrings())
			{
				stream << std::format("({}(\"{}\"))", Settings::XorText(), clss.fullName);
			}
			else
			{
				stream << std::format("(\"{}\");", clss.fullName);
			}
			stream << "\n\t\treturn ptr;\n";
		}
		else
		{
			stream << std::format("\n\t\tstatic auto ptr = UObject::GetByIndex<UClass>(0x{:X});\n\t\treturn ptr;\n", clss.index);
		}
		
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

		stream << "};\n\n";
	}


	PrintFileEnding(stream, FileType::Class);
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

		stream << "struct " << parm.parameterStructName << "\n{\n";

		for (auto member : parm.selfAsStruct.members)
		{
			stream << std::format("\t{:{}}{:{}}//{}({})\n", member.type, 50, member.name += ";", 75, member.offset, member.size);
		}

		stream << "};\n\n";
	}

	PrintFileEnding(stream, FileType::Parameter);
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
			
		stream << "}\n\n";
	}

	PrintFileEnding(stream, FileType::Function);
}

Generator::Generator()
{
	predefinedMembers["Class CoreUObject.Object"] =
	{
		{ "static class FUObjectArray*", "GObjects", 0x00 },
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

	//------------------------------------------------------------------

	predefinedFunctions["Class CoreUObject.Object"] =
	{
		{
			false, false, true, true, "void", "ProcessEvent", "class UFunction* func, void* parms",
			R"(return GetVFunction<void(*)(UObject*, class UFunction*, void*)>(this, /*PE-INDEX*/)(this, function, parms);)"
		},
		{
			true, true, true, false, "T*", "FindObject", "const std::string&& name",
			R"(for(int i = 0; i < GObjects->Num(); i++)
{
	UObject* obj = GObjects->GetByIndex(i);

	if(!obj)
	{
		continue;
	}

	if(obj.GetFullName() == name)
	{
		return static_cast<T*>(obj);
	}
}
return nullptr;)"
		},
		{
			true, false, true, true, "UClass*", "GetByIndex", "const std::string&& name",
			R"(return FindObject<UObject>(name);)"
		},
		{
			true, false, true, true, "UObject*", "int index",
			R"(return GObjects->ObjObjects.Objects[index].object;)"
		},
		{
			true, true, true, true, "T*", "GetCasted", "int index",
			R"(return static_cast<T*>(GetByIndex(index));)"
		},
		{

		}
	};
}

void Generator::GenerateBasicFile(const fs::path& sdkPath)
{
	std::ofstream stream;

	SetStream(sdkPath / "", stream, FileType::OtherHeader, "Basic");
	PrintFileHeader(stream, FileType::OtherHeader);

	stream << R"(class UObject;

template<class T>
class TArray
{
	friend class FString;

private:
	T* Data;
	int32_t NumElements;
	int32_t MaxElements;

public:
	inline int32_t Num()
	{
		return NumElements;
	}
	inline bool IsValid()
	{
		return Data != nullptr;
	}
	inline bool IsValidIndex(int32_t Index)
	{
		return Index >= 0 && Index < NumElements;
	}
	inline bool IsValidIndex(int32_t Index)
	{
		return Index < NumElements;
	}
	inline int32 GetSlack()
	{
		return MaxElements - NumElements;
	}
	inline void Add(T Element)
	{
		if (GetSlack() >= 1)
		{
			Data[NumElements] = Element;
			NumElements++;
		}
		else
		{
			Data = reinterpret_cast<T*>(realloc(Data, sizeof(T) * NumElements + 1));
			Data[NumElements++] = Element;
			MaxElements = NumElements;
		}
	}
	inline void Free()
	{
		static auto FreeInternal = reinterpret_cast<void(*)(void*)>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::Free);
		FreeInternal(Data);
		Data = nullptr;
		NumElements = 0;
		MaxElements = 0;
	}

	inline T& operator[](int32 index)
	{
		return Data[index];
	}
};)" << "\n\n";

	stream << R"(class FString : public TArray<wchar_t>
{
public:
	inline FString() = default;

	inline FString(const wchar_t* Wcha)
	{
		MaxElements = NumElements = *Wcha ? wcslen(Wcha) + 1 : 0;
		NumElements ? Data = const_cast<wchar_t*>(Wcha) : Data = nullptr;
	}

	inline std::string ToString()
	{
		if (IsValid())
		{
			std::wstring wStr(Data);
			return std::string(wStr.begin(), wStr.end());
		}
		return "";
	}
	inline std::wstring ToWString()
	{
		if(IsValid())
		{
			return std::wstring(data);
		}

		return L"";
	}
	inline const wchar_t* c_str()
	{
		return Data;
	}

	inline FString operator=(const wchar_t*&& Other)
	{
		return FString(Other);
	}
};)" << "\n\n";

	stream << R"(class FName
{
public:
	int32 ComparisonIndex;
	int32 Number;

	std::string ToString() const
	{
		if (!this)
		{
			return "";
		}

		static auto ToStr = reinterpret_cast<void(*)(const FName*, FString&)>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + 0x117C420);

		FString OutStr;
		ToStr(this, OutStr);

		std::string OutName = OutStr.ToString();
		OutStr.Free();

		if (number > 0)
		{
			OutName += '_' + std::to_string(Number);
		}

		auto pos = OutName.rfind('/');
		if (pos == std::string::npos)
		{
			return OutName;
		}

		return OutName.substr(pos + 1);
	}

	inline bool operator==(FName Other)
	{
		return ComparisonIndex == Other.ComparisonIndex;
	}
	inline bool operator!=(FName Other)
	{
		return !(operator==);
	}
};)" << "\n\n";

	stream << R"(template<typename _Value, typename _Key>
class TPair
{
public:
	_Value Value;
	_Key Key;
};


class UClass;
class UProperty;

)" << "\n\n";

	stream << R"(class FUObjectItem
{
public:
	UObject* Object;
	int32_t IndexAndFlags;
	int32_t SerialNumber;
	int8_t Pad_1337[0x8];

	bool IsPendingKill()
	{
		return !!(IndexAndFlags & (1 << 29));
	}
};)" << "\n\n";

	stream << R"(class TUObjectArray
{
public:
	FUObjectItem* Objects;
	int32_t MaxNumElements;
	int32_t NumElements;
};)" << "\n\n";

	stream << R"(class FUObjectArray
{
public:
	uint8_t Pad[0x10];
	TUObjectArray ObjObjects;

	inline UObject* ByIndex(int32_t Index)
	{
		return ObjObjects.Objects[Index].Object;
	}
	inline int32_t Num()
	{
		return ObjObjects.NumElements;
	}
};)" << "\n\n";

	stream << R"(template<typename ElementType>
class TSet
{
public:

};

template<typename Value, typename Key>
class TMap
{
public:

};)" << "\n\n";

	stream << R"(class FWeakObjectPtr
{
	int32_t		ObjectIndex;
	int32_t		ObjectSerialNumber;

public:
	inline bool operator==(FWeakObjectPtr Other)
	{
		return ObjectIndex == Other.ObjectIndex && ObjectSerialNumber == Other.ObjectSerialNumber;
	}
	inline bool operator!=(FWeakObjectPtr Other)
	{
		return !(operator==(Other));
	}
	inline FWeakObjectPtr& operator=(const FWeakObjectPtr& Other)
	{
		ObjectIndex = Other.ObjectIndex;
		ObjectSerialNumber = Other.ObjectSerialNumber;
	}

	inline UObject* GetUObject() const
	{
		return GObjects->ByIndex(ObjectIndex);
	}
};)" << "\n\n";

	stream << R"(template<typename ObjectType, class TWeakObjectPtrBase = FWeakObjectPtr>
class TWeakObjectPtr : public TWeakObjectPtrBase
{
	inline UObject* Get() const
	{
		return TWeakObjectPtrBase::GetUObject();
	}
	inline ObjectType* GetCastet() const
	{
		return static_cast<ObjectType*>(Get());
	}
	inline UObject* operator*()
	{
		return Get();
	}
	inline UObject* operator->()
	{
		return Get();
	}
};)" << "\n\n";

	stream << R"(class FScriptInterface
{
	UObject* ObjectPointer;
	void* InterfacePointer;

public:

	inline UObject* GetObjPtr()
	{
		return ObjectPointer;
	}
	inline void* GetInterface()
	{
		return InterfacePointer; //literally no fucking idea what's the use of a interface, but why not make a function to get it
	}
};)" << "\n\n";

	stream << R"(template<typename TObjectID>
class TPersistentObjectPtr
{
	mutable TWeakObjectPtr<UObject> WeakPtr;
	mutable int32_t TagAtLastTest;
	TObjectID ObjectID;
};)" << "\n\n";

	stream << R"(class FUniqueObjectGuid
{
public:
	int8_t Pad134[0x10];
};

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid>
{
public:

};

class FText
{
public:
	int8_t Pad_99[0x18];
};)" << "\n\n";

	PrintFileEnding(stream, FileType::OtherHeader);
}

fs::path Generator::genPath;
std::unordered_map<std::string, std::vector<Generator::PredefinedMember>> Generator::predefinedMembers;
std::unordered_map<std::string, std::vector<Generator::PredefinedFunction>> Generator::predefinedFunctions;