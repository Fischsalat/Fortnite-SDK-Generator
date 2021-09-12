#pragma once
#include "ClassWrapper.h"
#include "Global.h"

class UEObjectStore
{

public:

	static bool Initialize();

	UEObject GetByIndex(int32 index) const;
	int32 GetNumObjects() const;
	int32 GetMaxNumObjects() const;

	static int32 CountObjects(UEClass staticClass, std::string objectName);

	template<typename UE_Type>
	static UE_Type FindObject(std::string objectName);
	
	static UEClass FindClass(std::string className);

	class UObjectIterator begin();
	class UObjectIterator begin() const;

	class UObjectIterator end();
	class UObjectIterator end() const;
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