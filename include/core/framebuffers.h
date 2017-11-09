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

	void Create(int width, int height);
	void Resize(int width, int height);

	void Begin();
	void End();
};

#endif // !FRAMEBUFFERS_H
