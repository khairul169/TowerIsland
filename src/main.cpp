#include "main.h"

Main *gMain;

// main procedure entry
int main(int argc, char **argv)
{
	gMain = new Main();
	gMain->init();
	return 0;
}

void Main::init()
{
	isActive = true;
	flTime = 0.0f;

	// Init window
	window = new Window();
	window->init();

	// Setup camera
	camera = new Camera();
	camera->setPerspective(radians(60.0f), window->getAspectRatio(), 0.1f, 100.0f);
	camera->setLookAt(vec3(4, 4, 3), vec3(0, 1, 0), vec3(0, 1, 0));

	// Setup physics
	physicsMgr = new PhysicsManager();
	physicsMgr->init();

	// Our game is ready
	ready();

	// Mainloop
	while (!window->canDestroyed())
	{
		window->loop();
		physicsMgr->loop();
		this->loop();
		window->swapBuffers();
	}
}

void Main::ready()
{
	// Load shaders
	materialShaders = new MaterialShaders();
	materialShaders->load("material");

	// Load mesh
	m_floor = new StaticMesh();
	m_floor->loadMesh("floor/floor.obj");

	m_crate = new StaticMesh();
	m_crate->loadMesh("crate/crate.obj");
	m_crate->position = vec3(0, 1.0f, 0);
	m_crate->scaling = vec3(0.5f);

	m_sphere = new StaticMesh();
	m_sphere->loadMesh("sphere/sphere.obj");
	m_sphere->position = vec3(0.5f, 3.0f, 0);
	m_sphere->scaling = vec3(0.5f);

	// Create physics objects
	p_crate = physicsMgr->createObject();
	p_crate->createCubeBody(1.0f, vec3(1.0f));
	p_crate->setPosition(m_crate->position);

	p_sphere = physicsMgr->createObject();
	p_sphere->createSphereBody(1.0f, 0.5f);
	p_sphere->setPosition(m_sphere->position);
}

static float flRotation = 0.0f;

void Main::loop()
{
	// Get time
	flTime = (float)glfwGetTime();
	flDelta = flTime-flLastTime;

	// Reset time
	if (flTime > 3600.0f)
	{
		flTime = flTime - 3600.0f;
	}

	// Update camera
	flRotation = fmod(flRotation + 30.0f * flDelta, 360.0f);
	vec3 origin = vec3(5 * sin(radians(flRotation)), 2, 5 * cos(radians(flRotation)));
	camera->setLookAt(origin, vec3(0, 0, 0), vec3(0, 1, 0));

	// Shaders binding
	materialShaders->bind();
	materialShaders->updateCamera(camera);

	// Draw our mesh
	m_floor->draw();

	m_crate->position = p_crate->getPosition();
	//m_crate->rotation.y = cos(radians(60.0f));
	m_crate->draw();

	//m_sphere->position.x = 2 * cos(radians(flRotation));
	//m_sphere->position.z = 2 * sin(radians(flRotation));
	m_sphere->position = p_sphere->getPosition();
	m_sphere->quaternion = p_sphere->getQuaternion();
	m_sphere->draw();

	// Store last time value to a variable
	flLastTime = flTime;
}
