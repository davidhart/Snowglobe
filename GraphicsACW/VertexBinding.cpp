#include "VertexBinding.h"
#include "VertexBuffer.h"
#include <exception>
#include <cassert>
#include <vector>

VertexBinding::VertexBinding(glex& glex) :
	_glex(glex),
	_vaoHandle(0)
{
}

VertexBinding::~VertexBinding()
{
	try
	{
		if (_vaoHandle != 0)
		{
			Dispose();
		}
	}
	catch (std::exception&)
	{

	}
}

void VertexBinding::Create(ArrayElement* elements, unsigned int numelements)
{
	if (_vaoHandle != 0)
		Dispose();

	_glex.glGenVertexArrays(1, &_vaoHandle);

	Bind();

	std::vector<bool> bound(numelements, false);

	const VertexBuffer* active = NULL;

	for (unsigned int i = 0; i < numelements; ++i)
	{
		if (bound[i])
			continue;

		active = &elements[i].buffer;

		// TODO: element array

		_glex.glBindBuffer(GL_ARRAY_BUFFER, active->_vbHandle);

		for (unsigned int j = i; j < numelements; ++j)
		{
			if (bound[j] || active != &elements[j].buffer)
				continue;

			GLenum type = GL_FLOAT;

			if (AE_FLOAT == elements[j].type)
				type = GL_FLOAT;
			else if (AE_INT == elements[j].type)
				type = GL_INT;

			if (AE_VERTEX == elements[j].attribute)
			{
				glEnableClientState(GL_VERTEX_ARRAY);

				glVertexPointer(elements[j].numcomponents, type, elements[j].stride, (void*)(elements[j].offset)); 
			}
			else if (AE_NORMAL == elements[j].attribute)
			{
				glEnableClientState(GL_NORMAL_ARRAY);

				assert(3 == elements[j].numcomponents);

				glNormalPointer(type, elements[j].stride, (void*)(elements[j].offset));
			}
			else if (AE_TEXCOORD0 == elements[j].attribute)
			{
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);

				glTexCoordPointer(elements[j].numcomponents, type, elements[j].stride, (void*)(elements[j].offset));
			}

			// TODO : indices
		}
	}

	Unbind();
}

void VertexBinding::Dispose()
{
	assert(_vaoHandle != 0);

	_glex.glDeleteVertexArrays(1, &_vaoHandle);
	_vaoHandle = 0;
}

void VertexBinding::Bind()
{
	assert(_vaoHandle != 0);

	_glex.glBindVertexArray(_vaoHandle);
}

void VertexBinding::Unbind()
{
	_glex.glBindVertexArray(0);
}