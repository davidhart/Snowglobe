// David Hart - 2012
#pragma once

#include "Uncopyable.h"
#include "GXBase.h"

class Renderer;
class Texture;

enum AttachTarget
{
	FB_COLOR,
	FB_DEPTH,
};

struct FramebufferElement
{
	AttachTarget target;
	unsigned index;
	Texture* texture;
};

class Framebuffer : public Uncopyable
{
public:

	Framebuffer();
	~Framebuffer();

	void Create(const Renderer& renderer, const FramebufferElement* elements, unsigned count);
	void Dispose();

	void Bind();
	void Unbind();

private:

	glex* _glex;
	GLuint _fbHandle;

};