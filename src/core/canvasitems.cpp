#include "main.h"

void FramebufferCanvas::Draw(vec2 pos, vec2 size)
{
	// Setup shaders
	mShadersMgr->mCanvas->Bind();

	// Set uniforms
	mShadersMgr->mCanvas->SetTransform(pos, size, true);
	mShadersMgr->mCanvas->SetTexture(fbo->renderTex);

	// Draw the mesh
	mesh->Draw();
}

TextureCanvas::TextureCanvas(const char* texturePath)
{
	// Setup quad mesh
	this->mesh = new QuadMesh();

	// Load texture
	this->texture = new Texture();
	this->texture->LoadTexture(texturePath, false);
}

void TextureCanvas::Draw(vec2 pos)
{
	// Draw canvas
	Draw(pos, vec2(texture->width, texture->height));
}

void TextureCanvas::Draw(vec2 pos, float size)
{
	// Draw canvas
	Draw(pos, vec2(texture->width, texture->height) * size);
}

void TextureCanvas::Draw(vec2 pos, vec2 size)
{
	// Setup shaders
	mShadersMgr->mCanvas->Bind();

	// Set uniforms
	mShadersMgr->mCanvas->SetTransform(pos, size);
	mShadersMgr->mCanvas->SetTexture(texture->texID);

	// Draw the mesh
	mesh->Draw();
}
