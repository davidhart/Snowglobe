// David Hart - 2011

#include "Application.h"
#include "MyWindow.h"

#include <sstream>

const float Application::ANIMATION_SPEED_INCREMENT = 0.5f;
const float Application::ANIMATION_SPEED_MIN = 0.0f;
const float Application::ANIMATION_SPEED_MAX = 15.0f;
const float Application::DAYCYCLE_SPEED = 0.5f;

const Vector3 Application::COLOR_SUNSET = Vector3(1.0f, 0.9f, 0.6f);
const Vector3 Application::COLOR_SUNRISE = Vector3(0.75f, 0.9f, 1.0f);
const Vector3 Application::COLOR_DAYLIGHT = Vector3(1.0f, 1.0f, 1.0f);

const Vector3 Application::BG_COLOR_SUNSET = Vector3(0.9f, 0.6f, 0.0f);
const Vector3 Application::BG_COLOR_SUNRISE = Vector3(0.5f, 0.7f, 0.75f);
const Vector3 Application::BG_COLOR_DAYLIGHT = Vector3(0.254f, 0.741f, 1.0f);
const Vector3 Application::BG_COLOR_NIGHTLIGHT = Vector3(0.0f, 0.13f, 0.4f);

// Task to generate new trees on a separate thread
class TreeTask : public ITask
{
public:

	TreeTask(Application& app, TreeBuilder& treebuilder) : 
		_app(app),
		_treebuilder(treebuilder)
	{
	}

	void DoTask()
	{
		_treebuilder.CreateTree();
	}

	void TaskComplete()
	{
		// Report back to the application when the tree is complete
		_app.TreeTaskComplete();
	}

	void Release()
	{
		delete this;
	}

private:

	Application& _app;
	TreeBuilder& _treebuilder;

};

Application::Application() : 
	_numTreeLeaves(0),
	_sunDirection(0, -1, 0),
	_sunMode(true),
	_cameraPitch(0),
	_cameraYaw(0),
	_season(SEASON_SPRING),
	_elapsed(0.0f),
	_animationSpeed(1.0f),
	_lengthSpring(25.0f),
	_lengthSummer(20.0f),
	_lengthAutumn(25.0f),
	_lengthWinter(25.0f),
	_treeReady(false),
	_currentView(0)
{
	for (int i = 0; i < 4; ++i)
	{
		_cameraKeyDown[i] = false;
	}
}

