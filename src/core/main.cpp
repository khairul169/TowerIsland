#include "main.h"

Main *mainObject;

// main procedure entry
int main(int argc, char **argv)
{
	mainObject = new Main();
	mainObject->init();
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
		// Window & render loop
		window->loop();

		// Physics loop
		physicsMgr->loop();

		// Scene loop
		this->loop();

		// Swap gl buffers
		window->swapBuffers();
	}

	// Destroy everything
	physicsMgr->destroy();
	window->free();
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
	m_sphere->position = vec3(0.0f, 3.0f, 0);
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
static bool hasJumping = false;

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
	//flRotation = 60.0f;
	vec3 origin = vec3(5 * sin(radians(flRotation)), 2, 5 * cos(radians(flRotation)));
	camera->setLookAt(origin, vec3(0, 0, 0), vec3(0, 1, 0));

	vec3 cubeDir = vec3(0.0f);
	vec3 forwardDir = Utils::getAxis(camera->getView(), 2);
	vec3 strafeDir = Utils::getAxis(camera->getView(), 0);
	
	if (window->getKey(GLFW_KEY_W))
		cubeDir -= forwardDir;

	if (window->getKey(GLFW_KEY_S))
		cubeDir += forwardDir;

	if (window->getKey(GLFW_KEY_A))
		cubeDir -= strafeDir;

	if (window->getKey(GLFW_KEY_D))
		cubeDir += strafeDir;

	if (length(cubeDir) > 0.0f)
		cubeDir = normalize(cubeDir);

	vec3 lv = p_sphere->getLinearVelocity();
	vec3 motion = cubeDir * 1.5f;

	if (length(motion) > 0.0f)
	{
		lv.x = Utils::lerp(lv.x, motion.x, 5 * flDelta);
		lv.z = Utils::lerp(lv.z, motion.z, 5 * flDelta);
	}

	if (window->getKey(GLFW_KEY_SPACE) && !hasJumping)
	{
		hasJumping = true;
		lv.y = 3.0f;
	}
	if (!window->getKey(GLFW_KEY_SPACE) && hasJumping)
	{
		hasJumping = false;
	}
	
	p_sphere->setLinearVelocity(lv);

	m_crate->position = p_crate->getPosition();
	m_crate->quaternion = p_crate->getQuaternion();

	m_sphere->position = p_sphere->getPosition();
	m_sphere->quaternion = p_sphere->getQuaternion();

	// Store last time value to a variable
	flLastTime = flTime;
}

void Main::render()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set default clear color
	glClearColor(152.0f / 255.0f, 186.0f / 255.0f, 242.0f / 255.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull face
	glEnable(GL_CULL_FACE);

	// Draw our mesh
	m_floor->draw();
	m_crate->draw();
	m_sphere->draw();
}
