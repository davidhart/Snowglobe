#include "Lightning.h"


#include "Util.h"
#include "Renderer.h"
#include <cmath>

Lightning::Lightning() :
	_elapsed (-1.0f),
	_flashDuration(2.0f)
{

}

void Lightning::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "textured_unlit.vsh");
	_fragShader.CreateFromFile(renderer, "textured_unlit.fsh");

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_model.Read("lightning.obj");

	unsigned int stride = _model.GetVertexStride();
	_vertexBuffer.Create(renderer, _model.GetVertexData(), _model.GetNumVertices() * stride);
	_indexBuffer.Create(renderer, _model.GetIndexData(), _model.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_vertexBuffer, "in_tex", 2, AE_FLOAT, stride, _model.GetTexCoordOffset()),
		ArrayElement(_vertexBuffer, "in_vertex", 3, AE_FLOAT, stride, _model.GetVertexOffset()),
	};

	_texture.Create(renderer, "lightning.jpg");

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");

	_shaderProgram.Use();
	_shaderProgram.SetUniform(diffuseMap, 0);
	_vertBinding.Create(renderer, _shaderProgram, vertexLayout, 2, _indexBuffer, AE_UINT);
}

void Lightning::Dispose()
{
	_vertBinding.Dispose();
	_vertexBuffer.Dispose();
	_indexBuffer.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();

	_texture.Dispose();
}

void Lightning::Draw(const Renderer& renderer)
{
	Matrix4 identity;
	Matrix4::Identity(identity);

	Draw(renderer, identity);
}

void Lightning::DrawReflection(const Renderer& renderer)
{
	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1, -1, 1));

	Draw(renderer, mirror);
}

void Lightning::Draw(const Renderer& renderer, const Matrix4& modelMatrix)
{
	if (!ShouldDraw())
		return;

	renderer.EnableCullFace(false);
	renderer.EnableBlend(true);
	renderer.BlendMode(BLEND_ADDITIVE);
	renderer.EnableDepthWrite(false);

	_shaderProgram.Use();

	Matrix4 rotation;
	Matrix4::RotationAxis(rotation, Vector3(0, 1, 0), 5.0f);
	Matrix4 translation;
	Matrix4::Translation(translation, Vector3(1.6f, 2.5f, 0.0f));

	_shaderProgram.SetUniform(_standardUniforms.Model, modelMatrix * translation * rotation);

	_texture.Bind();

	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _model.GetNumIndices());

	renderer.EnableCullFace(true);
	renderer.EnableBlend(false);
	renderer.EnableDepthWrite(true);
}

bool Lightning::ShouldDraw() const
{
	if (_elapsed < 0)
		return false;

	return std::sin(_elapsed * 5000.0f) > 0.8f;
}

void Lightning::Update(float delta)
{
	if (_elapsed < 0)
		return;

	_elapsed += delta;

	if (_elapsed > _flashDuration)
		_elapsed = -1;
}

void Lightning::Flash()
{
	_elapsed = 0.0f;
}