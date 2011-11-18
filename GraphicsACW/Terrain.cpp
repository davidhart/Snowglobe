#include "Terrain.h"
#include "Util.h"
#include "Renderer.h"

Terrain::Terrain()
{

}

void Terrain::Create(const Renderer& renderer)
{
	std::string shadersource;
	Util::ReadTextFileToString("textured_unlit.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("textured_unlit.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_terrainModel.Read("terrain.obj");

	unsigned int stride = _terrainModel.GetVertexStride();
	_terrainBuffer.Create(renderer, _terrainModel.GetVertexData(), _terrainModel.GetNumVertices() * stride);
	_terrainIndices.Create(renderer, _terrainModel.GetIndexData(), _terrainModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_terrainBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _terrainModel.GetTexCoordOffset()),
		ArrayElement(_terrainBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _terrainModel.GetNormalOffset()),
		ArrayElement(_terrainBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _terrainModel.GetVertexOffset()),
	};

	_texture.Create(renderer, "grass.jpg");

	_shaderProgram.Use();
	_shaderProgram.SetUniform("diffuseMap", 0);

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

	_texture.Dispose();
}

void Terrain::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();

	Matrix4 identity;
	Matrix4::Identity(identity);
	
	_shaderProgram.SetUniform("model", identity);

	_texture.Bind();

	renderer.UpdateStandardUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
}

void Terrain::DrawReflection(const Renderer& renderer)
{
	_shaderProgram.Use();

	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1,-1,1));
	
	_shaderProgram.SetUniform("model", mirror);

	_texture.Bind();

	renderer.UpdateStandardUniforms(_shaderProgram);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
}