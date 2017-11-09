#ifndef GUI_H
#define GUI_H

#include "main.h"

class GUI
{
private:
	TextureCanvas* m_TitleImage;

public:
	void Init();
	void Redraw();
};

#endif // !GUI_H
