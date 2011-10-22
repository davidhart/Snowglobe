#pragma once

#include "Uncopyable.h"
#include "GXBase.h"
#include "Shader.h"

class ShaderProgram : public Uncopyable
{
	
public:

	ShaderProgram();
	~ShaderProgram();

	void Create(Renderer& renderer, VertexShader& vertexShader, FragmentShader& fragmentShader);
	void Dispose();

	void Use();

private:

	glex* _glex;
	GLuint _spHandle;

};