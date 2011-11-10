#include "House.h"
#include "Util.h"
#include "Renderer.h"

House::House()
{

}

void House::Create(const Renderer& renderer)
{
	std::string shadersource;
	Util::ReadTextFileToString("house.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("house.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_houseModel.Read("house.obj");

	unsigned int stride = _houseModel.GetVertexStride();
	_houseBuffer.Create(renderer, _houseModel.GetVertexData(), _houseModel.GetNumVertices() * stride);
	_houseIndices.Create(renderer, _houseModel.GetIndexData(), _houseModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		{ _houseBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _houseModel.GetTexCoordOffset() },
		{ _houseBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _houseModel.GetNormalOffset() },
		{ _houseBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _houseModel.GetVertexOffset() },
	};

	_shaderProgram.Use();

	Matrix4 houseRotation;
	Matrix4::RotationAxis(houseRotation, Vector3(0, 1, 0), 0.5f);
	Matrix4 houseTranslation;
	Matrix4::Translation(houseTranslation, Vector3(-1.5f, 0, -2.5f));
	_shaderProgram.SetUniform("model", houseRotation * houseTranslation);

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
}

void House::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();
	renderer.UpdateViewProjectionUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _houseModel.GetNumIndices());
}