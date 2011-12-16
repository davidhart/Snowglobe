// David Hart - 2011
//
// class ParticleEmitter
//   ParticleEmitter represents a self contained fountain of particles effect.
//  
//   To draw and update a ParticleEmitter it should be added to a ParticleSystem. ParticleSystem 
//   will not take care of the destruction of the the ParticleEmitter.
//
//   For a ParticleEmitter to draw the texture and number of particles must first be set and then
//   BeginEmit must be called. EndEmit should be called a brief period before calling RemoveParticle
//   to allow for it to transition out smoothly
//
//
// class ParticleSystem
//   ParticleSystem provides a mechanism for the efficient batched drawing of
//   multiple particle fountain effects. Individual particles are stateless so the properties of each
//   particle are calculated per frame by a specifically designed vertes shader which requires a quad
//   to be instanced.
//
//   Draw() should be called after drawing the scene and also after drawing reflected scene for the
//   additive blending to give the desired effect

#pragma once

#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "ObjFile.h"
#include "Renderer.h"
#include <vector>

class Texture;

class ParticleEmitter
{
	friend class ParticleSystem;

public:

	ParticleEmitter();

	void SetPosition(const Vector3& position);
	void GetPosition(Vector3& position) const;

	void SetWindDirection(const Vector3& windDirection);
	void GetWindDirection(Vector3& windDirection) const;

	void SetNumParticles(unsigned int numParticles);
	unsigned int GetNumParticles() const;

	void SetTexture(Texture* texture);
	Texture* GetTexture() const;

	void SetShape(float shape);
	float GetShape() const;

	void SetSpread(float spread);
	float GetSpread() const;

	void SetParticleSize(float size);
	float GetParticleSize() const;

	void SetSpeed(float speed);
	float GetSpeed() const;

	void SetHeight(float height);
	float GetHeight() const;

	void BeginEmit();
	void EndEmit();

private:

	void Update(float delta);

	Vector3 _position;
	Vector3 _windDirection;
	Texture* _texture;
	unsigned int _numParticles;
	float _shape;
	float _spread;
	float _particleSize;
	float _speed;
	float _height;

	bool _emitting;
	float _elapsed;
	float _endEmissionTime;
};

class ParticleSystem
{

public:

	ParticleSystem();
	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);
	void DrawReflected(const Renderer& renderer);
	void Update(float delta);

	void AddEmitter(ParticleEmitter* emitter);
	void RemoveEmitter(ParticleEmitter* emitter);

private:

	void Draw(const Renderer& renderer, const Matrix4& modelMatrix);
	
	std::vector<ParticleEmitter*> _particleEmitters;
	VertexBuffer _instancedQuadVertices;
	VertexBuffer _instancedQuadIndices;
	ObjFile _quadModel;
	VertexBinding _vertBinding;

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shader;
	Renderer::StandardUniformBlock _standardUniforms;

	// Per emitter uniforms
	Uniform _uniformTime;
	Uniform _uniformEndTime;
	Uniform _uniformNumParticles;
	Uniform _uniformParticleSystemShape;
	Uniform _uniformParticleSystemHeight;
	Uniform _uniformParticleSpread;
	Uniform _uniformWindDirection;
	Uniform _uniformParticleSize;
	Uniform _uniformParticleSpeed;
};

inline void ParticleEmitter::SetPosition(const Vector3& position)
{
	_position = position;
}

inline void ParticleEmitter::GetPosition(Vector3& position) const
{
	position = _position;
}

inline void ParticleEmitter::SetWindDirection(const Vector3& windDirection)
{
	_windDirection = windDirection;
}

inline void ParticleEmitter::GetWindDirection(Vector3& windDirection) const
{
	windDirection = _windDirection;
}

inline void ParticleEmitter::SetNumParticles(unsigned int numParticles)
{
	_numParticles = numParticles;
}

inline unsigned int ParticleEmitter::GetNumParticles() const
{
	return _numParticles;
}

inline void ParticleEmitter::SetTexture(Texture* texture)
{
	_texture = texture;
}

inline Texture* ParticleEmitter::GetTexture() const
{
	return _texture;
}

inline void ParticleEmitter::SetShape(float shape)
{
	_shape = shape;
}

inline float ParticleEmitter::GetShape() const
{
	return _shape;
}

inline void ParticleEmitter::SetSpread(float spread)
{
	_spread = spread;
}

inline float ParticleEmitter::GetSpread() const
{
	return _spread;
}

inline void ParticleEmitter::SetParticleSize(float size)
{
	_particleSize = size;
}

inline float ParticleEmitter::GetParticleSize() const
{
	return _particleSize;
}

inline void ParticleEmitter::SetSpeed(float speed)
{
	_speed = speed;
}

inline float ParticleEmitter::GetSpeed() const
{
	return _speed;
}

inline void ParticleEmitter::SetHeight(float height)
{
	_height = height;
}

inline float ParticleEmitter::GetHeight() const
{
	return _height;
}