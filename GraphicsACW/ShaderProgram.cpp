#include "ShaderProgram.h"
#include "Renderer.h"
#include "Matrix4.h"
#include <string>
#include <iostream>

ShaderProgram::ShaderProgram() : 
	_glex (NULL),
	_spHandle (0)
{
}

ShaderProgram::~ShaderProgram()
{
	assert(_spHandle == 0);
}

void ShaderProgram::Create(const Renderer& renderer, const VertexShader& vertexShader, const FragmentShader& fragmentShader)
{
	if (_spHandle != 0)
		Dispose();

	assert(_spHandle == 0);

	_glex = renderer.GetEx();

	_spHandle = _glex->glCreateProgram();

	_glex->glAttachShader(_spHandle, vertexShader._shHandle);
	_glex->glAttachShader(_spHandle, fragmentShader._shHandle);

	_glex->glLinkProgram(_spHandle);

	GLint loglength;

	_glex->glGetProgramiv(_spHandle, GL_INFO_LOG_LENGTH, &loglength);

	if (loglength != 0)
	{
		std::string log;
		log.resize(loglength);

		_glex->glGetProgramInfoLog(_spHandle, loglength, &loglength, &log[0]);

		std::cout << log << std::endl;
	}
}

void ShaderProgram::Dispose()
{
	assert(_spHandle != 0);

	_glex->glDeleteProgram(_spHandle);
	_spHandle = 0;
	_glex = NULL;
}

void ShaderProgram::Use()
{
	_glex->glUseProgram(_spHandle);
}

int ShaderProgram::GetAttributeIndex(const char* attribute)
{
	return _glex->glGetAttribLocation(_spHandle, attribute);
}

void ShaderProgram::SetUniform(const char* uniform, const Matrix4& value) const
{
	int location = _glex->glGetUniformLocation(_spHandle, uniform);
	if (location >= 0)
	{
		_glex->glUniformMatrix4fv(location, 1, false, (const float*)&value);
	}
}

void ShaderProgram::SetUniform(const char* uniform, int value) const
{
	int location = _glex->glGetUniformLocation(_spHandle, uniform);
	if (location >= 0)
	{
		_glex->glUniform1i(location, value);
	}
}