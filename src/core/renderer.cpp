#include "main.h"

void VisualRender::init()
{
	// Setup screen space
	screenFramebuffers = new Framebuffers();
	screenFramebuffers->create(mainObject->window->width, mainObject->window->height);

	// Setup framebuffer screen quad
	screenQuad = new QuadMesh();

	// Load shaders
	screenShaders = new ScreenShaders();
	screenShaders->load("screen");
}

void VisualRender::resized()
{
	// Resize framebuffer size
	screenFramebuffers->resize(mainObject->window->width, mainObject->window->height);
}

void VisualRender::loop()
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Render to our framebuffer
	screenFramebuffers->begin();

	// Render whole scene
	mainObject->render();

	// End rendering
	screenFramebuffers->end();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Render the framebuffer to screen
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Fullscreen drawing
	glViewport(0, 0, mainObject->window->width, mainObject->window->height);

	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	screenShaders->bind();
	screenShaders->setUniforms(screenFramebuffers->renderTexID, screenFramebuffers->depthTexID, mainObject->flTime);

	// Draw screen quad
	screenQuad->draw();
}

void VisualRender::free()
{

}
