#pragma once
#include "Global.h"

struct PredefinedMember
{
	std::string type;
	std::string name;
	int32 size;
};

static std::unordered_map< std::string, std::vector<PredefinedMember>> predefinedMembers;



void InitalizePredefinedMembers(void);