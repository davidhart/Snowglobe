#include "House.h"
#include "Util.h"
#include "Renderer.h"

House::House()
{

}

void House::Create(const Renderer& renderer)
{
	std::string shadersource;
	Util::ReadTextFileToString("textured_lit.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("textured_lit.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_houseModel.Read("house.obj");

	unsigned int stride = _houseModel.GetVertexStride();
	_houseBuffer.Create(renderer, _houseModel.GetVertexData(), _houseModel.GetNumVertices() * stride);
	_houseIndices.Create(renderer, _houseModel.GetIndexData(), _houseModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_houseBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _houseModel.GetTexCoordOffset()),
		ArrayElement(_houseBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _houseModel.GetNormalOffset()),
		ArrayElement(_houseBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _houseModel.GetVertexOffset()),
	};

	_houseTexture.Create(renderer, "house_diffuse.jpg");

	_shaderProgram.Use();
	_shaderProgram.SetUniform("diffuseMap", 0);
	_vertBinding.Create(renderer, vertexLayout, 3, _houseIndices, AE_UINT);
}

void House::Dispose()
{
	_vertBinding.Dispose();
	_houseBuffer.Dispose();
	_houseIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();

	_houseTexture.Dispose();
}

void House::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();

	Matrix4 houseRotation;
	Matrix4::RotationAxis(houseRotation, Vector3(0, 1, 0), -0.4f);
	Matrix4 houseTranslation;
	Matrix4::Translation(houseTranslation, Vector3(-1.3f, 0, -3.4f));
	_shaderProgram.SetUniform("model", houseTranslation * houseRotation);

	_houseTexture.Bind();

	renderer.UpdateStandardUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _houseModel.GetNumIndices());
}

void House::DrawReflection(const Renderer& renderer)
{
	_shaderProgram.Use();

	Matrix4 houseRotation;
	Matrix4::RotationAxis(houseRotation, Vector3(0, 1, 0), -0.4f);
	Matrix4 houseTranslation;
	Matrix4::Translation(houseTranslation, Vector3(-1.3f, 0, -3.4f));

	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1,-1,1));
	_shaderProgram.SetUniform("model", mirror * houseTranslation * houseRotation);
	_houseTexture.Bind();
	
	renderer.UpdateStandardUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _houseModel.GetNumIndices());
}