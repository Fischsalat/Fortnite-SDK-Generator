#pragma once
#include "NakedClasses.h"
#include "Global.h"


static std::string MakeValidCppName(std::string name)
{
	for (int i = 0; i < name.length(); i++)
	{
		for (int i = 0; i < name.length(); i++)
		{
			if (   name[i] == ' '
				|| name[i] == '.'
				|| name[i] == ','
				|| name[i] == ':'
				|| name[i] == ';'
				|| name[i] == '-'
				|| name[i] == '+'
				|| name[i] == '<'
				|| name[i] == '>'
				|| name[i] == '&'
				|| name[i] == '|'
				|| name[i] == '^'
				|| name[i] == '#'
				|| name[i] == '%'
				|| name[i] == '@'
				|| name[i] == '/'
				|| name[i] == '\\'
				|| name[i] == '\''
				|| name[i] == '"'
				|| name[i] == ')'
				|| name[i] == '('
				|| name[i] == '['
				|| name[i] == ']'
				|| name[i] == '{'
				|| name[i] == '}')
			{
				name[i] == '_';
			}
		}
		return name;
	}
}

template<typename T>
T* FindObject(std::string objectName)
{
	for (int i = 0; i < GObjects->Num(); i++)
	{
		UObject* obj = GObjects->ByIndex(i);

		if (!obj)
			return nullptr;

		std::string fullName = obj->GetFullName();

		if (fullName == objectName)
		{
			return static_cast<T*>(obj);
		}
	}
	return nullptr;
}

inline UClass* FindClass(std::string className)
{
	return FindObject<UClass>(className);
}

