#pragma once
#include "Package.h"
#include "ObjectStore.h"
#include "Generator.h"

bool CompareProperties(UEProperty left, UEProperty right)
{
	if (left.GetOffset() == right.GetOffset() && left.IsA<UE_boolProperty>() && right.IsA<UE_boolProperty>())
	{
		return left.Cast<UE_boolProperty>() < right.Cast<UE_boolProperty>();
	}
	return left.GetOffset() < right.GetOffset();
}

void Package::Process()
{
	for (auto obj : UEObjectStore())
	{
		if (obj.GetPackage() == packageObj)
		{
			if (obj.IsA(UEEnum::StaticClass()))
			{
				GenerateEnumClass(UEEnum(obj.GetUObject()));
			}
			else if (obj.IsA(UEClass::StaticClass()))
			{
				GenerateClass(UEClass(obj.GetUObject()));
			}
			else if (obj.IsA(UEStruct::StaticClass()))
			{
				GenerateScritStruct(UEStruct(obj.GetUObject()));
			}
		}
	}
}


void Package::GenerateMembers(const std::vector<UEProperty>& memberVector, const UEStruct& super, std::vector<Package::Member>& outMembers)
{
	std::unordered_map<std::string, int32> nameCache;
	int32 padCount = 0;

	int32 lastPropertyOffset = super.GetStructSize();
	int32 lastPropertySize = 0;

	bool bLastPropertyWasBitField;

	for (auto&& member : memberVector)
	{
		Package::Member mObj;

		if (member.IsA(UE_boolProperty::StaticClass()))
		{
			UE_boolProperty boolProp = member.Cast<UE_boolProperty>();

			mObj.type = boolProp.GetTypeStr();
			mObj.name = boolProp.GetCppName();
			mObj.size = boolProp.GetElementSize();
		}

		mObj.type = member.GetPropertyType().second;
		mObj.name = member.GetCppName();
		mObj.size = member.GetElementSize(); 
		mObj.offset = member.GetOffset();
		mObj.comment = std::format("0x{:04X}(0x{:04X})({})", mObj.offset, mObj.size, member.GetFlagsAsString());

		int32 lastPropertyEnd = lastPropertyOffset + lastPropertySize;

		if (mObj.offset > lastPropertyEnd)
			outMembers.emplace_back(GenerateBytePadding(padCount, lastPropertyEnd, mObj.offset - lastPropertyEnd, "Fixing size after last property"));
		
		lastPropertyOffset = mObj.offset;
		lastPropertySize = mObj.size;

		const auto it = nameCache.find(mObj.name);
		if (it == std::end(nameCache))
		{
			nameCache[mObj.name] = 1;
		}
		else
		{
			++nameCache[mObj.name];
			mObj.name += std::format("{:2d}", it->second);
		}


		outMembers.emplace_back(std::move(mObj));
	}
}

Package::Function Package::GenerateFunction(const UEFunction& function, const UEStruct& super)
{
	if(!function.IsValid() || !super.IsValid())
		return Package::Function();

	Package::Function func;

	func.selfAsStruct = GenerateScritStruct(function); // UFunction : public UStruct

	func.cppName = function.GetCppName();
	func.superName = function.GetSuper().GetUniqueName();
	func.parameterStructName = func.superName + "_" + func.cppName + "_" + "Params";
	func.numParams = function.GetNumParams();
	func.bHasReturnValue = false;
	func.allFlags = function.GetFlagsAsString();

	for (UEProperty childParam = function.GetChildren().Cast<UEProperty>(); childParam.IsValid(); childParam = childParam.GetNext().Cast<UEProperty>())
	{
		Function::Parameter param;

		param.bIsConst = childParam.HasFlag(EPropertyFlags::ConstParm) ? true : false;
		param.bIsReference = childParam.HasFlag(EPropertyFlags::ReferenceParm) ? true : false;

		if (childParam.HasFlag(EPropertyFlags::ReturnParm))
		{
			param.paramType = Function::Parameter::ParameterType::Return;
			func.bHasReturnValue = true;
			func.returnType = childParam.GetPropertyType().second;
		}
		else if (childParam.HasFlag(EPropertyFlags::OutParm))
			param.paramType = Function::Parameter::ParameterType::Out;
		else
			param.paramType = Function::Parameter::ParameterType::Normal;
		

		if (param.bIsConst)
			param.typedName += "const ";

		param.typedName += childParam.GetPropertyType().second;

		if (param.bIsReference)
			param.typedName += "&";

		param.typedName += " " + childParam.GetCppName();

		param.nameOnly = childParam.GetCppName();

		func.params.emplace_back(std::move(param));
	}

	allFunctions.emplace_back(std::move(func));

	return func;
}

