#pragma once
#include "Global.h"
// Settings for SDK Generation [offsets in Global.h]

namespace Settings
{
	
	std::string GetPath()
	{
		return "C:\\SDKGen";
	}
	
	bool ShouldUseNamespaceForSDK()
	{
		return true;
	}

	std::string GetSDKNamespace()
	{
		return "SDK";
	}
	
	bool ShouldUseNamespaceForParams()
	{
		return true;
	}
	
	std::string GetParamNamespace()
	{
		return "param";
	}

	bool ShouldUseStrings()
	{
		return true;
	}
};