void Application::Create(MyWindow& window)
{
	_renderer.Create(&window);
	_debugFont.Create(_renderer, window);

	_dome.Create(_renderer);

	_tree.CreateAssets(_renderer);

	// Create and copy initial tree design
	_treeBuilder.CreateTree();
	_tree.CopyTreeBuffers(_renderer, _treeBuilder);

	// Queue a second tree to replace the current one next spring
	_workerThread.QueueTask(new TreeTask(*this, _treeBuilder));

	_house.Create(_renderer);
	_base.Create(_renderer);
	_terrain.Create(_renderer);
	_pond.Create(_renderer);
	_lightning.Create(_renderer);
	_particleSystem.Create(_renderer);

	// Configure particle emitter for chimney
	_smokeEmitter.SetPosition(Vector3(-2.69f, 2.1f, -4.4f));
	_smokeEmitter.SetWindDirection(Vector3(2.0f, 0.0f, 5.0f));
	_smokeEmitter.SetNumParticles(70);
	_smokeTexture.Create(_renderer, "assets/smoke_alpha.tga");
	_smokeEmitter.SetTexture(&_smokeTexture);
	_smokeEmitter.SetShape(1.4f);
	_smokeEmitter.SetSpread(1.2f);
	_smokeEmitter.SetParticleSize(0.8f);
	_smokeEmitter.SetSpeed(0.3f);
	_smokeEmitter.SetHeight(3.5f);
	_particleSystem.AddEmitter(&_smokeEmitter);

	// Configure particle emitter for burning bush effect
	_fireEmitter.SetPosition(Vector3(2.0f, 1.0f, 0.0f));
	_fireEmitter.SetWindDirection(Vector3(0.0f));
	_fireEmitter.SetNumParticles(60);
	_fireTexture.Create(_renderer, "assets/fire.tga");
	_fireEmitter.SetTexture(&_fireTexture);
	_fireEmitter.SetShape(1.2f);
	_fireEmitter.SetSpread(2.0f);
	_fireEmitter.SetParticleSize(1.0f);
	_fireEmitter.SetSpeed(0.3f);
	_fireEmitter.SetHeight(4.5f);
	_particleSystem.AddEmitter(&_fireEmitter);
	
	_snowParticles.Create(_renderer, 5000);
	
	_snowDrift.Create(_renderer);

	// Initialise perspective matrix
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 85, (float)window.Width() / window.Height(), 0.1f, 1000);
	_renderer.ProjectionMatrix(perspective);

	// Initialise viewpoints
	// View 0: Default view of entire snowglobe
	_viewTargets[0] = Vector3(0, 2, 0);
	_viewDirections[0] = Vector3(0, 4, 9) - _viewTargets[0];

	// View 1: Zoomed in view of tree
	_viewTargets[1] = Vector3(2, 2.5f, 0);
	_viewDirections[1] = Vector3(2, 2.5f, 3.5f) - _viewTargets[1];

	// View 2: First person view by the house
	_viewTargets[2] = Vector3(-1.6f, 0.9f, 0);
	_viewDirections[2] = Vector3(0.00f, 0, -0.01f);

	// View 4: Zoomed in view of house
	_viewTargets[3] = Vector3(-0.6f, 2.5f, -3.5f);
	_viewDirections[3] = Vector3(-0.6f, 2.5f, 1.5f) - _viewTargets[3];

	// Initialise default view matrix
	UpdateViewMatrix();

	// Configure lights for sunlight mode
	// Light 0: directional sunlight
	Light::Directional(_directionalLights[0], _sunDirection);
	_directionalLights[0].SetDiffuseColor(Vector3(1));
	_directionalLights[0].SetSpecularColor(Vector3(1));
	_directionalLights[0].SetSpecularPower(200);
	
	// Light 1-3: disabled
	Light::Off(_directionalLights[1]);
	Light::Off(_directionalLights[2]);
	Light::Off(_directionalLights[3]);

	// By default use the sunlight mode
	for (int i = 0; i < 4; ++i)
		_renderer.SetLight(i, _directionalLights[i]);

	// Configure lights for 4 spotlight mode
	Light::Spot(_spotLights[0], Vector3(-3.8f, 11.5f, 0), Vector3(0.7f, -1, 0), 11, 13.5f, 1);
	_spotLights[0].SetDiffuseColor(Vector3(1));
	_spotLights[0].SetSpecularColor(Vector3(1));
	_spotLights[0].SetSpecularPower(200);

	Light::Spot(_spotLights[1], Vector3(3.8f, 11.5f, 0), Vector3(-0.7f, -1, 0), 11, 13.5f, 1);
	_spotLights[1].SetDiffuseColor(Vector3(1.0f, 0.6f, 0.6f));
	_spotLights[1].SetSpecularColor(Vector3(1.0f, 0.6f, 0.6f));
	_spotLights[1].SetSpecularPower(200);

	Light::Spot(_spotLights[2], Vector3(0.0f, 11.5f, -3.8f), Vector3(0, -1, 0.7f), 11, 13.5f, 1);
	_spotLights[2].SetDiffuseColor((Vector3(0.6f, 1.0f, 0.6f)));
	_spotLights[2].SetSpecularColor((Vector3(0.6f, 1.0f, 0.6f)));
	_spotLights[2].SetSpecularPower(200);

	Light::Spot(_spotLights[3], Vector3(0.0f, 11.5f, 3.8f), Vector3(0, -1, -0.7f), 11, 13.5f, 1);
	_spotLights[3].SetDiffuseColor(Vector3(0.75f, 0.75f, 1.0f));
	_spotLights[3].SetSpecularColor(Vector3(0.75f, 0.75f, 1.0f));
	_spotLights[3].SetSpecularPower(200);
}

void Application::Draw()
{
	_renderer.Clear();

	_renderer.ViewMatrix(_view);

	// Draw opaque objects
	_tree.Draw(_renderer);
	_house.Draw(_renderer);
	_terrain.Draw(_renderer);
	_snowDrift.Draw(_renderer);
	_base.Draw(_renderer);

	// Draw terrain stencil
	_pond.DrawStencilMask(_renderer);

	// Invert scene for reflection
	_renderer.ClipPlane(Vector4(0, -1, 0, 0));
	_renderer.CullFace(C_FRONT);
	_renderer.EnableStencilTest(true);
	_renderer.StencilTest(STENCIL_EQUAL, 1);
	FlipLights();

	// Draw reflected opaque objects
	_terrain.DrawReflection(_renderer);
	_snowDrift.DrawReflection(_renderer);
	_tree.DrawReflection(_renderer);
	_house.DrawReflection(_renderer);

	// Draw reflected transparent objects
	_particleSystem.DrawReflected(_renderer);
	_snowParticles.DrawReflected(_renderer);
	_lightning.DrawReflection(_renderer);

	// Flip scene back the right way up
	FlipLights();
	_renderer.CullFace(C_BACK);
	_renderer.ClipPlane(Vector4(0, 0, 0, 0));
	_renderer.EnableStencilTest(false);
	
	// Draw transparent objects
	_dome.DrawBack(_renderer);
	_pond.Draw(_renderer);
	_snowParticles.Draw(_renderer);
	_particleSystem.Draw(_renderer);
	_lightning.Draw(_renderer);
	_dome.DrawFront(_renderer);

	PrintHud();
}

