#include "main.h"

void Framebuffers::Create(int width, int height)
{
	// Create framebuffer
	glGenFramebuffers(1, &fbID);
	glBindFramebuffer(GL_FRAMEBUFFER, fbID);

	if (renderTex)
	{
		// Generate render texture ID
		glGenTextures(1, &renderTexID);

		glBindTexture(GL_TEXTURE_2D, renderTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

		// Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Nearest filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexID, 0);

		// Set the list of draw buffers.
		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}

	// Depth buffer
	glGenRenderbuffers(1, &depthID);
	glBindRenderbuffer(GL_RENDERBUFFER, depthID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthID);

	if (depthTex)
	{
		// Generate depth texture ID
		glGenTextures(1, &depthTexID);

		glBindTexture(GL_TEXTURE_2D, depthTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);

		// Texture wrapping
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// Linear filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexID, 0);

		// Set the list of draw buffers.
		GLenum drawBuffers[1] = { GL_DEPTH_ATTACHMENT };
		glDrawBuffers(1, drawBuffers);
	}

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		return;

	// Set variable
	this->width = width;
	this->height = height;
}

void Framebuffers::Resize(int width, int height)
{
	if (renderTex)
	{
		// Resize framebuffer texture size
		glBindTexture(GL_TEXTURE_2D, renderTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	}

	if (depthTex)
	{
		// Resize framebuffer texture size
		glBindTexture(GL_TEXTURE_2D, depthTexID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}

	// Resize depth render buffer
	glBindRenderbuffer(GL_RENDERBUFFER, depthID);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

	this->width = width;
	this->height = height;
}

void Framebuffers::Begin()
{
	mVisualRender->mCurrentRenderingFBO = this;

	glBindFramebuffer(GL_FRAMEBUFFER, fbID);
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffers::End()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	mVisualRender->mCurrentRenderingFBO = nullptr;
}
