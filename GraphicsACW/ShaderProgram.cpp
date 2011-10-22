#include "ShaderProgram.h"
#include "Renderer.h"
#include <string>
#include <iostream>

ShaderProgram::ShaderProgram() : 
	_glex (nullptr),
	_spHandle (0)
{
}

ShaderProgram::~ShaderProgram()
{
	assert(_spHandle == 0);
}

void ShaderProgram::Create(Renderer& renderer, VertexShader& vertexShader, FragmentShader& fragmentShader)
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
	_glex = nullptr;
}

void ShaderProgram::Use()
{
	_glex->glUseProgram(_spHandle);
}