#pragma once

#include "GXBase.h"
#include "Renderer.h"
#include "Dome.h"
#include "Tree.h"
#include "House.h"
#include "Base.h"
#include "Terrain.h"
#include "Pond.h"
#include "ParticleSystem.h"
#include "SnowParticles.h"
#include "SnowDrift.h"
#include "Texture.h"

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

	void Update(float dt);
	void UpdateViewMatrix();

	Renderer _renderer;
	Dome _dome;
	Tree _tree;
	House _house;
	Base _base;
	Terrain _terrain;
	Pond _pond;
	ParticleSystem _particleSystem;
	ParticleEmitter _fireEmitter;
	ParticleEmitter _smokeEmitter;
	Texture _smokeTexture;
	Texture _fireTexture;

	SnowParticles _snowParticles;
	SnowDrift _snowDrift;

	Light _spotLights[4];
	Light _directionalLights[4];

	Matrix4 _view;

	bool _cursorKeyDown[4];
	bool _sunMode;
	Vector3 _sunDirection;

	float _cameraPitch;
	float _cameraYaw;

	enum eSeason
	{
		SEASON_SPRING,
		SEASON_SUMMER,
		SEASON_AUTUMN,
		SEASON_WINTER,
	};
	eSeason _season;
	float _elapsed;
	float _animationSpeed;

	const float _lengthSpring;
	const float _lengthSummer;
	const float _lengthAutumn;
	const float _lengthWinter;

	static const float ANIMATION_SPEED_INCREMENT;
	static const float ANIMATION_SPEED_MAX;
	static const float ANIMATION_SPEED_MIN;
};

