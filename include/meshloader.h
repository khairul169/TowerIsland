#ifndef MESHLOADER_H
#define MESHLOADER_H

#include "main.h"
#include <vector>

// Objloader
#include "tinyobjloader/tiny_obj_loader.h"

using namespace std;
using namespace tinyobj;

class StaticMesh: public Mesh
{
private:
	attrib_t attrib;
	vector<shape_t> shape;
	vector<material_t> material;

public:
	void loadMesh(const char* path);
	
private:
	void parse();
};

class Mesh
{
protected:
	GLuint vertexBuffer, uvBuffer;

public:
	// todo
};

#endif // !MESHLOADER_H
