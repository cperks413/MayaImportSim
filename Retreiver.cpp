#include "Retreiver.h"
#include <maya/MDGModifier.h>
#include <maya/MSelectionList.h>
#include <maya/MFnSet.h>
#include <maya/MItDag.h>
#include <maya/MPlug.h>


MDagPath Retreiver::retreive(const char* name)
{
	MStatus stat;

	// iterate through all the nodes in the DAG
	MItDag dagIter(MItDag::kDepthFirst, MFn::kInvalid, &stat);
	for (; !dagIter.isDone(); dagIter.next()) {
		MDagPath dagPath;
		stat = dagIter.getPath(dagPath);

		MFnDagNode nodeFn;
		nodeFn.setObject(dagPath);
		if (strcmp(nodeFn.name().asChar(), name) == 0) {
			return dagPath;
		}
	}

	return MDagPath();
}

void Retreiver::create(const char* name)
{
	MFnDependencyNode fnpSphere;
	MObject objpSphere = fnpSphere.create("polyCylinder");
	MFnDagNode fnpSphereTrans;

	MObject objpSphereTrans = fnpSphereTrans.create("transform", name);
	MFnDagNode fnpSphereShape;
	MObject objpSphereShape = fnpSphereShape.create("mesh", objpSphereTrans);

	MPlug rPlug = fnpSphere.findPlug("radius");
	rPlug.setValue(0.25);

	MDGModifier dgMod;
	MPlug srcPlug = fnpSphere.findPlug("output");
	MPlug destPlug = fnpSphereShape.findPlug("inMesh");

	dgMod.connect(srcPlug, destPlug);

	dgMod.doIt();

	MSelectionList sl;
	sl.add("initialShadingGroup");
	MObject initialShadingGroupObject;
	sl.getDependNode(0, initialShadingGroupObject);
	MFnSet fns(initialShadingGroupObject);
	fns.addMember(objpSphereShape);
}