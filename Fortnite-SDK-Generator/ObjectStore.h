#pragma once
#include "ClassWrapper.h"


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

	class UObjectIterator Begin();
	class UObjectIterator End();

	class UObjectIterator Begin() const;
	class UObjectIterator End() const;
};

class UObjectIterator
{
	UEObjectStore objectsToIterate;
	UEObject currentObject;
	int32 currentIndex;

public:

	UObjectIterator(const UEObjectStore& storeToIt);
	UObjectIterator(const UEObjectStore& storeToIt, int32 index);

	UObjectIterator(const UObjectIterator& otherIt);
	UObjectIterator(UObjectIterator&& otherIt);

	UObjectIterator& operator=(UObjectIterator otherIt);
	bool operator==(UObjectIterator otherIt) const;
	bool operator!=(UObjectIterator otherIt) const;

	UEObject operator*() const;
	UEObject operator->() const;

	UObjectIterator& operator++();
	UObjectIterator operator++(int32);

};