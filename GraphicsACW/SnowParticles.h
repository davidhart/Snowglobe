// David Hart - 2011
// 
// class SnowParticles
//   The responsibility of SnowParticles is to the drawing and state of the snow particle
//   effect. The snow particle effect is not implemented in particle system because the 
//   particle system shader is not currently flexible enough to implement the correct 
//   initial particle position calculation which gives the effect the snow is falling from the
//   upper hemisphere of the globe.
//   
//   SnowParticles employs the same rendering techniques as ParticleSystem but utilises a
//   specifically designed vertex shader to calculate the stateless position of the instanced
//   billboarded quad particles.

#pragma once

#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "ObjFile.h"
#include "Renderer.h"

class SnowParticles
{

public:

	SnowParticles();
	void Create(const Renderer& renderer, unsigned int numParticles);
	void Dispose();
	void Draw(const Renderer& renderer);
	void DrawReflected(const Renderer& renderer);
	void Update(float delta);
	void BeginEmit();
	void EndEmit();

private:

	VertexBuffer _instancedQuadBuffer;
	VertexBuffer _instancedQuadIndices;
	ObjFile _instancedQuadModel;
	VertexBinding _vertBinding;
	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shader;
	Renderer::StandardUniformBlock _standardUniforms;
	Uniform _uniformTime;
	Uniform _uniformEndTime;
	Texture _texture;

	bool _emitting;
	float _elapsed;
	float _endEmissionTime;
	unsigned int _numParticles;

};