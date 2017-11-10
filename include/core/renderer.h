#ifndef RENDERER_H
#define	RENDERER_H

#include "main.h"
#include "window.h"
#include "shaders.h"
#include "mesh.h"
#include "framebuffers.h"
#include "canvasitems.h"

// Variables
const bool iEnablePostProcessing = false;

class VisualRender
{
public:
	// Screen framebuffer
	Framebuffers* mViewportFBO;

private:
	// Canvas
	FramebufferCanvas* mViewportCanvas;

	// Chromatic post processing
	Framebuffers* mChromaticPostFX;
	QuadMesh* mChromaticMesh;
	PostFxShaders* mChromaticShaders;

private:
	// Variables
	Camera *mDefaultCamera;

public:
	// Variables
	Framebuffers* mCurrentRenderingFBO;

	// Shadowmapping
	Framebuffers* mShadowmapFBO;
	Camera* mShadowmapCamera;
	bool mRenderingShadowFBO;

	// Watermesh
	Framebuffers* mWaterReflectionFBO;
	Framebuffers* mWaterRefractionFBO;
	Camera* mWaterReflectionCam;

public:
	// Methods
	void Init();
	void Resized();
	void Loop();
	void Free();
};

#endif // !RENDERER_H
