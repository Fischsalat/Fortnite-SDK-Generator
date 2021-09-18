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
void UEObject::operator=(UEObject other)
{
	object = other.object;
}
//----------------------------------------
UObject* UEObject::GetUObject() const
{
	return object;
}
//----------------------------------------
bool UEObject::IsValid() const
{
	return object != nullptr;
}
//----------------------------------------
void* UEObject::GetAddress() const
{
	return object;
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
int32 UEObject::GetComparisonIndex() const
{
	return object->name.comparisonIndex;
}
//----------------------------------------
FName UEObject::GetFName() const
{
	return object->name;
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
std::string UEObject::GetCppName() const
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

	return MakeValidCppName(name += GetName());
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
	for (UEObject obj = GetOuter(); obj.IsValid(); obj = obj.GetOuter())
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
	return false;
}
//----------------------------------------
bool UEObject::IsA(UEClass staticClass) const
{
	if (GetClass().IsValid())
	{
		for (UEClass clss = GetClass(); clss.IsValid(); clss = clss.GetSuper().Cast<UEClass>())
		{
			if (clss == staticClass)
				return true;
		}
		return false;
	}
	return false;
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
		std::string fullEnumName = tempArray[i].value.ToString();
		int d = fullEnumName.find_last_of(":");

		d != NPOS ? returnVec.push_back(fullEnumName.substr(d + 1)) : returnVec.push_back(fullEnumName);
	}
	return returnVec;
}
//----------------------------------------
std::string UEEnum::GetEnumTypeAsStr() const
{
	std::string temp = GetName();

	return temp[0] == 'E' ?  "enum class " + temp : "enum class E" + temp;
}
//-----------------------------------------------
UEClass UEEnum::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Enum");
	return staticClass;
}
//UESTruct
//-----------------------------------------------------------------------------------------------
std::string UEStruct::GetUniqueName() const
{
	std::string name;

	if (UEObjectStore::CountObjects(UEStruct::StaticClass(), GetName()) > 1)
		name += GetCppName() + "_" + std::to_string(GetFName().number);
	else
		return name += GetCppName();
}
//----------------------------------------
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
std::string UEFunction::GetFlagsAsString() const
{
	return static_cast<UFunction*>(object)->GetFlagsAsString();
}
//----------------------------------------
bool UEFunction::HasFlag(EFunctionFlags flag) const
{
	return static_cast<UFunction*>(object)->functionFlags & flag;
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
	return static_cast<UProperty*>(object)->propertyFlags;
}
//----------------------------------------
std::string UEProperty::GetFlagsAsString() const
{
	return static_cast<UProperty*>(object)->GetFlagsAsString();
}
//----------------------------------------
bool UEProperty::HasFlag(EPropertyFlags flag) const
{
	return static_cast<UProperty*>(object)->propertyFlags & flag;
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
	return std::format("TArray<{}>", GetInnerProperty().GetPropertyType().second);
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
	return std::format("struct {}", GetInnerStruct().GetCppName()); // this may cause wrong prefixed
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
	return std::format("class {}*", GetObjPropertyClass().GetCppName());
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
// UE_TextProperty
//-----------------------------------------------------------------------------------------------
std::string UE_TextProperty::GetTypeStr() const
{
	return "class FText";
}
//-----------------------------------------------
UEClass UE_TextProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.TextProperty");
	return staticClass;
}
// UE_StringProperty
//-----------------------------------------------------------------------------------------------
std::string UE_StrProperty::GetTypeStr() const
{
	return "class FString";
}
//-----------------------------------------------
UEClass UE_StrProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.StrProperty");
	return staticClass;
}
// UE_NameProperty
//-----------------------------------------------------------------------------------------------
std::string UE_NameProperty::GetTypeStr() const
{
	return "class FName";
}
//-----------------------------------------------
UEClass UE_NameProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.NameProperty");
	return staticClass;
}
// UE_EnumProperty
//-----------------------------------------------------------------------------------------------
std::pair<PropertyType, std::string> UE_EnumProperty::GetUnerlyingType() const
{
	return UEProperty(static_cast<UEnumProperty*>(object)->underlayingType).GetPropertyType();
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
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.EnumProperty");
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
	return std::format("TSet<{}>", GetElementProperty().GetPropertyType().second);
}
//-----------------------------------------------
UEClass UE_SetProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.SetProperty");
	return staticClass;
}
// UE_MapProperty
//-----------------------------------------------------------------------------------------------
UEProperty UE_MapProperty::GetKeyProperty() const
{
	return UEProperty(static_cast<UMapProperty*>(object)->keyProp);
}
//----------------------------------------
UEProperty UE_MapProperty::GetValueProperty() const
{
	return UEProperty(static_cast<UMapProperty*>(object)->valueProp);
}
//----------------------------------------
int32 UE_MapProperty::GetKeyOffset() const
{
	return static_cast<UMapProperty*>(object)->keyOffset;
}
//----------------------------------------
int32 UE_MapProperty::GetValueOffset() const
{
	return static_cast<UMapProperty*>(object)->valueOffset;
}
//----------------------------------------
int32 UE_MapProperty::GetElementOffset() const
{
	return static_cast<UMapProperty*>(object)->elementOffset;
}
//----------------------------------------
int32 UE_MapProperty::GetMapSize() const
{
	return static_cast<UMapProperty*>(object)->mapSize;
}
//----------------------------------------
int32 UE_MapProperty::GetAlignmet() const
{
	return static_cast<UMapProperty*>(object)->alignment;
}
//----------------------------------------
std::string UE_MapProperty::GetTypeStr() const
{
	return std::format("TMap<{}, {}>", GetKeyProperty().GetPropertyType().second, GetValueProperty().GetPropertyType().second);
}
//-----------------------------------------------
UEClass UE_MapProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.MapProperty");
	return staticClass;
}
// UE_MulticastDelegateProperty
//-----------------------------------------------------------------------------------------------
UEFunction UE_MulticastDelegateProperty::GetSignatureFunction() const
{
	return UEFunction(static_cast<UMulticastDelegateProperty*>(object)->signatureFunction);
}
//----------------------------------------
std::string UE_MulticastDelegateProperty::GetTypeStr() const
{
	return "struct FScriptMulticastDelegate";
}
//-----------------------------------------------
UEClass UE_MulticastDelegateProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.MulticastDelegateProperty");
	return staticClass;
}
// UE_WeakObjectProperty
//-----------------------------------------------------------------------------------------------
std::string UE_WeakObjectProperty::GetTypeStr() const
{
	return std::format("struct TWeakObjectPtr<{}>", GetObjPropertyClass().GetName());
}
//-----------------------------------------------
UEClass UE_WeakObjectProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.WeakObjectProperty");
	return staticClass;
}
// UE_InterfaceProperty
//-----------------------------------------------------------------------------------------------
std::string UE_InterfaceProperty::GetTypeStr() const
{
	return std::format("struct FScriptInterface<{}>", GetObjPropertyClass().GetName());
}
//-----------------------------------------------
UEClass UE_InterfaceProperty::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.InterfaceProperty");
	return staticClass;
}
// UE_BoolProperty
//-----------------------------------------------------------------------------------------------
uint8 UE_boolProperty::GetFieldSize() const
{
	return static_cast<UBoolProperty*>(object)->fieldSize;
}
//----------------------------------------
uint8 UE_boolProperty::GetByteOffset() const
{
	return static_cast<UBoolProperty*>(object)->byteOffset;
}
//----------------------------------------
uint8 UE_boolProperty::GetByteMask() const
{
	return static_cast<UBoolProperty*>(object)->byteMask;
}
//----------------------------------------
uint8 UE_boolProperty::GetFieldMask() const
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
		return "uint8";
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

