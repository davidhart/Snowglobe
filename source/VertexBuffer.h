#pragma once

#include "Uncopyable.h"
#include "glex.h"

class Renderer;

class VertexBuffer : public Uncopyable
{
	friend class VertexBinding;

public:

	explicit VertexBuffer();
	~VertexBuffer();

	void Create(const Renderer& renderer, const void* data, unsigned int size);
	void UpdateRegion(unsigned int offset, const void* newdata, unsigned int size);
	void Dispose();

	inline unsigned int Size() const { return _vbSize; }

private:

	glex* _glex;
	unsigned int _vbSize;
	GLuint _vbHandle;
};