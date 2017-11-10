#include "main.h"

void GUI::Init()
{
	// Init canvas
	m_TitleImage = new TextureCanvas("assets/sprites/title.png");

	mWaterReflection = new FramebufferCanvas(mVisualRender->mWaterReflectionFBO);
	mWaterRefraction = new FramebufferCanvas(mVisualRender->mWaterRefractionFBO);
}

void GUI::Redraw()
{
	// Render main gui
	mMain->RenderGUI();

	// Render other gui
	m_TitleImage->Draw(vec2(0, mWindow->height - m_TitleImage->texture->height));

	//mWaterReflection->Draw(vec2(10, 10), vec2(mWindow->width, mWindow->height) / 4.0f);
	//mWaterRefraction->Draw(vec2(mWindow->width - (mWindow->width / 4) - 10, 10), vec2(mWindow->width, mWindow->height) / 4.0f);
}
