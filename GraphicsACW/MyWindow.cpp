// David Hart - 2011

#include "MyWindow.h"

using namespace gxbase;

MyWindow::MyWindow() :
	_gxApp(NULL),
	_loaded(false),
	_prevKeyStatePlus(false),
	_prevKeyStateMinus(false)
{
	SetSize(1280, 768);
	SetStencilBits(8);
	SetDepthBits(24);
}

void MyWindow::OnCreate()
{
	std::string file;

	if (_gxApp->ArgCount() == 1)
	{
		file = "default.cfg";
	}
	else if (_gxApp->ArgCount() == 2)
	{
		file = _gxApp->Arg(1);
	}
	else
	{
		MessageBoxA(GetSafeHwnd(), "Invalid Command Line Arguments usage:\nGraphicsACW.exe\nGraphicsACW.exe \"file\"", "Error", MB_OK);
		Close();
	}

	GLWindowEx::OnCreate();

	if (!_config.Read(file.c_str(), _application))
	{
		MessageBoxA(GetSafeHwnd(), (std::string("Invalid config file (") + file + std::string(")")).c_str(), "Error", MB_OK);
		Close();
	}
	else
	{
		_application.Create(*this);
		_loaded = true;
	}
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
	if (_loaded)
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
	{
		if (down && !_prevKeyStatePlus)
			_application.IncrAnimationSpeed();

		_prevKeyStatePlus = down;
	}
	else if (VK_SUBTRACT == key)
	{
		if (down && !_prevKeyStateMinus)
			_application.DecrAnimationSpeed();

		_prevKeyStateMinus = down;
	}
	else if ('R' == key)
	{
		_application.ResetAnimationSpeed();
	}

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

void MyWindow::SetGXApp(gxbase::App* gxApp)
{
	_gxApp = gxApp;
}