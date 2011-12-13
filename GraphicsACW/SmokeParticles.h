#pragma once

#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Shader.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "ObjFile.h"
#include "Renderer.h"

class SmokeParticles
{

public:

	SmokeParticles();
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