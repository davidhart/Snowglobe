// David Hart - 2011
// 
// class Base
//   Base is a basic renderable object which utilises the textured_lit
//   shaders.

#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Renderer.h"
#include "Texture.h"

class Base
{

public:

	Base();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);

private:

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	Renderer::StandardUniformBlock _standardUniforms;
	VertexBuffer _baseBuffer;
	VertexBuffer _baseIndices;
	VertexBinding _vertBinding;
	ObjFile _baseModel;
	Texture _texture;
};