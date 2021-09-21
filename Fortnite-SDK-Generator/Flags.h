// Copyright 1998-2021 Epic Games, Inc. All Rights Reserved.
/* [For the flags and the general data types like UObject, TArray etc.] */
#pragma once


enum class EPropertyFlags : uint64
{
	None = 0,

	Edit							= 0x0000000000000001,
	ConstParm						= 0x0000000000000002,
	BlueprintVisible				= 0x0000000000000004,
	ExportObject					= 0x0000000000000008,
	BlueprintReadOnly				= 0x0000000000000010,
	Net								= 0x0000000000000020,
	EditFixedSize					= 0x0000000000000040,
	Parm							= 0x0000000000000080,	
	OutParm							= 0x0000000000000100,
	ZeroConstructor					= 0x0000000000000200,
	ReturnParm						= 0x0000000000000400,
	DisableEditOnTemplate			= 0x0000000000000800,
	Transient						= 0x0000000000002000,
	Config							= 0x0000000000004000,
	DisableEditOnInstance			= 0x0000000000010000,
	EditConst						= 0x0000000000020000,
	GlobalConfig					= 0x0000000000040000,
	InstancedReference				= 0x0000000000080000,
	DuplicateTransient				= 0x0000000000200000,
	SubobjectReference				= 0x0000000000400000,
	SaveGame						= 0x0000000001000000,
	NoClear							= 0x0000000002000000,
	ReferenceParm					= 0x0000000008000000,
	BlueprintAssignable				= 0x0000000010000000,
	Deprecated						= 0x0000000020000000,
	IsPlainOldData					= 0x0000000040000000,
	RepSkip							= 0x0000000080000000,
	RepNotify						= 0x0000000100000000,
	Interp							= 0x0000000200000000,
	NonTransactional				= 0x0000000400000000,
	EditorOnly						= 0x0000000800000000,
	NoDestructor					= 0x0000001000000000,
	AutoWeak						= 0x0000004000000000,
	ContainsInstancedReference		= 0x0000008000000000,
	AssetRegistrySearchable			= 0x0000010000000000,
	SimpleDisplay					= 0x0000020000000000,
	AdvancedDisplay					= 0x0000040000000000,
	Protected						= 0x0000080000000000,
	BlueprintCallable				= 0x0000100000000000,
	BlueprintAuthorityOnly			= 0x0000200000000000,
	TextExportTransient				= 0x0000400000000000,
	NonPIEDuplicateTransient		= 0x0000800000000000,
	ExposeOnSpawn					= 0x0001000000000000,
	PersistentInstance				= 0x0002000000000000,
	UObjectWrapper					= 0x0004000000000000,
	HasGetValueTypeHash				= 0x0008000000000000,
	NativeAccessSpecifierPublic		= 0x0010000000000000,
	NativeAccessSpecifierProtected	= 0x0020000000000000,
	NativeAccessSpecifierPrivate	= 0x0040000000000000,
	SkipSerialization				= 0x0080000000000000,
};

enum class EFunctionFlags : uint32
{
	None = 0x00000000,

	Final							= 0x00000001,
	RequiredAPI						= 0x00000002,
	BlueprintAuthorityOnly			= 0x00000004,
	BlueprintCosmetic				= 0x00000008,
	Net								= 0x00000040,
	NetReliable						= 0x00000080,
	NetRequest						= 0x00000100,
	Exec							= 0x00000200,
	Native							= 0x00000400,
	Event							= 0x00000800,
	NetResponse						= 0x00001000,
	Static							= 0x00002000,
	NetMulticast					= 0x00004000,
	UbergraphFunction				= 0x00008000,
	MulticastDelegate				= 0x00010000,
	Public							= 0x00020000,
	Private							= 0x00040000,
	Protected						= 0x00080000,
	Delegate						= 0x00100000,
	NetServer						= 0x00200000,
	HasOutParms						= 0x00400000,
	HasDefaults						= 0x00800000,
	NetClient						= 0x01000000,
	DLLImport						= 0x02000000,
	BlueprintCallable				= 0x04000000,
	BlueprintEvent					= 0x08000000,
	BlueprintPure					= 0x10000000,
	EditorOnly						= 0x20000000,
	Const							= 0x40000000,
	NetValidate						= 0x80000000,

	AllFlags						= 0xFFFFFFFF,
};

inline bool operator&(EPropertyFlags left, EPropertyFlags right)
{
	return ((int64)left & (int64)right) == (int64)right;
}

inline bool operator&(EFunctionFlags left, EFunctionFlags right)
{
	return ((int32)left & (int32)right) == (int32)right;
}
