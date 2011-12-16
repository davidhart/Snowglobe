// David Hart - 2011

#include "ParticleSystem.h"
#include "Util.h"
#include "Renderer.h"
#include "Texture.h"
#include <algorithm>

ParticleEmitter::ParticleEmitter() :
	_position(0),
	_windDirection(0),
	_texture(NULL),
	_numParticles(0),
	_shape(1),
	_spread(1),
	_particleSize(1),
	_speed(1),
	_height(1),
	_emitting(false),
	_elapsed(0.0f),
	_endEmissionTime(-100.0f)
{
}

void ParticleEmitter::BeginEmit()
{
	_elapsed = 0.0f;
	_emitting = true;
}

void ParticleEmitter::EndEmit()
{
	_emitting = false;
	_endEmissionTime = _elapsed;
}

void ParticleEmitter::Update(float delta)
{
	_elapsed += delta;
}

ParticleSystem::ParticleSystem()
{
}

void ParticleSystem::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "assets/particle_emitter.vsh");
	_fragShader.CreateFromFile(renderer, "assets/textured_unlit.fsh");
	_shader.Create(renderer, _vertShader, _fragShader);

	Util::CreateObjFileWithBuffers("assets/quad.obj", renderer, _quadModel, _instancedQuadVertices, _instancedQuadIndices);

	unsigned int stride = _quadModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_instancedQuadVertices, "in_tex", 2, AE_FLOAT, stride, _quadModel.GetTexCoordOffset()),
		ArrayElement(_instancedQuadVertices, "in_vertex", 3, AE_FLOAT, stride, _quadModel.GetVertexOffset()),
	};

	_vertBinding.Create(renderer, _shader, vertexLayout, 2, _instancedQuadIndices, AE_UINT);

	renderer.GetStandardUniforms(_shader, _standardUniforms);

	Uniform diffuseMap = _shader.GetUniform("diffuseMap");
	_uniformTime = _shader.GetUniform("time");
	_uniformEndTime = _shader.GetUniform("endTime");
	_uniformNumParticles = _shader.GetUniform("numParticles");
	_uniformParticleSystemShape = _shader.GetUniform("particleSystemShape");
	_uniformParticleSystemHeight = _shader.GetUniform("particleSystemHeight");
	_uniformParticleSpread = _shader.GetUniform("particleSpread");
	_uniformWindDirection = _shader.GetUniform("windDirection");
	_uniformParticleSize = _shader.GetUniform("particleSize");
	_uniformParticleSpeed = _shader.GetUniform("particleSpeed");

	_shader.Use();
	_shader.SetUniform(diffuseMap, 0);
}

void ParticleSystem::Dispose()
{
	_vertBinding.Dispose();
	_instancedQuadVertices.Dispose();
	_instancedQuadIndices.Dispose();

	_shader.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();
}

void ParticleSystem::Draw(const Renderer& renderer)
{
	Matrix4 identity;
	Matrix4::Identity(identity);

	Draw(renderer, identity);
}

void ParticleSystem::DrawReflected(const Renderer& renderer)
{
	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1,-1,1));

	Draw(renderer, mirror);
}

void ParticleSystem::Draw(const Renderer& renderer, const Matrix4& modelMatrix)
{
	renderer.EnableCullFace(false);
	renderer.BlendMode(BLEND_ADDITIVE);
	renderer.EnableBlend(true);
	renderer.EnableDepthWrite(false);

	_shader.Use();
	renderer.UpdateStandardUniforms(_shader, _standardUniforms);

	Matrix4 translation;

	for (unsigned int i = 0; i < _particleEmitters.size(); ++i)
	{
		ParticleEmitter& emitter = *_particleEmitters[i];
		emitter._texture->Bind();

		Matrix4::Translation(translation, emitter._position);
		_shader.SetUniform(_standardUniforms.Model, modelMatrix * translation);
		_shader.SetUniform(_uniformTime, emitter._elapsed);
		_shader.SetUniform(_uniformEndTime, emitter._emitting ? emitter._elapsed : emitter._endEmissionTime);
		_shader.SetUniform(_uniformNumParticles, (float)emitter._numParticles);
		_shader.SetUniform(_uniformParticleSystemShape, emitter._shape);
		_shader.SetUniform(_uniformParticleSystemHeight, emitter._height);
		_shader.SetUniform(_uniformParticleSpread, emitter._spread);
		_shader.SetUniform(_uniformWindDirection, emitter._windDirection);
		_shader.SetUniform(_uniformParticleSize, emitter._particleSize);
		_shader.SetUniform(_uniformParticleSpeed, emitter._speed);

		renderer.DrawInstances(_vertBinding, PT_TRIANGLES, 0, _quadModel.GetNumIndices(), _particleEmitters[i]->_numParticles);
	}

	renderer.EnableCullFace(true);
	renderer.EnableBlend(false);
	renderer.EnableDepthWrite(true);
}

void ParticleSystem::Update(float delta)
{
	for (unsigned int i = 0; i < _particleEmitters.size(); ++i)
	{
		_particleEmitters[i]->Update(delta);
	}
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
{
	// We should not be adding the same emitter more than once
	assert(std::find(_particleEmitters.begin(), _particleEmitters.end(), emitter) 
			== _particleEmitters.end());

	_particleEmitters.push_back(emitter);
}

void ParticleSystem::RemoveEmitter(ParticleEmitter* emitter)
{
	std::vector<ParticleEmitter*>::iterator it = 
		std::find(_particleEmitters.begin(), _particleEmitters.end(), emitter);
	_particleEmitters.erase(it);

	// Something went wrong if there are more than one pointer to the same emitter
	assert(std::find(_particleEmitters.begin(), _particleEmitters.end(), emitter) 
			== _particleEmitters.end());
}