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
	void DrawStencilMask(const Renderer& renderer);

private:

	VertexShader _vertShader;
	FragmentShader _fragShader;
	FragmentShader _fragDepthClear;
	ShaderProgram _shaderProgram;
	ShaderProgram _shaderDepthClear;
	Renderer::StandardUniformBlock _standardUniforms;
	Renderer::StandardUniformBlock _depthClearUniforms;
	Texture _texture;
	VertexBuffer _pondBuffer;
	VertexBuffer _pondIndices;
	VertexBinding _vertBinding;
	VertexBinding _vertDepthClearBinding;
	ObjFile _pondModel;

};