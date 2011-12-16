// David Hart - 2011

#include "Pond.h"
#include "Util.h"
#include "Renderer.h"

Pond::Pond()
{

}

void Pond::Create(const Renderer& renderer)
{
	_texture.Create(renderer, "assets/pond_diffuse.tga");

	_vertShader.CreateFromFile(renderer, "assets/textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "assets/textured_lit.fsh");
	_fragDepthClear.CreateFromFile(renderer, "assets/depth_clear.fsh");

	_shaderProgram.Create(renderer, _vertShader, _fragShader);
	_shaderDepthClear.Create(renderer, _vertShader, _fragDepthClear);

	Util::CreateObjFileWithBuffers("assets/pond.obj", renderer, _pondModel, _pondBuffer, _pondIndices);

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
		ArrayElement(_pondBuffer, "in_tex", 2, AE_FLOAT, stride, _pondModel.GetTexCoordOffset()),
		ArrayElement(_pondBuffer, "in_normal", 3, AE_FLOAT, stride, _pondModel.GetNormalOffset()),
		ArrayElement(_pondBuffer, "in_vertex", 3, AE_FLOAT, stride, _pondModel.GetVertexOffset()),
	};

	_vertBinding.Create(renderer, _shaderProgram, vertexLayout, 3, _pondIndices, AE_UINT);
	_vertDepthClearBinding.Create(renderer, _shaderDepthClear, vertexLayout, 3, _pondIndices, AE_UINT);

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