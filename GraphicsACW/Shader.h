#pragma once

#include "Uncopyable.h"
#include "GXBase.h"

class Renderer;

class Shader : public Uncopyable
{

	friend class ShaderProgram;

public:

	virtual ~Shader();

	void Create(const Renderer& renderer, const char* shadersource);
	void Dispose();

protected:
	
	void SetType(GLenum type);
	Shader();

private:
	
	GLenum _type;
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