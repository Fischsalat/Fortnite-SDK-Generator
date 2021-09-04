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

template<typename Value, typename Key>
class TPair
{
public:
	Value value;
	Key key;
};

class FName
{
public:
	int32 comparisonIndex;
	int32 number;

	std::string ToString() const;

	inline bool operator==(FName other)
	{
		return comparisonIndex == other.comparisonIndex;
	}
	inline bool operator!=(FName other)
	{
		return comparisonIndex != other.comparisonIndex;
	}
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
	EPropertyFlags propFlags;
	int32_t offsetInternal;
	int8 pad_30[0x4];
	UProperty* propertyLinkNext;
	int8 pad_35[0x18];
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


