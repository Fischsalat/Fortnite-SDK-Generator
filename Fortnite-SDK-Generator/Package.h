#pragma once
#include "Global.h"
#include "ClassWrapper.h"
#include "ObjectStore.h"

class Package
{
	Package(const UEObject& packageObj);

	std::string GetPackageName() const;

	void GenerateScriptStruct();
};