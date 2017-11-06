#ifndef TEXLOADER_H
#define TEXLOADER_H

#include "main.h"

class Texture
{
private:
	int width, height, bpp;

	GLuint texID;

public:
	bool loadTexture(const char* path);
	
	GLuint getTextureID() { return texID; }
};

#endif // !TEXLOADER_H
