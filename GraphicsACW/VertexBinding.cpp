#include "VertexBinding.h"
#include "VertexBuffer.h"
#include "Renderer.h"

#include <exception>
#include <cassert>
#include <vector>

VertexBinding::VertexBinding() :
	_glex(nullptr),
	_vaoHandle(0)
{
}

VertexBinding::~VertexBinding()
{
	assert(_vaoHandle == 0);
}

void VertexBinding::Create(Renderer& renderer, const ArrayElement* elements, unsigned int numelements)
{
	if (_vaoHandle != 0)
		Dispose();

	_glex = renderer.GetEx();

	_glex->glGenVertexArrays(1, &_vaoHandle);

	Bind();

	for (unsigned int i = 0; i < numelements; ++i)
	{
		const ArrayElement& element = elements[i];
		// TODO: element array

		_glex->glBindBuffer(GL_ARRAY_BUFFER, element.buffer._vbHandle);

		GLenum type = GL_FLOAT;

		if (AE_FLOAT == element.type)
			type = GL_FLOAT;
		else if (AE_INT == element.type)
			type = GL_INT;

		if (AE_VERTEX == element.attribute)
		{
			glEnableClientState(GL_VERTEX_ARRAY);

			glVertexPointer(element.numcomponents, type, element.stride, (void*)element.offset); 
		}
		else if (AE_NORMAL == element.attribute)
		{
			glEnableClientState(GL_NORMAL_ARRAY);

			assert(3 == element.numcomponents);

			glNormalPointer(type, element.stride, (void*)element.offset);
		}
		else if (AE_TEXCOORD0 == element.attribute)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);

			glTexCoordPointer(element.numcomponents, type, element.stride, (void*)element.offset);
		}
		else if (AE_COLOR == element.attribute)
		{
			glEnableClientState(GL_COLOR_ARRAY);

			glColorPointer(element.numcomponents, type, element.stride, (void*)element.offset);
		}
	}

	Unbind();
}

void VertexBinding::Dispose()
{
	assert(_vaoHandle != 0);

	_glex->glDeleteVertexArrays(1, &_vaoHandle);
	_vaoHandle = 0;
	_glex = nullptr;
}

void VertexBinding::Bind()
{
	assert(_vaoHandle != 0);

	_glex->glBindVertexArray(_vaoHandle);
}

void VertexBinding::Unbind()
{
	_glex->glBindVertexArray(0);
}