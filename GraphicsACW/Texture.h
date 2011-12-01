#pragma once

#include "Uncopyable.h"
#include "GXBase.h"

class Renderer;

enum TextureRepeatMode
{
	T_REPEAT = 0,
	T_CLAMP_EDGE = 1,
};

enum TextureFilterMode
{
	T_LINEAR = 0,
	T_NEAREST = 1,
};

class Texture : public Uncopyable
{

public:

	Texture();
	~Texture();

	void Create(const Renderer& renderer, const char* filename, TextureRepeatMode repeat = T_CLAMP_EDGE, TextureFilterMode filter = T_LINEAR);
	void Dispose();
	void Bind(unsigned int textureIndex = 0) const;

private:

	gxbase::Image _image;
	glex* _glex;
	GLuint _textureHandle;

};