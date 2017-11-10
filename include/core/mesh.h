#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "main.h"
#include <vector>

// Assimp Importer
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

using namespace std;

class PhysicsObject;

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

	// Physics Object
	PhysicsObject* physicsBody;

public:
	Mesh()
	{
		position = vec3(0.0f);
		quaternion = quat();
		scaling = vec3(1.0f);
		physicsBody = NULL;
	}

	void Create(vector<float> buffers, vector<unsigned int> indices);
	void Draw();
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

	void LoadMesh(const char* path);
	void Draw();
	
private:
	void Parse(const aiScene* scene);
};

class QuadMesh
{
protected:
	GLuint vao, vertexID;

public:
	QuadMesh();
	void Draw();
};

class PlaneMesh
{
protected:
	GLuint vao, vertexID;

public:
	PlaneMesh();
	void Draw();
};

// WaterMesh
class WaterMesh : public PlaneMesh
{
public:
	WaterMesh();
	void Draw();
};

#endif // !MESHLOADER_H
