#pragma once

#include "Uncopyable.h"
#include "GXBase.h"

class Renderer;

class Texture : public Uncopyable
{

public:

	Texture();
	~Texture();

	void Create(const Renderer& renderer, const char* filename);
	void Dispose();
	void Bind(unsigned int textureIndex = 0) const;

private:

	glex* _glex;
	GLuint _textureHandle;
	gxbase::Image _image;

};