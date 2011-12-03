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
	Texture _texture;

	float _elapsed;
	unsigned int _numParticles;

};