//UEPackage
UEClass UEPackage::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class CoreUObject.Package");
	return staticClass;
}
//UEActor
//-----------------------------------------------------------------------------------------------
UEClass UEActor::StaticClass()
{
	static UEClass staticClass = UEObjectStore::FindClass("Class Engine.Actor");
	return staticClass;
}

std::pair<PropertyType, std::string> UEProperty::GetPropertyType() const
{
	if (IsA<UE_byteProperty>()) { return { PropertyType::ByteProperty, Cast<UE_byteProperty>().GetTypeStr() }; };
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
	if (IsA<UE_ArrayProperty>()) { return { PropertyType::ArrayProperty, Cast<UE_ArrayProperty>().GetTypeStr() }; };
	if (IsA<UE_EnumProperty>()) { return { PropertyType::EnumProperty, Cast<UE_EnumProperty>().GetTypeStr() }; };
	if (IsA<UE_SetProperty>()) { return { PropertyType::SetProperty, Cast<UE_SetProperty>().GetTypeStr() }; };
	if (IsA<UE_MapProperty>()) { return { PropertyType::MapProperty, Cast<UE_MapProperty>().GetTypeStr() }; };
	if (IsA<UE_InterfaceProperty>()) { return { PropertyType::InterfaceProperty, Cast<UE_InterfaceProperty>().GetTypeStr() }; };
	if (IsA<UE_MulticastDelegateProperty>()) { return { PropertyType::MulticastDelegateProperty, Cast<UE_MulticastDelegateProperty>().GetTypeStr() }; };
	if (IsA<UE_WeakObjectProperty>()) { return { PropertyType::WeakObjectProperty, Cast<UE_WeakObjectProperty>().GetTypeStr() }; };
	if (IsA<UE_ObjectProperty>()) { return { PropertyType::ObjectProperty, Cast<UE_ObjectProperty>().GetTypeStr() }; };
	if (IsA<UE_ObjectProperty>()) { return { PropertyType::Unknown, Cast<UEProperty>().GetName() }; };
	return { PropertyType::Unknown, GetClass().GetName() };
}