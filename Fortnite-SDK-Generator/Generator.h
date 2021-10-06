#pragma once
#include "ObjectStore.h"
#include "Package.h"

class Generator
{
private:
	friend class Package;

	struct PredefinedMember
	{
		std::string type;
		std::string name;
		int32 size;
	};
	struct PredefinedFunction
	{
		bool bDefineInHeader;
		std::string templateText;
		std::string declaration;
		std::string outOfLineDeclaration;
		std::string body;
	};
	enum class FileType
	{
		Parameter,
		Function,
		Struct,
		Class,
		OtherHeader,
		OtherSource
	};

	static fs::path genPath;
	static std::unordered_map<std::string, std::vector<Generator::PredefinedMember>> predefinedMembers;
	static std::unordered_map<std::string, std::vector<Generator::PredefinedFunction>> predefinedFunctions;

public:
	Generator();

	static void Generate();

private:
	void ProcessPackages(const fs::path& sdkPath, std::vector<std::string>& outNames);

	void CreateSDKHeaderFile(const fs::path& sdkPath, const std::vector<std::string>& packageNames);

	static void SetStream(const fs::path&& sdkPath, std::ofstream& stream, FileType type, std::string packageName);

	void PrintFileHeader(std::ofstream& stream, const FileType& ft) const;
	void PrintFileEnding(std::ofstream& stream, const FileType& ft) const;

	void GenerateStructsFile(const std::vector<Package::Struct>& structs, const std::vector<Package::Enum>& enums, std::string packageName) const;
	void GenerateClassFile(const std::vector<Package::Class>& classes, std::string packageName) const;
	void GenerateParameterFile(const std::vector<Package::Function>& parameters, std::string packageName) const;
	void GenerateFunctionFile(const std::vector<Package::Function>& functions, std::string packageName) const;

	void GenerateBasicFile(const fs::path& sdkPath);
};