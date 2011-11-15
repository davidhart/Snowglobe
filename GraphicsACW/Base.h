#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"

class Renderer;

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
	VertexBuffer _baseBuffer;
	VertexBuffer _baseIndices;
	VertexBinding _vertBinding;
	ObjFile _baseModel;

};