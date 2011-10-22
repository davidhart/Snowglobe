#pragma once

#include "Uncopyable.h"
#include "GXBase.h"

class Renderer;

class Shader : public Uncopyable
{

	friend class ShaderProgram;

public:

	virtual ~Shader();

	void Create(Renderer& renderer, const char* shadersource);
	void Dispose();

protected:
	
	Shader();
	GLenum _type;

private:

	glex* _glex;
	GLuint _shHandle;

};

class VertexShader : public Shader
{

public:

	VertexShader();

};

class FragmentShader : public Shader
{

public:

	FragmentShader();

};
