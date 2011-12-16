// David Hart - 2011

#include "Application.h"
#include "MyWindow.h"

const float Application::ANIMATION_SPEED_INCREMENT = 0.5f;
const float Application::ANIMATION_SPEED_MIN = 0.0f;
const float Application::ANIMATION_SPEED_MAX = 15.0f;

const Vector3 Application::COLOR_SUNSET = Vector3(1.0f, 0.9f, 0.6f);
const Vector3 Application::COLOR_SUNRISE = Vector3(0.75f, 0.9f, 1.0f);
const Vector3 Application::COLOR_DAYLIGHT = Vector3(1.0f, 1.0f, 1.0f);

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
	_lengthWinter(25.0f)
{
	for (int i = 0; i < 4; ++i)
	{
		_cameraKeyDown[i] = false;
	}
}

void Application::Create(MyWindow& window)
{
	_renderer.Create(&window);

	_dome.Create(_renderer);
	_tree.Create(_renderer, _treePattern, _numTreeLeaves);
	_house.Create(_renderer);
	_base.Create(_renderer);
	_terrain.Create(_renderer);
	_pond.Create(_renderer);
	_lightning.Create(_renderer);
	_particleSystem.Create(_renderer);

	_smokeEmitter.SetPosition(Vector3(-2.69f, 2.1f, -4.4f));
	_smokeEmitter.SetWindDirection(Vector3(2.0f, 0.0f, 5.0f));
	_smokeEmitter.SetNumParticles(200);
	_smokeTexture.Create(_renderer, "assets/smoke_alpha.jpg");
	_smokeEmitter.SetTexture(&_smokeTexture);
	_smokeEmitter.SetShape(1.4f);
	_smokeEmitter.SetSpread(1.2f);
	_smokeEmitter.SetParticleSize(0.8f);
	_smokeEmitter.SetSpeed(0.3f);
	_smokeEmitter.SetHeight(3.5f);
	_particleSystem.AddEmitter(&_smokeEmitter);

	_fireEmitter.SetPosition(Vector3(2.0f, 1.0f, 0.0f));
	_fireEmitter.SetWindDirection(Vector3(0.0f));
	_fireEmitter.SetNumParticles(320);
	_fireTexture.Create(_renderer, "assets/fire.jpg");
	_fireEmitter.SetTexture(&_fireTexture);
	_fireEmitter.SetShape(1.2f);
	_fireEmitter.SetSpread(2.0f);
	_fireEmitter.SetParticleSize(1.0f);
	_fireEmitter.SetSpeed(0.3f);
	_fireEmitter.SetHeight(4.5f);
	_particleSystem.AddEmitter(&_fireEmitter);
	
	_snowParticles.Create(_renderer, 5000);
	_snowDrift.Create(_renderer);

	Matrix4 perspective;
	Matrix4::PerspectiveFov(perspective, 85, (float)window.Width() / window.Height(), 0.1f, 1000);
	_renderer.ProjectionMatrix(perspective);

	_renderer.SetAmbient(Vector3(0.2f));

	UpdateViewMatrix();
	// Sun lights
	Light::Directional(_directionalLights[0], _sunDirection);
	_directionalLights[0].SetDiffuseColor(Vector3(1));
	_directionalLights[0].SetSpecularColor(Vector3(1));
	_directionalLights[0].SetSpecularPower(200);
	
	Light::Off(_directionalLights[1]);
	Light::Off(_directionalLights[2]);
	Light::Off(_directionalLights[3]);

	// Spotlights
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

	for (int i = 0; i < 4; ++i)
		_renderer.SetLight(i, _directionalLights[i]);
}

