#pragma once
#include "CoreClasses.h"


typedef void(__fastcall* tToString)(const class FName*, FString&);
tToString fToString = reinterpret_cast<tToString>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::FNameToString);

typedef void(__thiscall* tGetFullName)(const class UObject*, FString&, UObject*);
tGetFullName fGetFullName = reinterpret_cast<tGetFullName>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::GetFullName);

std::string FName::ToString() const
{
	if (!this)
		return "";

	FString outStr;
	fToString(this, outStr);

	std::string outName = outStr.ToString();

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
}

std::string UObject::GetFullName() const
{
	if (!this)
		return "";

	FString outName;
	fGetFullName(this, outName, nullptr);

	std::string name = outName.ToString();

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