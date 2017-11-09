#include "main.h"

void GUI::Init()
{
	// Init canvas
	m_TitleImage = new TextureCanvas("assets/sprites/title.png");
}

void GUI::Redraw()
{
	// Render main gui
	mMain->RenderGUI();

	// Render other gui
	m_TitleImage->Draw(vec2(0, mWindow->height-(m_TitleImage->texture->height * 0.8f)), 0.8f);
}