void Application::Draw()
{
	_renderer.Clear();

	_renderer.ViewMatrix(_view);

	_tree.Draw(_renderer);
	_house.Draw(_renderer);
	_terrain.Draw(_renderer);
	_snowDrift.Draw(_renderer);
	_base.Draw(_renderer);
	_dome.DrawBack(_renderer);

	_pond.DrawStencilMask(_renderer);

	_renderer.ClipPlane(Vector4(0, -1, 0, 0));
	_renderer.CullFace(C_FRONT);
	_renderer.EnableStencilTest(true);
	_renderer.StencilTest(STENCIL_EQUAL, 1);

	FlipLights();

	_terrain.DrawReflection(_renderer);
	_snowDrift.DrawReflection(_renderer);
	_tree.DrawReflection(_renderer);
	_house.DrawReflection(_renderer);
	_particleSystem.DrawReflected(_renderer);
	_snowParticles.DrawReflected(_renderer);
	_lightning.DrawReflection(_renderer);

	FlipLights();

	_renderer.CullFace(C_BACK);
	_renderer.ClipPlane(Vector4(0, 0, 0, 0));
	_renderer.EnableStencilTest(false);
	
	_pond.Draw(_renderer);

	_snowParticles.Draw(_renderer);
	_particleSystem.Draw(_renderer);
	_lightning.Draw(_renderer);
	_dome.DrawFront(_renderer);
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
	if (_cameraKeyDown[KEY_LEFT])
		_cameraYaw -= delta;
	if (_cameraKeyDown[KEY_RIGHT])
		_cameraYaw += delta;
	if (_cameraKeyDown[KEY_UP])
		_cameraPitch -= delta;
	if (_cameraKeyDown[KEY_DOWN])
		_cameraPitch += delta;

	UpdateViewMatrix();

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

	Matrix4 sunRotation;
	Matrix4::RotationAxis(sunRotation, Vector3(0, 0, 1), dt);

	Vector4 sunTemp (_sunDirection, 0);
	sunRotation.Transform(sunTemp);
	_sunDirection = Vector3(sunTemp.x(), sunTemp.y(), sunTemp.z());

	if (_sunMode)
	{
		Light::Directional(_directionalLights[0], _sunDirection);
		float daylight = Util::Max(0.0f, _sunDirection.dot(Vector3(0, -1, 0)));
		float sunrise = Util::Max(0.0f, _sunDirection.dot(Vector3(1, 0, 0)));
		float sunset = Util::Max(0.0f, _sunDirection.dot(Vector3(-1, 0, 0)));

		Vector3 color = COLOR_SUNRISE * sunrise + COLOR_DAYLIGHT * daylight + sunset * COLOR_SUNSET;

		_directionalLights[0].SetDiffuseColor(color);
		_directionalLights[0].SetSpecularColor(color);
		_renderer.SetLight(0, _directionalLights[0]);
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
		_tree.Grow();
		_house.SnowEnd();
	}
}

void Application::Dispose()
{
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

	_renderer.Dispose();
}

void Application::UpdateViewMatrix()
{
	Matrix4 lookat;
	Matrix4::LookAt(lookat, Vector3(0, 2, 9), Vector3(0, 0, 0), Vector3(0, 1, 0));
	Matrix4 pitch;
	Matrix4::RotationAxis(pitch, Vector3(1, 0, 0), _cameraPitch);
	Matrix4 yaw;
	Matrix4::RotationAxis(yaw, Vector3(0, 1, 0), _cameraYaw);
	Matrix4 translation;
	Matrix4::Translation(translation, Vector3(0, -2.0f, 0));

	_view = lookat * pitch * yaw * translation;
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
	}
}

void Application::NextTreeMode()
{
	_tree.NextDrawMode();
}

void Application::IncrAnimationSpeed()
{
	std::cout << "Speed: " << _animationSpeed << "x" << std::endl;
	_animationSpeed += ANIMATION_SPEED_INCREMENT;
	_animationSpeed = Util::Min(_animationSpeed, ANIMATION_SPEED_MAX);
}

void Application::DecrAnimationSpeed()
{
	_animationSpeed -= ANIMATION_SPEED_INCREMENT;
	_animationSpeed = Util::Max(_animationSpeed, ANIMATION_SPEED_MIN);
	std::cout << "Speed: " << _animationSpeed << "x" << std::endl;
}

void Application::ResetAnimationSpeed()
{
	_animationSpeed = 1;
	std::cout << "Speed: " << _animationSpeed << std::endl;
}

void Application::SetTreePattern(const std::string& pattern)
{
	_treePattern = pattern;
}

void Application::SetNumTreeLeaves(unsigned int numLeaves)
{
	_numTreeLeaves = numLeaves;
}