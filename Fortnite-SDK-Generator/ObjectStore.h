#pragma once
#include "ClassWrapper.h"
#include "Global.h"

class UEObjectStore
{

public:
	static void DumpObjects();

	static bool Initialize();
	static UEObject StaticGetByIndex(int32 index);

	UEObject GetByIndex(int32 index) const;
	int32 GetNumObjects() const;
	int32 GetMaxNumObjects() const;

	static int32 CountObjects(UEClass staticClass, std::string objectName);

	template<typename UE_Type>
	static UE_Type FindObject(std::string objectName);
	
	static UEClass FindClass(std::string className);

	static void GetAllPackages(std::vector<UEObject>& outPackages);

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
	UObjectIterator(UObjectIterator&& otherIt) noexcept;

	UObjectIterator& operator=(const UObjectIterator& otherIt);
	bool operator==(const UObjectIterator& otherIt) const;
	bool operator!=(const UObjectIterator& otherIt) const;

	UEObject operator*() const;
	UEObject operator->() const;

	UObjectIterator& operator++();
	UObjectIterator operator++(int32);

};