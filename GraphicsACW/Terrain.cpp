// David Hart - 2011

#include "Terrain.h"
#include "Util.h"
#include "Renderer.h"
#include "Maths.h"

Terrain::Terrain()
{

}

void Terrain::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "assets/textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "assets/textured_lit.fsh");
	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	Util::CreateObjFileWithBuffers("assets/terrain.obj", renderer, _terrainModel, _terrainBuffer, _terrainIndices);
	
	_texture.Create(renderer, "assets/grass.jpg", T_REPEAT, T_LINEAR);

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	_shaderProgram.Use();
	
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");
	_shaderProgram.SetUniform(diffuseMap, 0);

	Uniform shadowMap = _shaderProgram.GetUniform("shadowMap");
	_shaderProgram.SetUniform(shadowMap, 3);

	Uniform matSpecular = _shaderProgram.GetUniform("matSpecular");
	_shaderProgram.SetUniform(matSpecular, Vector3(0.1f));

	unsigned int stride = _terrainModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_terrainBuffer, "in_tex", 2, AE_FLOAT, stride, _terrainModel.GetTexCoordOffset()),
		ArrayElement(_terrainBuffer, "in_normal", 3, AE_FLOAT, stride, _terrainModel.GetNormalOffset()),
		ArrayElement(_terrainBuffer, "in_vertex", 3, AE_FLOAT, stride, _terrainModel.GetVertexOffset()),
	};

	_vertBinding.Create(renderer, _shaderProgram, vertexLayout, 3, _terrainIndices, AE_UINT);
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

	_texture.Bind(0);

	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
}