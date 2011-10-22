#pragma once

#include "Uncopyable.h"
#include "glex.h"

class VertexBuffer;
class Renderer;

enum AttributeSlot
{
	AE_VERTEX = -1,
	AE_NORMAL = -2,
	AE_TEXCOORD0 = -3,
	AE_COLOR = -4,
	AE_INDICIES = -5,
};

enum ElementType
{
	AE_FLOAT,
	AE_INT,
};

struct ArrayElement
{
	const VertexBuffer& buffer;
	AttributeSlot attribute;
	unsigned int numcomponents;
	ElementType type;
	unsigned int stride;
	unsigned int offset;
};

class VertexBinding : public Uncopyable
{

public:

	explicit VertexBinding();
	~VertexBinding();
	void Create(Renderer& renderer, const ArrayElement* elements, unsigned int numelements);
	void Dispose();

	void Bind();
	void Unbind();

private:

	glex* _glex;
	GLuint _vaoHandle;

};