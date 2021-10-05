#pragma once
#include "Global.h"
#include "Flags.h"
#include "Flags.h"

class UObject;

template<class T>
class TArray
{
	friend class FString;

private: 
	T* data;
	int32 numElements;
	int32 maxElements;

public: 
	inline int32 Num()
	{
		return numElements;
	}
	inline bool IsValid()
	{
		return data != nullptr;
	}
	inline bool IsValidIndex(int32 index)
	{
		return index >= 0 && index < numElements;
	}
	inline bool IsValidIndex(uint32 index)
	{
		return index < numElements;
	}
	inline int32 GetSlack()
	{
		return maxElements - numElements;
	}
	inline void Add(T element)
	{
		if (GetSlack() >= 1)
		{
			data[numElements] = element;
			numElements++;
		}
		else
		{
			data = reinterpret_cast<T*>(realloc(data, sizeof(T) * numElements + 1));
			data[numElements++] = element;
			maxElements = numElements;
		}
	}
	inline void Free()
	{
		static auto FreeInternal = reinterpret_cast<void(*)(void*)>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + Offset::Free);
		FreeInternal(data);
		data = nullptr;
		numElements = 0;
		maxElements = 0;
	}

	inline T& operator[](int32 index)
	{
		return data[index];
	}
};

class FString : public TArray<wchar_t>
{
public:
	inline FString() = default;

	inline FString(const wchar_t* wcha)
	{
		maxElements = numElements = *wcha ? wcslen(wcha) + 1 : 0;
		numElements ? data = const_cast<wchar_t*>(wcha) : data = nullptr;
	}

	inline std::string ToString()
	{
		if (IsValid())
		{
			std::wstring wStr(data);
			return std::string(wStr.begin(), wStr.end());
		}
		return "";
	}
	inline std::wstring ToWString()
	{
		if(IsValid())
			return std::wstring(data);

		return L"";
	}
	inline const wchar_t* c_str()
	{
		return data;
	}

	inline FString operator=(const wchar_t*&& other)
	{
		return FString(other);
	}
};

class FName
{
public:
	int32 comparisonIndex;
	int32 number;

	inline std::string ToString() const
	{
		if (!this)
		{
			return "";
		}

		static auto ToStr = reinterpret_cast<void(*)(const FName*, FString&)>(reinterpret_cast<uintptr_t>(GetModuleHandle(0)) + 0x117C420);

		FString outStr;
		ToStr(this, outStr);

		std::string outName = outStr.ToString();
		outStr.Free();

		if (number > 0)
			outName += '_' + std::to_string(number);

		auto pos = outName.rfind('/');
		if (pos == std::string::npos)
			return outName;

		return outName.substr(pos + 1);
	}

	inline bool operator==(FName other)
	{
		return comparisonIndex == other.comparisonIndex;
	}
	inline bool operator!=(FName other)
	{
		return !(operator==(other));
	}
};


template<typename Value, typename Key>
class TPair
{
public:
	Value value;
	Key key;
};


class UClass;
class UProperty;


class UObject
{
public:
	void** vft;
	int32 flags;
	int32 internalIndex;
	UClass* privateClass;
	FName name;
	UObject* outer;

	inline std::string GetName() const
	{
		return name.ToString();
	}
	std::string GetFullName() const;
};

class FUObjectItem
{
public:
	UObject* object;
	int32 indexAndFlags;
	int32 serialNumber;
	int8 paddingTEST[0x8];
};

class TUObjectArray
{
public:
	FUObjectItem* objects;
	int32 maxNumElements;
	int32 numElements;
};

class FUObjectArray
{
public:
	uint8 pad[0x10];
	TUObjectArray objObjects;

	inline UObject* ByIndex(int32 index)
	{
		return objObjects.objects[index].object;
	}
	inline int32 Num()
	{
		return objObjects.numElements;
	}
};

class UField : public UObject
{
public:
	UField* next;
};

class UEnum : public UField
{
public:
	enum class ECppForm : int64
	{
		Regular,
		Namespaced,
		EnumClass
	};
	FString cppType;
	TArray<TPair<FName, int64>> names;
	ECppForm cppForm;
	uint8 pad_0[0x10];
};

class UStruct : public UField
{
public:
	UStruct* super;
	UField* children;
	int32 propertySize;
	int32 minAlignment;
	uint8 pad_5[0x40];
};

class UClass : public UStruct
{
public:
	uint8 pad_10[0x88];
	UObject* defaultObject;
};

class UFunction : public UStruct
{
public:
	