Package::Struct Package::GenerateScritStruct(const UEStruct& strct)
{
	if (!strct.IsValid())
		return Package::Struct();

	std::string structName = strct.GetUniqueName();

	if (structName.find("Default__") != NPOS || structName.find("Uninitialized") != NPOS || structName.find("placeholder") != NPOS)
		return Package::Struct();


	Package::Struct str;

	str.fullName = strct.GetFullName();
	str.cppName = structName;
	str.inheritedSize = 0;
	str.structSize = 0;
	
	UEStruct super = strct.GetSuper();

	if (super.IsValid())
	{
		str.cppFullName = std::format("struct {} : public {}", str.cppName, super.GetCppName());
		str.inheritedSize = super.GetStructSize();
		// case of UObject being super = sizeof(UObject) since UObject is not a UStruct (has no structSize member)
	}
	else
	{
		str.cppFullName = std::format("struct {}", str.cppName);
	}
	str.structSize = strct.GetStructSize();

	std::vector<UEProperty> propertyMembers;
	for (UEProperty prop = strct.GetChildren().Cast<UEProperty>(); prop.IsValid(); prop = prop.GetNext().Cast<UEProperty>())
	{
		if(prop.GetElementSize() > 0 
			&& prop.IsA(UEProperty::StaticClass())
			&& (!super.IsValid() || (super != strct && prop.GetOffset() >= super.GetStructSize())))
		{
			propertyMembers.push_back(prop);
		}
	}
	std::sort(std::begin(propertyMembers), std::end(propertyMembers), CompareProperties);

	GenerateMembers(propertyMembers, strct, str.members);

	allStructs.push_back(str);
	return str;
}

Package::Class Package::GenerateClass(const UEClass& clss)
{
	if (!clss.IsValid())
		return Package::Class();

	std::string className = clss.GetUniqueName();

	if (className.find("Default__") != NPOS || className.find("Uninitialized") != NPOS || className.find("placeholder") != NPOS)
		return Package::Class();



	Package::Class cls;

	cls.fullName = clss.GetFullName();
	cls.cppName = className;
	cls.inheritedSize = 0;
	cls.structSize = 0;

	UEClass super = clss.GetSuper().Cast<UEClass>();

	if (super.IsValid())
	{
		cls.cppFullName = std::format("class {} : public {}", cls.cppName, super.GetCppName());
		cls.inheritedSize = super.GetStructSize();
	}
	else
	{
		cls.cppFullName = std::format("struct {}", cls.cppName);
	}
	cls.structSize = clss.GetStructSize();

	std::vector<UEProperty> propertyMembers;
	for (UEField fild = clss.GetChildren(); fild.IsValid(); fild = fild.GetNext())
	{
		if (fild.IsA(UEProperty::StaticClass())
			&& !fild.Cast<UEProperty>().GetElementSize() > 0
			&& (!super.IsValid() || (super != clss && fild.Cast<UEProperty>().GetOffset() >= super.GetStructSize())))
		{
			propertyMembers.push_back(fild.Cast<UEProperty>());
		}
		else if (fild.IsA(UEFunction::StaticClass()))
		{
			cls.functions.emplace_back(GenerateFunction(UEFunction(fild.GetUObject()), clss));
		}
			
	}
	std::sort(std::begin(propertyMembers), std::end(propertyMembers), CompareProperties);

	GenerateMembers(propertyMembers, clss, cls.members);

	allClasses.push_back(cls);
	return cls;
}

Package::Enum Package::GenerateEnumClass(const UEEnum& enm)
{
	if (!enm.IsValid())
		return Package::Enum();

	Package::Enum enumStruct;

	enumStruct.fullName = enm.GetFullName();
	enumStruct.name = enm.GetEnumTypeAsStr();
	enumStruct.members = enm.GetAllNames();
	enumStruct.underlayingType = "uint8";

	allEnums.push_back(enumStruct);
	return enumStruct;
}


Package::Member Package::GenerateBytePadding(int32 id, int32 offset, int32 padSize, std::string reason)
{
	Member padMember;
	padMember.name = std::format("UnknownData%02d[0x%X]", id, padSize);
	padMember.type = "uint8";
	padMember.size = padSize;
	padMember.offset = offset;
	padMember.comment = std::move(reason);

	return padMember;
}
/*
Package::Member Package::GenerateBitPadding(int32 id, int32 offset, int32 padSize, std::string reason)
{
	
}
*/
