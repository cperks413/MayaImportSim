#include "Agent.h"

Agent::Agent(unsigned int ID)
{
	id = ID;
};

void Agent::addX(float input)
{
	xPositions.push_back(input);
}
void Agent::addY(float input)
{
	yPositions.push_back(input);
}
void Agent::addOrientation(float input)
{
	orientations.push_back(input);
}
void Agent::addStateID(unsigned int input)
{
	stateIDs.push_back(input);
}