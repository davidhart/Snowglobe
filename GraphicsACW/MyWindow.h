#pragma once

#include "GXBase.h"
#include "Renderer.h"
#include "Dome.h"
#include "Tree.h"

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

	Renderer _renderer;
	Dome _dome;
	Tree _tree;
};

