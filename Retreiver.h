#pragma once
#include "Maya/MDagPath.h"
#include "Maya/MFnTransform.h"
#include <vector>

class Retreiver
{
public:
	std::vector<MDagPath> dagPathList;

	MDagPath Retreiver::retreive(const char* name);
	void Retreiver::create(const char* name);
};