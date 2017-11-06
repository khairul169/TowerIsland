#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

using namespace glm;

#include "window.h"
#include "shaders.h"
#include "camera.h"
#include "texloader.h"
#include "meshloader.h"

#include "utils.h"

#ifdef _WIN32
#pragma warning(disable:4996)
#endif

#define PROJECT_NAME "Tower Island"

class Main
{
private:
	// window handler
	Window *window;
	Camera *camera;

	// variables
	bool isActive;

	float flTime;
	float flLastTime;
	float flDelta;

	StaticMesh *mesh;

public:
	MaterialShaders *materialShaders;

public:
	void init();
	void ready();
	void loop();

	Camera *getCamera() { return camera; }
};

extern Main *gMain;

#endif