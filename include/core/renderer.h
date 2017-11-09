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
private:
	// Screen framebuffer
	Framebuffers* mViewportFBO;
	Framebuffers* mChromaticPostFX;

	// Canvas
	FramebufferCanvas* mViewportCanvas;

	// Mesh
	QuadMesh* mChromaticMesh;

	// Shaders
	PostFxShaders* mChromaticShaders;

public:
	// Shadowmapping
	Framebuffers* mShadowmapFBO;
	Camera* mShadowmapCamera;
	bool mRenderingShadowFBO;

public:
	// Methods
	void Init();
	void Resized();
	void Loop();
	void Free();
};

#endif // !RENDERER_H
