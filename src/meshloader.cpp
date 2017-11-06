#include "main.h"

// Obj loader
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader/tiny_obj_loader.h"

void StaticMesh::loadMesh(const char* path)
{
	// Load mesh object
	string basepath = "assets/models/";
	string fname = basepath + string(path) + ".obj";
	string err;

	if (!LoadObj(&attrib, &shape, &material, &err, fname.c_str(), basepath.c_str(), true))
	{
		printf("Cannot load obj %s! err: %s\n", fname.c_str(), err.c_str());
		return;
	}
}

void StaticMesh::parse()
{
	for (int s = 0; s < shapes.size(); s++)
	{

	}
}
