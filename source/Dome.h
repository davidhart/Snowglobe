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
	
public:

	Dome();
	void Create(const Renderer& renderer);
	void Dispose();

	void DrawBack(const Renderer& renderer);
	void DrawFront(const Renderer& renderer);

private:

	void Draw(const Renderer& renderer, float normalScale, const Vector4& rimColor);

	ObjFile _domeModel;

	VertexBuffer _vertexBuffer;
	VertexBuffer _indexBuffer;
	VertexBinding _vertBinding;

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shader;

	Uniform _uniformNormalScale;
	Uniform _uniformRimColor;
	Renderer::StandardUniformBlock _standardUniforms;

};