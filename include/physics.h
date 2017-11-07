#ifndef PHYSICS_H
#define PHYSICS_H

#include "main.h"
#include <vector>

// ODE library
#define dSINGLE
#include "ode/ode.h"

const int MAX_BODIES = 1024;
const int MAX_CONTACTS = 2;

class PhysicsObject;
class PhysicsManager
{
public:
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;

private:
	vector<PhysicsObject*> objects;

public:
	void init();
	void loop();
	void destroy();

	PhysicsObject *createObject();
};

class PhysicsObject
{
public:
	dBodyID body;
	dGeomID geometry;

private:
	PhysicsManager *mgr;

public:
	PhysicsObject(PhysicsManager *mgr);

	void createSphereBody(float mass, float radius);
	void createCubeBody(float mass, vec3 surface);

	void setPosition(vec3 pos);

	vec3 getPosition();
	quat getQuaternion();
};

#endif // !PHYSICS_H