void Application::TreeTaskComplete()
{
	_tree.CopyTreeBuffers(_renderer, _treeBuilder);
	_treeReady = true;
}

void Application::FlipLights()
{
	Light* lights = _sunMode ? _directionalLights : _spotLights;
	for (int i = 0; i < 4; ++i)
	{
		lights[i].ReflectInYAxis();
		_renderer.SetLight(i, lights[i]);
	}
}

void Application::Update(float delta)
{
	// Synchronise any completed background tasks
	_workerThread.Syncrhonise();

	// Update camera orientation
	if (_cameraKeyDown[KEY_LEFT])
		_cameraYaw += delta;
	if (_cameraKeyDown[KEY_RIGHT])
		_cameraYaw -= delta;
	if (_cameraKeyDown[KEY_UP])
		_cameraPitch += delta;
	if (_cameraKeyDown[KEY_DOWN])
		_cameraPitch -= delta;

	UpdateViewMatrix();

	// Animate components
	float dt = delta * _animationSpeed;
	_elapsed += dt;

	_tree.Update(dt);
	_snowDrift.Update(dt);
	_snowParticles.Update(dt);
	_particleSystem.Update(dt);
	_house.Update(dt);
	_lightning.Update(dt);

	_fireEmitter.SetPosition(Vector3(2.0f, 1.0f * _tree.GetFireScale(), 0.0f));
	_fireEmitter.SetHeight(4.0f * _tree.GetFireScale());
	_fireEmitter.SetParticleSize(1.0f * _tree.GetFireScale());
	_fireEmitter.SetSpread(2.0f * _tree.GetFireScale());

	if (_sunMode)
	{
		// Update sun orientation
		Matrix4 sunRotation;
		Matrix4::RotationAxis(sunRotation, Vector3(0, 0, 1), dt * DAYCYCLE_SPEED);

		Vector4 sunTemp (_sunDirection, 0);
		sunRotation.Transform(sunTemp);
		_sunDirection = Vector3(sunTemp.x(), sunTemp.y(), sunTemp.z()).normalize();
		Light::Directional(_directionalLights[0], _sunDirection);

		// Calculate sun directional light color
		float daylight = Util::Max(0.0f, _sunDirection.dot(Vector3(0, -1, 0)));
		float sunrise = Util::Max(0.0f, _sunDirection.dot(Vector3(1, 0, 0)));
		float sunset = Util::Max(0.0f, _sunDirection.dot(Vector3(-1, 0, 0)));
		float night = Util::Max(0.0f, _sunDirection.dot(Vector3(0, 1, 0)));

		Vector3 color = COLOR_SUNRISE * sunrise + COLOR_DAYLIGHT * daylight + sunset * COLOR_SUNSET;

		_directionalLights[0].SetDiffuseColor(color);
		_directionalLights[0].SetSpecularColor(color);
		_renderer.SetLight(0, _directionalLights[0]);


		// Make sunrise and sunset transitions narrower for background colour transitions
		sunset = pow(sunset, 6);
		sunrise = pow(sunrise, 6);
		
		// Renormalise blend weights and calcualte final clear color
		float normFactor = daylight + sunrise + sunset + night;

		// Calculate background colour
		color = BG_COLOR_SUNRISE * sunrise / normFactor
						+ BG_COLOR_DAYLIGHT * daylight / normFactor
						+ BG_COLOR_SUNSET * sunset / normFactor
						+ BG_COLOR_NIGHTLIGHT * night / normFactor;

		_renderer.SetClearColor(Vector4(color, 1));
		_renderer.SetAmbient(color);
	}

	// Transition summer
	if (_season == SEASON_SPRING && _elapsed > _lengthSpring)
	{
		_elapsed -= _lengthSpring;
		_season = SEASON_SUMMER;
	}

	// Transition autumn
	if (_season == SEASON_SUMMER && _elapsed > _lengthSummer)
	{
		_elapsed -= _lengthSummer;
		_season = SEASON_AUTUMN;
		_tree.Die();
	}

	// Transition winter
	if (_season == SEASON_AUTUMN && _elapsed > _lengthAutumn)
	{
		_season = SEASON_WINTER;
		_elapsed -= _lengthAutumn;
		_snowDrift.Raise();
		_smokeEmitter.BeginEmit();
		_fireEmitter.BeginEmit();
		_lightning.Flash();
		_snowParticles.BeginEmit();
		_house.SnowBegin();
	}

	// Transition spring
	if (_season == SEASON_WINTER && _elapsed > _lengthWinter)
	{
		_season = SEASON_SPRING;
		_elapsed -= _lengthWinter;
		_snowDrift.Lower();
		_smokeEmitter.EndEmit();
		_fireEmitter.EndEmit();
		_snowParticles.EndEmit();

		if (_treeReady)
		{
			_tree.CopyTreeBuffers(_renderer, _treeBuilder);
			_treeReady = false;
			_workerThread.QueueTask(new TreeTask(*this, _treeBuilder));
		}

		_tree.Grow();
		_house.SnowEnd();
	}
}

