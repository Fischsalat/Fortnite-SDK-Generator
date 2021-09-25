#include "Predefined.h"

void InitalizePredefinedMembers(void)
{
	predefinedMembers["Class CoreUObject.Object"] =
	{
		{ "void*", "vft", 0x08 },
		{ "int32_t", "flags", 0x04 },
		{ "int32_t", "index", 0x04 },
		{ "class UClass*", "objClass", 0x08 },
		{ "class FName", "name", 0x08},
		{ "class UObject", "outer", 0x08 }
	};

	predefinedMembers["Class CoreUObject.Field"] =
	{
		{ "class UField*", "next", 0x08 },
	};

	predefinedMembers["Class CoreUObject.Enum"] =
	{
		{ "class FString", "cppType", 0x0C },
		{ "TArray<TPari<FName, int64_t>>", "names", 0x0C},
		{ "int64_t", " cppForm", 0x08 },
		{ "uint8_t", "pad1[0x10]", 0x10 }
	};

	predefinedMembers["Class CoreUObject.Struct"] =
	{
		{ "class UStruct*", "super", 0x08 },
		{ "class UField*", "children", 0x08 },
		{ "int32_t", " propertySize", 0x04 },
		{ "int32_t", "minAlignment", 0x04 },
		{ "uint8_t", "pad2[0x40]", 0x40 }
	};

	predefinedMembers["Class CoreUObject.Class"] =
	{
		{ "uint8_t", "pad3[0x88]", 0x88 },
		{ "class UObject*", "defaultObject", 0x08 }
	};

	predefinedMembers["Class CoreUObject.Function"] =
	{
		{ "uint32_t", "funcFlags", 0x04 },
		{ "uint16_t", "repOffset", 0x02 },
		{ "int8_t", " numberParams", 0x01 },
		{ "int8_t", "pad[0x1]", 0x01 },
		{ "int16_t", "sizeParams", 0x02 },
		{ "int16_t", "offsetReturnValue", 0x02 },
		{ "uint8_t", "pad5[0x1C]", 0x1C },
		{ "void*", "func", 0x8 }
	};

	predefinedMembers["Class CoreUObject.Property"] =
	{
		{ "uint32_t", "arrayDim", 0x04 },
		{ "uint32_t", "elementSize", 0x04 },
		{ "int8_t", "pad6[0x10]", 0x10 },
		{ "uint64_t", "propertyFlags", 0x08 },
		{ "uint32_t", "offsetInternal", 0x04 },
		{ "int8_t", "pad7[0x04]", 0x04 },
		{ "uint8_t", "pad5[0x1C]", 0x1C },
		{ "class UProperty*", "propertyLinkNext", 0x8 },
		{ "int8_t", "pad7[0x18]", 0x18 }
	};
}
