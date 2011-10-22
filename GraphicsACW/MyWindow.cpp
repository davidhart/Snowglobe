#include "MyWindow.h"
#include <iostream>

using namespace gxbase;

MyWindow::MyWindow()
{
	SetSize(1280, 768);
}

void MyWindow::OnCreate()
{
	GLWindowEx::OnCreate();

	_renderer.Create(this);

	static float triangle[] = 
	{
		-0.9f, -0.9f, 1.0f, 0.0f, 0.0f,
		 0.9f, -0.9f, 0.0f, 1.0f, 0.0f,
		0.0f,0.9f, 0.0f, 0.0f, 1.0f,
	};

	_testBuffer.Create(_renderer, triangle, sizeof(triangle));

	ArrayElement vertLayout[] = 
	{
		{ _testBuffer, AE_VERTEX, 2, AE_FLOAT, 20, 0 },
		{ _testBuffer, AE_COLOR, 3, AE_FLOAT, 20, 8 }
	};

	_testBinding.Create(_renderer, vertLayout, 2);

	_testVertShader.Create(_renderer, "#version 130\n out vec3 color;\n void main() { gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; color = gl_Color.xyz; }");
	_testFragShader.Create(_renderer, "#version 130\n in vec3 color;\n void main() { gl_FragColor = vec4(color, 1); }");

	_testShader.Create(_renderer, _testVertShader, _testFragShader);
}

void MyWindow::OnDisplay()
{
	static double time = App::GetTime();
	double delta = App::GetTime() - time;

	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();
	glRotated(delta * 180, 0, 1, 0);

	_testShader.Use();

	_testBinding.Bind();

	_renderer.Draw(PT_TRIANGLES, 0, 3);

	_testBinding.Unbind();

	glPopMatrix();

	SwapBuffers();
}

void MyWindow::OnIdle()
{
	Redraw();
}

void MyWindow::OnDestroy()
{
	_testBinding.Dispose();
	_testBuffer.Dispose();

	_testShader.Dispose();

	_testVertShader.Dispose();
	_testFragShader.Dispose();

	_renderer.Dispose();

	GLWindowEx::OnDestroy();
}

void MyWindow::OnKeyboard(int key, bool down)
{
	if (VK_F9 == key && down)
	{
		bool full = !GetFullscreen();
		SetFullscreen(full);

		std::cout << full << std::endl;
	}

	if (VK_ESCAPE == key && down)
		Close();
}