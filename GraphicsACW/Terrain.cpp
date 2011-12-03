#include "Terrain.h"
#include "Util.h"
#include "Renderer.h"
#include "Maths.h"

Terrain::Terrain()
{

}

void Terrain::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "textured_lit.fsh");
	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	Util::CreateObjFileWithBuffers("terrain.obj", renderer, _terrainModel, _terrainBuffer, _terrainIndices);
	
	unsigned int stride = _terrainModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_terrainBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _terrainModel.GetTexCoordOffset()),
		ArrayElement(_terrainBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _terrainModel.GetNormalOffset()),
		ArrayElement(_terrainBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _terrainModel.GetVertexOffset()),
	};

	_texture.Create(renderer, "grass.jpg");

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	_shaderProgram.Use();
	
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");
	_shaderProgram.SetUniform(diffuseMap, 0);

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
	
	_shaderProgram.SetUniform(_standardUniforms.Model, identity);

	_texture.Bind();

	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
}

void Terrain::DrawReflection(const Renderer& renderer)
{
	_shaderProgram.Use();

	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1,-1,1));
	
	_shaderProgram.SetUniform(_standardUniforms.Model, mirror);

	_texture.Bind();

	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
}