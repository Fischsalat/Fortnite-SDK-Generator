#pragma once
#include "Global.h"
// Settings for SDK Generation [offsets in Global.h]

namespace Settings
{
	
	__forceinline std::string GetPath()
	{
		return "C:\\SDKGen";
	}
	
	__forceinline bool ShouldUseNamespaceForSDK()
	{
		return true;
	}

	__forceinline std::string GetSDKNamespace()
	{
		return "SDK";
	}
	
	__forceinline bool ShouldUseNamespaceForParams()
	{
		return true;
	}
	
	__forceinline std::string GetParamNamespace()
	{
		return "param";
	}

	__forceinline bool ShouldUseStrings()
	{
		return true;
	}

	__forceinline bool ShouldUsePrecompiledHeaders()
	{
		return true;
	}

	__forceinline std::string GetGameVersion()
	{
		return "1.2";
	}
};