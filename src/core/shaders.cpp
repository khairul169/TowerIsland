#include "main.h"

#include <stdlib.h>
#include <vector>

void Shaders::load(const char *name)
{
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
	checkShader(vertID);

	// Compile shader
	glShaderSource(fragID, 1, &fragmentSource, NULL);
	glCompileShader(fragID);

	// Check shader
	checkShader(fragID);

	// Link the program
	programID = glCreateProgram();
	glAttachShader(programID, vertID);
	glAttachShader(programID, fragID);
	glLinkProgram(programID);

	// Check the program
	checkShader(programID);

	glDetachShader(programID, vertID);
	glDetachShader(programID, fragID);

	glDeleteShader(vertID);
	glDeleteShader(fragID);

	shadersLoaded();
}

void Shaders::checkShader(int id)
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

void Shaders::shadersLoaded()
{
	// Shaders loaded.
	printf("Shaders %s loaded.\n", shaderName);
}

void Shaders::bind()
{
	glUseProgram(programID);
}

GLuint Shaders::getProgramID()
{
	return programID;
}

// Material Shaders

void MaterialShaders::shadersLoaded()
{
	Shaders::shadersLoaded();

	// Perspective
	projection = mat4(0.0f);
	view = mat4(0.0f);
	model = mat4(1.0f);

	// Update projection
	updateProjection();

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

void MaterialShaders::setModelMatrix(mat4 mat)
{
	model = mat;
}

void MaterialShaders::updateCamera(Camera *cam)
{
	projection = cam->getProjection();
	view = cam->getView();
}

void MaterialShaders::updateProjection()
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

void MaterialShaders::setColor(vec3 col)
{
	glUniform3f(colorUniform, col.x, col.y, col.z);
}

void MaterialShaders::setTexture(Texture *tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->getTextureID());
	glUniform1i(diffuseUniform, 0);
}

void ScreenShaders::shadersLoaded()
{
	Shaders::shadersLoaded();

	// Uniforms
	renderedTextureUniform = glGetUniformLocation(programID, "renderedTexture");
	depthTextureUniform = glGetUniformLocation(programID, "depthTexture");

	timeUniform = glGetUniformLocation(programID, "time");
}

void ScreenShaders::setUniforms(GLuint renderTex, GLuint depthTex, float time)
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
