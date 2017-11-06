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
	camera->setLookAt(vec3(4, 3, 3), vec3(0, 0, 0), vec3(0, 1, 0));

	// Our game is ready
	ready();

	// Mainloop
	while (!window->canDestroyed())
	{
		window->loop();
		loop();
		window->swapBuffers();
	}
}

void Main::ready()
{
	// Load shaders
	materialShaders = new MaterialShaders();
	materialShaders->load("material");

	mesh = new StaticMesh();
	mesh->loadMesh("crate/crate.obj");
}

static float rotation = 0.0f;

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
	rotation = fmod(rotation + 30.0f * flDelta, 360.0f);
	vec3 origin = vec3(5 * sin(radians(rotation)), 2, 5 * cos(radians(rotation)));
	camera->setLookAt(origin, vec3(0, 0, 0), vec3(0, 1, 0));

	// Shaders binding
	materialShaders->bind();
	materialShaders->updateCamera(camera);
	
	mesh->draw();

	flLastTime = flTime;
}
