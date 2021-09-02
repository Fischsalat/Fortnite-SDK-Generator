#pragma once
#include "Global.h"
#include "Flags.h"

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
	inline bool IsValidIndex(int32 index)
	{
		return index >= 0 && index < numElements;
	}
	inline T& operator[](int32 index)
	{
		return data[index];
	}
};

class FString : public TArray<wchar_t>
{
public:
	inline std::string ToString()
	{
		std::wstring wStr(data);
		return std::string(wStr.begin(), wStr.end());
	}
	inline std::wstring ToWString()
	{
		return std::wstring(data);
	}
};


class FName
{
public:
	int32 comparisonIndex;
	int32 number;

	std::string ToString() const;
};

class UClass;
class UProperty;


class UObject
{
public:
	void** Vft;
	int32 flags;
	int32 internalIndex;
	UClass* privateClass;
	FName name;
	UObject* outer;

	std::string GetName() const;
	std::string GetFullName() const;
};

class FUObjectItem
{
public:
	UObject* object;
	int32 indexAndFlags;
	int32 serialNumber;
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

};

class UStruct : public UField
{
public:
	UStruct* super;
	UField* children;
	int32 propertySize;
	int32 minAlignment;
	uint8 pad_0[0x40];
};

class UScriptStruct : public UStruct
{
public:

};

class UClass : public UStruct
{
public:

};

class UFunction : public UStruct
{
public:
	
	uint32 functionFlags;
	uint16 repOffset;
	uint8 numberParams;
	int8 pad_0[0x1];
	uint16_t sizeParams;
	uint16_t offsetReturnValue;
	uint8 pad_1[0x1C];
	void* func;
};


class UProperty : public UField
{
public:
	int32 arrayDim;
	int32 elementSize;
	int8 pad_0[0x10];
	EPropertyFlags flags;
	int32_t offsetInternal;
	int8 pad_1[0x4];
	UProperty* propertyLinkNext;
	int8 pad_2[0x18];
};

class UStructProperty : public UProperty
{
public:

};

class UObjectPropertyBase : public UProperty
{
public:

};

class UObjectProperty : public UObjectPropertyBase
{
public:

};

class UArrayProperty : public UProperty
{
public:

};

class UDelegateProperty : public UProperty
{
public:

};

class UMulticastDelegateProperty : public UProperty
{
public:

};

class UMapProperty : public UProperty
{
public:

};

class USetProperty : public UProperty
{
public:

};

class UEnumProperty : public UProperty
{
public:

};


class UNumericProperty : public UProperty
{
public:

};


