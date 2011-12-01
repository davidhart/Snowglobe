#include "MyWindow.h"
#include "Util.h"
#include "ObjFile.h"
#include "Matrix4.h"
#include "LSystem.h"
#include "Terrain.h"
#include <string>
#include <iostream>

using namespace gxbase;

MyWindow::MyWindow() :
	_growTree (true)
{
	SetSize(1280, 768);
}

void MyWindow::OnCreate()
{
	GLWindowEx::OnCreate();

	glEnable(GL_MULTISAMPLE_ARB);

	_renderer.Create(this);

	LSystem test;
	// regular tree
	/*
	test.AddRule("[]", "[--^B[]>>>B[]>>>B[]>>>B[]]");
	std::string result;
	test.EvaluateRules("[]", result, 3);
	*/

	// tree with mutations
	test.AddRule("[]", "[---^B[]>>>B[]>>>vB[]>>>^B[]]");
	test.AddRule("[]", "[---^B[]>>>>>>>B[]]");
	test.AddRule("[]", "[---^B[]>>>>B[]>>>>B[]]");

	std::string result;
	test.EvaluateRules("[]", result, 8);

	// crimbo tree
	/*test.AddRule("L", "[---B[]^^>>>--BL>>>>BL>>>>BL]");
	test.AddRule("L", "[---B[]^^>>>>--BL>>>>BL>>>>BL]");

	test.AddRule("[]", "[--B[]^^>>>>>--BL>>>BL>>>BL>>>BL]");
	test.AddRule("[]", "[--B[]^^>>>--BL>>>BL>>>BL>>>BL]");

	std::string result;
	test.EvaluateRules("[]", result, 5);*/
	

	_dome.Create(_renderer);
	_tree.Create(_renderer, result, 2, 4000);
	_house.Create(_renderer);
	_base.Create(_renderer);
	_terrain.Create(_renderer);
	_pond.Create(_renderer);
	_smoke.Create(_renderer, 200);

	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 75, (float)Width() / Height(), 0.1f, 1000);
	_renderer.ProjectionMatrix(perspective);

	Light light;
	Light::Spot(light, Vector3(6, 6, 0), Vector3(-1, -1, 0), 30, 32, 1);
	//Light::Point(light, Vector3(6, 6, 0));
	//Light::Directional(light, Vector3(-1, -1, 0));
	light.SetDiffuseColor(Vector3(1));
	light.SetSpecularColor(Vector3(1));
	light.SetSpecularPower(200);
	_renderer.SetLight(0, light);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	std::cout << _tree.MaxBranchDepth() << std::endl;
}

void MyWindow::OnDisplay()
{
	static float startTime = (float)App::GetTime();
	static float prevTime = (float)App::GetTime();
	float time = (float)App::GetTime();
	float elapsed = time - startTime;
	float delta = time - prevTime;

	_smoke.Update(delta);

	if (_growTree)
		_tree.Grow(delta);
	else
		_tree.Shrink(delta);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Matrix4 view;
	//Matrix4::LookAt(view, Vector3(0, 10, 0), Vector3(0, 0, 0), Vector3(0, 0, 1));
	Matrix4::LookAt(view, Vector3(9, 2, 0), Vector3(0, 2, 0), Vector3(0, 1, 0));
	Matrix4 rotation;
	//Matrix4::RotationAxis(rotation, Vector3(0, 1, 0), 0);
	Matrix4::RotationAxis(rotation, Vector3(0, 1, 0), elapsed);

	_renderer.ViewMatrix(view * rotation);

	_tree.Draw(_renderer);
	_house.Draw(_renderer);
	_terrain.Draw(_renderer);
	_base.Draw(_renderer);
	_dome.DrawBack(_renderer);

	_smoke.Draw(_renderer);

	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glDepthMask(GL_FALSE);
	glEnable(GL_STENCIL_TEST);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); 
	_pond.Draw(_renderer);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	glDepthMask(GL_TRUE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glCullFace(GL_FRONT);

	_renderer.ClipPlane(Vector4(0, -1, 0, 0));

	_terrain.DrawReflection(_renderer);
	_tree.DrawReflection(_renderer);
	_house.DrawReflection(_renderer);
	_smoke.DrawReflected(_renderer);

	glCullFace(GL_BACK);
	_renderer.ClipPlane(Vector4(0, 0, 0, 0));

	_pond.Draw(_renderer);
	glDisable(GL_STENCIL_TEST);

	_dome.DrawFront(_renderer);
	
	SwapBuffers();

	prevTime = time;
}

void MyWindow::OnIdle()
{
	Redraw();
}

void MyWindow::OnDestroy()
{
	_base.Dispose();
	_dome.Dispose();
	_tree.Dispose();
	_house.Dispose();
	_terrain.Dispose();
	_pond.Dispose();
	_smoke.Dispose();

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

	if (VK_ADD == key)
	{
		_growTree = true;
	}
	else if (VK_SUBTRACT == key)
	{
		_growTree = false;
	}
}

void MyWindow::OnResize(int w, int h)
{
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 75, (float)Width() / Height(), 0.1f, 100);
	_renderer.ProjectionMatrix(perspective);

	GLWindowEx::OnResize(w, h);
}