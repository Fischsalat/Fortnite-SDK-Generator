#pragma once
#include "ClassWrapper.h"
#include "ObjectStore.h"


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
bool UEObject::IsValid() const
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
	
	if (IsA<UEClass>())
	{
		if (IsA<UEActor>())
			name += 'A';
		else if (IsA<UEObject>())
			name += 'U';
	}
	else
	{
		name += 'F';
	}

	return name += GetName();
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
UEObject UEObject::GetPackage() const
{
	UEObject packageObj;
	for (UEObject obj = *this; obj.IsValid(); obj = obj.GetOuter())
	{
		packageObj = obj;
	}
	return packageObj;
}
//----------------------------------------
template<typename T>
bool UEObject::IsA() const
{
	if (GetClass().IsValid())
	{
		for (UEClass clss = GetClass(); clss.IsValid(); clss = clss.GetSuper().Cast<UEClass>())
		{
			if (clss == T::StaticClass())
				return true;
		}
		return false;
	}
}
//----------------------------------------
template<typename UE_Type>
UE_Type UEObject::Cast() const
{
	return UE_Type(object);
}
//-----------------------------------------------
UEClass UEObject::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Object");
	return staticClass;
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
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Field");
	return staticClass;
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
std::string UEEnum::GetEnumTypeAsStr() const
{
	std::string temp = GetName();

	return temp[0] == 'E' ?  temp : "E" + temp;
}
//-----------------------------------------------
UEClass UEEnum::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Enum");
	return staticClass;
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
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Struct");
	return staticClass;
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
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Class");
	return staticClass;
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
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Function");
	return staticClass;
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
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Property");
	return staticClass;
}
//UE_Properties
//-----------------------------------------------------------------------------------------------
// UE_ArrayProperty
//-----------------------------------------------------------------------------------------------
UEProperty UE_ArrayProperty::GetInnerProperty() const
{
	return UEProperty(static_cast<UArrayProperty*>(object)->innerProperty);
}
//----------------------------------------
std::string UE_ArrayProperty::GetTypeStr() const
{
	return "TArray<" + GetInnerProperty().GetPropertyType().second + ">";
}
//-----------------------------------------------
UEClass UE_ArrayProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.ArrayProperty");
	return staticClass;
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
	return "struct " + GetInnerStruct().GetPrefixedName(); // this may cause wrong prefixed
}
//-----------------------------------------------
UEClass UE_StructProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.StructProperty");
	return staticClass;
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
	return "class " + GetObjPropertyClass().GetName() + "*";
}
//-----------------------------------------------
UEClass UE_ObjectProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.ObjectProperty");
	return staticClass;
}
// UE_ClassProperty
//-----------------------------------------------------------------------------------------------
UEClass UE_ClassProperty::GetMetaClass() const
{
	return UEClass(static_cast<UClassProperty*>(object)->metaClass);
}
//----------------------------------------
std::string UE_ClassProperty::GetTypeStr() const
{
	return "class UClass*";
}
//-----------------------------------------------
UEClass UE_ClassProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.ClassProperty");
	return staticClass;
}
// UE_EnumProperty
//-----------------------------------------------------------------------------------------------
UE_EnumProperty UE_EnumProperty::GetUnerlyingType() const
{
	return UE_EnumProperty(static_cast<UEnumProperty*>(object)->underlayingType);
}
//----------------------------------------
UEEnum UE_EnumProperty::GetEnum() const
{
	return UEEnum(static_cast<UEnumProperty*>(object)->propEnum);
}
//----------------------------------------
std::string UE_EnumProperty::GetTypeStr() const
{
	return GetEnum().GetEnumTypeAsStr();
}
//-----------------------------------------------
UEClass UE_EnumProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.BoolProperty");
	return staticClass;
}
// UE_SetProperty
//-----------------------------------------------------------------------------------------------
UEProperty UE_SetProperty::GetElementProperty() const
{
	return static_cast<USetProperty*>(object)->elementProp;
}
//----------------------------------------
int32 UE_SetProperty::GetElementOffset() const
{
	return static_cast<USetProperty*>(object)->elementOffset;
}
//----------------------------------------
int32 UE_SetProperty::GetSize() const
{
	return static_cast<USetProperty*>(object)->setSize;
}
//----------------------------------------
int32 UE_SetProperty::GetAlignmet() const
{
	return static_cast<USetProperty*>(object)->alignment;
}
//----------------------------------------
std::string UE_SetProperty::GetTypeStr() const
{
	return "TSet<" + GetElementProperty().GetPropertyType().second + ">";
}
//-----------------------------------------------
UEClass UE_SetProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.SetProperty");
	return staticClass;
}
// UE_BoolProperty
//-----------------------------------------------------------------------------------------------
int8 UE_boolProperty::GetFieldSize() const
{
	return static_cast<UBoolProperty*>(object)->fieldSize;
}
//----------------------------------------
int8 UE_boolProperty::GetByteOffset() const
{
	return static_cast<UBoolProperty*>(object)->byteOffset;
}
//----------------------------------------
int8 UE_boolProperty::GetByteMask() const
{
	return static_cast<UBoolProperty*>(object)->byteMask;
}
//----------------------------------------
int8 UE_boolProperty::GetFieldMask() const
{
	return static_cast<UBoolProperty*>(object)->fieldMask;
}
//----------------------------------------
bool UE_boolProperty::IsNormalBool() const
{
	return GetFieldMask() == 0xFF;
}
//----------------------------------------
bool UE_boolProperty::IsBitField() const
{
	return !IsNormalBool();
}
//----------------------------------------
std::string UE_boolProperty::GetTypeStr() const
{
	return IsNormalBool() ? "bool" : "uint8";
}
//-----------------------------------------------
UEClass UE_boolProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.BoolProperty");
	return staticClass;
}
// UE_NumericalProperty
//-----------------------------------------------------------------------------------------------
UEClass UE_NumericalProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.NumericProperty");
	return staticClass;
}
// UE_NumericalProperties
//-----------------------------------------------------------------------------------------------
UEEnum UE_byteProperty::GetEnum() const
{
	return UEEnum(static_cast<UByteProperty*>(object)->enm);
}
//----------------------------------------
std::string UE_byteProperty::GetTypeStr() const
{
	if (GetEnum().IsValid())
		return GetEnum().GetEnumTypeAsStr();
	else
		return "int8";
}
//-----------------------------------------------
UEClass UE_byteProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.ByteProperty");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_uint16Property::GetTypeStr() const
{
	return "uint16";
}
//-----------------------------------------------
UEClass UE_uint16Property::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.UInt16Property");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_uint32Property::GetTypeStr() const
{
	return "uint32";
}
//-----------------------------------------------
UEClass UE_uint32Property::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.UInt32Property");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_uint64Property::GetTypeStr() const
{
	return "uint64";
}
//-----------------------------------------------
UEClass UE_uint64Property::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.UInt64Property");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_int8Property::GetTypeStr() const
{
	return "int8";
}
//-----------------------------------------------
UEClass UE_int8Property::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Int8Property");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_int16Property::GetTypeStr() const
{
	return "int16";
}
//-----------------------------------------------
UEClass UE_int16Property::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Int16Property");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_intProperty::GetTypeStr() const
{
	return "int32";
}
//-----------------------------------------------
UEClass UE_intProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.IntProperty");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_int64Property::GetTypeStr() const
{
	return "int64";
}
//-----------------------------------------------
UEClass UE_int64Property::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Int64Property");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_floatProperty::GetTypeStr() const
{
	return "float";
}
//-----------------------------------------------
UEClass UE_floatProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.FloatProperty");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------
std::string UE_doubleProperty::GetTypeStr() const
{
	return "double";
}
//-----------------------------------------------
UEClass UE_doubleProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.DoubleProperty");
	return staticClass;
}
//-----------------------------------------------------------------------------------------------


