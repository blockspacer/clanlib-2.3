
#pragma once

#include "ui_screen.h"

class SplashScreen : public UIScreen
{
public:
	SplashScreen(CL_GUIManager *gui);
	~SplashScreen();

private:
	void on_render(CL_GraphicContext &gc, const CL_Rect &rect);
	void on_button_click();

	CL_Label *label;
	CL_PushButton *button;
};
