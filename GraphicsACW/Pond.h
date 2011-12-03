#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Texture.h"
#include "Renderer.h"

class Pond
{

public:

	Pond();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);

private:

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	Renderer::StandardUniformBlock _standardUniforms;
	Texture _texture;
	VertexBuffer _pondBuffer;
	VertexBuffer _pondIndices;
	VertexBinding _vertBinding;
	ObjFile _pondModel;

};