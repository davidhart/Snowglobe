#include "Framebuffer.h"
#include "Renderer.h"
#include "Texture.h"

Framebuffer::Framebuffer() :
	_glex(NULL),
	_fbHandle(0)
{
}

Framebuffer::~Framebuffer()
{
	assert(_fbHandle == 0);
}

void Framebuffer::Create(const Renderer& renderer, const FramebufferElement* elements, unsigned count)
{
	assert(count != 0);

	if (_fbHandle != 0)
		Dispose();

	assert(_fbHandle == 0);

	_glex = renderer.GetEx();

	_glex->glGenFramebuffers(1, &_fbHandle);

	_glex->glBindFramebuffer(GL_FRAMEBUFFER, _fbHandle);

	bool hasColor0 = false;

	for (unsigned i = 0; i < count; ++i)
	{
		const FramebufferElement* element = elements + i;

		assert(element->target == FB_COLOR || (element->target == FB_DEPTH && element->index == 0));

		GLuint target;

		if (element->target == FB_COLOR)
		{
			target = GL_COLOR_ATTACHMENT0 + element->index;

			if (element->index == 0)
			{
				hasColor0 = true;
			}
		}
		else if (element->target == FB_DEPTH)
		{
			target = GL_DEPTH_ATTACHMENT;
		}

		_glex->glFramebufferTexture2D(GL_FRAMEBUFFER, target, GL_TEXTURE_2D, element->texture->_textureHandle, 0);
	}

	if (!hasColor0)
	{
		glReadBuffer(GL_NONE);
		glDrawBuffer(GL_NONE);
	}

	_glex->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Dispose()
{
	assert(_fbHandle != 0);

	_glex->glDeleteFramebuffers(1, &_fbHandle);

	_fbHandle = 0;
	_glex = NULL;
}

void Framebuffer::Bind()
{
	_glex->glBindFramebuffer(GL_FRAMEBUFFER, _fbHandle);
}

void Framebuffer::Unbind()
{
	_glex->glBindFramebuffer(GL_FRAMEBUFFER, 0);
}