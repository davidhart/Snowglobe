#include "MyWindow.h"

using namespace gxbase;

MyWindow::MyWindow()
{
	SetSize(1280, 768);
	SetStencilBits(8);
	SetDepthBits(24);
}

void MyWindow::OnCreate()
{
	GLWindowEx::OnCreate();

	if (!_config.Read("default.cfg", _application))
	{
		MessageBoxA(GetSafeHwnd(), "Invalid config file (default.cfg)", "Error", MB_OK);
		Close();
	}
	_application.Create(*this);
}

void MyWindow::OnDisplay()
{
	static float startTime = (float)App::GetTime();
	static float prevTime = (float)App::GetTime();
	float time = (float)App::GetTime();
	float elapsed = time - startTime;
	float delta = time - prevTime;

	_application.Update(delta);
	_application.Draw();
	
	SwapBuffers();

	prevTime = time;
}

void MyWindow::OnIdle()
{
	Redraw();
}

void MyWindow::OnDestroy()
{
	_application.Dispose();

	GLWindowEx::OnDestroy();
}

void MyWindow::OnKeyboard(int key, bool down)
{
	if (VK_F9 == key && down)
	{
		bool full = !GetFullscreen();
		SetFullscreen(full);
	}

	if (VK_ESCAPE == key && down)
		Close();

	if (VK_ADD == key)
		_application.IncrAnimationSpeed();
	else if (VK_SUBTRACT == key)
		_application.DecrAnimationSpeed();

	if (key == VK_RIGHT)
		_application.CameraKeyEvent(KEY_RIGHT, down);
	if (key == VK_LEFT)
		_application.CameraKeyEvent(KEY_LEFT, down);
	if (key == VK_UP)
		_application.CameraKeyEvent(KEY_UP, down);
	if (key == VK_DOWN)
		_application.CameraKeyEvent(KEY_DOWN, down);

	if (key == 'S' && down)
		_application.ToggleSunMode();

	if (key == 'M' && down)
		_application.NextTreeMode();
}

void MyWindow::OnResize(int w, int h)
{
	GLWindowEx::OnResize(w, h);
	_application.Resize(w, h);
}