#include "Renderer.h"
#include "VertexBinding.h"
#include <cassert>

Renderer::Renderer() :
	_glex (NULL)
{
}

Renderer::~Renderer()
{
	assert(_glex == NULL);
}

void Renderer::Create(glex* glex)
{
	assert(glex != NULL);

	_glex = glex;
}

glex* Renderer::GetEx() const
{
	assert(_glex != NULL);

	return _glex;
}

void Renderer::Dispose()
{
	assert(_glex != NULL);

	_glex = NULL;
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