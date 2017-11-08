#include "main.h"

char* Utils::getFileString(char *fname)
{
	char *string = NULL;
	FILE *f = fopen(fname, "rb");

	if (f)
	{
		fseek(f, 0, SEEK_END);
		long fsize = ftell(f);
		fseek(f, 0, SEEK_SET);

		string = (char*)malloc(fsize + 1);
		if (string)
			fread(string, fsize, 1, f);
		fclose(f);

		string[fsize] = 0;
	}

	return string;
}

vec3 Utils::getAxis(mat4 matrix, int axis)
{
	if (axis < 0 || axis > 2)
		return vec3();

	return vec3(matrix[0][axis], matrix[1][axis], matrix[2][axis]);
}

float Utils::lerp(float a, float b, float f)
{
	return a + f * (b - a);
}
