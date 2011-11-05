#include "MyWindow.h"
#include "Util.h"
#include "ObjFile.h"
#include "Matrix4.h"

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

	_dome.Create(_renderer);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 90, (float)Width() / Height(), 0.1f, 100);
	glMultMatrixf((float*)&perspective);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glMatrixMode(GL_MODELVIEW);
	gluLookAt(9, 1, 0, 0, 0, 0, 0, 1, 0);

}

void MyWindow::OnDisplay()
{
	static double time = App::GetTime();
	double delta = App::GetTime() - time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glRotated(delta * 70, 0, 1, 0);
	glScaled(6, 6, 6);

	_dome.Draw(_renderer);

	glPopMatrix();

	SwapBuffers();
}

void MyWindow::OnIdle()
{
	Redraw();
}

void MyWindow::OnDestroy()
{
	_dome.Dispose();

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
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 90, (float)Width() / Height(), 0.1f, 100);
	glMultMatrixf((float*)&perspective);

	glMatrixMode(GL_MODELVIEW);

	GLWindowEx::OnResize(w, h);
}