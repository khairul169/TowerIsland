#ifndef CANVASITEMS_H
#define CANVASITEMS_H

#include "main.h"

class FramebufferCanvas
{
private:
	Framebuffers* fbo;
	QuadMesh* mesh;

public:
	FramebufferCanvas(Framebuffers* fb) {
		this->fbo = fb;
		this->mesh = new QuadMesh();
	}
	void Draw(vec2 pos, vec2 size);
};

class TextureCanvas
{
private:
	QuadMesh* mesh;

public:
	Texture* texture;

public:
	TextureCanvas(const char* texturePath);

	void Draw(vec2 pos);
	void Draw(vec2 pos, float size);
	void Draw(vec2 pos, vec2 size);
};

#endif // !CANVASITEMS_H
