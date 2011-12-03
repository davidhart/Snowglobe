#pragma once

#include "Uncopyable.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ObjFile.h"
#include "Matrix4.h"
#include "Renderer.h"

class Dome : public Uncopyable
{
	
private:

	ObjFile _domeModel;

	VertexBuffer _vertexBuffer;
	VertexBuffer _indexBuffer;

	VertexBinding _frontBinding;
	VertexBinding _backBinding;

	ShaderProgram _frontShader;
	ShaderProgram _backShader;

	Renderer::StandardUniformBlock _standardUniformBack;
	Renderer::StandardUniformBlock _standardUniformFront;

	VertexShader _vertShader;
	FragmentShader _backFragShader;
	FragmentShader _frontFragShader;

public:

	Dome();
	void Create(const Renderer& renderer);
	void Dispose();

	void DrawBack(const Renderer& renderer);
	void DrawFront(const Renderer& renderer);

};