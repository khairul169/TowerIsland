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

public:
	mat4 mDepthBiasMatrix;

private:
	// Vertex uniform
	GLuint modelMatrix, viewMatrix;
	GLuint matrixUniform, normalMatrix;
	GLuint uClippingPlane;

	// Fragment uniform
	GLuint colorUniform;
	GLuint diffuseUniform;

	// Lighting
	GLuint lightPosUniform, lightAmbientUniform, lightColorUniform;
	GLuint shadowDepthMatrix, shadowDepthTex;

public:
	// Plane clipping
	vec4 mClippingPlane;

public:
	void Bind();
	void ShadersLoaded();

	void SetModelMatrix(mat4 mat);
	void UpdateCamera(Camera *cam);
	void UpdateProjection();

	void SetColor(vec3 col);
	void SetColor(vec4 col);
	void SetTexture(Texture *tex, GLuint shadowDepthID);
};

// Water Shaders
class WaterShaders: public Shaders
{
private:
	mat4 mModel, mModelViewProjection;

	// Vertex uniform
	GLuint uMVP;
	GLuint uUVSize;

	// Fragment uniform
	GLuint uTime, uColor;
	GLuint uReflectionTex, uRefractionTex, uDuDvMap;

	// Shadowmap
	GLuint shadowDepthMatrix, shadowDepthTex;

	// Textures
	Texture* mDuDvTexture;

public:
	void Bind(vec3 pos = vec3(0.0f), vec3 size = vec3(1.0f), GLuint shadowDepthID = 0);
	void ShadersLoaded();

	void SetColor(vec4 col);
	void SetTextures(GLuint reflectionTexID, GLuint refractionTexID);
};

////////////////////////////////////////////////////////////////////////

class PostFxShaders: public Shaders
{
private:
	GLuint mProjectionID;
	GLuint renderedTextureUniform, depthTextureUniform;

public:
	void ShadersLoaded();
	void Bind();
	void SetUniforms(GLuint renderTex, GLuint depthTex);
};

class DepthMapShaders: public Shaders
{
private:
	GLuint mProjectionID;
	GLuint depthTextureUniform;

public:
	void ShadersLoaded();
	void Bind();
	void SetDepthTex(GLuint depthTex);
};

class CanvasShaders : public Shaders
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

	// Water shaders
	WaterShaders* mWater;

public:
	void Init();
};

#endif // !SHADERS_H
