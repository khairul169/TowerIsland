#include "main.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

bool Texture::LoadTexture(const char* path, bool mipmaps)
{
	// Load image
	unsigned char* image = stbi_load(path, &width, &height, &bpp, 0);

	// Failed to load image
	if (image == NULL)
	{
		printf("Failed to load %s\n", path);
		return false;
	}

	// Generate opengl texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	if (bpp == STBI_rgb_alpha)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		hasAlpha = true;
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		hasAlpha = false;
	}

	// Texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	if (mipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	texID = textureID;
	glBindTexture(GL_TEXTURE_2D, 0);

	printf("Texture %s loaded.\n", path);

	// Free image data
	stbi_image_free(image);
	return true;
}
