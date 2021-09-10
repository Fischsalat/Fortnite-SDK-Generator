#pragma once
#include "Global.h"
#include "ClassWrapper.h"
#include "ObjectStore.h"

class Package
{
private:
	struct Member
	{
		std::string type;
		std::string name;
		int32 offset;
		int32 size;
	}; 
	struct Enum
	{
		std::string fullName;
		std::string underlayingType;
		std::string name;
	};
	struct Function
	{
		std::string fullName;
		std::string cppName;
		int32 flags; // not finished
	}; 
	struct Struct
	{
		std::string fullName;
		std::string cppName;
		int32 inheritedSize;
		int32 structSize;
		std::vector<Member> membersTypes;
		std::vector<Function> functions;
	};
	
};