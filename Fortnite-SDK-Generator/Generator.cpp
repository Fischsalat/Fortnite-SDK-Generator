#pragma once
#include "Generator.h"


void Generator::ProcessPackages(const fs::path& sdkPath)
{
	std::vector<UEObject> packageObjects;
	std::unordered_map<UEObject, bool> processedObjects;

	UEObjectStore::GetAllPackages(packageObjects);

	for (auto packageObj : packageObjects)
	{
		Package pack(packageObj);

		pack.Process(packageObj, processedObjects);
	}
}

void Generator::PrintFileHeader(std::ostream& stream, const Generator::FileType& ft) const
{
	stream << std::format("#pragma once\n\n//Fortnite {}\n\n", Settings::GetGameVersion());

	stream << "#ifdef _MSC_VER\n\t#pragma pack(push, 0x8)\n#endif\n\n";

	if (ft == FileType::Function && Settings::ShouldUsePrecompiledHeaders())
		stream << "#include \"pch.h\"\n#include \"../pch.h\"\n\n";
	else if (ft == FileType::Function)
		stream << "#include \"../SDK.h\"\n\n";

	if (Settings::ShouldUseNamespaceForSDK)
		stream << "namespace " << Settings::GetSDKNamespace() << "\n{\n";

	if (ft == FileType::Function && Settings::ShouldUseNamespaceForParams())
		stream << "namespace " << Settings::GetParamNamespace() << "\n{\n";	
}

void Generator::PrintFileEnding(std::ostream& stream, const Generator::FileType& ft) const
{
	if (ft == FileType::Function && Settings::ShouldUseNamespaceForParams())
		stream << "\n\n}\n";

	if (Settings::ShouldUseNamespaceForSDK())
		stream << "\n}\n\n";

	stream << "#ifdef _MSC_VER\n\t#pragma pack(pop)\n#endif\n";
}