#pragma once
#include "Global.h"
#include "ClassWrapper.h"
#include "ObjectStore.h"

bool CompareProperties(UEProperty left, UEProperty right);

class Package
{
private: 
	UEObject packageObj;

public:
	Package(UEObject _packageObj) : packageObj(_packageObj)
	{
	}

private:
	struct Member
	{
		bool bIsBitField;					// ture/false like bruh, I don't think I need to document the states of a bool
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
	struct Function
	{
		std::string fullName;				// Function Engine.PlayerController.SwitchLevel
		std::string cppName;				// SwitchLevel
		std::string cppFullName;			// void APlayerController::SwitchLevel(FString URL);
		int32 numParams;					// 1
		int32 flags;						// 0x400
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
	struct Class : public Struct
	{
		std::vector<Function> functions;
	};

private:

	static std::vector<Struct> structs;
	static std::vector<Class> classes;


	bool IsDependend(const UEObject& obj);

	void GeneratePrequesites(const UEObject& obj, std::unordered_map<UEObject, bool>& processedObjects);

	void GenerateMemberPrequesites(const UEObject& obj, std::unordered_map<UEObject, bool>& processedObjects);
	

	void PrintClass(const Class& clss);
	void PrintStruct(const Struct& strct);
	void PrintEnum(const Enum& enm);
	void PrintFunction(const Function& func);
	void PrintMembers();

	void GenerateMembers(const std::vector<UEProperty>&, const UEStruct& super, std::vector<Member>& outMembers);
	Function GenerateFunction(const UEFunction& function);
	Struct GenerateScritStruct(const UEStruct& strct);
	Class GenerateClass(const UEClass& clss);
	Enum GenerateEnumClass(const UEEnum& enm);

	Member GenerateBytePadding(int32 id, int32 offset, int32 padSize, std::string reason);
	Member GenerateBitPadding(); // fix later

};