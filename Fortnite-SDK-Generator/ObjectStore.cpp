#include "ObjectStore.h"


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
UE_Type UEObjectStore::FindObject(std::string objectName)
{
	for (int i = 0; i < GObjects->Num(); i++)
	{
		UObject* obj = GObjects->ByIndex(i);

		if (!obj)
			return nullptr;

		if (objectName == obj->GetFullName())
			return UEObject(obj).Cast<T>();

	}
	return UEObject(nullptr).Cast<T>();
}

UEClass UEObjectStore::FindClass(std::string className)
{
	UEObjectStore::FindObject<UEClass>(className);
}