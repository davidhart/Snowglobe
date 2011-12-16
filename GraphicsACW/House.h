// David Hart - 2011
// 
// class House
//   House requires a two textures and a single pass of the multitextured_lit.fsh shader.
//   SnowBegin() should be called at the start of winter to allow the snow texture to transition in
//   SnowEnd() should be called at the end of winter for the snow to transition out

#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Texture.h"
#include "Renderer.h"

class House
{

public:

	House();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

	void Update(float dt);
	void SnowBegin();
	void SnowEnd();

private:

	void Draw(const Renderer& renderer, const Matrix4& modelMatrix);
	float GetTextureBlend() const;

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	Renderer::StandardUniformBlock _standardUniforms;
	Uniform _uniformTextureBlend;
	VertexBuffer _houseBuffer;
	VertexBuffer _houseIndices;
	VertexBinding _vertBinding;
	ObjFile _houseModel;
	Texture _houseTexture;
	Texture _houseSnowTexture;
	float _elapsed;

	enum eSnowState
	{
		SNOW_MELTING,
		SNOW_VISIBLE,
		SNOW_ACCUMULATE,
		SNOW_MELTED,
	};
	eSnowState _state;

	static const float SNOW_TRANSITION_IN_TIME;
	static const float SNOW_TRANSITION_OUT_TIME;
};