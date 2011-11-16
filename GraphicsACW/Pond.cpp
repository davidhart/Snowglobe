#include "Pond.h"
#include "Util.h"
#include "Renderer.h"

Pond::Pond()
{

}

void Pond::Create(const Renderer& renderer)
{
	std::string shadersource;
	Util::ReadTextFileToString("basic.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("basic.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_pondModel.Read("pond.obj");

	unsigned int stride = _pondModel.GetVertexStride();
	_pondBuffer.Create(renderer, _pondModel.GetVertexData(), _pondModel.GetNumVertices() * stride);
	_pondIndices.Create(renderer, _pondModel.GetIndexData(), _pondModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_pondBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _pondModel.GetTexCoordOffset()),
		ArrayElement(_pondBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _pondModel.GetNormalOffset()),
		ArrayElement(_pondBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _pondModel.GetVertexOffset()),
	};

	_shaderProgram.Use();

	Matrix4 identity;
	Matrix4::Identity(identity);
	_shaderProgram.SetUniform("model", identity);

	_vertBinding.Create(renderer, vertexLayout, 3, _pondIndices, AE_UINT);
}

void Pond::Dispose()
{
	_vertBinding.Dispose();
	_pondBuffer.Dispose();
	_pondIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void Pond::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());
}