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
	_indexBuffer.Create(renderer, _domeModel.GetIndexData(), _domeModel.GetNumIndices() * sizeof(float));

	std::string shaderSource;
	Util::ReadTextFileToString("dome.vsh", shaderSource);

	_vertShader.Create(renderer, shaderSource.c_str());

	Util::ReadTextFileToString("domeFront.fsh", shaderSource);
	_frontFragShader.Create(renderer, shaderSource.c_str());

	Util::ReadTextFileToString("domeBack.fsh", shaderSource);
	_backFragShader.Create(renderer, shaderSource.c_str());

	_frontShader.Create(renderer, _vertShader, _frontFragShader);
	_backShader.Create(renderer, _vertShader, _backFragShader);

	ArrayElement frontVertLayout[] = 
	{
		{ _vertexBuffer, _frontShader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, 0, 0 },
	};

	_frontBinding.Create(renderer, frontVertLayout, 1, _indexBuffer, AE_UINT);

	ArrayElement backVertLayout[] =
	{
		{ _vertexBuffer, _backShader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, 0, 0 },
	};

	_backBinding.Create(renderer, backVertLayout, 1, _indexBuffer, AE_UINT);
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

void Dome::Draw(const Renderer& renderer)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDisable(GL_DEPTH_TEST);

	glCullFace(GL_FRONT);
	_backShader.Use();
	renderer.Draw(_backBinding, PT_TRIANGLES, 0, _domeModel.GetNumIndices());

	glCullFace(GL_BACK);
	_frontShader.Use();
	renderer.Draw(_frontBinding, PT_TRIANGLES, 0, _domeModel.GetNumIndices());

	glDisable(GL_BLEND);
}