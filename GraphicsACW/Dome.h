// David Hart - 2011
//
// class Dome 
//	 Handles loading and drawing of glass dome object. The dome is drawn with two passes
//   of the dome.fsh shader.  The dome model requires vertex normals but does not require texture 
//   coordinates.
//   
//   DrawBack() must be called after drawing the base and should be called after drawing the scene 
//   but before drawing the reflection to minimise overdraw. 
//	 DrawFront() must be called last to produce the desired additive blending effect.

#pragma once

#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "ObjFile.h"
#include "Matrix4.h"
#include "Renderer.h"

class Dome
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