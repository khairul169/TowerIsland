#ifndef RENDERER_H
#define	RENDERER_H

#include "main.h"
#include "window.h"
#include "shaders.h"
#include "mesh.h"
#include "framebuffers.h"

class VisualRender
{
private:
	// Screen framebuffer
	Framebuffers* screenFramebuffers;

	// Framebuffer quad
	QuadMesh* screenQuad;

	// Shaders
	ScreenShaders* screenShaders;

public:
	// Methods
	void init();
	void resized();
	void loop();
	void free();
};

#endif // !RENDERER_H
