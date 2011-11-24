#include "SmokeParticles.h"
#include "Util.h"
#include "Renderer.h"

SmokeParticles::SmokeParticles() :
	_elapsed(0.0f),
	_numParticles(0)
{
}

void SmokeParticles::Create(const Renderer& renderer, unsigned int numParticles)
{
	assert(numParticles > 0);
	_numParticles = numParticles;

	_texture.Create(renderer, "smoke_alpha.jpg");

	std::string shadersource;
	Util::ReadTextFileToString("particle_smoke.vsh", shadersource);

	_vertShader.Create(renderer, shadersource.c_str());

	Util::ReadTextFileToString("textured_unlit.fsh", shadersource);
	_fragShader.Create(renderer, shadersource.c_str());

	_shader.Create(renderer, _vertShader, _fragShader);

	_instancedQuadModel.Read("quad.obj");

	unsigned int stride = _instancedQuadModel.GetVertexStride();
	_instancedQuadBuffer.Create(renderer, _instancedQuadModel.GetVertexData(), _instancedQuadModel.GetNumVertices() * stride);
	_instancedQuadIndices.Create(renderer, _instancedQuadModel.GetIndexData(), _instancedQuadModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_instancedQuadBuffer, _shader.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _instancedQuadModel.GetTexCoordOffset()),
		ArrayElement(_instancedQuadBuffer, _shader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _instancedQuadModel.GetVertexOffset()),
	};

	_shader.Use();
	_shader.SetUniform("diffuseMap", 0);
	_shader.SetUniform("particleSystemShape", 1.4f);
	_shader.SetUniform("particleSystemHeight", 3.5f);
	_shader.SetUniform("particleSpread", 1.2f);
	_shader.SetUniform("windDirection", Vector3(2.0f, 0.0f, 5.0f));
	_shader.SetUniform("particleSize", 0.8f);
	_shader.SetUniform("particleSpeed", 0.3f);
	_vertBinding.Create(renderer, vertexLayout, 2, _instancedQuadIndices, AE_UINT);
}

void SmokeParticles::Dispose()
{
	_vertBinding.Dispose();
	_instancedQuadBuffer.Dispose();
	_instancedQuadIndices.Dispose();

	_shader.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();

	_texture.Dispose();
}

void SmokeParticles::Draw(const Renderer& renderer)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glDepthMask(GL_FALSE);
	_texture.Bind();

	_shader.Use();

	_shader.SetUniform("time", _elapsed);

	Matrix4 model;
	Matrix4::Translation(model, Vector3(-2.69f, 2.1f, -4.4f));
	_shader.SetUniform("model", model);

	renderer.UpdateStandardUniforms(_shader);
	renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _instancedQuadModel.GetNumIndices(), _numParticles);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void SmokeParticles::DrawReflected(const Renderer& renderer)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glDepthMask(GL_FALSE);
	_texture.Bind();

	_shader.Use();

	_shader.SetUniform("time", _elapsed);

	Matrix4 model;
	Matrix4 flip;
	Matrix4::Translation(model, Vector3(-2.69f, 2.1f, -4.4f));
	Matrix4::Scale(flip, Vector3(1, -1, 1));
	_shader.SetUniform("model", flip * model);

	renderer.UpdateStandardUniforms(_shader);
	renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _instancedQuadModel.GetNumIndices(), _numParticles);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void SmokeParticles::Update(float delta)
{
	_elapsed += delta;
}