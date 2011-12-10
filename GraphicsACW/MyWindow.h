#pragma once

#include "GXBase.h"
#include "Renderer.h"
#include "Dome.h"
#include "Tree.h"
#include "House.h"
#include "Base.h"
#include "Terrain.h"
#include "Pond.h"
#include "SmokeParticles.h"
#include "SnowParticles.h"
#include "SnowDrift.h"

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
	House _house;
	Base _base;
	Terrain _terrain;
	Pond _pond;
	SmokeParticles _smoke;
	SnowParticles _snowParticles;
	SnowDrift _snowDrift;

	Light _spotLights[4];
	Light _directionalLights[4];

	bool _cursorKeyDown[4];
	bool _sunMode;
	Vector3 _sunDirection;

	float _cameraPitch;
	float _cameraYaw;
};

