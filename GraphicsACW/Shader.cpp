#include "Shader.h"
#include "Renderer.h"
#include <string>
#include <iostream>

Shader::Shader() :
	_glex (nullptr),
	_shHandle (0),
	_type (0)
{
}

Shader::~Shader()
{
	assert(_shHandle == 0);
}

void Shader::Create(Renderer& renderer, const char* shadersource)
{
	if (_shHandle != 0)
		Dispose();

	assert(_shHandle == 0);

	_glex = renderer.GetEx();

	_shHandle = _glex->glCreateShader(_type);

	_glex->glShaderSource(_shHandle, 1, &shadersource, nullptr);

	_glex->glCompileShader(_shHandle);


	GLint loglength;
	_glex->glGetShaderiv(_shHandle, GL_INFO_LOG_LENGTH, &loglength);
	
	if (loglength != 0)
	{
		std::string log;
		log.resize(loglength);

		_glex->glGetShaderInfoLog(_shHandle, loglength, &loglength, &log[0]);

		std::cout << log << std::endl;
	}
}


void Shader::Dispose()
{
	assert(_shHandle != 0);

	_glex->glDeleteShader(_shHandle);

	_shHandle = 0;
	_glex = nullptr;
}

VertexShader::VertexShader()
{
	_type = GL_VERTEX_SHADER;
}

FragmentShader::FragmentShader()
{
	_type = GL_FRAGMENT_SHADER;
}