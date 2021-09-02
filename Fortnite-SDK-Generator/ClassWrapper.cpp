#pragma once
#include "ClassWrapper.h"

//UEObject
//-----------------------------------------------------------------------------------------------
bool UEObject::IsVaild() const
{
	return object != nullptr;
}
//----------------------------------------
bool UEObject::operator==(UEObject other) const
{
	return object == other.object;
}
//----------------------------------------
bool UEObject::operator!=(UEObject other) const
{
	return object != other.object;
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
//----------------------------------------
int32 UEStruct::GetStructSize() const
{
	return static_cast<UStruct*>(object)->propertySize;
}
//----------------------------------------
int32 UEStruct::GetAlignmetn() const
{
	return static_cast<UStruct*>(object)->minAlignment;
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
//-----------------------------------------------
UEClass UEClass::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Class"));
}
//UEFunction
//-----------------------------------------------------------------------------------------------
EFunctionFlags UEFunction::GetFunctionFlags() const
{
	return static_cast<UFunction*>(object)->functionFlags;
}
//----------------------------------------
uint8 UEFunction::GetNumParams() const
{
	return static_cast<UFunction*>(object)->numberParams;
}
//----------------------------------------
uint16 UEFunction::GetParamSize() const
{
	return static_cast<UFunction*>(object)->sizeParams;
}
uint16 UEFunction::GetRetOffset() const
{
	return static_cast<UFunction*>(object)->offsetReturnValue;
}
//-----------------------------------------------
UEClass UEFunction::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Function"));
}
//UEProperty
//-----------------------------------------------------------------------------------------------
int32 UEProperty::GetElementSize() const
{
	return static_cast<UProperty*>(object)->elementSize;
}
//----------------------------------------
int32 UEProperty::GetArrayDim() const
{
	return static_cast<UProperty*>(object)->arrayDim;
}
//----------------------------------------
EPropertyFlags UEProperty::GetPropertyFlags() const
{
	return static_cast<UProperty*>(object)->propFlags;
}
//----------------------------------------
int32 UEProperty::GetOffset() const
{
	return static_cast<UProperty*>(object)->offsetInternal;
}
//----------------------------------------
UEProperty UEProperty::GetNextProperty() const
{
	return UEProperty(static_cast<UProperty*>(object)->propertyLinkNext);
}
//-----------------------------------------------
UEClass UEProperty::StaticClass() const
{
	return UEClass(FindClass("Class CoreUObject.Property"));
}
