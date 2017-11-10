#ifndef GUI_H
#define GUI_H

#include "main.h"

class GUI
{
private:
	TextureCanvas* m_TitleImage;

	FramebufferCanvas* mWaterReflection;
	FramebufferCanvas* mWaterRefraction;

public:
	void Init();
	void Redraw();
};

#endif // !GUI_H
