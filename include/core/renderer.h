#ifndef RENDERER_H
#define	RENDERER_H

#include "main.h"
#include "window.h"
#include "shaders.h"
#include "mesh.h"
#include "framebuffers.h"
#include "canvasitems.h"

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
	// Methods
	void init();
	void resized();
	void loop();
	void free();
};

#endif // !RENDERER_H
