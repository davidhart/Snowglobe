#include "SnowDrift.h"
#include "Util.h"
#include "Renderer.h"
#include "Maths.h"

const float SnowDrift::DRIFT_RAISE_TIME = 10.0f;
const float SnowDrift::DRIFT_LOWER_TIME = 10.0f;
const float SnowDrift::DRIFT_LOWER_OFFSET = -0.5f;

SnowDrift::SnowDrift() :
	_state(DRIFT_LOWERED),
	_elapsed(0)
{

}

void SnowDrift::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "assets/textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "assets/textured_lit.fsh");
	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	Util::CreateObjFileWithBuffers("assets/snow.obj", renderer, _terrainModel, _vertexBuffer, _indexBuffer);
	
	unsigned int stride = _terrainModel.GetVertexStride();
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_vertexBuffer, "in_tex", 2, AE_FLOAT, stride, _terrainModel.GetTexCoordOffset()),
		ArrayElement(_vertexBuffer, "in_normal", 3, AE_FLOAT, stride, _terrainModel.GetNormalOffset()),
		ArrayElement(_vertexBuffer, "in_vertex", 3, AE_FLOAT, stride, _terrainModel.GetVertexOffset()),
	};

	_texture.Create(renderer, "assets/snow.jpg");

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	_shaderProgram.Use();
	
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");
	_shaderProgram.SetUniform(diffuseMap, 0);

	_vertBinding.Create(renderer, _shaderProgram, vertexLayout, 3, _indexBuffer, AE_UINT);
}

void SnowDrift::Dispose()
{
	_vertBinding.Dispose();
	_vertexBuffer.Dispose();
	_indexBuffer.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();

	_texture.Dispose();
}

void SnowDrift::Draw(const Renderer& renderer, const Matrix4& matrix)
{
	if (_state != DRIFT_LOWERED)
	{
		Vector3 offset (0, 0, 0);

		if (_state == DRIFT_RAISING)
		{
			offset.y(DRIFT_LOWER_OFFSET - DRIFT_LOWER_OFFSET * _elapsed / DRIFT_RAISE_TIME);
		}
		else if (_state == DRIFT_LOWERING)
		{
			offset.y(DRIFT_LOWER_OFFSET * _elapsed / DRIFT_LOWER_TIME);
		}

		Matrix4 translation;
		Matrix4::Translation(translation, offset);

		_shaderProgram.Use();
		_shaderProgram.SetUniform(_standardUniforms.Model, matrix * translation);

		_texture.Bind();

		renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
		renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _terrainModel.GetNumIndices());
	}
}

void SnowDrift::Draw(const Renderer& renderer)
{
	Matrix4 identity;
	Matrix4::Identity(identity);

	Draw(renderer, identity);
}

void SnowDrift::DrawReflection(const Renderer& renderer)
{
	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1,-1,1));
	
	Draw(renderer, mirror);
}

void SnowDrift::Update(float dt)
{
	_elapsed += dt;

	if (_state == DRIFT_LOWERING && _elapsed > DRIFT_LOWER_TIME)
	{
		_state = DRIFT_LOWERED;
		_elapsed = 0;
	}
	else if (_state == DRIFT_RAISING && _elapsed > DRIFT_RAISE_TIME)
	{
		_state = DRIFT_RAISED;
		_elapsed = 0;
	}
}

void SnowDrift::Raise()
{
	_state = DRIFT_RAISING;
	_elapsed = 0;
}

void SnowDrift::Lower()
{
	_state = DRIFT_LOWERING;
	_elapsed = 0;
}