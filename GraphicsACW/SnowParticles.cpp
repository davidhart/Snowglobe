#include "SnowParticles.h"
#include "Util.h"
#include "Renderer.h"

SnowParticles::SnowParticles() :
	_emitting(false),
	_elapsed(0.0f),
	_endEmissionTime(-100.0f),
	_numParticles(0)
{
}

void SnowParticles::Create(const Renderer& renderer, unsigned int numParticles)
{
	assert(numParticles > 0);
	_numParticles = numParticles;

	_texture.Create(renderer, "snow_alpha.jpg");

	_vertShader.CreateFromFile(renderer, "particle_snow.vsh");
	_fragShader.CreateFromFile(renderer, "textured_unlit.fsh");
	_shader.Create(renderer, _vertShader, _fragShader);

	Util::CreateObjFileWithBuffers("quad.obj", renderer, _instancedQuadModel, _instancedQuadBuffer, _instancedQuadIndices);

	unsigned int stride = _instancedQuadModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_instancedQuadBuffer, _shader.GetAttributeIndex("in_tex"), 2, AE_FLOAT, stride, _instancedQuadModel.GetTexCoordOffset()),
		ArrayElement(_instancedQuadBuffer, _shader.GetAttributeIndex("in_vertex"), 3, AE_FLOAT, stride, _instancedQuadModel.GetVertexOffset()),
	};

	renderer.GetStandardUniforms(_shader, _standardUniforms);
	Uniform diffuseMap = _shader.GetUniform("diffuseMap");
	Uniform particleSpread = _shader.GetUniform("particleSpread");
	Uniform particleSize = _shader.GetUniform("particleSize");
	Uniform particleSpeed = _shader.GetUniform("particleSpeed");
	_uniformTime = _shader.GetUniform("time");
	_uniformEndTime = _shader.GetUniform("endTime");

	_shader.Use();
	_shader.SetUniform(diffuseMap, 0);
	_shader.SetUniform(particleSpread, 6.0f);
	_shader.SetUniform(particleSize, 0.05f);
	_shader.SetUniform(particleSpeed, 0.3f);

	_vertBinding.Create(renderer, vertexLayout, 2, _instancedQuadIndices, AE_UINT);
}

void SnowParticles::Dispose()
{
	_vertBinding.Dispose();
	_instancedQuadBuffer.Dispose();
	_instancedQuadIndices.Dispose();

	_shader.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();

	_texture.Dispose();
}

void SnowParticles::Draw(const Renderer& renderer)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glDepthMask(GL_FALSE);
	_texture.Bind();

	_shader.Use();

	_shader.SetUniform(_uniformTime, _elapsed);

	Matrix4 model;
	Matrix4::Translation(model, Vector3(0.0f, 1.5f, 0.0f));
	_shader.SetUniform(_standardUniforms.Model, model);

	_shader.SetUniform(_uniformEndTime, _emitting ? _elapsed : _endEmissionTime);

	renderer.UpdateStandardUniforms(_shader, _standardUniforms);
	renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _instancedQuadModel.GetNumIndices(), _numParticles);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void SnowParticles::DrawReflected(const Renderer& renderer)
{
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_COLOR, GL_ONE);
	glDepthMask(GL_FALSE);
	_texture.Bind();

	_shader.Use();

	_shader.SetUniform(_uniformTime, _elapsed);

	Matrix4 model;
	Matrix4 flip;
	Matrix4::Translation(model, Vector3(0.0f, 1.5f, 0.0f));
	Matrix4::Scale(flip, Vector3(1, -1, 1));
	_shader.SetUniform(_standardUniforms.Model, flip * model);

	renderer.UpdateStandardUniforms(_shader, _standardUniforms);
	renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _instancedQuadModel.GetNumIndices(), _numParticles);

	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
}

void SnowParticles::Update(float delta)
{
	_elapsed += delta;
}

void SnowParticles::BeginEmit()
{
	_elapsed = 0.0f;
	_emitting = true;
}

void SnowParticles::EndEmit()
{
	_emitting = false;
	_endEmissionTime = _elapsed;
}