#pragma once
#include "ObjectStore.h"
#include "Package.h"

class Generator
{
private:
	std::vector<Package> packagesToProcess;

private:
	enum class FileType
	{
		Parameter,
		Function,
		Struct,
		Class
	};

public:
	void ProcessPackages(const fs::path& sdkPath);

private:
	void PrintFileHeader(std::ostream& stream, const FileType& ft) const;
	void PrintFileEnding(std::ostream& stream, const FileType& ft) const;
};

