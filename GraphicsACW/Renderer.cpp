#include "Renderer.h"
#include <cassert>

Renderer::Renderer() :
	_glex (nullptr)
{
}

Renderer::~Renderer()
{
	assert(_glex == nullptr);
}

void Renderer::Create(glex* glex)
{
	assert(glex != nullptr);

	_glex = glex;
}

glex* Renderer::GetEx()
{
	assert(_glex != nullptr);

	return _glex;
}

void Renderer::Dispose()
{
	assert(_glex != nullptr);

	_glex = nullptr;
}

void Renderer::Draw(Primitive primitive, unsigned int offset, unsigned int indices)
{
	glDrawArrays(primitive, offset, indices);
}