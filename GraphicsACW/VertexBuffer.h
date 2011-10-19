#pragma once

#include "Uncopyable.h"
#include "glex.h"

class VertexBuffer : public Uncopyable
{
	friend class VertexBinding;

public:

	explicit VertexBuffer(glex& glex);
	~VertexBuffer();

	void Create(const void* data, unsigned int size);
	void UpdateRegion(unsigned int offset, const void* newdata, unsigned int size);
	void Dispose();

private:

	glex& _glex;
	unsigned int _vbSize;
	GLuint _vbHandle;
};