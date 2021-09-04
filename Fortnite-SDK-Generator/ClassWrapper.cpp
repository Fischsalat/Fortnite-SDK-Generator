#pragma once
#include "ClassWrapper.h"
#include "ObjectStore.h"

//UEActor
//-----------------------------------------------------------------------------------------------
UEClass UEActor::StaticClass()
{
	return UEObjectStore::FindClass("Class Engine.Actor");
}
//UEObject
//-----------------------------------------------------------------------------------------------
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
bool UEObject::IsVaild() const
{
	return object != nullptr;
}
//----------------------------------------
int32 UEObject::GetFlags() const
{
	return object->flags;
}
//----------------------------------------
int32 UEObject::GetInernalIndex() const
{
	return object->internalIndex;
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
std::string UEObject::GetPrefixedName() const
{
	std::string name;

	if (this->IsA<UEActor>())
		name += 'A';
	else if (this->IsA<UEObject>())
		name += 'U';
	else
		name += 'F';
	
	return name += this->GetName();
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
//----------------------------------------
template<typename T>
bool UEObject::IsA() const
{
	if (this->GetClass().IsVaild())
	{
		for (UEClass clss = this->GetClass(); clss.IsVaild(); clss = clss.GetSuper().Cast<UEClass>())
		{
			if (clss == T::StaticClass())
				return true;
		}
		return false;
	}
}
//----------------------------------------
template<typename T>
T UEObject::Cast() const
{
	return reinterpret_cast<T>(this);
}
//-----------------------------------------------
UEClass UEObject::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Object");
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
UEClass UEField::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Field");
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
UEClass UEEnum::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Enum");
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
UEClass UEStruct::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Struct");
}
//UEClass
//-----------------------------------------------------------------------------------------------
UEObject UEClass::GetDefaultObject() const
{
	return UEObject(static_cast<UClass*>(object)->defaultObject);
}
//-----------------------------------------------
UEClass UEClass::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Class");
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
UEClass UEFunction::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Function");
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
UEClass UEProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Property");
}
//UE_Properties
//-----------------------------------------------------------------------------------------------
// UE_ArrayProperty
//-----------------------------------------------------------------------------------------------
std::string UE_ArrayProperty::GetArrayType() const
{
	return static_cast<UArrayProperty*>(object)->innerProperty->innrStruct->GetName();
}
//----------------------------------------
std::string UE_ArrayProperty::GetTypeStr() const
{
	return "TArray<" + GetArrayType() + ">" + " ";
}
//-----------------------------------------------
UEClass UE_ArrayProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.ArrayProperty");
}
// UE_StructProperty
//-----------------------------------------------------------------------------------------------
UEStruct UE_StructProperty::GetInnerStruct() const
{
	return UEStruct(static_cast<UStructProperty*>(object)->innrStruct);
}
//----------------------------------------
std::string UE_StructProperty::GetTypeStr() const
{
	return "struct " + GetInnerStruct().GetName() + " ";
}
//-----------------------------------------------
UEClass UE_StructProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.StructProperty");
}
// UE_ObjectProperty
//-----------------------------------------------------------------------------------------------
UEClass UE_ObjectProperty::GetObjPropertyClass() const
{
	return UEClass(static_cast<UObjectProperty*>(object)->propertyClass);
}
//----------------------------------------
std::string UE_ObjectProperty::GetTypeStr() const
{
	return "class " + GetObjPropertyClass().GetName() + " ";
}
//-----------------------------------------------
UEClass UE_StructProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.ObjectProperty");
}
// UE_NumericalProperty
//-----------------------------------------------------------------------------------------------
UEClass UE_NumericalProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.NumericProperty");
}
// UE_NumericalProperties
//-----------------------------------------------------------------------------------------------
std::string UE_byteProperty::GetTypeStr() const
{
	return "uint16 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_byteProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.UInt16Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_uint16Property::GetTypeStr() const
{
	return "uint16 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_uint16Property::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.UInt16Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_uint32Property::GetTypeStr() const
{
	return "uint32 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_uint32Property::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.UInt32Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_uint64Property::GetTypeStr() const
{
	return "uint64 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_uint64Property::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.UInt64Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_int8Property::GetTypeStr() const
{
	return "int8 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_int8Property::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Int8Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_int16Property::GetTypeStr() const
{
	return "int16 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_int16Property::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Int16Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_intProperty::GetTypeStr() const
{
	return "int32 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_intProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.IntProperty");
}
//-----------------------------------------------------------------------------------------------
std::string UE_int64Property::GetTypeStr() const
{
	return "int64 " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_int64Property::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.Int64Property");
}
//-----------------------------------------------------------------------------------------------
std::string UE_floatProperty::GetTypeStr() const
{
	return "float " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_floatProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.FloatProperty");
}
//-----------------------------------------------------------------------------------------------
std::string UE_doubleProperty::GetTypeStr() const
{
	return "double " + GetName() + " ";
}
//-----------------------------------------------
UEClass UE_doubleProperty::StaticClass()
{
	return UEObjectStore::FindClass("Class CoreUObject.DoubleProperty");
}
//-----------------------------------------------------------------------------------------------

