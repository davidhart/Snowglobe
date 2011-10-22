#pragma once

#include "GXBase.h"
#include "Uncopyable.h"

class glex;

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

	void Draw(Primitive primitive, unsigned int offset, unsigned int indices);

	glex* GetEx();

private:

	glex* _glex;

};
