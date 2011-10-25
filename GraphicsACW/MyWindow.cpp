#include "MyWindow.h"
#include "Util.h"
#include "ObjFile.h"

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

	_model.Read("teapot.obj");

	static float triangle[] = 
	{
		-0.9f, -0.9f, 0.0f, 1.0f, 0.0f, 0.0f,
		 0.9f, -0.9f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f,0.9f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

	_vertexBuffer.Create(_renderer, _model.GetVertexData(), _model.GetNumVertices() * _model.GetVertexStride());

	static unsigned char indices[] =
	{
		0, 1, 2,
	};

	_indexBuffer.Create(_renderer, _model.GetIndexData(), _model.GetNumIndices() * sizeof(float));

	std::string shaderSource;
	Util::ReadTextFileToString("basic.vsh", shaderSource);

	_testVertShader.Create(_renderer, shaderSource.c_str());

	Util::ReadTextFileToString("basic.fsh", shaderSource);
	_testFragShader.Create(_renderer, shaderSource.c_str());

	_testShader.Create(_renderer, _testVertShader, _testFragShader);

	ArrayElement vertLayout[] = 
	{
		{ _vertexBuffer, _testShader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, 0, 0 },
	};

	_testBinding.Create(_renderer, vertLayout, 1, _indexBuffer, AE_UINT);

	glMatrixMode(GL_PROJECTION_MATRIX);
	gluPerspective(90, 1.3, 0.001, 100); 

	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	gluLookAt(5, 5, 0, 0, 0, 0, 0, 1, 0);
}

void MyWindow::OnDisplay()
{
	static double time = App::GetTime();
	double delta = App::GetTime() - time;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW_MATRIX);
	glPushMatrix();

	glRotated(delta * 180, 0, 1, 0);

	_testShader.Use();

	_renderer.Draw(_testBinding, PT_TRIANGLES, 0, _model.GetNumIndices());

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
	_vertexBuffer.Dispose();
	_indexBuffer.Dispose();

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
	}

	if (VK_ESCAPE == key && down)
		Close();
}