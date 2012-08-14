// David Hart - 2011
// 
// class Application
//   Application's responsibility is rendering and updating the objects
//   within the scene

#pragma once

#include "Renderer.h"
#include "Dome.h"
#include "Tree.h"
#include "TreeBuilder.h"
#include "House.h"
#include "Base.h"
#include "Terrain.h"
#include "Pond.h"
#include "ParticleSystem.h"
#include "SnowParticles.h"
#include "SnowDrift.h"
#include "Texture.h"
#include "Lightning.h"
#include "Framebuffer.h"

#include "DebugSprite.h"
#include "DebugFont.h"
#include "WorkerThread.h"

class MyWindow;

enum eCameraKey
{
	KEY_UP = 0,
	KEY_DOWN = 1,
	KEY_LEFT = 2,
	KEY_RIGHT = 3,
};

class Application
{

public:

	Application();
	void Create(MyWindow& window);
	void Dispose();
	void Draw();
	void Update(float delta);
	void Resize(int width, int height);

	void CameraKeyEvent(eCameraKey key, bool down);
	void ToggleSunMode();
	void NextTreeMode();
	void IncrAnimationSpeed();
	void DecrAnimationSpeed();
	void ResetAnimationSpeed();
	void AddTreePattern(const std::string& seed, const LSystem& lsystem, unsigned int numLeaves, unsigned int iterations);
	void SetNumTreeLeaves(unsigned int numLeaves);

	void TreeTaskComplete();

	void SetView(unsigned int view);

private:

	void PrintHud();
	void UpdateViewMatrix();
	void FlipLights();

	void CreateRenderTexture();

	void SetShadowMatricesSpot();
	void SetShadowMatricesSun();

	Renderer _renderer;
	Dome _dome;
	Tree _tree;
	House _house;
	Base _base;
	Terrain _terrain;
	Pond _pond;
	Lightning _lightning;
	ParticleSystem _particleSystem;
	ParticleEmitter _fireEmitter;
	ParticleEmitter _smokeEmitter;
	Texture _smokeTexture;
	Texture _fireTexture;

	Framebuffer _testBuffer;
	Texture _testTexture;
	Texture _testTexture2;

	TreeBuilder _treeBuilder;
	bool _treeReady;
	bool _resized;


	DebugSprite _debugSprite;
	DebugFont _debugFont;
	WorkerThread _workerThread;

	SnowParticles _snowParticles;
	SnowDrift _snowDrift;

	Light _spotLights[4];
	Light _directionalLights[4];

	Matrix4 _view;
	Matrix4 _projection;

	Matrix4 _shadowView;
	Matrix4 _shadowProjection;

	std::string _treePattern;
	unsigned int _numTreeLeaves;

	bool _cameraKeyDown[4];
	Vector3 _sunDirection;
	bool _sunMode;

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

	float _lengthSpring;
	float _lengthSummer;
	float _lengthAutumn;
	float _lengthWinter;

	Vector3 _viewDirections[4];
	Vector3 _viewTargets[4];
	unsigned int _currentView;

	int _width;
	int _height;

	static const float ANIMATION_SPEED_INCREMENT;
	static const float ANIMATION_SPEED_MAX;
	static const float ANIMATION_SPEED_MIN;
	static const float DAYCYCLE_SPEED;

	static const Vector3 COLOR_SUNSET;
	static const Vector3 COLOR_SUNRISE;
	static const Vector3 COLOR_DAYLIGHT;

	static const Vector3 BG_COLOR_SUNSET;
	static const Vector3 BG_COLOR_SUNRISE;
	static const Vector3 BG_COLOR_DAYLIGHT;
	static const Vector3 BG_COLOR_NIGHTLIGHT;
};