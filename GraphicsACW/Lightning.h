#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Texture.h"
#include "Renderer.h"

class Lightning
{

public:

	Lightning();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

	void Flash();
	void Update(float delta);

private:

	void Draw(const Renderer& renderer, const Matrix4& modelMatrix);
	bool ShouldDraw();


	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	Renderer::StandardUniformBlock _standardUniforms;
	VertexBuffer _vertexBuffer;
	VertexBuffer _indexBuffer;
	VertexBinding _vertBinding;
	ObjFile _model;
	Texture _texture;
	float _elapsed;
	float _flashDuration;
};