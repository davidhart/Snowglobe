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
	_sunMode(true),
	_sunDirection(0, -1, 0),
	_cameraPitch(0),
	_cameraYaw(0)
{
	for (int i = 0; i < 4; ++i)
	{
		_cursorKeyDown[i] = false;
	}

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
	_tree.Create(_renderer, result, 2, 2000);
	_house.Create(_renderer);
	_base.Create(_renderer);
	_terrain.Create(_renderer);
	_pond.Create(_renderer);
	_smoke.Create(_renderer, 200);
	_snow.Create(_renderer, 10000);

	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 85, (float)Width() / Height(), 0.1f, 1000);
	_renderer.ProjectionMatrix(perspective);

	_renderer.SetAmbient(Vector3(0.2f));

	// Sun lights
	Light::Directional(_directionalLights[0], _sunDirection);
	_directionalLights[0].SetDiffuseColor(Vector3(1));
	_directionalLights[0].SetSpecularColor(Vector3(1));
	_directionalLights[0].SetSpecularPower(200);
	
	Light::Off(_directionalLights[1]);
	Light::Off(_directionalLights[2]);
	Light::Off(_directionalLights[3]);

	// Spotlights
	Light::Spot(_spotLights[0], Vector3(-0.8f, 7.5f, 0), Vector3(0.6f, -1, 0), 19, 26, 1);
	_spotLights[0].SetDiffuseColor(Vector3(1));
	_spotLights[0].SetSpecularColor(Vector3(1));
	_spotLights[0].SetSpecularPower(200);

	Light::Spot(_spotLights[1], Vector3(0.8f, 7.5f, 0), Vector3(-0.6f, -1, 0), 19, 26, 1);
	_spotLights[1].SetDiffuseColor(Vector3(1.0f, 0.6f, 0.6f));
	_spotLights[1].SetSpecularColor(Vector3(1.0f, 0.6f, 0.6f));
	_spotLights[1].SetSpecularPower(200);

	Light::Spot(_spotLights[2], Vector3(0, 7.5f, -0.8f), Vector3(0, -1, 0.6f), 19, 26, 1);
	_spotLights[2].SetDiffuseColor((Vector3(0.6f, 1.0f, 0.6f)));
	_spotLights[2].SetSpecularColor((Vector3(0.6f, 1.0f, 0.6f)));
	_spotLights[2].SetSpecularPower(200);

	Light::Spot(_spotLights[3], Vector3(0, 7.5f, 0.8f), Vector3(0, -1, -0.6f), 19, 26, 1);
	_spotLights[3].SetDiffuseColor(Vector3(0.75f, 0.75f, 1.0f));
	_spotLights[3].SetSpecularColor(Vector3(0.75f, 0.75f, 1.0f));
	_spotLights[3].SetSpecularPower(200);

	for (int i = 0; i < 4; ++i)
		_renderer.SetLight(i, _directionalLights[i]);

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

	if (_cursorKeyDown[0])
		_cameraYaw += delta;
	if (_cursorKeyDown[1])
		_cameraYaw -= delta;
	if (_cursorKeyDown[2])
		_cameraPitch += delta;
	if (_cursorKeyDown[3])
		_cameraPitch -= delta;

	_smoke.Update(delta);
	_snow.Update(delta);
	_tree.Update(delta);

	Matrix4 sunRotation;
	Matrix4::RotationAxis(sunRotation, Vector3(0, 0, 1), delta);

	Vector4 sunTemp (_sunDirection, 0);
	sunRotation.Transform(sunTemp);
	_sunDirection = Vector3(sunTemp.x(), sunTemp.y(), sunTemp.z());

	if (_sunMode)
	{
		Light::Directional(_directionalLights[0], _sunDirection);
		_renderer.SetLight(0, _directionalLights[0]);
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	Matrix4 view;
	Matrix4::LookAt(view, Vector3(9, 2, 0), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Matrix4 pitch;
	Matrix4::RotationAxis(pitch, Vector3(0, 0, 1), _cameraPitch);
	Matrix4 yaw;
	Matrix4::RotationAxis(yaw, Vector3(0, 1, 0), _cameraYaw);
	Matrix4 t;
	Matrix4::Translation(t, Vector3(0, -2.0f, 0));

	_renderer.ViewMatrix(view * pitch * yaw * t);

	_tree.Draw(_renderer);
	_house.Draw(_renderer);
	_terrain.Draw(_renderer);
	_base.Draw(_renderer);
	_dome.DrawBack(_renderer);

	_pond.DrawStencilMask(_renderer);

	_renderer.ClipPlane(Vector4(0, -1, 0, 0));

	glCullFace(GL_FRONT);
	glEnable(GL_STENCIL_TEST);

	_terrain.DrawReflection(_renderer);
	_tree.DrawReflection(_renderer);
	_house.DrawReflection(_renderer);
	_smoke.DrawReflected(_renderer);
	_snow.DrawReflected(_renderer);

	glCullFace(GL_BACK);
	_renderer.ClipPlane(Vector4(0, 0, 0, 0));

	glDisable(GL_STENCIL_TEST);
	
	_pond.Draw(_renderer);

	_snow.Draw(_renderer);
	_smoke.Draw(_renderer);

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
	_snow.Dispose();

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
		_tree.Grow();
	}
	else if (VK_SUBTRACT == key)
	{
		_tree.Die();
	}

	if (key == VK_RIGHT)
	{
		_cursorKeyDown[0] = down;
	}
	if (key == VK_LEFT)
	{
		_cursorKeyDown[1] = down;
	}
	if (key == VK_UP)
	{
		_cursorKeyDown[2] = down;
	}
	if (key == VK_DOWN)
	{
		_cursorKeyDown[3] = down;
	}
	if (key == 'S' && down)
	{
		_sunMode = !_sunMode;

		if (_sunMode)
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				_renderer.SetLight(i, _directionalLights[i]);
			}
		}
		else
		{
			for (unsigned int i = 0; i < 4; ++i)
			{
				_renderer.SetLight(i, _spotLights[i]);
			}
		}
	}
	if (key == 'M' && down)
	{
		_tree.NextDrawMode();
	}
}

void MyWindow::OnResize(int w, int h)
{
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 75, (float)Width() / Height(), 0.1f, 100);
	_renderer.ProjectionMatrix(perspective);

	GLWindowEx::OnResize(w, h);
}