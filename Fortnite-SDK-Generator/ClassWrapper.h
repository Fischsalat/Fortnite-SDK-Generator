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

	inline std::string GetName() const;

	inline UEObject GetOuter() const;

	inline UEClass GetClass() const;

	UEClass StaticClass() const;
};

class UEField : public UEObject
{
public:
	using UEObject::UEObject;

	inline UEField GetNext() const;

	UEClass StaticClass() const;
};

class UEEnum
{
	UEClass StaticClass() const;
};

class UEStruct : public UEField
{
public:
	using UEField::UEField;

	UEClass StaticClass() const;
};

class UEClass : public UEStruct
{
public:
	using UEStruct::UEStruct;

	UEClass StaticClass() const;
};

class UEProperty
{
	UEClass StaticClass() const;
};

