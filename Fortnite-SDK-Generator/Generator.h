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
	void PrintFileHeader(std::ofstream& stream, const FileType& ft) const;
	void PrintFileEnding(std::ofstream& stream, const FileType& ft) const;

	void GenerateStructsFile(std::ofstream& stream, const std::vector<Package::Struct>& structs, const std::vector<Package::Enum>& enums) const;
	void GenerateClassFile(std::ofstream& stream, const std::vector<Package::Class>& classes) const;
	void GenerateParameterFile(std::ofstream& stream, const std::vector<Package::Function>& parameters) const;
	void GenerateFunctionFile(std::ofstream& stream, const std::vector<Package::Function>& functions) const;
};

