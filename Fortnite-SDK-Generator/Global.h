#pragma once
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>

#include "Offsets.h"

#define  NPOS std::string::npos

typedef __int8 int8;
typedef __int16 int16;
typedef __int32 int32;
typedef __int64 int64;

typedef unsigned __int8 uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
typedef unsigned __int64 uint64;

static class FUObjectArray* GObjects;