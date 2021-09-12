#pragma once
#include "Package.h"
#include "ObjectStore.h"

bool CompareProperties(UEProperty left, UEProperty right)
{
	if (left.GetOffset() == right.GetOffset() && left.IsA<UE_boolProperty>() && right.IsA<UE_boolProperty>())
	{
		return left.Cast<UE_boolProperty>() < right.Cast<UE_boolProperty>();
	}
	return left.GetOffset() < right.GetOffset();
}





void Package::GenerateMembers(const std::vector<UEProperty>& memberVector, const UEStruct& super, std::vector<Package::Member>& outMembers)
{
	std::unordered_map<std::string, int32> nameCache;
	int32 padCount = 0;

	int32 lastPropertyOffset = super.GetStructSize();
	int32 lastPropertySize = 0;

	for (auto&& member : memberVector)
	{
		Package::Member mObj;

		mObj.bIsBitField = member.IsA<UE_boolProperty>() ? member.Cast<UE_boolProperty>().IsBitField() : false;

		mObj.type = member.GetPropertyType().second;
		mObj.name = member.GetCppName();
		mObj.size = member.GetElementSize(); 
		mObj.offset = member.GetOffset();
		mObj.comment = std::format("0x%X4d(0x%X4d)", mObj.offset, mObj.size);

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
			mObj.name += std::format("%2d", it->second);
		}


		outMembers.emplace_back(std::move(mObj));
	}
}
/*
Package::Function Package::GenerateFunction(const UEFunction& function)
{

}
*/
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

	std::vector<UEProperty> properties;
	for (UEProperty prop = strct.GetChildren().Cast<UEProperty>(); prop.IsValid(); prop = prop.GetNext().Cast<UEProperty>())
	{
		if(prop.GetElementSize() > 0 
			&& !prop.IsA(UEStruct::StaticClass())
			&& !prop.IsA(UEFunction::StaticClass())
			&& !prop.IsA(UEEnum::StaticClass())
			&& (!super.IsValid() || (super != strct && prop.GetOffset() >= super.GetStructSize())))
		{
			properties.push_back(prop);
		}
	}
	std::sort(std::begin(properties), std::end(properties), CompareProperties);

	GenerateMembers(properties, strct, str.members);

	return str;
}
/*
Package::Class Package::GenerateClass(const UEClass& clss)
{

}

Package::Enum Package::GenerateEnumClass(const UEEnum& enm)
{

}
*/

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
	Member padMember;
	padMember.name = std::format("UnknownData%02d[0x%X]", id, padSize);
	padMember.type = "uint8";
	padMember.size = padSize; // fix this
	padMember.offset = offset;
	padMember.comment = std::move(reason);

	return padMember;
}
*/
void Package::PrintClass(const Package::Class& clss)
{
	std::format(R"(%s)"); //fix
}

void Package::PrintStruct(const Package::Struct& clss)
{
	std::format(R"(%s)"); //fix
}

void Package::PrintEnum(const Package::Enum& enm)
{

}

void Package::PrintMembers(const std::vector<UEProperty>& mem, std::vector<Member> outMembers)
{

}

void Package::PrintFunction(const Package::Function& func)
{

}