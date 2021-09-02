#pragma once
#include "ClassWrapper.h"

//UEObject
//-----------------------------------------------------------------------------------------------
bool UEObject::IsVaild() const
{
	return object != nullptr;
}
//----------------------------------------
std::string UEObject::GetName() const
{
	return object->GetName();
}
//----------------------------------------
std::string UEObject::GetFullName() const
{
	return object->GetFullName();
}
//----------------------------------------
UEObject UEObject::GetOuter() const
{
	return UEObject(object->outer);
}
//----------------------------------------
UEClass UEObject::GetClass() const
{
	return UEClass(object->privateClass);
}
//-----------------------------------------------
UEClass UEObject::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Object"));
}
//UEField
//-----------------------------------------------------------------------------------------------
bool UEField::IsNextValid() const
{
	return static_cast<UField*>(object)->next != nullptr;
}
//----------------------------------------
UEField UEField::GetNext() const
{
	return UEField(static_cast<UField*>(object)->next);
}
//-----------------------------------------------
UEClass UEField::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Field"));
}
//UEEnum
//-----------------------------------------------------------------------------------------------
const UEnum::ECppForm UEEnum::GetCppForm() const
{
	return static_cast<UEnum*>(object)->cppForm;
}
//----------------------------------------
std::string UEEnum::GetSingleName(int32 index) const
{
	UEnum* tempEnum = static_cast<UEnum*>(object);

	if (tempEnum->names.IsValidIndex(index))
		return tempEnum->names[index].value.ToString();

	return "";
}
//----------------------------------------
std::vector<std::string> UEEnum::GetAllNames() const
{
	std::vector<std::string> returnVec;

	TArray<TPair<FName, int64>> tempArray = static_cast<UEnum*>(object)->names;

	for (int i = 0; i < tempArray.Num(); i++)
	{
		returnVec.push_back(tempArray[i].value.ToString());
	}
	return returnVec;
}
//----------------------------------------
std::string UEEnum::GetEnumType() const
{
	std::string temp = static_cast<UEnum*>(object)->cppType.ToString();

	return temp.substr(0, temp.find_first_of(":"));
}
//-----------------------------------------------
UEClass UEEnum::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Enum"));
}
//UESTruct
//-----------------------------------------------------------------------------------------------
UEStruct UEStruct::GetSuper() const
{
	return UEStruct(static_cast<UStruct*>(object)->super);
}
//----------------------------------------
UEField UEStruct::GetChildren() const
{
	return UEField(static_cast<UStruct*>(object)->children);
}
//-----------------------------------------------
UEClass UEStruct::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Struct"));
}
//UEClass
//-----------------------------------------------------------------------------------------------
UEObject UEClass::GetDefaultObject() const
{
	return UEObject(static_cast<UClass*>(object)->defaultObject);
}

