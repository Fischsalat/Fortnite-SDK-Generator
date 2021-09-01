#pragma once
#include "Classes.h"
#include "Global.h"


template<typename T>
inline T* FindObject(std::string objectName)
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
