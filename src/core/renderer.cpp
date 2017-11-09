#include "main.h"

void VisualRender::init()
{
	// Setup screen space
	mViewportFBO = new Framebuffers();
	mViewportFBO->create(mWindow->width, mWindow->height);

	// Setup canvas
	mViewportCanvas = new FramebufferCanvas(mViewportFBO);
}

void VisualRender::resized()
{
	// Resize framebuffer size
	mViewportFBO->resize(mWindow->width, mWindow->height);
}

void VisualRender::loop()
{
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set default clear color
	glClearColor(152.0f / 255.0f, 186.0f / 255.0f, 242.0f / 255.0f, 1.0f);

	// Enable depth test & culling
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Render to our framebuffer
	mViewportFBO->begin();

	// Render spatial scene
	mMain->Render();

	// End rendering
	mViewportFBO->end();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Reset viewport
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, mWindow->width, mWindow->height);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Disable depth test & face culling
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Render framebuffer texture
	mViewportCanvas->Draw(vec2(0, 0), vec2(mWindow->width, mWindow->height));

	// Draw the user interface
	mInterface->Redraw();
}

void VisualRender::free()
{

}
