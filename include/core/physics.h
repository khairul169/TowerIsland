#ifndef PHYSICS_H
#define PHYSICS_H

#include "main.h"
#include <vector>

// ODE library
#define dSINGLE
#include "ode/ode.h"

const int MAX_BODIES = 1024;
const int MAX_CONTACTS = 10;

class PhysicsManager;

class PhysicsObject
{
public:
	size_t instanceID;
	dBodyID body;
	dGeomID geometry;

private:
	PhysicsManager *mgr;

public:
	PhysicsObject() {}
	PhysicsObject(PhysicsManager *mgr);

	void createSphereBody(float mass, float radius);
	void createCubeBody(float mass, vec3 surface);
	void createTriMesh(vector<float> vertices, vector<unsigned int> indices);

	void setPosition(vec3 pos);
	void setLinearVelocity(vec3 lv);

	vec3 getPosition();
	quat getQuaternion();
	vec3 getLinearVelocity();

	virtual void onColliding(PhysicsObject* with);
};

class PhysicsManager
{
public:
	dWorldID world;
	dSpaceID space;
	dJointGroupID contactgroup;

private:
	size_t numOfInstances;
	vector<PhysicsObject*> objects;

	float accumulator;

public:
	void init();
	void loop();
	void destroy();

	PhysicsObject *createObject();
};

#endif // !PHYSICS_H
