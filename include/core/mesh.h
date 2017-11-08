#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "main.h"
#include <vector>

// Assimp Importer
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace std;

class Material
{
public:
	vec3 color;
	Texture *diffuse;

public:
	Material() {}
};

class Mesh
{
public:
	// Property
	size_t numIndices;
	GLuint vertexBuffer, indicesBuffer, vao;

	// Mat
	Material *material;

	// Transform
	vec3 position;
	quat quaternion;
	vec3 scaling;

public:
	Mesh()
	{
		position = vec3(0.0f);
		quaternion = quat();
		scaling = vec3(1.0f);
	}

	void create(vector<float> buffers, vector<unsigned int> indices);
	void draw();
};

class StaticMesh
{
private:
	// Mesh attributes
	string basepath;
	string filename;

	// Mesh handler
	vector<Mesh*> meshes;

public:
	// Transform
	vec3 position;
	quat quaternion;
	vec3 scaling;

public:
	StaticMesh()
	{
		position = vec3(0.0f);
		quaternion = quat();
		scaling = vec3(1.0f);
	}

	void loadMesh(const char* path);
	void draw();
	
private:
	void parse(const aiScene* scene);
};

class QuadMesh
{
private:
	GLuint vao, vertexID;

public:
	QuadMesh();

	void draw();
};

#endif // !MESHLOADER_H
