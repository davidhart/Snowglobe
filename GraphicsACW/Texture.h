// David Hart - 2011
// 
// class Texture
//   Texture is a wrapper around the OpenGL Texture object, with the added functionality of
//   loading .tga, .bmp and .jpg files File loading is handled by gxbase::Image. 
//   
//   The active texture unit may
//   be specified by providing a texture index as an argument ot the bind method.
//

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

enum TextureFormat
{
	T_DEPTH,
	T_RGB,
	T_RGBA,
};

class Texture : public Uncopyable
{
	friend class Framebuffer;
public:

	Texture();
	~Texture();

	void Create(const Renderer& renderer, const char* filename, 
		TextureRepeatMode repeat = T_CLAMP_EDGE, TextureFilterMode filter = T_LINEAR);

	void Create(const Renderer& renderer, unsigned width, unsigned height, TextureFormat format, 
		TextureRepeatMode repeat = T_CLAMP_EDGE, TextureFilterMode filter = T_LINEAR);
	
	void Dispose();
	void Bind(unsigned int textureIndex = 0, bool generateMipmap = false) const;

	void CompareRToTexture(bool enabled);

private:

	void GenTexture(const Renderer& renderer, TextureRepeatMode repeat, TextureFilterMode filter);

	gxbase::Image _image;
	glex* _glex;
	GLuint _textureHandle;

};