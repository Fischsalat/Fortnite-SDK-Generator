#pragma once
#include "ClassWrapper.h"

class ObjectsIterator;

class UEObjectStore
{
public:

	static bool Initialize();

	UEObject GetByIndex(int32 index) const;
	int32 GetNumObjects() const;
	int32 GetMaxNumObjects() const;

	template<typename UE_Type>
	static UE_Type FindObject(std::string objectName);
	
	static UEClass FindClass(std::string className);

	UObjectIterator Begin();
	UObjectIterator End();

	UObjectIterator Begin() const;
	UObjectIterator End() const;
};

class UObjectIterator
{

	UObjectIterator& operator=(UObjectIterator otherIt);
	bool operator==(UObjectIterator otherIt);
	bool operator!=(UObjectIterator otherIt);

};