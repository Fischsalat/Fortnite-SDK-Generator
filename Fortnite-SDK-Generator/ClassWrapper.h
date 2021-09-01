#pragma once
#include "Global.h"
#include "NakedClasses.h"

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

	inline bool IsVaild() const
	{
		return object != nullptr;
	}

	inline std::string GetName() const
	{
		return object->GetName();
	}
	inline UEObject GetOuter() const
	{
		return UEObject(object->outer);
	}
	inline UEClass GetClass() const
	{
		return UEClass(object->privateClass);
	}
};

class UEField : public UEObject
{
public:
	using UEObject::UEObject;

	inline UEField GetNext() const
	{
		return reinterpret_cast<UField*>(object)->next;
	}
};

class UEStruct : public UEField
{
public:
	using UEField::UEField;

};

class UEClass : public UEStruct
{
public:
	using UEStruct::UEStruct;
};

class UEEnum
{

};

class UEProperty
{

};

