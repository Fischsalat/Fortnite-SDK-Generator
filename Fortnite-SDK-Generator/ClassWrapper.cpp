#pragma once
#include "ClassWrapper.h"

//UEObject
//-----------------------------------------------------------------------------------------------
bool UEObject::IsVaild() const
{
	return object != nullptr;
}
std::string UEObject::GetName() const
{
	return object->GetName();
}
UEObject UEObject::GetOuter() const
{
	return UEObject(object->outer);
}
UEClass UEObject::GetClass() const
{
	return UEClass(object->privateClass);
}
UEClass UEObject::StaticClass() const
{
	UEClass(FindClass("Class CoreUObject.Object"));
}
//UEField
//-----------------------------------------------------------------------------------------------
UEField UEField::GetNext() const
{
	return reinterpret_cast<UField*>(object)->next;
}
UEClass UEField::StaticClass() const
{
	UEClass(FindClass("Class CoreUObject.Field"));
}
//UEEnum
//-----------------------------------------------------------------------------------------------