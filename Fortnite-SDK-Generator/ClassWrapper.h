#pragma once
#include "Global.h"
#include "CoreClasses.h"
#include "CoreFunctions.h"

class UEField;
class UEClass;

class UEObject
{
protected:
public:	
	UObject* object;

public:
	UEObject() : object(nullptr)
	{
	}
	UEObject(UObject* obj) : object(obj) 
	{
	}

	bool operator==(UEObject other) const;
	bool operator!=(UEObject other) const;

	bool IsValid() const;
	int32 GetFlags() const;
	int32 GetInernalIndex() const;
	std::string GetName() const;
	std::string GetFullName() const;
	std::string GetPrefixedName() const;
	UEObject GetOuter() const;
	UEClass GetClass() const;
	UEObject GetPackage() const;

	template<typename T>
	bool IsA() const;
	
	template<typename T>
	T Cast() const;

	static UEClass StaticClass();
};

class UEField : public UEObject
{
public:
	using UEObject::UEObject;

	bool IsNextValid() const;
	UEField GetNext() const;

	static UEClass StaticClass();
};

class UEEnum : public UEField
{
public:
	using UEField::UEField;
	
	const UEnum::ECppForm GetCppForm() const;
	std::string GetSingleName(int32 index) const;
	std::vector<std::string> GetAllNames() const;
	std::string GetEnumTypeAsStr() const;

	static UEClass StaticClass();
};

class UEStruct : public UEField
{
public:
	using UEField::UEField;

	UEStruct GetSuper() const;
	UEField GetChildren() const;
	int32 GetStructSize() const;
	int32 GetAlignmetn() const;

	static UEClass StaticClass();
};

class UEClass : public UEStruct
{
public:
	using UEStruct::UEStruct;

	UEObject GetDefaultObject() const;

	static UEClass StaticClass();
};

class UEFunction : public UEStruct
{
public:
	using UEStruct::UEStruct;

	EFunctionFlags GetFunctionFlags() const;
	uint8 GetNumParams() const;
	uint16 GetParamSize() const;
	uint16 GetRetOffset() const;

	static UEClass StaticClass();
};

class UEProperty : public UEField
{
public:
	UEField::UEField;

	int32 GetElementSize() const;
	int32 GetArrayDim() const;
	EPropertyFlags GetPropertyFlags() const;
	int32 GetOffset() const;
	UEProperty GetNextProperty() const;

	static UEClass StaticClass();

	std::pair<enum class PropertyType, std::string> GetPropertyType() const;
};

class UE_ArrayProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UEProperty GetInnerProperty() const;
	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_StructProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UEStruct GetInnerStruct() const;
	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_ObjectProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UEClass GetObjPropertyClass() const;
	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_ClassProperty : public UE_ObjectProperty
{
public:
	using UE_ObjectProperty::UE_ObjectProperty;

	UEClass GetMetaClass() const;
	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_boolProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	int8 GetFieldSize() const;
	int8 GetByteOffset() const;
	int8 GetByteMask() const;
	int8 GetFieldMask() const;
	bool IsNormalBool() const;
	bool IsBitField() const;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_EnumProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UE_EnumProperty GetUnerlyingType() const;
	UEEnum GetEnum() const;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_SetProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	UEProperty GetElementProperty() const;
	int32 GetElementOffset() const;
	int32 GetSize() const;
	int32 GetAlignmet() const;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_NumericalProperty : public UEProperty
{
public:
	using UEProperty::UEProperty;

	static UEClass StaticClass(); 
};

class UE_byteProperty : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	UEEnum GetEnum() const;
	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_uint16Property : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_uint32Property : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_uint64Property : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_int8Property : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_int16Property : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_intProperty : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_int64Property : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_floatProperty : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};

class UE_doubleProperty : public UE_NumericalProperty
{
public:
	using UE_NumericalProperty::UE_NumericalProperty;

	std::string GetTypeStr() const;

	static UEClass StaticClass();
};


class UEActor : public UEObject
{
public:

	static UEClass StaticClass();
};

enum class PropertyType //control c control v from guitrrs dumper
{
	Unknown,
	StructProperty,
	ObjectProperty,
	SoftObjectProperty,
	FloatProperty,
	ByteProperty,
	BoolProperty,
	IntProperty,
	Int8Property,
	Int16Property,
	Int64Property,
	UInt16Property,
	UInt32Property,
	UInt64Property,
	NameProperty,
	DelegateProperty,
	SetProperty,
	ArrayProperty,
	WeakObjectProperty,
	StrProperty,
	TextProperty,
	MulticastSparseDelegateProperty,
	EnumProperty,
	DoubleProperty,
	MulticastDelegateProperty,
	ClassProperty,
	MulticastInlineDelegateProperty,
	MapProperty,
	InterfaceProperty,
	FieldPathProperty,
	SoftClassProperty
};

/*
class UObjectBase;										|
class UObjectBaseUtility;								|
class	UObject;										|
class		UField;										|
class			UEnum;									|
class			UProperty;								|
class				UByteProperty;						|
class				UUInt16Property;					|
class				UUInt32Property;					|
class				UUInt64Property;					|
class				UInt8Property;						|
class				UInt16Property;						|
class				UIntProperty;						|
class				UInt64Property;						|
class				UBoolProperty;
class				UFloatProperty;						|
class				UDoubleProperty;					|
class				UObjectPropertyBase;				|
class				UObjectProperty;					|
class					UClassProperty;					|
class					UInterfaceProperty;
class					UWeakObjectProperty;
class					ULazyObjectProperty;
class					UAssetObjectProperty;
class				UNameProperty;
class				UStructProperty;					|
class				UStrProperty;
class				UTextProperty;
class				UArrayProperty;						|
class				UDelegateProperty;
class				UMulticastDelegateProperty;
class				UMapProperty;
class				USetProperty;
class				UEnumProperty;						|
class			UStruct;								|
class				UFunction;							|
class				UClass;								|
class				UScriptStruct;						|
*/