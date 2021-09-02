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

	bool IsVaild() const;

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

	UEClass StaticClass() const;
};

class UEClass : public UEStruct
{
public:
	using UEStruct::UEStruct;

	UEObject GetDefaultObject() const;

	UEClass StaticClass() const;
};

class UEProperty
{
	UEClass StaticClass() const;
};

