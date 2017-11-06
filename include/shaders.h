#ifndef SHADERS_H
#define SHADERS_H

#include "main.h"
#include "camera.h"
#include "texloader.h"

class Shaders
{
protected:
	GLuint vertID;
	GLuint fragID;
	GLuint programID;

public:
	bool isLoaded;
	char *shaderName;

public:
	Shaders() {
		isLoaded = false;
	}

	void load(const char *name);
	void bind();

	virtual void shadersLoaded();

	GLuint getProgramID();

private:
	void checkShader(int id);
};

class MaterialShaders: public Shaders
{
private:
	mat4 projection, view, model;
	mat4 modelViewProjection;

	// Vertex uniform
	GLuint matrixUniform;

	// Fragment uniform
	GLuint colorUniform;
	GLuint diffuseUniform;

public:
	void shadersLoaded();

	void setModelMatrix(mat4 mat);
	void updateCamera(Camera *cam);
	void updateProjection();

	void setColor(vec3 col);
	void setTexture(Texture *tex);
};

#endif // !SHADERS_H
