#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Texture.h"

class Renderer;

class House
{

public:

	House();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer);
	void DrawReflection(const Renderer& renderer);

private:

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	VertexBuffer _houseBuffer;
	VertexBuffer _houseIndices;
	VertexBinding _vertBinding;
	ObjFile _houseModel;
	Texture _houseTexture;

};