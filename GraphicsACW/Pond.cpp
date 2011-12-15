#include "Pond.h"
#include "Util.h"
#include "Renderer.h"

Pond::Pond()
{

}

void Pond::Create(const Renderer& renderer)
{
	_texture.Create(renderer, "pond_diffuse.tga");

	_vertShader.CreateFromFile(renderer, "textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "textured_lit.fsh");
	_fragDepthClear.CreateFromFile(renderer, "depth_clear.fsh");

	_shaderProgram.Create(renderer, _vertShader, _fragShader);
	_shaderDepthClear.Create(renderer, _vertShader, _fragDepthClear);

	Util::CreateObjFileWithBuffers("pond.obj", renderer, _pondModel, _pondBuffer, _pondIndices);

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.GetStandardUniforms(_shaderDepthClear , _depthClearUniforms);
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");

	Matrix4 identity;
	Matrix4::Identity(identity);
	
	_shaderProgram.Use();
	_shaderProgram.SetUniform(_standardUniforms.Model, identity);
	_shaderProgram.SetUniform(diffuseMap, 0);

	_shaderDepthClear.Use();
	_shaderProgram.SetUniform(_depthClearUniforms.Model, identity);
	
	unsigned int stride = _pondModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_pondBuffer, _shaderProgram.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _pondModel.GetTexCoordOffset()),
		ArrayElement(_pondBuffer, _shaderProgram.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _pondModel.GetNormalOffset()),
		ArrayElement(_pondBuffer, _shaderProgram.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _pondModel.GetVertexOffset()),
	};
	_vertBinding.Create(renderer, vertexLayout, 3, _pondIndices, AE_UINT);

	ArrayElement vertexLayoutDepthClear[] =
	{
		ArrayElement(_pondBuffer, _shaderDepthClear.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _pondModel.GetTexCoordOffset()),
		ArrayElement(_pondBuffer, _shaderDepthClear.GetAttributeIndex("in_normal"), 3, AE_FLOAT, stride, _pondModel.GetNormalOffset()),
		ArrayElement(_pondBuffer, _shaderDepthClear.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _pondModel.GetVertexOffset()),
	};
	_vertDepthClearBinding.Create(renderer, vertexLayoutDepthClear, 3, _pondIndices, AE_UINT);

}

void Pond::Dispose()
{
	_vertBinding.Dispose();
	_vertDepthClearBinding.Dispose();
	_pondBuffer.Dispose();
	_pondIndices.Dispose();

	_shaderProgram.Dispose();
	_shaderDepthClear.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
	_fragDepthClear.Dispose();

	_texture.Dispose();
}

void Pond::DrawStencilMask(const Renderer& renderer)
{
	renderer.EnableStencilTest(true);
	renderer.StencilTest(STENCIL_ALWAYS, 1);
	renderer.StencilOp(STENCIL_KEEP, STENCIL_KEEP, STENCIL_REPLACE);

	renderer.EnableDepthWrite(false);
	renderer.EnableColorWrite(false);

	_texture.Bind();
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());

	renderer.EnableDepthWrite(true);
	renderer.EnableDepthTest(false);
	
	renderer.StencilTest(STENCIL_EQUAL, 1);
	renderer.StencilOp(STENCIL_KEEP, STENCIL_KEEP, STENCIL_KEEP);

	_shaderDepthClear.Use();
	renderer.UpdateStandardUniforms(_shaderDepthClear, _depthClearUniforms);
	renderer.Draw(_vertDepthClearBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());
	
	renderer.EnableStencilTest(false);
	renderer.EnableDepthTest(true);
	renderer.EnableColorWrite(true);
}

void Pond::Draw(const Renderer& renderer)
{
	renderer.EnableBlend(true);
	renderer.BlendMode(BLEND_ALPHA);

	_texture.Bind();
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());

	renderer.EnableBlend(false);
}