	EFunctionFlags functionFlags;
	uint16 repOffset;
	uint8 numberParams;
	int8 pad_15[0x1];
	uint16 sizeParams;
	uint16 offsetReturnValue;
	uint8 pad_20[0x1C];
	void* func;

	std::string GetFlagsAsString() const;
};

class UScriptStruct : public UStruct
{
public:
	uint8 pad_7[0x18];
};


class UProperty : public UField
{
public:
	int32 arrayDim;
	int32 elementSize;
	int8 pad_25[0x10];
	EPropertyFlags propertyFlags;
	int32_t offsetInternal;
	int8 pad_30[0x4];
	UProperty* propertyLinkNext;
	int8 pad_35[0x18];

	std::string GetFlagsAsString() const;
};

class UStructProperty : public UProperty
{
public:
	UScriptStruct* innrStruct;
};

class UObjectPropertyBase : public UProperty
{
public:
	UClass* propertyClass;
};

class UObjectProperty : public UObjectPropertyBase
{
public:

};

class UClassProperty : public UObjectProperty
{
public:
	UClass* metaClass;
	int8 pad_001[0x18];
};

class UArrayProperty : public UProperty
{
public:
	UStructProperty* innerProperty;
};

class UDelegateProperty : public UProperty
{
public:

};

class UMulticastDelegateProperty : public UProperty
{
public:
	UFunction* signatureFunction;
};

class UBoolProperty : public UProperty
{
public:
	uint8 fieldSize;
	uint8 byteOffset;
	uint8 byteMask;
	uint8 fieldMask;
	uint8 pad_100[0x4];
};

class UMapProperty : public UProperty
{
public:
	class UProperty* keyProp;
	class UProperty* valueProp;
	int32 keyOffset;
	int32 valueOffset;
	int32 elementOffset;
	int32 hashNextIdOffset;
	int32 hashIndexOffset;
	int32 mapSize;
	int32 moreElementOffset;
	int32 alignment;
	int32 moreMapSize;
	int8 mapPad[0x14];
};


class USetProperty : public UProperty
{
public:
	UProperty* elementProp; 
	int32 elementOffset;
	int32 hashNextIdOffset;
	int32 hashIndexOffset;
	int32 setSize;
	int32 moreElementOffset;
	int32 alignment;
	int32 anotherSetSize;
	int32 padInt32;
};
class UNumericProperty : public UProperty
{
public:

}; 

class UEnumProperty : public UProperty
{
public:
	UNumericProperty* underlayingType;
	UEnum* propEnum;
};

class UByteProperty : public UNumericProperty
{
public:
	UEnum* enm;
};




template<typename ElementType>
class TSet
{
public:

};

template<typename Value, typename Key>
class TMap
{
public:

};


class FWeakObjectPtr
{
	int32		objectIndex;
	int32		objectSerialNumber;

public:
	inline bool operator==(FWeakObjectPtr other)
	{
		return objectIndex == other.objectIndex && objectSerialNumber == other.objectSerialNumber;
	}
	inline bool operator!=(FWeakObjectPtr other)
	{
		return !(operator==(other));
	}
	inline FWeakObjectPtr& operator=(FWeakObjectPtr other)
	{
		objectIndex = other.objectIndex;
		objectSerialNumber = other.objectSerialNumber;
	}

	inline UObject* GetUObject() const
	{
		return GObjects->ByIndex(objectIndex);
	}
};

template<typename ObjectType, class TWeakObjectPtrBase = FWeakObjectPtr>
class TWeakObjectPtr : public TWeakObjectPtrBase
{
	inline UObject* Get() const
	{
		return TWeakObjectPtrBase::GetUObject();
	}
	inline ObjectType* GetCastet() const
	{
		return static_cast<ObjectType*>(Get());
	}
	inline UObject* operator*()
	{
		return Get();
	}
	inline UObject* operator->()
	{
		return Get();
	}
};

class FScriptInterface
{
	UObject* objectPointer;
	void* interfacePointer;

public:

	inline UObject* GetObjPtr()
	{
		return objectPointer;
	}
};

template<typename TObjectID>
class TPersistentObjectPtr
{
	mutable TWeakObjectPtr<UObject> weakPtr;
	mutable int32 tagAtLastTest;
	TObjectID objectID;
};

class FUniqueObjectGuid
{
public:
	int8 pad134[0x10];
};

class FLazyObjectPtr : public TPersistentObjectPtr<FUniqueObjectGuid>
{
public:

};

class FText
{
public:
	int8 pad_99[0x18];
};