#include "Dome.h"
#include "Util.h"
#include "Renderer.h"

Dome::Dome()
{

}

void Dome::Create(const Renderer& renderer)
{
	_domeModel.Read("dome.obj");

	_vertexBuffer.Create(renderer, _domeModel.GetVertexData(), _domeModel.GetNumVertices() * _domeModel.GetVertexStride());
	_indexBuffer.Create(renderer, _domeModel.GetIndexData(), _domeModel.GetNumIndices() * sizeof(unsigned int));

	std::string shaderSource;
	Util::ReadTextFileToString("dome.vsh", shaderSource);

	_vertShader.Create(renderer, shaderSource.c_str());

	Util::ReadTextFileToString("domeFront.fsh", shaderSource);
	_frontFragShader.Create(renderer, shaderSource.c_str());

	Util::ReadTextFileToString("domeBack.fsh", shaderSource);
	_backFragShader.Create(renderer, shaderSource.c_str());

	_frontShader.Create(renderer, _vertShader, _frontFragShader);
	_backShader.Create(renderer, _vertShader, _backFragShader);

	Matrix4 identity;
	Matrix4::Identity(identity);

	renderer.GetStandardUniforms(_frontShader, _standardUniformFront);
	renderer.GetStandardUniforms(_backShader, _standardUniformBack);

	_backShader.Use();
	_backShader.SetUniform(_standardUniformBack.Model, identity);

	_frontShader.Use();
	_frontShader.SetUniform(_standardUniformBack.Model, identity);


	ArrayElement frontVertLayout[] = 
	{
		ArrayElement(_vertexBuffer, _frontShader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, _domeModel.GetVertexStride(), _domeModel.GetVertexOffset()),
		ArrayElement(_vertexBuffer, _frontShader.GetAttributeIndex("in_normal"), 3, AE_FLOAT, _domeModel.GetVertexStride(), _domeModel.GetNormalOffset()),
	};

	_frontBinding.Create(renderer, frontVertLayout, 2, _indexBuffer, AE_UINT);

	ArrayElement backVertLayout[] =
	{
		ArrayElement(_vertexBuffer, _backShader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, _domeModel.GetVertexStride(), _domeModel.GetVertexOffset()),
		ArrayElement(_vertexBuffer, _backShader.GetAttributeIndex("in_normal"), 3, AE_FLOAT, _domeModel.GetVertexStride(), _domeModel.GetNormalOffset()),
	};

	_backBinding.Create(renderer, backVertLayout, 2, _indexBuffer, AE_UINT);
}

void Dome::Dispose()
{
	_frontBinding.Dispose();
	_backBinding.Dispose();

	_vertexBuffer.Dispose();
	_indexBuffer.Dispose();

	_frontShader.Dispose();
	_backShader.Dispose();

	_frontFragShader.Dispose();
	_backFragShader.Dispose();
	_vertShader.Dispose();
}

void Dome::DrawBack(const Renderer& renderer)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_backShader.Use();
	renderer.UpdateStandardUniforms(_backShader, _standardUniformBack);
	glCullFace(GL_FRONT);
	renderer.Draw(_backBinding, PT_TRIANGLES, 0, _domeModel.GetNumIndices());

	glCullFace(GL_BACK);

	glDisable(GL_BLEND);
}

void Dome::DrawFront(const Renderer& renderer)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_frontShader.Use();
	renderer.UpdateStandardUniforms(_frontShader, _standardUniformFront);
	renderer.Draw(_frontBinding, PT_TRIANGLES, 0, _domeModel.GetNumIndices());

	glDisable(GL_BLEND);
}