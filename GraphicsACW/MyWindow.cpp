#include "MyWindow.h"
#include "Util.h"
#include "ObjFile.h"
#include "Matrix4.h"
#include "LSystem.h"
#include <string>

using namespace gxbase;

MyWindow::MyWindow()
{
	SetSize(1280, 768);
}

void MyWindow::OnCreate()
{
	GLWindowEx::OnCreate();

	_renderer.Create(this);

	LSystem test;
	test.AddRule("L", "B[L>>L>>L>>L]");
	std::string result;
	test.EvaluateRules("[L>>L>>L>>L]", result, 5);

	_dome.Create(_renderer);
	_tree.Create(_renderer, result);

	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 90, (float)Width() / Height(), 0.1f, 100);
	_renderer.ProjectionMatrix(perspective);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

void MyWindow::OnDisplay()
{
	static double time = App::GetTime();
	double delta = App::GetTime() - time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Matrix4 view;
	Matrix4::LookAt(view, Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(1, 0, 0));
	Matrix4::LookAt(view, Vector3(9, 1, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Matrix4 rotation;
	Matrix4::RotationAxis(rotation, Vector3(0, 1, 0), delta);
	_renderer.ViewMatrix(view * rotation);

	_dome.DrawBack(_renderer);
	_tree.Draw(_renderer);
	_dome.DrawFront(_renderer);

	SwapBuffers();
}

void MyWindow::OnIdle()
{
	Redraw();
}

void MyWindow::OnDestroy()
{
	_dome.Dispose();
	_tree.Dispose();

	_renderer.Dispose();

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
}

void MyWindow::OnResize(int w, int h)
{
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 90, (float)Width() / Height(), 0.1f, 100);
	_renderer.ProjectionMatrix(perspective);

	GLWindowEx::OnResize(w, h);
}