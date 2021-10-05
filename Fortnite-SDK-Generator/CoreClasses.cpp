#pragma once
#include "CoreClasses.h"


typedef void(__fastcall* tToString)(const class FName*, FString&);
tToString fToString = reinterpret_cast<tToString>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::FNameToString);

typedef void(__thiscall* tGetFullName)(const class UObject*, FString&, UObject*);
tGetFullName fGetFullName = reinterpret_cast<tGetFullName>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::GetFullName);
/*
std::string FName::ToString() const
{
	if (!this)
		return "";

	FString outStr;
	fToString(this, outStr);

	std::string outName = outStr.ToString();
	outStr.Free();

	if (number > 0)
		outName += '_' + std::to_string(number);

	auto pos = outName.rfind('/');
	if (pos == std::string::npos)
		return outName;

	return outName.substr(pos + 1);
}

std::string UObject::GetName() const
{
	if (!this)
		return "";

	return name.ToString();
}*/

std::string UObject::GetFullName() const
{
	if (privateClass)
	{
		std::string temp;

		for (UObject* outr = outer; outr != nullptr; outr = outr->outer)
		{
			temp = outer->GetName() + "." + temp;
		}

		std::string retName = privateClass->GetName() + " ";
		retName += temp;
		retName += GetName();

		return retName;
	}
	return "";
}

	/*
	if (!this)
		return "";

	FString outName;
	fGetFullName(this, outName, nullptr);

	std::string name = outName.ToString();
	outName.Free();

	if (name.find(":") != NPOS)
		name.replace(name.find(":"), 1, ".");

	size_t first = name.find_first_of("/");
	if (first == NPOS)
	{
		return name;
	}
	else
	{
		size_t second = name.find_last_of("/");

		size_t eraseLength = second - first;

		if (name.length() > eraseLength)
			name.erase(first, eraseLength + 1);
	}

	return name;
}
*/

std::string UProperty::GetFlagsAsString() const
{
	std::vector<const char*> buffer;

	if (propertyFlags & EPropertyFlags::Edit) { buffer.push_back("Edit"); }
	if (propertyFlags & EPropertyFlags::ConstParm) { buffer.push_back("ConstParm"); }
	if (propertyFlags & EPropertyFlags::BlueprintVisible) { buffer.push_back("BlueprintVisible"); }
	if (propertyFlags & EPropertyFlags::ExportObject) { buffer.push_back("ExportObject"); }
	if (propertyFlags & EPropertyFlags::BlueprintReadOnly) { buffer.push_back("BlueprintReadOnly"); }
	if (propertyFlags & EPropertyFlags::Net) { buffer.push_back("Net"); }
	if (propertyFlags & EPropertyFlags::EditFixedSize) { buffer.push_back("EditFixedSize"); }
	if (propertyFlags & EPropertyFlags::Parm) { buffer.push_back("Parm"); }
	if (propertyFlags & EPropertyFlags::OutParm) { buffer.push_back("OutParm"); }
	if (propertyFlags & EPropertyFlags::ZeroConstructor) { buffer.push_back("ZeroConstructor"); }
	if (propertyFlags & EPropertyFlags::ReturnParm) { buffer.push_back("ReturnParm"); }
	if (propertyFlags & EPropertyFlags::DisableEditOnTemplate) { buffer.push_back("DisableEditOnTemplate"); }
	if (propertyFlags & EPropertyFlags::Transient) { buffer.push_back("Transient"); }
	if (propertyFlags & EPropertyFlags::Config) { buffer.push_back("Config"); }
	if (propertyFlags & EPropertyFlags::DisableEditOnInstance) { buffer.push_back("DisableEditOnInstance"); }
	if (propertyFlags & EPropertyFlags::EditConst) { buffer.push_back("EditConst"); }
	if (propertyFlags & EPropertyFlags::GlobalConfig) { buffer.push_back("GlobalConfig"); }
	if (propertyFlags & EPropertyFlags::InstancedReference) { buffer.push_back("InstancedReference"); }
	if (propertyFlags & EPropertyFlags::DuplicateTransient) { buffer.push_back("DuplicateTransient"); }
	if (propertyFlags & EPropertyFlags::SubobjectReference) { buffer.push_back("SubobjectReference"); }
	if (propertyFlags & EPropertyFlags::SaveGame) { buffer.push_back("SaveGame"); }
	if (propertyFlags & EPropertyFlags::NoClear) { buffer.push_back("NoClear"); }
	if (propertyFlags & EPropertyFlags::ReferenceParm) { buffer.push_back("ReferenceParm"); }
	if (propertyFlags & EPropertyFlags::BlueprintAssignable) { buffer.push_back("BlueprintAssignable"); }
	if (propertyFlags & EPropertyFlags::Deprecated) { buffer.push_back("Deprecated"); }
	if (propertyFlags & EPropertyFlags::IsPlainOldData) { buffer.push_back("IsPlainOldData"); }
	if (propertyFlags & EPropertyFlags::RepSkip) { buffer.push_back("RepSkip"); }
	if (propertyFlags & EPropertyFlags::RepNotify) { buffer.push_back("RepNotify"); }
	if (propertyFlags & EPropertyFlags::Interp) { buffer.push_back("Interp"); }
	if (propertyFlags & EPropertyFlags::NonTransactional) { buffer.push_back("NonTransactional"); }
	if (propertyFlags & EPropertyFlags::EditorOnly) { buffer.push_back("EditorOnly"); }
	if (propertyFlags & EPropertyFlags::NoDestructor) { buffer.push_back("NoDestructor"); }
	if (propertyFlags & EPropertyFlags::AutoWeak) { buffer.push_back("AutoWeak"); }
	if (propertyFlags & EPropertyFlags::ContainsInstancedReference) { buffer.push_back("ContainsInstancedReference"); }
	if (propertyFlags & EPropertyFlags::AssetRegistrySearchable) { buffer.push_back("AssetRegistrySearchable"); }
	if (propertyFlags & EPropertyFlags::SimpleDisplay) { buffer.push_back("SimpleDisplay"); }
	if (propertyFlags & EPropertyFlags::AdvancedDisplay) { buffer.push_back("AdvancedDisplay"); }
	if (propertyFlags & EPropertyFlags::Protected) { buffer.push_back("Protected"); }
	if (propertyFlags & EPropertyFlags::BlueprintCallable) { buffer.push_back("BlueprintCallable"); }
	if (propertyFlags & EPropertyFlags::BlueprintAuthorityOnly) { buffer.push_back("BlueprintAuthorityOnly"); }
	if (propertyFlags & EPropertyFlags::TextExportTransient) { buffer.push_back("TextExportTransient"); }
	if (propertyFlags & EPropertyFlags::NonPIEDuplicateTransient) { buffer.push_back("NonPIEDuplicateTransient"); }
	if (propertyFlags & EPropertyFlags::ExposeOnSpawn) { buffer.push_back("ExposeOnSpawn"); }
	if (propertyFlags & EPropertyFlags::PersistentInstance) { buffer.push_back("PersistentInstance"); }
	if (propertyFlags & EPropertyFlags::UObjectWrapper) { buffer.push_back("UObjectWrapper"); }
	if (propertyFlags & EPropertyFlags::HasGetValueTypeHash) { buffer.push_back("HasGetValueTypeHash"); }
	if (propertyFlags & EPropertyFlags::NativeAccessSpecifierPublic) { buffer.push_back("NativeAccessSpecifierPublic"); }
	if (propertyFlags & EPropertyFlags::NativeAccessSpecifierProtected) { buffer.push_back("NativeAccessSpecifierProtected"); }
	if (propertyFlags & EPropertyFlags::NativeAccessSpecifierPrivate) { buffer.push_back("NativeAccessSpecifierPrivate"); }

	switch (buffer.size())
	{
	case 0:
		return std::string();
	case 1:
		return std::string(buffer[0]);
	default:
		std::ostringstream os;
		std::copy(buffer.begin(), buffer.end() - 1, std::ostream_iterator<const char*>(os, ", "));
		os << *buffer.rbegin();
		return os.str();
	}
}

