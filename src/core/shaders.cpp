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
	modelMatrix = glGetUniformLocation(programID, "model");
	viewMatrix = glGetUniformLocation(programID, "view");
	matrixUniform = glGetUniformLocation(programID, "modelViewProjection");
	normalMatrix = glGetUniformLocation(programID, "normal");

	// Frag Uniforms
	colorUniform = glGetUniformLocation(programID, "color");
	diffuseUniform = glGetUniformLocation(programID, "diffuse");

	lightPosUniform = glGetUniformLocation(programID, "lightPos");
	lightAmbientUniform = glGetUniformLocation(programID, "lightAmbient");
	lightColorUniform = glGetUniformLocation(programID, "lightColor");
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
	vec4 lightAmbient = vec4(1.0f, 1.0f, 1.0f, 0.8f);
	vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);

	glUniform3f(lightPosUniform, lightPos.x, lightPos.y, lightPos.z);
	glUniform4f(lightAmbientUniform, lightAmbient.x, lightAmbient.y, lightAmbient.z, lightAmbient.w);
	glUniform3f(lightColorUniform, lightColor.x, lightColor.y, lightColor.z);
}

void MaterialShaders::SetColor(vec3 col)
{
	glUniform4f(colorUniform, col.x, col.y, col.z, 1.0f);
}

void MaterialShaders::SetColor(vec4 col)
{
	glUniform4f(colorUniform, col.x, col.y, col.z, col.w);
}

void MaterialShaders::SetTexture(Texture *tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->texID);
	glUniform1i(diffuseUniform, 0);
}

void ScreenShaders::ShadersLoaded()
{
	Shaders::ShadersLoaded();

	// Uniforms
	mProjectionID = glGetUniformLocation(programID, "mProjection");

	renderedTextureUniform = glGetUniformLocation(programID, "renderedTexture");
	depthTextureUniform = glGetUniformLocation(programID, "depthTexture");

	timeUniform = glGetUniformLocation(programID, "time");
}

void ScreenShaders::Bind()
{
	Shaders::Bind();

	// Projection Matrix
	mProjection = ortho(0.0f, 1.0f, 0.0f, 1.0f);
	glUniformMatrix4fv(mProjectionID, 1, GL_FALSE, &mProjection[0][0]);
}

void ScreenShaders::SetUniforms(GLuint renderTex, GLuint depthTex, float time)
{
	// Render Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTex);
	glUniform1i(renderedTextureUniform, 0);

	// Depth Texture
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, depthTex);
	glUniform1i(depthTextureUniform, 1);

	// Time
	glUniform1f(timeUniform, time);
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

void ShadersManager::Init()
{
	mMaterial = new MaterialShaders();
	mMaterial->Load("material");

	mCanvas = new CanvasShaders();
	mCanvas->Load("canvas");
}
