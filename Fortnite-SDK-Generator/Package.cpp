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

void Package::Process(std::vector<int32>& packageMembers)
{
	for (auto index : packageMembers)
	{
		UEObject obj = ObjectStore->GetByIndex(index);

		if (!obj.IsValid() )
			continue;

		std::string name = obj.GetName();

		if (name.find("Default__") != NPOS || name.find("Uninitialized") != NPOS || name.find("placeholder") != NPOS)
			continue;

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

	int32 lastPropertyOffset = super.GetStructSize();
	int32 lastPropertySize = 0;

	for (auto&& member : memberVector)
	{
		Package::Member mObj;

		Member bitPad;
		bool bIsBitPadRequired = false;

		if (member.IsA(UE_boolProperty::StaticClass()))
		{
			UE_boolProperty boolProp = member.Cast<UE_boolProperty>();

			if (boolProp.IsBitField())
			{
				mObj.bIsBitField = true;
				mObj.bitFieldSize = 1;

				const uint8_t missing = boolProp.GetMissingBitCount();

				if (missing > 0)
				{
					bIsBitPadRequired = true;
					bitPad = GenerateBitPadding(boolProp.GetOffset(), missing, "FIX BIT-FIELD SIZE");
				}
			}
		}

		mObj.type = member.GetPropertyType();
		mObj.name = member.GetValidName();
		mObj.size = member.GetElementSize();
		mObj.offset = member.GetOffset();
		mObj.comment = std::format("0x{:04X}(0x{:04X})({})", mObj.offset, mObj.size, member.GetFlagsAsString());

		int32 lastPropertyEnd = lastPropertyOffset + lastPropertySize;

		if (mObj.offset > lastPropertyEnd)
		{
			outMembers.emplace_back(GenerateBytePadding(lastPropertyEnd, mObj.offset - lastPropertyEnd, "Fixing size after last property"));
		}
		
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

		outMembers.push_back(mObj);

		if (bIsBitPadRequired)
		{
			outMembers.push_back(bitPad);
		}
	}
}

Package::Function Package::GenerateFunction(const UEFunction& function, const UEStruct& super)
{
	if(!function.IsValid() || !super.IsValid())
		return Package::Function();

	Package::Function func;

	func.selfAsStruct = GenerateScritStruct(function); // UFunction : public UStruct

	func.cppName = function.GetName();
	func.fullName = function.GetFullName();
	func.superName = super.GetUniqueName();
	func.parameterStructName = func.superName + "_" + func.cppName + "_" + "Params";
	func.numParams = function.GetNumParams();
	func.bHasReturnValue = false;
	func.allFlags = function.GetFlagsAsString();

	for (UEProperty childParam = UEProperty(function.GetChildren().GetUObject()); childParam.IsValid(); childParam = UEProperty(childParam.GetNext().GetUObject()))
	{
		Function::Parameter param;

		param.bIsConst = childParam.HasFlag(EPropertyFlags::ConstParm) ? true : false;
		param.bIsReference = childParam.HasFlag(EPropertyFlags::ReferenceParm) ? true : false;

		if (childParam.HasFlag(EPropertyFlags::ReturnParm))
		{
			param.paramType = Function::Parameter::ParameterType::Return;
			func.bHasReturnValue = true;
			func.returnType = childParam.GetPropertyType();
		}
		else if (childParam.HasFlag(EPropertyFlags::OutParm))
			param.paramType = Function::Parameter::ParameterType::Out;
		else
			param.paramType = Function::Parameter::ParameterType::Normal;
		

		if (param.bIsConst)
			param.typedName += "const ";

		param.typedName += childParam.GetPropertyType();

		if (param.bIsReference)
			param.typedName += "&";

		param.typedName += " " + childParam.GetCppName();

		param.nameOnly = childParam.GetCppName();

		func.params.push_back(param);
	}

	allFunctions.push_back(func);

	return func;
}

Package::Struct Package::GenerateScritStruct(const UEStruct& strct)
{
	if (!strct.IsValid())
		return Package::Struct();

	Package::Struct str;

	str.fullName = strct.GetFullName();
	str.cppName = strct.GetUniqueName();
	str.inheritedSize = 0;
	str.structSize = 0;
	
	UEStruct super = strct.GetSuper();

	if (super.IsValid())
	{
		str.cppFullName = std::format("struct {} : public {}", str.cppName, super.GetCppName());
		str.inheritedSize = super.GetStructSize();
	}
	else
	{
		str.cppFullName = std::format("struct {}", str.cppName);
	}
	str.structSize = strct.GetStructSize();

	int32 possibleSizeFix = 0;
	int32 offsetForPad = 0;

	std::vector<UEProperty> propertyMembers;
	for (UEProperty prop = UEProperty(strct.GetChildren().GetUObject()); prop.IsValid(); prop = UEProperty(prop.GetNext().GetUObject()))
	{
		if(prop.GetElementSize() > 0 
			&& prop.IsA(UEProperty::StaticClass())
			&& (!super.IsValid() || (super != strct && prop.GetOffset() >= super.GetStructSize())))
		{
			propertyMembers.push_back(prop);
		}
		if (!prop.GetNext().IsValid())
		{
			if ((prop.GetOffset() + prop.GetElementSize()) == str.structSize)
			{
				offsetForPad = prop.GetOffset() + prop.GetElementSize();
				possibleSizeFix = str.structSize - offsetForPad;
			}
		}
	}
	std::sort(std::begin(propertyMembers), std::end(propertyMembers), CompareProperties);

	GenerateMembers(propertyMembers, strct, str.members);

	if (possibleSizeFix > 0)
	{
		str.members.push_back(GenerateBytePadding(offsetForPad, possibleSizeFix, "FIXING SIZE"));
	}
	

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
	cls.index = clss.GetInernalIndex();

	UEClass super = clss.GetSuper().Cast<UEClass>();

	if (super.IsValid())
	{
		cls.cppFullName = std::format("class {} : public {}", cls.cppName, super.GetCppName());
		cls.inheritedSize = super.GetStructSize();
	}
	else
	{
		cls.cppFullName = std::format("class {}", cls.cppName);
	}
	cls.structSize = clss.GetStructSize();

	std::vector<UEProperty> propertyMembers;
	for (UEField fild = clss.GetChildren(); fild.IsValid(); fild = fild.GetNext())
	{
		if (fild.IsA(UEProperty::StaticClass())	&& UEProperty(fild.GetUObject()).GetElementSize() > 0)
		{
			propertyMembers.push_back(UEProperty(fild.GetUObject()));
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
	enumStruct.underlayingType = "uint8_t";

	allEnums.push_back(enumStruct);
	return enumStruct;
}


Package::Member Package::GenerateBytePadding(int32 offset, int32 padSize, std::string reason) const
{
	static int64 padCount = 0x0;
	Member padMember;
	padMember.name = std::format("UnknownData{:04d}[0x{:X}]", padCount, padSize);
	padMember.type = "uint8_t";
	padMember.size = padSize;
	padMember.offset = offset;
	padMember.comment = std::move(reason);
	padCount++;

	return padMember;
}

Package::Member Package::GenerateBitPadding(int32 offset, int32 padSize, std::string reason) const
{
	static int64 padCount = 0xFFF;
	Member padMember;
	padMember.name = std::format("UnknownData{:04d}", padCount, padSize);
	padMember.type = "uint8_t";
	padMember.size = 0;
	padMember.offset = offset;
	padMember.bIsBitField = true;
	padMember.bitFieldSize = padSize;
	padMember.comment = std::move(reason);
	padCount++;

	return padMember;
}

