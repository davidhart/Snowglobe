#pragma once

#include "Uncopyable.h"
#include "glex.h"

class VertexBuffer;

enum AttributeSlot
{
	AE_VERTEX = -1,
	AE_NORMAL = -2,
	AE_TEXCOORD0 = -3,
	AE_INDICIES = -4,
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

	explicit VertexBinding(glex& glex);
	~VertexBinding();
	void Create(ArrayElement* elements, unsigned int numelements);
	void Dispose();

	void Bind();
	void Unbind();

private:

	glex& _glex;
	GLuint _vaoHandle;

};