std::string UFunction::GetFlagsAsString() const
{
	std::vector<const char*> buffer;

	if (functionFlags & EFunctionFlags::Final) { buffer.push_back("Final"); }
	if (functionFlags & EFunctionFlags::RequiredAPI) { buffer.push_back("RequiredAPI"); }
	if (functionFlags & EFunctionFlags::BlueprintAuthorityOnly) { buffer.push_back("BlueprintAuthorityOnly"); }
	if (functionFlags & EFunctionFlags::BlueprintCosmetic) { buffer.push_back("BlueprintCosmetic"); }
	if (functionFlags & EFunctionFlags::Net) { buffer.push_back("Net"); }
	if (functionFlags & EFunctionFlags::NetReliable) { buffer.push_back("NetReliable"); }
	if (functionFlags & EFunctionFlags::NetRequest) { buffer.push_back("NetRequest"); }
	if (functionFlags & EFunctionFlags::Exec) { buffer.push_back("Exec"); }
	if (functionFlags & EFunctionFlags::Native) { buffer.push_back("Native"); }
	if (functionFlags & EFunctionFlags::Event) { buffer.push_back("Event"); }
	if (functionFlags & EFunctionFlags::NetResponse) { buffer.push_back("NetResponse"); }
	if (functionFlags & EFunctionFlags::Static) { buffer.push_back("Static"); }
	if (functionFlags & EFunctionFlags::NetMulticast) { buffer.push_back("NetMulticast"); }
	if (functionFlags & EFunctionFlags::MulticastDelegate) { buffer.push_back("MulticastDelegate"); }
	if (functionFlags & EFunctionFlags::Public) { buffer.push_back("Public"); }
	if (functionFlags & EFunctionFlags::Private) { buffer.push_back("Private"); }
	if (functionFlags & EFunctionFlags::Protected) { buffer.push_back("Protected"); }
	if (functionFlags & EFunctionFlags::Delegate) { buffer.push_back("Delegate"); }
	if (functionFlags & EFunctionFlags::NetServer) { buffer.push_back("NetServer"); }
	if (functionFlags & EFunctionFlags::HasOutParms) { buffer.push_back("HasOutParms"); }
	if (functionFlags & EFunctionFlags::HasDefaults) { buffer.push_back("HasDefaults"); }
	if (functionFlags & EFunctionFlags::NetClient) { buffer.push_back("NetClient"); }
	if (functionFlags & EFunctionFlags::DLLImport) { buffer.push_back("DLLImport"); }
	if (functionFlags & EFunctionFlags::BlueprintCallable) { buffer.push_back("BlueprintCallable"); }
	if (functionFlags & EFunctionFlags::BlueprintEvent) { buffer.push_back("BlueprintEvent"); }
	if (functionFlags & EFunctionFlags::BlueprintPure) { buffer.push_back("BlueprintPure"); }
	if (functionFlags & EFunctionFlags::Const) { buffer.push_back("Const"); }
	if (functionFlags & EFunctionFlags::NetValidate) { buffer.push_back("NetValidate"); }

	switch (buffer.size())
	{
	case 0:
		return std::string();
	case 1:
		return std::string(buffer[0]);
	default:
		std::ostringstream os;
		std::copy(buffer.begin(), buffer.end() - 1, std::ostream_iterator<const char*>(os, ", "));
		os << *buffer.rbegin();
		return os.str();
	}

}