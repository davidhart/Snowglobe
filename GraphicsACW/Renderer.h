#pragma once

#include "GXBase.h"
#include "Uncopyable.h"
#include "Matrix4.h"

class glex;
class VertexBinding;
class ShaderProgram;

enum Primitive
{
	PT_TRIANGLES = GL_TRIANGLES,
	PT_POINTS = GL_POINTS,
	PT_LINES = GL_LINES,
	PT_LINESTRIP = GL_LINE_STRIP,
	PT_LINELOOP = GL_LINE_LOOP,
};

class Renderer : public Uncopyable
{

public:

	Renderer();
	~Renderer();
	void Create(glex* glex);
	void Dispose();

	void Draw(VertexBinding& binding, Primitive primitive, unsigned int offset, unsigned int indices) const;

	void ProjectionMatrix(const Matrix4& projection);
	void ViewMatrix(const Matrix4& view);
	void UpdateViewProjectionUniforms(const ShaderProgram& shader) const;

	glex* GetEx() const;

private:

	glex* _glex;
	Matrix4 _projection;
	Matrix4 _view;
};
