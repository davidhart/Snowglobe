// David Hart - 2011
//
// class Pond
//   The pond class has the responsibilities of drawing the pond surface and of
//   creating the stencil mask for the reflection.
//   
//   DrawStencilMask should be called after drawing the scene and before drawing the
//   reflection. After calling DrawStencilMask the user should set up and enable stencil
//   testing with an equal to one test. 
//   
//   To preserve the correct draw order without, requiring
//   copies of the depth buffer, DrawStencilMask utilises a shader specicially designed to clear
//   the depth buffer in the areas that the stencil test passes. This allows for the reflection
//   to be drawn with the correct depth order, however the caller should implement Plane Clipping
//   to prevent any oddities where the reflected object may intersect the reflection plane.
//
//   After drawing the reflection Draw() should be called to restore the surface depth buffer values
//   and to draw the alpha blended surface.

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