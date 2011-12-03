#include "Renderer.h"
#include "VertexBinding.h"
#include "ShaderProgram.h"
#include <cassert>

Renderer::Renderer() :
	_glex (NULL)
{
}

Renderer::~Renderer()
{
	assert(_glex == NULL);
}

void Renderer::Create(glex* glex)
{
	assert(glex != NULL);

	_glex = glex;
}

glex* Renderer::GetEx() const
{
	assert(_glex != NULL);

	return _glex;
}

void Renderer::Dispose()
{
	assert(_glex != NULL);

	_glex = NULL;
}

void Renderer::Draw(VertexBinding& binding, Primitive primitive, unsigned int offset, unsigned int indices) const
{
	binding.Bind();

	if (binding._hasIndices)
	{
		glDrawElements(primitive, indices, binding._indicesType, (void*)offset);
	}
	else
	{
		glDrawArrays(primitive, offset, indices);
	}

	binding.Unbind();
}

void Renderer::DrawInstances(VertexBinding& binding, Primitive primitive, unsigned int offset, unsigned int indices, unsigned int instances) const
{
	binding.Bind();

	_glex->glDrawElementsInstanced(primitive, indices, binding._indicesType, (void*)offset, instances);

	binding.Unbind();
}

void Renderer::ProjectionMatrix(const Matrix4& projection)
{
	_projection = projection;
}

void Renderer::ViewMatrix(const Matrix4& view)
{
	_view = view;
}

void Renderer::ClipPlane(const Vector4& clipPlane)
{
	_clipPlane = clipPlane;
}

void Renderer::SetLight(unsigned int id, const Light& light)
{
	assert(id < Light::MAX_LIGHTS);

	_lights[id] = light;
}

void Renderer::GetLight(unsigned int id, Light& light) const
{
	assert(id < Light::MAX_LIGHTS);
	
	light = _lights[id];
}

void Renderer::SetAmbient(const Vector3& ambient)
{
	_ambient = ambient;
}

void Renderer::UpdateStandardUniforms(const ShaderProgram& shaderprogram, const StandardUniformBlock& uniforms) const
{
	// TODO: evaluate these on change only as they are unlikely to change frequently
	shaderprogram.SetUniform(uniforms.View, _view);
	shaderprogram.SetUniform(uniforms.Projection, _projection);
	shaderprogram.SetUniform(uniforms.ClipPlane, _clipPlane);
	shaderprogram.SetUniform(uniforms.Ambient, _ambient);

	for (unsigned int i = 0; i < Light::MAX_LIGHTS; ++i)
	{
		_lights[i].UpdateShaderUniforms(shaderprogram, uniforms.Lights[i]);
	}
}

void Renderer::GetStandardUniforms(const ShaderProgram& shaderProgram, StandardUniformBlock & uniforms) const
{
	uniforms.Model = shaderProgram.GetUniform("model");
	uniforms.View = shaderProgram.GetUniform("view");
	uniforms.Projection = shaderProgram.GetUniform("projection");
	uniforms.ClipPlane = shaderProgram.GetUniform("clipPlane");

	uniforms.Ambient = shaderProgram.GetUniform("ambient");

	for (unsigned int i = 0; i < Light::MAX_LIGHTS; ++i)
	{
		Light::GetShaderUniforms(shaderProgram, i, uniforms.Lights[i]);
	}
}