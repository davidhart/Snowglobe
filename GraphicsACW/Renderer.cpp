#include "Renderer.h"
#include "VertexBinding.h"
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

glex* Renderer::GetEx() const
{
	assert(_glex != nullptr);

	return _glex;
}

void Renderer::Dispose()
{
	assert(_glex != nullptr);

	_glex = nullptr;
}

void Renderer::Draw(VertexBinding& binding, Primitive primitive, unsigned int offset, unsigned int indices) const
{
	binding.Bind();

	if (binding._hasIndices)
	{
		glDrawElements(primitive, indices, binding._indicesType, (void*)offset);
	}
	else
	{
		glDrawArrays(primitive, offset, indices);
	}

	binding.Unbind();
}