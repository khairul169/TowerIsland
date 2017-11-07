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
	dWorldSetGravity(world, 0.0f, -0.98f, 0.0f);

	// Create the floor plane
	dCreatePlane(space, 0, 1, 0, 0);

	// Set ERP
	dWorldSetERP(world, 0.2f);
	dWorldSetCFM(world, 1e-5f);

	// This function sets the velocity that interpenetrating objects will separate at. The default value is infinity.
	dWorldSetContactMaxCorrectingVel(world, 0.9f);
	dWorldSetContactSurfaceLayer(world, 0.001f);

	// Set auto disable
	dWorldSetAutoDisableFlag(world, 1);
}

static void nearCallback(void *data, dGeomID o1, dGeomID o2)
{
	// Temporary index for each contact
	int i;

	// Get the dynamics body for each geom
	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);

	// Create an array of dContact objects to hold the contact joints
	dContact contact[MAX_CONTACTS];

	// Set the joint properties of each contact.
	for (i = 0; i < MAX_CONTACTS; i++)
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
		for (i = 0; i < numc; i++)
		{
			dJointID c = dJointCreateContact(gMain->physicsMgr->world, gMain->physicsMgr->contactgroup, contact + i);
			dJointAttach(c, b1, b2);
		}
	}
}

void PhysicsManager::loop()
{
	// Simulate collision
	dSpaceCollide(space, 0, &nearCallback);

	// Step the world
	dWorldStep(world, 1.0f/30.0f);

	// Remove all temporary collision joints now that the world has been stepped
	dJointGroupEmpty(contactgroup);
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
	objects.push_back(obj);
	return obj;
}

// Physics object
PhysicsObject::PhysicsObject(PhysicsManager *mgr)
{
	body = dBodyCreate(mgr->world);
	setPosition(vec3(0.0f));

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

void PhysicsObject::setPosition(vec3 pos)
{
	dBodySetPosition(body, pos.x, pos.y, pos.z);
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
