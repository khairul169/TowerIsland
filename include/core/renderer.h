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

	// Canvas
	FramebufferCanvas *mViewportCanvas;

public:
	// Methods
	void init();
	void resized();
	void loop();
	void free();
};

#endif // !RENDERER_H
