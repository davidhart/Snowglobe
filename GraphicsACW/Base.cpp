#include "Base.h"
#include "Util.h"
#include "Renderer.h"

Base::Base()
{

}

void Base::Create(const Renderer& renderer)
{
	std::string shadersource;
	Util::ReadTextFileToString("basic.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("basic.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_baseModel.Read("base.obj");

	unsigned int stride = _baseModel.GetVertexStride();
	_baseBuffer.Create(renderer, _baseModel.GetVertexData(), _baseModel.GetNumVertices() * stride);
	_baseIndices.Create(renderer, _baseModel.GetIndexData(), _baseModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_baseBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _baseModel.GetNormalOffset()),
		ArrayElement(_baseBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _baseModel.GetVertexOffset()),
	};

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	
	Matrix4 identity;
	Matrix4::Identity(identity);

	_shaderProgram.Use();
	_shaderProgram.SetUniform(_standardUniforms.Model, identity);

	_vertBinding.Create(renderer, vertexLayout, 2, _baseIndices, AE_UINT);
}

void Base::Dispose()
{
	_vertBinding.Dispose();
	_baseBuffer.Dispose();
	_baseIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void Base::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _baseModel.GetNumIndices());
}