void Application::Dispose()
{
	_workerThread.Shutdown();

	_base.Dispose();
	_dome.Dispose();
	_tree.Dispose();
	_house.Dispose();
	_terrain.Dispose();
	_pond.Dispose();
	_particleSystem.Dispose();
	_lightning.Dispose();
	_smokeTexture.Dispose();
	_fireTexture.Dispose();
	_snowParticles.Dispose();
	_snowDrift.Dispose();

	_debugFont.Dispose();
	_renderer.Dispose();
}

void Application::UpdateViewMatrix()
{
	// Orbit view around current view target
	Matrix4 pitch;
	Matrix4::RotationAxis(pitch, Vector3(1, 0, 0), _cameraPitch);
	Matrix4 yaw;
	Matrix4::RotationAxis(yaw, Vector3(0, 1, 0), _cameraYaw);

	Matrix4 rot = yaw * pitch;

	Matrix4 lookat;
	Matrix4::LookAt(lookat, 
					(rot * Vector4(_viewDirections[_currentView], 0)).xyz() + _viewTargets[_currentView], 
					_viewTargets[_currentView],
					(rot * Vector4(0, 1, 0, 0)).xyz());
		
	_view = lookat;
}

void Application::Resize(int width, int height)
{
	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 75, (float)width / height, 0.1f, 100);
	_renderer.ProjectionMatrix(perspective);
}

void Application::CameraKeyEvent(eCameraKey key, bool down)
{
	_cameraKeyDown[key] = down;
}

void Application::ToggleSunMode()
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

		// Reset ambient and background colour
		_renderer.SetAmbient(Vector3(0.4f));
		_renderer.SetClearColor(Vector4(0.329f, 0.407f, 0.482f, 1.0f));
	}
}

void Application::NextTreeMode()
{
	_tree.NextDrawMode();
}

void Application::IncrAnimationSpeed()
{
	_animationSpeed += ANIMATION_SPEED_INCREMENT;
	_animationSpeed = Util::Min(_animationSpeed, ANIMATION_SPEED_MAX);
}

void Application::DecrAnimationSpeed()
{
	_animationSpeed -= ANIMATION_SPEED_INCREMENT;
	_animationSpeed = Util::Max(_animationSpeed, ANIMATION_SPEED_MIN);
}

void Application::ResetAnimationSpeed()
{
	_animationSpeed = 1;
}

void Application::AddTreePattern(const std::string& seed, const LSystem& lsystem, unsigned int numLeaves, unsigned int iterations)
{
	_treeBuilder.AddPattern(seed, lsystem, numLeaves, iterations);
}

void Application::SetNumTreeLeaves(unsigned int numLeaves)
{
	_numTreeLeaves = numLeaves;
}

void Application::SetView(unsigned int view)
{
	_currentView = view;
	_cameraPitch = 0;
	_cameraYaw = 0;
}

void Application::PrintHud()
{
	_debugFont.Print("David Hart - OpenGL Graphics Demo 2012", 0, 13);
	
	std::stringstream ss;
	
	ss << "Playback Speed: ";
	if (_animationSpeed > 0) ss << _animationSpeed << "x";
	else ss << "PAUSED";

	_debugFont.Print(ss.str(), 0, 13*3);

	_debugFont.Print("Controls:", 0, 13*4);
	_debugFont.Print("  1,2,3,4       Change view", 0, 13*5);
	_debugFont.Print("  Cursor Keys   Rotate view", 0, 13*6);
	_debugFont.Print("  S             Toggle sun/spotlight", 0, 13*7);
	_debugFont.Print("  M             Cycle tree render modes", 0, 13*8);
	_debugFont.Print("  +,-,R         Increase, Decrease, Reset animation speed", 0, 13*9);
	_debugFont.Print("  F9            Toggle vullscreen", 0, 13*10);
}