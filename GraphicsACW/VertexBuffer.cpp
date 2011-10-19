#include "VertexBuffer.h"

#include <cassert>
#include <exception>

VertexBuffer::VertexBuffer(glex& glex) :
	_glex(glex),
	_vbSize(0),
	_vbHandle(0)
{
}

VertexBuffer::~VertexBuffer()
{
	if (_vbHandle != 0)
	{
		try
		{
			Dispose();
		}
		catch (const std::exception&)
		{

		}
	}
}

void VertexBuffer::Create(const void* data, unsigned int size)
{
	if (_vbHandle == 0)
	{
		_glex.glGenBuffers(1, &_vbHandle);
		_vbHandle = size;
	}

	assert(_vbHandle != 0);

	_glex.glBindBuffer(GL_ARRAY_BUFFER, _vbHandle);

	_glex.glBufferData(GL_ARRAY_BUFFER, size, (void*)data, GL_DYNAMIC_DRAW);

	_glex.glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::UpdateRegion(unsigned int offset, const void* newdata, unsigned int size)
{
	assert(_vbHandle != 0);
	assert(offset + size <= _vbSize);

	_glex.glBindBuffer(GL_ARRAY_BUFFER, _vbHandle);

	_glex.glBufferSubData(GL_ARRAY_BUFFER, offset, size, (void*)newdata);

	_glex.glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::Dispose()
{
	assert(_vbHandle != 0);

	_glex.glDeleteBuffers(1, &_vbHandle);
	_vbSize = 0;
}