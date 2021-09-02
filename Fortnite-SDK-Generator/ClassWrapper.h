#pragma once
#include "Global.h"
#include "NakedClasses.h"
#include "CoreFunctions.h"

class UEClass;

class UEObject
{
protected:
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

	bool IsVaild() const;
	int32 GetFlags() const;
	int32 GetInernalIndex() const;
	std::string GetName() const;
	std::string GetFullName() const;
	UEObject GetOuter() const;
	UEClass GetClass() const;

	UEClass StaticClass() const;
};

class UEField : public UEObject
{
public:
	using UEObject::UEObject;

	bool IsNextValid() const;
	UEField GetNext() const;

	UEClass StaticClass() const;
};

class UEEnum : public UEField
{
public:
	using UEField::UEField;
	
	const UEnum::ECppForm GetCppForm() const;
	std::string GetSingleName(int32 index) const;
	std::vector<std::string> GetAllNames() const;
	std::string GetEnumType() const;

	UEClass StaticClass() const;
};

class UEStruct : public UEField
{
public:
	using UEField::UEField;

	UEStruct GetSuper() const;
	UEField GetChildren() const;
	int32 GetStructSize() const;
	int32 GetAlignmetn() const;

	UEClass StaticClass() const;
};

class UEClass : public UEStruct
{
public:
	using UEStruct::UEStruct;

	UEObject GetDefaultObject() const;

	UEClass StaticClass() const;
};

class UEFunction : public UEStruct
{
public:
	using UEStruct::UEStruct;

	EFunctionFlags GetFunctionFlags() const;
	uint8 GetNumParams() const;
	uint16 GetParamSize() const;
	uint16 GetRetOffset() const;

	UEClass StaticClass() const;
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

	UEClass StaticClass() const;
};

