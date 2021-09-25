#pragma once
#include "Global.h"
// Settings for SDK Generation [offsets in Global.h]

namespace Settings
{
	__forceinline const std::string GetPath()
	{
		return "C:/SDKGen";
	}
	
	__forceinline const bool ShouldUseNamespaceForSDK()
	{
		return true;
	}

	__forceinline const std::string GetSDKNamespace()
	{
		return "SDK";
	}
	
	__forceinline const bool ShouldUseNamespaceForParams()
	{
		return true;
	}
	
	__forceinline const std::string GetParamNamespace()
	{
		return "param";
	}

	__forceinline const bool ShouldUseStrings()
	{
		return true;
	}

	__forceinline const bool ShouldUsePrecompiledHeaders()
	{
		return true;
	}

	__forceinline const std::string GetGameVersion()
	{
		return "1.2";
	}
};