#include "MyWindow.h"

using namespace gxbase;

MyWindow::MyWindow() :
	_testBuffer(*this),
	_testBinding(*this)
{

}

void MyWindow::OnCreate()
{
	GLWindowEx::OnCreate();

	static float triangle[] = 
	{
		-0.9f, -0.9f,
		 0.9f, -0.9f,
		0.0f,0.9f,
	};

	_testBuffer.Create(triangle, sizeof(triangle));

	ArrayElement vertLayout[] = {
		{ _testBuffer, AE_VERTEX, 2, AE_FLOAT, 8, 0 }
	};

	_testBinding.Create(vertLayout, 1);
}

void MyWindow::OnDisplay()
{
	static double time = App::GetTime();
	double delta = App::GetTime() - time;

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotated(delta * 180, 0, 1, 0);

	_testBinding.Bind();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	_testBinding.Unbind();

	glPopMatrix();

	SwapBuffers();
}

void MyWindow::OnIdle()
{
	Redraw();
}