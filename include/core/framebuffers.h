#ifndef FRAMEBUFFERS_H
#define FRAMEBUFFERS_H

#include "main.h"

class Framebuffers
{
private:
	GLuint fbID, depthID;
	int width, height;
	
public:
	GLuint renderTexID, depthTexID;

	bool renderTex, depthTex;

public:
	Framebuffers(bool enableRenderTex = true, bool enableDepthTex = true)
	{
		this->renderTex = enableRenderTex;
		this->depthTex = enableDepthTex;
	}

	void create(int width, int height);
	void resize(int width, int height);

	void begin();
	void end();
};

#endif // !FRAMEBUFFERS_H
