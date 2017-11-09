#include "main.h"

// Objects handler
Main *mMain;
Window *mWindow;
Camera *mCamera;
ShadersManager *mShadersMgr;
PhysicsManager *mPhysicsMgr;
GUI *mInterface;

// main procedure entry
int main(int argc, char **argv)
{
	mMain = new Main();
	mMain->Init();
	return 0;
}

void Main::Init()
{
	isActive = true;
	flTime = 0.0f;

	// Init window
	mWindow = new Window();
	mWindow->init();

	// Setup camera
	mCamera = new Camera();
	mCamera->SetPerspective(radians(60.0f), mWindow->getAspectRatio(), 0.1f, 100.0f);
	mCamera->SetLookAt(vec3(4, 4, 3), vec3(0, 1, 0), vec3(0, 1, 0));

	// Shaders manager
	mShadersMgr = new ShadersManager();
	mShadersMgr->Init();

	// Setup physics
	mPhysicsMgr = new PhysicsManager();
	mPhysicsMgr->init();

	// Setup GUI
	mInterface = new GUI();
	mInterface->Init();

	// Our game is ready
	this->Ready();

	// Mainloop
	while (!mWindow->canDestroyed())
	{
		// Window & render loop
		mWindow->loop();

		// Physics loop
		mPhysicsMgr->loop();

		// Scene loop
		this->Loop();

		// Swap gl buffers
		mWindow->swapBuffers();
	}

	// Destroy everything
	mPhysicsMgr->destroy();
	mWindow->free();
}

void Main::Ready()
{
	// Load mesh
	m_floor = new StaticMesh();
	m_floor->LoadMesh("floor/floor.dae");

	m_crate = new StaticMesh();
	m_crate->LoadMesh("crate/crate.obj");
	m_crate->position = vec3(0, 1.0f, 0);
	m_crate->scaling = vec3(0.5f);

	m_sphere = new StaticMesh();
	m_sphere->LoadMesh("sphere/sphere.obj");
	m_sphere->position = vec3(0.0f, 2.5f, 0);
	m_sphere->scaling = vec3(0.5f);

	// Create physics objects
	p_crate = mPhysicsMgr->createObject();
	p_crate->createCubeBody(1.0f, vec3(1.0f));
	p_crate->setPosition(m_crate->position);

	p_sphere = mPhysicsMgr->createObject();
	p_sphere->createSphereBody(1.0f, 0.5f);
	p_sphere->setPosition(m_sphere->position);
}

static float flRotation = 0.0f;
static bool hasJumping = false;
static float flNextPrintFPS = 0.0f;

void Main::Loop()
{
	// Get time
	flTime = (float)glfwGetTime();
	
	if (flLastTime <= 0.0f)
		flLastTime = flTime + 0.01f;
	
	flDelta = flTime-flLastTime;

	flNextPrintFPS -= flDelta;
	if (flNextPrintFPS <= 0.0f)
	{
		//printf("FPS: %i\n", (int)(1.0f / flDelta));
		flNextPrintFPS = 1.0f;
	}

	// Reset time
	if (flTime > 3600.0f)
	{
		flTime = flTime - 3600.0f;
	}

	// Update camera
	flRotation = fmod(flRotation + 30.0f * flDelta, 360.0f);
	//flRotation = 60.0f;
	vec3 origin = vec3(5 * sin(radians(flRotation)), 2, 5 * cos(radians(flRotation)));
	mCamera->SetLookAt(origin, vec3(0, 0, 0), vec3(0, 1, 0));

	vec3 cubeDir = vec3(0.0f);
	vec3 forwardDir = Utils::getAxis(mCamera->view, 2);
	vec3 strafeDir = Utils::getAxis(mCamera->view, 0);
	
	if (mWindow->getKey(GLFW_KEY_W))
		cubeDir -= forwardDir;

	if (mWindow->getKey(GLFW_KEY_S))
		cubeDir += forwardDir;

	if (mWindow->getKey(GLFW_KEY_A))
		cubeDir -= strafeDir;

	if (mWindow->getKey(GLFW_KEY_D))
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

	if (mWindow->getKey(GLFW_KEY_SPACE) && !hasJumping)
	{
		hasJumping = true;
		lv.y = 3.0f;
	}
	if (!mWindow->getKey(GLFW_KEY_SPACE) && hasJumping)
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

void Main::Render()
{
	// Draw our mesh
	m_floor->Draw();
	m_crate->Draw();
	m_sphere->Draw();
}

void Main::RenderGUI()
{

}
