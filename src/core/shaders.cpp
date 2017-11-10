#include "main.h"

#include <stdlib.h>
#include <vector>

void Shaders::Load(const char *name)
{
	// Debug info
	printf("Loading %s shaders...\n", name);

	// Set shader name
	shaderName = new char[64];
	sprintf(shaderName, "%s", name);

	// Create the shaders
	vertID = glCreateShader(GL_VERTEX_SHADER);
	fragID = glCreateShader(GL_FRAGMENT_SHADER);

	// Shaders path
	char *fpath = "assets/shaders";

	// Load shader
	char vertexPath[64];
	char fragmentPath[64];

	sprintf(vertexPath, "%s/%s.vs", fpath, name);
	sprintf(fragmentPath, "%s/%s.fs", fpath, name);

	char *vertexSource = Utils::getFileString(vertexPath);
	char *fragmentSource = Utils::getFileString(fragmentPath);

	if (vertexSource == NULL)
	{
		printf("Cannot open %s\n", vertexPath);
		return;
	}

	if (fragmentSource == NULL)
	{
		printf("Cannot open %s\n", fragmentPath);
		return;
	}

	// Compile shader
	glShaderSource(vertID, 1, &vertexSource, NULL);
	glCompileShader(vertID);

	// Check shader
	CheckShader(vertID);

	// Compile shader
	glShaderSource(fragID, 1, &fragmentSource, NULL);
	glCompileShader(fragID);

	// Check shader
	CheckShader(fragID);

	// Link the program
	programID = glCreateProgram();
	glAttachShader(programID, vertID);
	glAttachShader(programID, fragID);
	glLinkProgram(programID);

	// Check the program
	CheckShader(programID);

	glDetachShader(programID, vertID);
	glDetachShader(programID, fragID);

	glDeleteShader(vertID);
	glDeleteShader(fragID);

	ShadersLoaded();
}

void Shaders::CheckShader(int id)
{
	GLint result = GL_FALSE;
	int infoLogLength;

	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);

	if (infoLogLength > 1) {
		std::vector<char> errorMessage(infoLogLength + 1);
		glGetShaderInfoLog(id, infoLogLength, NULL, &errorMessage[0]);
		printf("Shader %s: %s\n", shaderName, &errorMessage[0]);
	}
}

void Shaders::ShadersLoaded()
{
	// Shaders loaded.
	printf("Shaders %s loaded.\n\n", shaderName);
}

void Shaders::Bind()
{
	glUseProgram(programID);
}

// Material Shaders

void MaterialShaders::ShadersLoaded()
{
	Shaders::ShadersLoaded();

	// Perspective
	projection = mat4(0.0f);
	view = mat4(0.0f);
	model = mat4(1.0f);

	// Update projection
	UpdateProjection();

	// Vert Uniforms
	modelMatrix = glGetUniformLocation(programID, "uModelMatrix");
	viewMatrix = glGetUniformLocation(programID, "view");
	matrixUniform = glGetUniformLocation(programID, "modelViewProjection");
	normalMatrix = glGetUniformLocation(programID, "normal");
	uClippingPlane = glGetUniformLocation(programID, "mClippingPlane");

	// Frag Uniforms
	colorUniform = glGetUniformLocation(programID, "color");
	diffuseUniform = glGetUniformLocation(programID, "diffuse");

	lightPosUniform = glGetUniformLocation(programID, "lightPos");
	lightAmbientUniform = glGetUniformLocation(programID, "lightAmbient");
	lightColorUniform = glGetUniformLocation(programID, "lightColor");

	shadowDepthMatrix = glGetUniformLocation(programID, "shadowMatrix");
	shadowDepthTex = glGetUniformLocation(programID, "shadowTex");

	mDepthBiasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);
}

void MaterialShaders::Bind()
{
	Shaders::Bind();

	glUniform4f(uClippingPlane, mClippingPlane.x, mClippingPlane.y, mClippingPlane.z, mClippingPlane.z);
}

void MaterialShaders::SetModelMatrix(mat4 mat)
{
	model = mat;
}

void MaterialShaders::UpdateCamera(Camera *cam)
{
	projection = cam->projection;
	view = cam->view;
}

void MaterialShaders::UpdateProjection()
{
	modelViewProjection = projection * view * model;
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(viewMatrix, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &modelViewProjection[0][0]);

	mat4 normal = transpose(inverse(model));
	glUniformMatrix4fv(normalMatrix, 1, GL_FALSE, &normal[0][0]);

	vec3 lightPos = vec3(3, 2, 3);
	vec4 lightAmbient = vec4(1.0f, 1.0f, 1.0f, 0.75f);
	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.25f);

	glUniform3f(lightPosUniform, lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(lightAmbientUniform, lightAmbient.x, lightAmbient.y, lightAmbient.z, lightAmbient.w);
	glUniform4f(lightColorUniform, lightColor.x, lightColor.y, lightColor.z, lightColor.w);

	mat4 shadowMatrix = mDepthBiasMatrix * mVisualRender->mShadowmapCamera->projection * mVisualRender->mShadowmapCamera->view * model;
	glUniformMatrix4fv(shadowDepthMatrix, 1, GL_FALSE, &shadowMatrix[0][0]);
}

void MaterialShaders::SetColor(vec3 col)
{
	glUniform4f(colorUniform, col.x, col.y, col.z, 1.0f);
}

void MaterialShaders::SetColor(vec4 col)
{
	glUniform4f(colorUniform, col.x, col.y, col.z, col.w);
}

void MaterialShaders::SetTexture(Texture *tex, GLuint shadowDepthID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texID);
	glUniform1i(diffuseUniform, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowDepthID);
	glUniform1i(shadowDepthTex, 1);
}

