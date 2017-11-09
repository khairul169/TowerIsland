#ifndef SHADERS_H
#define SHADERS_H

#include "main.h"
#include "camera.h"
#include "texloader.h"

//////////////////// Shaders ///////////////

class Shaders
{
protected:
	GLuint vertID;
	GLuint fragID;

public:
	GLuint programID;

	bool isLoaded;
	char *shaderName;

public:
	Shaders() {
		isLoaded = false;
	}

	void Load(const char *name);
	void Bind();

	virtual void ShadersLoaded();

private:
	void CheckShader(int id);
};

class MaterialShaders: public Shaders
{
private:
	mat4 projection, view, model;
	mat4 modelViewProjection;

	// Vertex uniform
	GLuint modelMatrix, viewMatrix;
	GLuint matrixUniform, normalMatrix;

	// Fragment uniform
	GLuint colorUniform;
	GLuint diffuseUniform;

	// Lighting
	GLuint lightPosUniform, lightAmbientUniform, lightColorUniform;

public:
	void ShadersLoaded();

	void SetModelMatrix(mat4 mat);
	void UpdateCamera(Camera *cam);
	void UpdateProjection();

	void SetColor(vec3 col);
	void SetColor(vec4 col);
	void SetTexture(Texture *tex);
};

class ScreenShaders: public Shaders
{
private:
	GLuint mProjectionID;
	GLuint renderedTextureUniform, depthTextureUniform;
	GLuint timeUniform;

	mat4 mProjection;

public:
	void ShadersLoaded();
	void Bind();
	void SetUniforms(GLuint renderTex, GLuint depthTex, float time);
};

class CanvasShaders: public Shaders
{
private:
	mat4 mProjection, mModel;

	GLuint texID;
	GLuint mpMatrixID;

public:
	void ShadersLoaded();
	void UpdateMatrix();
	void SetTransform(vec2 pos, vec2 size, bool flip_y = false);
	void SetTexture(GLuint tex);
};

///////////////////////////////////// Shaders manager /////////////////////////

class ShadersManager
{
public:
	// Shaders
	MaterialShaders* mMaterial;
	CanvasShaders* mCanvas;

public:
	void Init();
};

#endif // !SHADERS_H
