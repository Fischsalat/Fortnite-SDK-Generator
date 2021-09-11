#include "ObjectStore.h"

//-----------------------------------------------------------------------------------------------
//UEObjectStore
//-----------------------------------------------------------------------------------------------
bool UEObjectStore::Initialize()
{
	GObjects = reinterpret_cast<FUObjectArray*>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::GObjects);

	if (!GObjects)
	{
		std::cout << "UObjectStore::Initialize() was false";
		return false;
	}

	return true;
}

UEObject UEObjectStore::GetByIndex(int32 index) const
{
	return UEObject(GObjects->objObjects.objects[index].object);
}

int32 UEObjectStore::GetNumObjects() const
{
	return GObjects->objObjects.numElements;
}

int32 UEObjectStore::GetMaxNumObjects() const
{
	return GObjects->objObjects.maxNumElements;
}

template<typename UE_Type>
int32 UEObjectStore::CountObjects(std::string objectName)
{
	int32 count = 0; 
	for (auto obj : UEObjectStore())
	{
		if (obj.IsA<UEStruct>())
		{
			if (obj.GetName() == objectName)
				count++;
		}
	}
	return count;
}

template<typename UE_Type>
UE_Type UEObjectStore::FindObject(std::string objectName)
{
	for (int i = 0; i < GObjects->Num(); i++)
	{
		UObject* obj = GObjects->ByIndex(i);

		if (!obj)
			return nullptr;

		if (objectName == obj->GetFullName())
			return UEObject(obj).Cast<UE_Type>();

	}
	return UEObject(nullptr).Cast<UE_Type>();
}

UEClass UEObjectStore::FindClass(std::string className)
{
	return UEObjectStore::FindObject<UEClass>(className);
}

UObjectIterator UEObjectStore::begin()
{
	return UObjectIterator(*this, 0);
}

UObjectIterator UEObjectStore::begin() const
{
	return UObjectIterator(*this, 0);
}

UObjectIterator UEObjectStore::end()
{
	return UObjectIterator(*this);
}

UObjectIterator UEObjectStore::end() const
{
	return UObjectIterator(*this);
}

//-----------------------------------------------------------------------------------------------
//UObjectIterator
//-----------------------------------------------------------------------------------------------
UObjectIterator::UObjectIterator(const UEObjectStore& storeToIt) 
	: objectsToIterate(storeToIt), currentIndex(storeToIt.GetNumObjects())
{
}
UObjectIterator::UObjectIterator(const UEObjectStore& storeToIt, int32 index)
	: objectsToIterate(storeToIt), currentIndex(index), currentObject(storeToIt.GetByIndex(index))
{
}

UObjectIterator::UObjectIterator(const UObjectIterator& otherIt)
	: objectsToIterate(otherIt.objectsToIterate), currentIndex(otherIt.currentIndex), currentObject(otherIt.currentObject)
{
}

UObjectIterator::UObjectIterator(UObjectIterator&& otherIt)
	: objectsToIterate(otherIt.objectsToIterate), currentIndex(otherIt.currentIndex), currentObject(otherIt.currentObject)
{
}

UObjectIterator& UObjectIterator::operator=(UObjectIterator otherIt)
{
	objectsToIterate = otherIt.objectsToIterate;
	currentIndex = otherIt.currentIndex;
	currentObject = otherIt.currentObject;
	return *this;
}

bool UObjectIterator::operator==(UObjectIterator otherIt) const
{
	return currentIndex == otherIt.currentIndex;
}

bool UObjectIterator::operator!=(UObjectIterator otherIt) const
{
	return currentIndex != otherIt.currentIndex;
}

UEObject UObjectIterator::operator*() const
{
	return currentObject;
}

UEObject UObjectIterator::operator->() const
{
	return currentObject;
}

UObjectIterator& UObjectIterator::operator++()
{
	currentObject = UEObject();
	for (++currentIndex; currentIndex < objectsToIterate.GetNumObjects(); ++currentIndex)
	{
		currentObject = objectsToIterate.GetByIndex(currentIndex);
		if (currentObject.IsValid())
			break;
	}

	return *this;
}

UObjectIterator UObjectIterator::operator++(int32)
{
	UObjectIterator	temp(*this);
	++(*this);
	return temp;
}