//UEActor
//-----------------------------------------------------------------------------------------------
UEClass UEActor::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class Engine.Actor");
	return staticClass;
}

std::pair<PropertyType, std::string> UEProperty::GetPropertyType() const
{
	if (IsA<UE_doubleProperty>()) { return { PropertyType::DoubleProperty,Cast<UE_doubleProperty>().GetTypeStr() }; };
	if (IsA<UE_floatProperty>()) { return { PropertyType::FloatProperty, Cast<UE_floatProperty>().GetTypeStr() }; };
	if (IsA<UE_intProperty>()) { return { PropertyType::IntProperty, Cast<UE_intProperty>().GetTypeStr() }; };
	if (IsA<UE_int16Property>()) { return { PropertyType::Int16Property,Cast<UE_int16Property>().GetTypeStr() }; };
	if (IsA<UE_int64Property>()) { return { PropertyType::Int64Property, Cast<UE_int64Property>().GetTypeStr() }; };
	if (IsA<UE_int8Property>()) { return { PropertyType::Int8Property, Cast<UE_int8Property>().GetTypeStr() }; };
	if (IsA<UE_uint16Property>()) { return { PropertyType::UInt16Property, Cast<UE_uint16Property>().GetTypeStr() }; };
	if (IsA<UE_uint32Property>()) { return { PropertyType::UInt32Property, Cast<UE_uint32Property>().GetTypeStr() }; }
	if (IsA<UE_uint64Property>()) { return { PropertyType::UInt64Property, Cast<UE_uint64Property>().GetTypeStr() }; };
	if (IsA<UE_TextProperty>()) { return { PropertyType::TextProperty, Cast<UE_TextProperty>().GetTypeStr() }; }
	if (IsA<UE_StrProperty>()) { return { PropertyType::TextProperty, Cast<UE_StrProperty>().GetTypeStr() }; };
	if (IsA<UE_ClassProperty>()) { return { PropertyType::ClassProperty, Cast<UE_ClassProperty>().GetTypeStr() }; };
	if (IsA<UE_StructProperty>()) { return { PropertyType::StructProperty, Cast<UE_StructProperty>().GetTypeStr() }; };
	if (IsA<UE_NameProperty>()) { return { PropertyType::NameProperty, Cast<UE_NameProperty>().GetTypeStr() }; };
	if (IsA<UE_boolProperty>()) { return { PropertyType::BoolProperty, Cast<UE_boolProperty>().GetTypeStr() }; }
	if (IsA<UE_byteProperty>()) { return { PropertyType::ByteProperty, Cast<UE_byteProperty>().GetTypeStr() }; };
	if (IsA<UE_ArrayProperty>()) { return { PropertyType::ArrayProperty, Cast<UE_ArrayProperty>().GetTypeStr() }; };
	if (IsA<UE_EnumProperty>()) { return { PropertyType::EnumProperty, Cast<UE_EnumProperty>().GetTypeStr() }; };
	if (IsA<UE_SetProperty>()) { return { PropertyType::SetProperty, Cast<UE_SetProperty>().GetTypeStr() }; };
	if (IsA<UE_MapProperty>()) { return { PropertyType::MapProperty, Cast<UE_MapProperty>().GetTypeStr() }; };
	if (IsA<UE_InterfaceProperty>()) { return { PropertyType::InterfaceProperty, Cast<UE_InterfaceProperty>().GetTypeStr() }; };
	if (IsA<UE_MulticastDelegateProperty>()) { return { PropertyType::MulticastDelegateProperty, Cast<UE_MulticastDelegateProperty>().GetTypeStr() }; };
	if (IsA<UE_WeakObjectProperty>()) { return { PropertyType::WeakObjectProperty, Cast<UE_WeakObjectProperty>().GetTypeStr() }; };
	if (IsA<UE_ObjectProperty>()) { return { PropertyType::ObjectProperty, Cast<UE_ObjectProperty>().GetTypeStr() }; };
	return { PropertyType::Unknown, GetClass().GetName() };
}