#pragma once
#include "Global.h"
#include "ClassWrapper.h"

bool CompareProperties(UEProperty left, UEProperty right);

class Package
{

private: 
	UEObject packageObj;
	static std::unordered_set<int32> generatedStructs;

public:
	Package(UEObject _packageObj) : packageObj(_packageObj)
	{
	}


public:
	struct Member
	{
		std::string type;					// int32 / float / class UClass* / enum class ETextGender / class UWorld* /
		std::string name;					// bIsBeingKicked : 1; bIsAlive, TimeStartedWaiting [no prefix]
		int32 offset;						// 0x30
		int32 size;							// 0x128

		std::string comment;
	}; 
	struct Enum
	{
		std::string fullName;				// Enum Engine.ETextGender
		std::string underlayingType;		// int32
		std::string name;					// ETextGender
		std::vector<std::string> members;	// { Idk = 0, SomeOther = 1, OneMore = 2 }
	};
	struct Struct
	{
		std::string fullName;				// ScriptStruct CoreUObject.Vector4
		std::string cppName;				// FVector4
		std::string cppFullName;			// struct FVector4 : FVector [not real, just string layout]
		int32 inheritedSize;				// 0x28
		int32 structSize;					// 0x56
		std::vector<Member> members;		// { W, Z, Y, X }
	};
	struct Function
	{

		enum class ParameterType
		{
			Normal,
			Out,
			Return
		};

		ParameterType paramType;
		Struct selfAsStruct;
		std::string fullName;				// Function Engine.PlayerController.SwitchLevel
		std::string paramName;
		std::string superName;				// APlayerController
		std::string cppName;				// SwitchLevel
		std::string cppFullName;			// void APlayerController::SwitchLevel(FString URL);
		int32 numParams;					// 1
		int32 flags;						// 0x400
	};
	struct Class : public Struct
	{
		std::vector<Function> functions;
	};

public:
	std::vector<Enum> allEnums;
	std::vector<Struct> allStructs;
	std::vector<Class> allClasses;

public:
	void Process(UEObject object);

private:
	bool IsDependend(const UEObject& obj);

	void GeneratePrequesites(const UEObject& obj);

	void GenerateMemberPrequesites(const UEObject& obj);
	

	void PrintClass(const Class& clss);
	void PrintStruct(const Struct& strct);
	void PrintEnum(const Enum& enm);
	void PrintFunction(const Function& func);
	void PrintMembers(const std::vector<UEProperty>& mem, std::vector<Member> outMembers);

	void GenerateMembers(const std::vector<UEProperty>&, const UEStruct& super, std::vector<Member>& outMembers);
	Function GenerateFunction(const UEFunction& function, const UEStruct& super);
	Struct GenerateScritStruct(const UEStruct& strct);
	Class GenerateClass(const UEClass& clss);
	Enum GenerateEnumClass(const UEEnum& enm);

	Member GenerateBytePadding(int32 id, int32 offset, int32 padSize, std::string reason);
	Member GenerateBitPadding(); // fix later

};