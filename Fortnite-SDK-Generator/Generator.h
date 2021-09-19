#pragma once
#include "ObjectStore.h"
#include "Package.h"

class Generator
{
private:
	friend class Package;

	fs::path genPath;

private:
	enum class FileType
	{
		Parameter,
		Function,
		Struct,
		Class
	};

public:
	void Generate();

private:
	void ProcessPackages(const fs::path& sdkPath, std::vector<std::string>& outNames);

	void CreateSDKHeaderFile(const fs::path& sdkPath, const std::vector<std::string>& packageNames);

	static void SetStream(const fs::path&& sdkPath, std::ofstream& stream, FileType type, std::string packageName);

	void PrintFileHeader(std::ofstream& stream, const FileType& ft) const;
	void PrintFileEnding(std::ofstream& stream, const FileType& ft) const;

	void GenerateStructsFile(std::ofstream& stream, const std::vector<Package::Struct>& structs, const std::vector<Package::Enum>& enums, std::string packageName) const;
	void GenerateClassFile(std::ofstream& stream, const std::vector<Package::Class>& classes, std::string packageName) const;
	void GenerateParameterFile(std::ofstream& stream, const std::vector<Package::Function>& parameters, std::string packageName) const;
	void GenerateFunctionFile(std::ofstream& stream, const std::vector<Package::Function>& functions, std::string packageName) const;
};

