#pragma once
#include "Global.h"
// Settings for SDK Generation [offsets in Global.h]

namespace Settings
{
	/// <summary>
	/// The folder to put the generated SDK in
	/// </summary>
	std::string path = "C:\\SDKGen";

	/// <summary>
	/// If the SDK should be in a namespace [Default: true]
	/// </summary>
	bool bUseNamespaceForSDK = true;

	std::string namespaceForSDK = "";

	/// <summary>
	/// If parameter files should have their own sub-namespace [Default: true]
	/// </summary>
	bool bMakeNamespaceForParams = true;

	/// <summary>
	/// The sub-namespace for parameters (only relevant if bMakeNamespaceForParams = true) [Default: param]
	/// </summary>
	std::string paramNamespaceName = "param";
};