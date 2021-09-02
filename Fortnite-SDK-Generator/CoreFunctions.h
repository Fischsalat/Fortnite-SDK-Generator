#pragma once
#include "NakedClasses.h"
#include "Global.h"


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

