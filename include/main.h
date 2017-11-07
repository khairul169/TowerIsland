#ifndef MAIN_H
#define MAIN_H

#ifdef _WIN32
#define WIN32

#pragma warning(disable:4996)
#pragma warning(disable:4819)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <cmath>

// GLM
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"

using namespace glm;

#include "window.h"
#include "shaders.h"
#include "camera.h"
#include "texloader.h"
#include "meshloader.h"
#include "physics.h"

#include "utils.h"

#define PROJECT_NAME "Tower Island"

class Main
{
public:
	// window handler
	Window *window;
	Camera *camera;
	PhysicsManager *physicsMgr;

private:
	// variables
	bool isActive;

	float flTime;
	float flLastTime;
	float flDelta;

	StaticMesh *m_floor;
	StaticMesh *m_crate;
	StaticMesh *m_sphere;

	PhysicsObject *p_crate;
	PhysicsObject *p_sphere;

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