#pragma once

#include "Uncopyable.h"
#include "GXBase.h"
#include "Shader.h"
class Matrix4;

class ShaderProgram : public Uncopyable
{

public:

	ShaderProgram();
	~ShaderProgram();

	void Create(const Renderer& renderer, const VertexShader& vertexShader, const FragmentShader& fragmentShader);
	void Dispose();

	int GetAttributeIndex(const char* attribute);
	void Use();
	void SetUniform(const char* uniform, const Matrix4& value) const;

private:

	glex* _glex;
	GLuint _spHandle;

};