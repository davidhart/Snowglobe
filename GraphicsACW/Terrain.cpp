#include "Terrain.h"
#include "Util.h"
#include "Renderer.h"

Terrain::Terrain()
{

}

void Terrain::Create(const Renderer& renderer)
{
	std::string shadersource;
	Util::ReadTextFileToString("house.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("house.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_terrainModel.Read("terrain.obj");

	unsigned int stride = _terrainModel.GetVertexStride();
	_terrainBuffer.Create(renderer, _terrainModel.GetVertexData(), _terrainModel.GetNumVertices() * stride);
	_terrainIndices.Create(renderer, _terrainModel.GetIndexData(), _terrainModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		{ _terrainBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _terrainModel.GetTexCoordOffset() },
		{ _terrainBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _terrainModel.GetNormalOffset() },
		{ _terrainBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _terrainModel.GetVertexOffset() },
	};

	_shaderProgram.Use();

	_vertBinding.Create(renderer, vertexLayout, 3, _terrainIndices, AE_UINT);
}

void Terrain::Dispose()
{
	_vertBinding.Dispose();
	_terrainBuffer.Dispose();
	_terrainIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void Terrain::Draw(const Renderer& renderer, bool flip)
{
	_shaderProgram.Use();

	Matrix4 modelmat;
	if (flip)
	{
		Matrix4::Scale(modelmat, Vector3(1,-1,1));
	}
	else
	{
		Matrix4::Scale(modelmat, Vector3(1,1,1));
	}
	
	_shaderProgram.SetUniform("model", modelmat);

	renderer.UpdateViewProjectionUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
}