// Water Shaders
void WaterShaders::ShadersLoaded()
{
	Shaders::ShadersLoaded();

	// Vert Uniforms
	uMVP = glGetUniformLocation(programID, "mModelViewProjection");
	uUVSize = glGetUniformLocation(programID, "mUVSize");

	// Frag Uniforms
	uTime = glGetUniformLocation(programID, "mTime");
	uColor = glGetUniformLocation(programID, "mColor");
	uReflectionTex = glGetUniformLocation(programID, "mReflectionTex");
	uRefractionTex = glGetUniformLocation(programID, "mRefractionTex");
	uDuDvMap = glGetUniformLocation(programID, "mDuDvMap");

	// Shadowmapping
	shadowDepthMatrix = glGetUniformLocation(programID, "shadowMatrix");
	shadowDepthTex = glGetUniformLocation(programID, "shadowTex");

	// Load textures
	mDuDvTexture = new Texture();
	mDuDvTexture->LoadTexture("assets/waterdudv.jpg");
}

void WaterShaders::Bind(vec3 pos, vec3 size, GLuint shadowDepthID)
{
	Shaders::Bind();

	// Set matrix
	mModel = translate(mat4(1.0f), pos);
	mModel = scale(mModel, size);

	mModelViewProjection = mCamera->projection * mCamera->view * mModel;
	glUniformMatrix4fv(uMVP, 1, GL_FALSE, &mModelViewProjection[0][0]);

	// Shadow depth matrix
	mat4 shadowMatrix = mShadersMgr->mMaterial->mDepthBiasMatrix * mVisualRender->mShadowmapCamera->projection * mVisualRender->mShadowmapCamera->view * mModel;
	glUniformMatrix4fv(shadowDepthMatrix, 1, GL_FALSE, &shadowMatrix[0][0]);

	// Time
	glUniform1f(uTime, mMain->flTime);

	// UV Size
	glUniform2f(uUVSize, size.x, size.z);

	// DuDv Map
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mDuDvTexture->texID);
	glUniform1i(uDuDvMap, 2);

	// Shadowmap
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, shadowDepthID);
	glUniform1i(shadowDepthTex, 3);
}

void WaterShaders::SetColor(vec4 col)
{
	glUniform4f(uColor, col.x, col.y, col.z, col.w);
}

void WaterShaders::SetTextures(GLuint reflectionTexID, GLuint refractionTexID)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectionTexID);
	glUniform1i(uReflectionTex, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractionTexID);
	glUniform1i(uRefractionTex, 1);
}

// Post effect shaders

void PostFxShaders::ShadersLoaded()
{
	Shaders::ShadersLoaded();

	// Uniforms
	mProjectionID = glGetUniformLocation(programID, "mProjection");

	renderedTextureUniform = glGetUniformLocation(programID, "renderedTexture");
	depthTextureUniform = glGetUniformLocation(programID, "depthTexture");
}

void PostFxShaders::Bind()
{
	Shaders::Bind();

	// Projection Matrix
	mat4 mProjection = ortho(0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(mProjectionID, 1, GL_FALSE, &mProjection[0][0]);
}

void PostFxShaders::SetUniforms(GLuint renderTex, GLuint depthTex)
{
	// Render Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTex);
	glUniform1i(renderedTextureUniform, 0);

	// Depth Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glUniform1i(depthTextureUniform, 1);
}

// DepthMapShaders

void DepthMapShaders::ShadersLoaded()
{
	Shaders::ShadersLoaded();

	// Uniforms
	mProjectionID = glGetUniformLocation(programID, "mProjection");
	depthTextureUniform = glGetUniformLocation(programID, "depthTexture");
}

void DepthMapShaders::Bind()
{
	Shaders::Bind();

	// Projection Matrix
	mat4 mProjection = ortho(0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(mProjectionID, 1, GL_FALSE, &mProjection[0][0]);
}

void DepthMapShaders::SetDepthTex(GLuint depthTex)
{
	// Depth Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glUniform1i(depthTextureUniform, 0);
}

// Canvas Shaders

void CanvasShaders::ShadersLoaded()
{
	Shaders::ShadersLoaded();

	// Uniforms
	mpMatrixID = glGetUniformLocation(programID, "modelProjection");
	texID = glGetUniformLocation(programID, "tex");
}

void CanvasShaders::UpdateMatrix()
{
	// Set ortho projection
	mProjection = ortho(0.0f, (float)mWindow->width, (float)mWindow->height, 0.0f);

	// Model Projection Matrix
	mat4 modelProjection = mProjection * mModel;
	glUniformMatrix4fv(mpMatrixID, 1, GL_FALSE, &modelProjection[0][0]);
}

void CanvasShaders::SetTransform(vec2 pos, vec2 size, bool flip_y)
{
	// Set model matrix
	if (flip_y)
	{
		mModel = translate(mat4(1.0f), vec3(pos.x, pos.y + size.y, 0.0f));
		mModel = scale(mModel, vec3(size.x, -size.y, 0.0f));
	}
	else
	{
		mModel = translate(mat4(1.0f), vec3(pos, 0.0f));
		mModel = scale(mModel, vec3(size, 0.0f));
	}

	UpdateMatrix();
}

void CanvasShaders::SetTexture(GLuint tex)
{
	// Set texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
	glUniform1i(texID, 0);
}

// Shaders manager
void ShadersManager::Init()
{
	mMaterial = new MaterialShaders();
	mMaterial->Load("material");

	mCanvas = new CanvasShaders();
	mCanvas->Load("canvas");

	mWater = new WaterShaders();
	mWater->Load("water");
}
