// David Hart - 2011
// 
// class Terrain
//   Terrain is a basic renderable object which utilises the textured_lit
//   shaders.

#pragma once

#include "ShaderProgram.h"
#include "ObjFile.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Texture.h"
#include "Renderer.h"

class Terrain
{

public:

	Terrain();

	void Create(const Renderer& renderer);
	void Dispose();

	void Draw(const Renderer& renderer, Texture& shadowMap);
	void DrawReflection(const Renderer& renderer);

private:

	VertexShader _vertShader;
	FragmentShader _fragShader;
	ShaderProgram _shaderProgram;
	VertexBuffer _terrainBuffer;
	VertexBuffer _terrainIndices;
	VertexBinding _vertBinding;
	Renderer::StandardUniformBlock _standardUniforms;
	ObjFile _terrainModel;
	Texture _texture;
};