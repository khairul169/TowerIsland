#ifndef UTILS_H
#define UTILS_H

#include "main.h"

class Utils
{
public:
	static char* getFileString(char *fname);
	static vec3 getAxis(mat4 matrix, int axis);
	static float lerp(float a, float b, float f);
};

#endif