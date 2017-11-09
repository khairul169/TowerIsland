#include "main.h"

void PhysicsManager::init()
{
	// Init ODE
	dInitODE();

	// Create the world & space
	world = dWorldCreate();
	space = dHashSpaceCreate(0);

	// Create joint group
	contactgroup = dJointGroupCreate(0);

	// Set world gravity
	dWorldSetGravity(world, 0.0f, -1.92f, 0.0f);

	// Set world damping
	dWorldSetDamping(world, 0.01f, 0.01f);

	// Create the floor plane
	//dCreatePlane(space, 0, 1, 0, 0);

	// Set ERP
	dWorldSetERP(world, 0.2f);
	dWorldSetCFM(world, 1e-5f);

	// This function sets the velocity that interpenetrating objects will separate at. The default value is infinity.
	dWorldSetContactMaxCorrectingVel(world, 0.9f);
	dWorldSetContactSurfaceLayer(world, 0.001f);

	// Set auto disable
	dWorldSetAutoDisableFlag(world, false);

	// set variables
	numOfInstances = 0;
	objects.clear();

	accumulator = 0.0f;
}

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	// Object is invalid
	if (o1 == NULL || o2 == NULL)
		return;

	// Get geometry body
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	
	// exit without doing anything if the two bodies are connected by a joint
	if (b1 && b2 && dAreConnectedExcluding(b1, b2, dJointTypeContact))
		return;

	dContact contact[MAX_CONTACTS];

	// Contacts
	for (int i = 0; i < MAX_CONTACTS; i++)
	{
		contact[i].surface.mode = dContactBounce | dContactSoftCFM;
		contact[i].surface.mu = dInfinity;
		contact[i].surface.mu2 = 0;
		contact[i].surface.bounce = 0.1f;
		contact[i].surface.bounce_vel = 0.1f;
		contact[i].surface.soft_cfm = 0.01f;
	}

	// Collision test
	if (int numc = dCollide(o1, o2, MAX_CONTACTS, &contact[0].geom, sizeof(dContact)))
	{
		for (int i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(mPhysicsMgr->world, mPhysicsMgr->contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

void PhysicsManager::loop()
{
	float step = 1.0f / 60.0f;
	accumulator += mMain->flDelta;

	//while (accumulator >= step)
	{
		// Simulate collision
		dSpaceCollide(space, 0, &nearCallback);

		// Step the world
		dWorldStep(world, 0.05f);

		// Remove all temporary collision joints now that the world has been stepped
		dJointGroupEmpty(contactgroup);

		accumulator -= step;
	}
}

void PhysicsManager::destroy()
{
	// Destroy everything
	dJointGroupDestroy(contactgroup);
	dSpaceDestroy(space);
	dWorldDestroy(world);
	dCloseODE();
}

// Create object
PhysicsObject *PhysicsManager::createObject()
{
	PhysicsObject *obj = new PhysicsObject(this);
	obj->instanceID = numOfInstances;
	objects.push_back(obj);
	numOfInstances++;
	return obj;
}

// Physics object
PhysicsObject::PhysicsObject(PhysicsManager *mgr)
{
	body = dBodyCreate(mgr->world);
	setPosition(vec3(0.0f));

	dBodySetData(body, (void*)this);

	this->mgr = mgr;
}

void PhysicsObject::createSphereBody(float mass, float radius)
{
	// Set mass
	dMass m;
	dMassSetZero(&m);
	dMassSetSphereTotal(&m, mass, radius);
	dBodySetMass(body, &m);

	// Create collision object
	geometry = dCreateSphere(mgr->space, radius);
	dGeomSetBody(geometry, body);
}

void PhysicsObject::createCubeBody(float mass, vec3 surface)
{
	// Set mass
	dMass m;
	dMassSetZero(&m);
	dMassSetBoxTotal(&m, mass, surface.x, surface.y, surface.z);
	dBodySetMass(body, &m);

	// Create collision object
	geometry = dCreateBox(mgr->space, surface.x, surface.y, surface.z);
	dGeomSetBody(geometry, body);
}

void PhysicsObject::createTriMesh(vector<float> vertices, vector<unsigned int> indices)
{
	const int mVertexCount = vertices.size() / 3;
	const int mIndexCount = indices.size();

	//float (*mVertices)[3] = new float[mVertexCount][3];
	dReal *mVertices = new dReal[mVertexCount];
	dTriIndex *mIndices = new dTriIndex[mVertexCount];

	/*for (int i = 0; i < mVertexCount; i++)
	{
		mVertices[i][0] = vertices[(i * 3) + 0];
		mVertices[i][1] = vertices[(i * 3) + 1];
		mVertices[i][2] = vertices[(i * 3) + 2];
	}*/

	for (int i = 0; i < mVertexCount; i++)
	{
		mVertices[i] = vertices[i];
	}

	for (int i = 0; i < mIndexCount; i++)
	{
		mIndices[i] = (dTriIndex)indices[i];
	}

	dTriMeshDataID m_dataID = dGeomTriMeshDataCreate();

	//dGeomTriMeshDataBuildSingle(m_dataID, mVertices[0], 3 * sizeof(float), mVertexCount, &mIndices[0], mIndexCount, 3 * sizeof(dTriIndex));
	//dGeomTriMeshDataPreprocess2(m_dataID, (1U << dTRIDATAPREPROCESS_BUILD_FACE_ANGLES), NULL);

	dGeomTriMeshDataBuildSimple(m_dataID, mVertices, mVertexCount, mIndices, mIndexCount);
	
	geometry = dCreateTriMesh(mgr->space, m_dataID, 0, 0, 0);
}

void PhysicsObject::setPosition(vec3 pos)
{
	dBodySetPosition(body, pos.x, pos.y, pos.z);
}

void PhysicsObject::setLinearVelocity(vec3 lv)
{
	dBodySetLinearVel(body, lv.x, lv.y, lv.z);
}

vec3 PhysicsObject::getPosition()
{
	const dReal *bPos = dBodyGetPosition(body);
	return vec3(bPos[0], bPos[1], bPos[2]);
}

quat PhysicsObject::getQuaternion()
{
	const dReal* quaternion = dBodyGetQuaternion(body);
	return quat(quaternion[0], quaternion[1], quaternion[2], quaternion[3]);
}

vec3 PhysicsObject::getLinearVelocity()
{
	const dReal *bVel = dBodyGetLinearVel(body);
	return vec3(bVel[0], bVel[1], bVel[2]);
}

void PhysicsObject::onColliding(PhysicsObject* with)
{
	printf("%i colliding with %i\n", instanceID, with->instanceID);
}
