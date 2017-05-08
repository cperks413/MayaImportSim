#include <string>
#include <iostream>
#include <fstream>
#include "Agent.h"
#include "Retreiver.h"
#include <vector>
#include <maya/MSimple.h>
#include <maya/MVector.h>
#include <maya/MFnAnimCurve.h>

DeclareSimpleCommand(ImportSim, "Autodesk", "2017");
MStatus ImportSim::doIt(const MArgList& array)
{
	MString argString = array.asString(0);
	std::ifstream myFile;
	std::string standardName = argString.asChar();
	myFile.open(standardName, std::ios::in | std::ios::binary);

	char* buffer;
	char* version;
	int* agentNum;
	float* timestep;
	std::vector<Agent> agentList;
	int framecount = 0;

	if (myFile.is_open())
	{
		//read in version
		version = new char[4];
		myFile.seekg(0, ios::beg);
		buffer = new char[4];
		myFile.read(buffer, 4);
		strcpy_s(version, 4, buffer);

		//read in amount of agents and timestep
		agentNum = new int;
		myFile.read((char*)agentNum, 4);

		timestep = new float;
		myFile.read((char*)timestep, 4);

		//for each agent, read in the id and create an agent.
		for (int i = 0; i < *agentNum; i++)
		{
			buffer = new char[4];
			unsigned int* agentID;
			agentID = new unsigned int;
			myFile.read((char*)agentID, 4);
			Agent tempAgent = Agent(*agentID);
			agentList.push_back(tempAgent);
		}

		//read in all of the locations for each frame and add them to the relevent agent.
		bool finished = false;
		framecount = 0;
		while (finished == false) {
			framecount += 1;
			for (int i = 0; i < *agentNum; i++)
			{
				float* xPosition = new float;
				float* yPosition = new float;
				float* orientation = new float;
				unsigned int* stateid = new unsigned int;

				myFile.read((char*)xPosition, 4);
				myFile.read((char*)yPosition, 4);
				myFile.read((char*)orientation, 4);
				myFile.read((char*)stateid, 4);

				agentList.at(i).addX(*xPosition);
				agentList.at(i).addY(*yPosition);
				agentList.at(i).addOrientation(*orientation);
				agentList.at(i).addStateID(*stateid);
			}
			int c = myFile.peek();
			if (c == EOF) {
				if (myFile.eof()) {
					finished = true;
				}
				else {
					std::cout << "error" << std::endl;
				}
			}
		}

		//outputFile.close();
		myFile.close();
	}
	else {
		std::cout << "File not Found" << "\n" << std::endl;
	}

	Retreiver main;
	//make an object for each agent
	for (int i = 0; i < agentList.size(); i++)
	{
		std::string s = "pSphere" + std::to_string(i);
		const char* strNum;
		strNum = s.c_str();
		//create the object and retreive the DAGpath
		main.create(strNum);
		MFnTransform node = main.retreive(strNum).transform();
		float xVal = agentList.at(i).xPositions.at(0);
		float yVal = agentList.at(i).yPositions.at(0);
		//set the DAGpath initial values.
		node.setTranslation(MVector(xVal, 0, yVal), MSpace::kTransform);
	}

	std::cout << "framecount: " << framecount << std::endl;

	//set the animation changes for each agent
	for (int i = 0; i < agentList.size(); i++)
	{
		std::string s = "pSphere" + std::to_string(i);
		const char* strNum;
		strNum = s.c_str();
		MDagPath mObject = main.retreive(strNum);
		MFnDagNode fnSet(mObject);
		const MObject attrX = fnSet.attribute("translateX");

		MFnAnimCurve acFnSetX;
		acFnSetX.create(mObject.transform(), attrX);

		const MObject attrZ = fnSet.attribute("translateZ");
		MFnAnimCurve acFnSetZ;
		acFnSetZ.create(mObject.transform(), attrZ);

		//set the locations for each frame
		for (int j = 1; j <= framecount; j++) {
			float x = agentList.at(i).xPositions.at(j - 1);
			float z = agentList.at(i).yPositions.at(j - 1);

			MTime tm((double)j, MTime::kFilm);
			if ((MS::kSuccess != acFnSetX.addKeyframe(tm, x)) ||
				(MS::kSuccess != acFnSetZ.addKeyframe(tm, z))) {
				cerr << "Error setting the keyframe\n";
			}
		}

	}


	return MS::kSuccess;
}