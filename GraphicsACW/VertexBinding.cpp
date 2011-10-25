#include "VertexBinding.h"
#include "VertexBuffer.h"
#include "ShaderProgram.h"
#include "Renderer.h"

#include <exception>
#include <cassert>
#include <vector>

VertexBinding::VertexBinding() :
	_glex(nullptr),
	_vaoHandle(0),
	_indicesType(AE_UBYTE),
	_hasIndices(false)
{
}

VertexBinding::~VertexBinding()
{
	assert(_vaoHandle == 0);
}

void VertexBinding::Create(const Renderer& renderer, const ArrayElement* elements, unsigned int numelements)
{
	CreateVAO(renderer);

	Bind();

	SetupAttribPointers(elements, numelements);

	Unbind();
}

void VertexBinding::Create(const Renderer& renderer, const ArrayElement* elements, unsigned int numelements, const VertexBuffer& indices, ElementType indicesType)
{
	CreateVAO(renderer);

	Bind();

	SetupAttribPointers(elements, numelements);
	SetupIndices(indices, indicesType);

	Unbind();
}

void VertexBinding::CreateVAO(const Renderer& renderer)
{
	if (_vaoHandle != 0)
		Dispose();

	_glex = renderer.GetEx();

	_glex->glGenVertexArrays(1, &_vaoHandle);
}

void VertexBinding::SetupAttribPointers(const ArrayElement* elements, unsigned int numelements)
{
	for (unsigned int i = 0; i < numelements; ++i)
	{
		const ArrayElement& element = elements[i];

		_glex->glBindBuffer(GL_ARRAY_BUFFER, element.buffer._vbHandle);

		GLenum type = GL_FLOAT;

		if (AE_FLOAT == element.type)
			type = GL_FLOAT;
		else if (AE_INT == element.type)
			type = GL_INT;

		_glex->glEnableVertexAttribArray(element.attribLocation);
		_glex->glVertexAttribPointer(element.attribLocation, element.numcomponents, type, GL_FALSE, element.stride,
				(void*)element.offset);
	}
}

void VertexBinding::SetupIndices(const VertexBuffer& indices, ElementType indicesType)
{
	_hasIndices = true;
	_indicesType = indicesType;

	assert(indicesType == AE_UBYTE || indicesType == AE_UINT || indicesType == AE_USHORT);

	_glex->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices._vbHandle);
}

void VertexBinding::Dispose()
{
	assert(_vaoHandle != 0);

	_glex->glDeleteVertexArrays(1, &_vaoHandle);
	_vaoHandle = 0;
	_glex = nullptr;
	_hasIndices = false;
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