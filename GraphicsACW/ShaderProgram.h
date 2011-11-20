#pragma once

#include "Uncopyable.h"
#include "GXBase.h"
#include "Shader.h"

class Matrix4;
class Vector4;
class Vector3;

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
	void SetUniform(const char* uniform, const Vector4& value) const;
	void SetUniform(const char* uniform, const Vector3& value) const;
	void SetUniform(const char* uniform, float value) const;
	void SetUniform(const char* uniform, int value) const;

private:

	glex* _glex;
	GLuint _spHandle;

};