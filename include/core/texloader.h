#ifndef TEXLOADER_H
#define TEXLOADER_H

#include "main.h"

class Texture
{
public:
	int width, height, bpp;
	bool hasAlpha;
	GLuint texID;

public:
	bool LoadTexture(const char* path, bool mipmaps = true);
};

#endif // !TEXLOADER_H
