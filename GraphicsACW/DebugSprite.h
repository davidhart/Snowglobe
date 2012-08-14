// David Hart - 2012

#pragma once

#include "VertexBuffer.h"
#include "Shader.h"
#include "VertexBinding.h"
#include "Renderer.h"

class Texture;

class DebugSprite
{

public:

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer, float x, float y, float w, float h, Texture& texture);
	void WindowSize(unsigned width, unsigned height);

private:

	VertexBuffer _vertBuffer;
	VertexBuffer _indexBuffer;
	VertexBinding _vertBinding;

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shader;

	Uniform _projectionUniform;
	Matrix4 _projectionMatrix;

};