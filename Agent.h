#pragma once
#include <vector>

class Agent
{
public:
	Agent(unsigned int ID);
	unsigned int id;
	std::vector<float> xPositions;
	std::vector<float> yPositions;
	std::vector<float> orientations;
	std::vector<unsigned int> stateIDs;

	void Agent::addX(float);
	void Agent::addY(float);
	void Agent::addOrientation(float);
	void Agent::addStateID(unsigned int);
};