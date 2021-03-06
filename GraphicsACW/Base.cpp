// David Hart - 2011

#include "Base.h"
#include "Util.h"
#include "Renderer.h"

Base::Base()
{

}

void Base::Create(const Renderer& renderer)
{
	_texture.Create(renderer, "assets/base_diffuse.tga");

	_vertShader.CreateFromFile(renderer, "assets/textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "assets/textured_lit.fsh");

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_baseModel.Read("assets/base.obj");

	unsigned int stride = _baseModel.GetVertexStride();
	_baseBuffer.Create(renderer, _baseModel.GetVertexData(), _baseModel.GetNumVertices() * stride);
	_baseIndices.Create(renderer, _baseModel.GetIndexData(), _baseModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_baseBuffer, "in_normal", 3, AE_FLOAT, stride, _baseModel.GetNormalOffset()),
		ArrayElement(_baseBuffer, "in_vertex", 3, AE_FLOAT, stride, _baseModel.GetVertexOffset()),
	};

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	
	Matrix4 identity;
	Matrix4::Identity(identity);

	_shaderProgram.Use();
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");
	_shaderProgram.SetUniform(diffuseMap, 0);

	Uniform shadowMap = _shaderProgram.GetUniform("shadowMap");
	_shaderProgram.SetUniform(shadowMap, 3);
	_shaderProgram.SetUniform(_standardUniforms.Model, identity);

	Uniform specularMat = _shaderProgram.GetUniform("matSpecular");
	_shaderProgram.SetUniform(specularMat, Vector3(1));

	_vertBinding.Create(renderer, _shaderProgram, vertexLayout, 2, _baseIndices, AE_UINT);
}

void Base::Dispose()
{
	_vertBinding.Dispose();
	_baseBuffer.Dispose();
	_baseIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
	_texture.Dispose();
}

void Base::Draw(const Renderer& renderer)
{
	_texture.Bind();
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _baseModel.GetNumIndices());
}