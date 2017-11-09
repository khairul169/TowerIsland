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
#include "glm/gtx/string_cast.hpp"

using namespace glm;

#include "window.h"
#include "shaders.h"
#include "camera.h"
#include "texloader.h"
#include "mesh.h"
#include "physics.h"
#include "renderer.h"
#include "framebuffers.h"
#include "interface.h"
#include "canvasitems.h"
#include "utils.h"

#define PROJECT_NAME "Tower Island"

class Main
{
public:
	// variables
	bool isActive;

	float flTime;
	float flLastTime;
	float flDelta;

private:
	StaticMesh *m_floor;
	StaticMesh *m_crate;
	StaticMesh *m_sphere;

	PhysicsObject *p_crate;
	PhysicsObject *p_sphere;

public:
	void Init();
	void Ready();
	void Loop();
	void Render();
	void RenderGUI();
};

extern Main *mMain;
extern Window *mWindow;
extern Camera *mCamera;
extern ShadersManager *mShadersMgr;
extern PhysicsManager *mPhysicsMgr;
extern GUI *mInterface;

#endif