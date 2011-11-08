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

	Matrix4 model;
	Matrix4::Scale(model, 6);

	_backShader.Use();
	_backShader.SetUniform("model", model);

	_frontShader.Use();
	_frontShader.SetUniform("model", model);


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

void Dome::DrawBack(const Renderer& renderer)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_backShader.Use();
	renderer.UpdateViewProjectionUniforms(_backShader);
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
	renderer.UpdateViewProjectionUniforms(_frontShader);
	renderer.Draw(_frontBinding, PT_TRIANGLES, 0, _domeModel.GetNumIndices());

	glDisable(GL_BLEND);
}