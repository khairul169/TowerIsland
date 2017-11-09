#include "main.h"

void PhysicsManager::Init()
{
	// Setup bullet
	mBroadphase = new btDbvtBroadphase();
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	mSolver = new btSequentialImpulseConstraintSolver;
	mWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);

	// Set gravity
	mWorld->setGravity(btVector3(0, -9.8, 0));

	// set variables
	numOfInstances = 0;
	objects.clear();

	accumulator = 0.0f;
}

void PhysicsManager::Loop()
{
	float step = 1.0f / 60.0f;
	accumulator += mMain->flDelta;

	while (accumulator >= step)
	{
		mWorld->stepSimulation(step, 10);
		accumulator -= step;
	}
}

void PhysicsManager::Destroy()
{
	// Destroy everything
	delete mWorld;
	delete mSolver;
	delete mDispatcher;
	delete mCollisionConfiguration;
	delete mBroadphase;
}

// Create object
PhysicsObject *PhysicsManager::CreateObject()
{
	PhysicsObject *obj = new PhysicsObject();
	obj->instanceID = numOfInstances;
	objects.push_back(obj);
	numOfInstances++;
	return obj;
}

// Physics Object

void PhysicsObject::CreateSphereBody(float mass, float radius)
{
	// Collision shape
	btVector3 mFallInertia(0, 0, 0);
	mColShape = new btSphereShape(radius);
	mColShape->calculateLocalInertia(mass, mFallInertia);

	// Motion state
	btDefaultMotionState* mMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo mConstructionInfo(mass, mMotionState, mColShape, mFallInertia);
	
	// Create rigidbody
	mBody = new btRigidBody(mConstructionInfo);
	mPhysicsMgr->mWorld->addRigidBody(mBody);
}

void PhysicsObject::CreateCubeBody(float mass, vec3 extents)
{
	// Collision shape
	btVector3 mFallInertia(0, 0, 0);
	mColShape = new btBoxShape(btVector3(extents.x, extents.y, extents.z));
	mColShape->calculateLocalInertia(mass, mFallInertia);

	// Motion state
	btDefaultMotionState* mMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo mConstructionInfo(mass, mMotionState, mColShape, mFallInertia);

	// Create rigidbody
	mBody = new btRigidBody(mConstructionInfo);
	mPhysicsMgr->mWorld->addRigidBody(mBody);
}

void PhysicsObject::CreateTriMesh(vector<float> vertices, vector<unsigned int> indices)
{
	btTriangleMesh *mTriMesh = new btTriangleMesh();

	for (size_t i = 0; i < indices.size()/3; i++)
	{
		int index0 = indices[i * 3 + 0];
		int index1 = indices[i * 3 + 1];
		int index2 = indices[i * 3 + 2];

		btVector3 vertex0(vertices[index0 * 3], vertices[index0 * 3 + 1], vertices[index0 * 3 + 2]);
		btVector3 vertex1(vertices[index1 * 3], vertices[index1 * 3 + 1], vertices[index1 * 3 + 2]);
		btVector3 vertex2(vertices[index2 * 3], vertices[index2 * 3 + 1], vertices[index2 * 3 + 2]);

		mTriMesh->addTriangle(vertex0, vertex1, vertex2);
	}

	// Create collision shape
	mColShape = new btBvhTriangleMeshShape(mTriMesh, false);// , true);

	// Motion state
	btDefaultMotionState* mMotionState = new btDefaultMotionState();
	btRigidBody::btRigidBodyConstructionInfo mConstructionInfo(0, mMotionState, mColShape, btVector3(0, 0, 0));

	// Create rigidbody
	mBody = new btRigidBody(mConstructionInfo);
	mPhysicsMgr->mWorld->addRigidBody(mBody);
}

void PhysicsObject::setPosition(vec3 pos)
{
	btTransform mTransform = mBody->getWorldTransform();
	mTransform.setOrigin(btVector3(pos.x, pos.y, pos.z));
	mBody->setWorldTransform(mTransform);
}

void PhysicsObject::setLinearVelocity(vec3 lv)
{
	mBody->setLinearVelocity(btVector3(lv.x, lv.y, lv.z));
}

vec3 PhysicsObject::getPosition()
{
	btVector3 mOrigin = mBody->getWorldTransform().getOrigin();
	return vec3(mOrigin.getX(), mOrigin.getY(), mOrigin.getZ());
}

quat PhysicsObject::getQuaternion()
{
	btQuaternion mQuat = mBody->getWorldTransform().getRotation();
	return quat(mQuat.getW(), mQuat.getX(), mQuat.getY(), mQuat.getZ());
}

vec3 PhysicsObject::getLinearVelocity()
{
	btVector3 mLinearVelocity = mBody->getLinearVelocity();
	return vec3(mLinearVelocity.getX(), mLinearVelocity.getY(), mLinearVelocity.getZ());
}

void PhysicsObject::onColliding(PhysicsObject* with)
{
	//printf("%i colliding with %i\n", instanceID, with->instanceID);
}
