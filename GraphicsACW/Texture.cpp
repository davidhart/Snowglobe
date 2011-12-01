#include "Texture.h"
#include "Renderer.h"
#include <cassert>

Texture::Texture() :
	_glex(NULL),
	_textureHandle(0)
{
}

Texture::~Texture()
{
	assert(_textureHandle == 0);
}

void Texture::Create(const Renderer& renderer, const char* filename, TextureMode mode)
{
	if (_textureHandle != 0)
		Dispose();

	_glex = renderer.GetEx();

	_image.Load(filename);

	assert(_image.IsValid());

	glGenTextures(1, &_textureHandle);

	glBindTexture(GL_TEXTURE_2D, _textureHandle);

	_image.glTexImage2D();

	glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

	if (mode == T_CLAMP_EDGE)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); // TODO: config file
}


void Texture::Dispose()
{
	assert(_textureHandle != 0);

	glDeleteTextures(1, &_textureHandle);
	_textureHandle = 0;
	_glex = NULL;
}

void Texture::Bind(unsigned int textureIndex) const
{
	_glex->glActiveTexture(GL_TEXTURE0 + textureIndex);

	glBindTexture(GL_TEXTURE_2D, _textureHandle);
}