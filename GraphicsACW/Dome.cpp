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

	_vertShader.CreateFromFile(renderer, "untextured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "dome.fsh");
	_shader.Create(renderer, _vertShader, _fragShader);

	Matrix4 identity;
	Matrix4::Identity(identity);

	renderer.GetStandardUniforms(_shader, _standardUniforms);
	_uniformNormalScale = _shader.GetUniform("normalScale");
	_uniformRimColor = _shader.GetUniform("rimColor");

	_shader.Use();
	_shader.SetUniform(_standardUniforms.Model, identity);

	ArrayElement vertLayout[] = 
	{
		ArrayElement(_vertexBuffer, _shader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, _domeModel.GetVertexStride(), _domeModel.GetVertexOffset()),
		ArrayElement(_vertexBuffer, _shader.GetAttributeIndex("in_normal"), 3, AE_FLOAT, _domeModel.GetVertexStride(), _domeModel.GetNormalOffset()),
	};

	_vertBinding.Create(renderer, vertLayout, 2, _indexBuffer, AE_UINT);
}

void Dome::Dispose()
{
	_vertBinding.Dispose();
	_vertexBuffer.Dispose();
	_indexBuffer.Dispose();

	_shader.Dispose();
	_fragShader.Dispose();
	_vertShader.Dispose();
}

void Dome::DrawBack(const Renderer& renderer)
{
	renderer.CullFace(C_FRONT);
	Draw(renderer, -1.0f, Vector4(0,0,0,0));
	renderer.CullFace(C_BACK);
}

void Dome::DrawFront(const Renderer& renderer)
{
	Draw(renderer, 1.0f, Vector4(1,1,1, 2.5f));
}

void Dome::Draw(const Renderer& renderer, float normalScale, const Vector4& rimColor)
{
	renderer.EnableBlend(true);
	renderer.BlendMode(BLEND_ADDITIVE);

	_shader.Use();
	renderer.UpdateStandardUniforms(_shader, _standardUniforms);
	_shader.SetUniform(_uniformNormalScale, normalScale);
	_shader.SetUniform(_uniformRimColor, rimColor);

	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _domeModel.GetNumIndices());

	renderer.EnableBlend(false);
}