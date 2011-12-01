#pragma once

#include "Uncopyable.h"
#include "GXBase.h"

class Renderer;

enum TextureMode
{
	T_REPEAT,
	T_CLAMP_EDGE,
};

class Texture : public Uncopyable
{

public:

	Texture();
	~Texture();

	void Create(const Renderer& renderer, const char* filename, TextureMode mode = T_CLAMP_EDGE);
	void Dispose();
	void Bind(unsigned int textureIndex = 0) const;

private:

	gxbase::Image _image;
	glex* _glex;
	GLuint _textureHandle;

};