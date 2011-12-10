#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Texture.h"
#include "Renderer.h"

class SnowDrift
{

public:

	SnowDrift();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

	void Update(float dt);
	void Raise();
	void Lower();

private:

	void Draw(const Renderer& renderer, const Matrix4& model);

	enum eDriftState
	{
		DRIFT_LOWERED,
		DRIFT_RAISING,
		DRIFT_RAISED,
		DRIFT_LOWERING,
	};

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	VertexBuffer _vertexBuffer;
	VertexBuffer _indexBuffer;
	VertexBinding _vertBinding;
	Renderer::StandardUniformBlock _standardUniforms;
	ObjFile _terrainModel;
	Texture _texture;
	eDriftState _state;
	float _elapsed;

	static const float DRIFT_RAISE_TIME;
	static const float DRIFT_LOWER_TIME;
	static const float DRIFT_LOWER_OFFSET;
};