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
	printf("Shaders \"%s\" loaded.\n", shaderName);
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

	// Uniforms
	matrixUniform = glGetUniformLocation(programID, "modelViewProjection");
	colorUniform = glGetUniformLocation(programID, "color");
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
	glUniformMatrix4fv(matrixUniform, 1, GL_FALSE, &modelViewProjection[0][0]);
}

void MaterialShaders::setColor(vec3 col)
{
	glUniform3f(colorUniform, col.x, col.y, col.z);
}
