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
	glStencilFunc(GL_ALWAYS, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_STENCIL_TEST);
	glDepthMask(GL_FALSE);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

	_texture.Bind();
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());

	glDepthMask(GL_TRUE);
	glDepthFunc(GL_ALWAYS);
	glStencilFunc(GL_EQUAL, 1, 1);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	_shaderDepthClear.Use();
	renderer.UpdateStandardUniforms(_shaderDepthClear, _depthClearUniforms);
	renderer.Draw(_vertDepthClearBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());
	glDisable(GL_STENCIL_TEST);
	glDepthFunc(GL_LESS);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void Pond::Draw(const Renderer& renderer)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	_texture.Bind();
	_shaderProgram.Use();
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _pondModel.GetNumIndices());

	glDisable(GL_BLEND);
}