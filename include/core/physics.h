#ifndef PHYSICS_H
#define PHYSICS_H

#include "main.h"
#include <vector>

// Bulet physics
#include "bullet3/btBulletDynamicsCommon.h"

class PhysicsManager;
class PhysicsObject
{
public:
	size_t instanceID;

	btCollisionShape* mColShape;
	btRigidBody* mBody;

public:
	void CreateSphereBody(float mass, float radius);
	void CreateCubeBody(float mass, vec3 extents);
	void CreateTriMesh(vector<float> vertices, vector<unsigned int> indices);

	void setPosition(vec3 pos);
	void setLinearVelocity(vec3 lv);

	vec3 getPosition();
	quat getQuaternion();
	vec3 getLinearVelocity();

	virtual void onColliding(PhysicsObject* with);
};

class PhysicsManager
{
private:
	btBroadphaseInterface* mBroadphase;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mDispatcher;
	btSequentialImpulseConstraintSolver* mSolver;

public:
	btDiscreteDynamicsWorld* mWorld;

	// Instances
	size_t numOfInstances;
	vector<PhysicsObject*> objects;

	float accumulator;

public:
	void Init();
	void Loop();
	void Destroy();

	PhysicsObject *CreateObject();
};

#endif // !PHYSICS_H
