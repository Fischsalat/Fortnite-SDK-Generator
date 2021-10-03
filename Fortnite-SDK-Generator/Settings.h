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
		return "params";
	}

	__forceinline const bool ShouldUseStrings()
	{
		return true;
	}

	__forceinline const bool ShouldUsePrecompiledHeaders()
	{
		return false;
	}

	__forceinline const bool ShouldXorStrings()
	{
		return false;
	}

	__forceinline const std::string XorText()
	{
		return "XORSTR";
	}

	__forceinline const std::string GetGameVersion()
	{
		return "1.2";
	}

	__forceinline const std::pair<std::vector<std::string>, std::vector<std::string>>& GetIncludes()
	{
		// std::pair<librarys, own headers> 
		static std::pair<std::vector<std::string>, std::vector<std::string>> inclues = 
		{
			{
				"windows.h",
				"iostream",
				"fstream",
			},
			{
				"Utils.h"
			}
		};
		return inclues;
	}

	__forceinline const bool ShouldIncludeUnrealTypedefs()
	{
		return true;
	}
};