#pragma once

#include "GXBase.h"
#include "Application.h"
#include "Config.h"

class MyWindow : public gxbase::GLWindowEx
{

public:

	MyWindow();
	void OnCreate();
	void OnDisplay();
	void OnIdle();
	void OnDestroy();
	void OnResize(int w, int h);
	void OnKeyboard(int key, bool down);

private:

	Application _application;
	Config _config;

};

