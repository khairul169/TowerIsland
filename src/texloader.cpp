#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

bool Texture::loadTexture(const char* path)
{
	// Load image
	unsigned char* image = stbi_load(path, &width, &height, &bpp, STBI_rgb);

	// Failed to load image
	if (image == NULL)
	{
		printf("Failed to load \"%s\"\n", path);
		return false;
	}

	// Generate opengl texture
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	// Texture parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	if (bpp == STBI_rgb)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	
	if (bpp == STBI_rgb_alpha)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

	glBindTexture(GL_TEXTURE_2D, 0);

	// Free image data
	stbi_image_free(image);
	return true;
}
