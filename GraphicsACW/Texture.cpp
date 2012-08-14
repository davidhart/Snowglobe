// David Hart - 2011

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

void Texture::Create(const Renderer& renderer, const char* filename, TextureRepeatMode repeat, TextureFilterMode filter)
{
	GenTexture(renderer, repeat, filter);

	_image.Load(filename);

	assert(_image.IsValid());

	_image.glTexImage2D();

	_glex->glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Create(const Renderer& renderer, unsigned width, unsigned height, TextureFormat format, 
		TextureRepeatMode repeat, TextureFilterMode filter)
{
	GenTexture(renderer, repeat, filter);

	GLuint glinFmt, glFmt, glType;

	if (format == T_DEPTH)
	{
		glinFmt = GL_DEPTH_COMPONENT32;
		glFmt = GL_DEPTH_COMPONENT;
		glType = GL_FLOAT;
	}
	else if(format == T_RGB)
	{
		glinFmt = GL_RGB;
		glFmt = GL_RGB;
		glType = GL_UNSIGNED_BYTE;
	}
	else if (format == T_RGBA)
	{
		glinFmt = GL_RGBA;
		glFmt = GL_RGBA;
		glType = GL_UNSIGNED_BYTE;
	}
	else
	{
		assert(false);
	}

	glTexImage2D(GL_TEXTURE_2D, 0, glinFmt, width, height, 0, glFmt, glType, NULL);
}

void Texture::GenTexture(const Renderer& renderer, TextureRepeatMode repeat, TextureFilterMode filter)
{
	if (_textureHandle != 0)
		Dispose();

	_glex = renderer.GetEx();

	glGenTextures(1, &_textureHandle);

	glBindTexture(GL_TEXTURE_2D, _textureHandle);

	if (repeat == T_CLAMP_EDGE)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	if (filter == T_LINEAR)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f); // TODO: config file
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}
}

void Texture::Dispose()
{
	assert(_textureHandle != 0);

	glDeleteTextures(1, &_textureHandle);
	_textureHandle = 0;
	_glex = NULL;
}

void Texture::Bind(unsigned int textureIndex, bool generateMipmap) const
{
	_glex->glActiveTexture(GL_TEXTURE0 + textureIndex);

	glBindTexture(GL_TEXTURE_2D, _textureHandle);
	
	if (generateMipmap)
	{
		_glex->glGenerateMipmap(GL_TEXTURE_2D);
	}
}

void Texture::CompareRToTexture(bool enabled)
{
	Bind(0);

	GLenum mode;

	if (enabled) mode = GL_COMPARE_REF_TO_TEXTURE;
	else mode = GL_NONE;

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, mode);
}