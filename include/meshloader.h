#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "main.h"
#include <vector>

// Objloader
#include "tinyobjloader/tiny_obj_loader.h"

using namespace std;
using namespace tinyobj;

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
	size_t numTris;
	GLuint vtb, uvb, nmb, vao;

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

	void draw();
};

class StaticMesh
{
private:
	// Mesh attributes
	string basepath;
	string filename;

	attrib_t attrib;
	vector<shape_t> shapes;
	vector<material_t> materials;

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
	void parse();
};

#endif // !MESHLOADER_H
