#include "House.h"
#include "Util.h"
#include "Renderer.h"

const float House::SNOW_TRANSITION_IN_TIME = 15.0f;
const float House::SNOW_TRANSITION_OUT_TIME = 10.0f;

House::House() :
	_elapsed(0.0f),
	_state(SNOW_MELTED)
{

}

void House::Create(const Renderer& renderer)
{
	_vertShader.CreateFromFile(renderer, "assets/textured_lit.vsh");
	_fragShader.CreateFromFile(renderer, "assets/multitextured_lit.fsh");

	_shaderProgram.Create(renderer, _vertShader, _fragShader);

	_houseModel.Read("assets/house.obj");

	unsigned int stride = _houseModel.GetVertexStride();
	_houseBuffer.Create(renderer, _houseModel.GetVertexData(), _houseModel.GetNumVertices() * stride);
	_houseIndices.Create(renderer, _houseModel.GetIndexData(), _houseModel.GetNumIndices() * sizeof(unsigned int));
	
	ArrayElement vertexLayout[] =
	{
		ArrayElement(_houseBuffer, "in_tex", 2, AE_FLOAT, stride, _houseModel.GetTexCoordOffset()),
		ArrayElement(_houseBuffer, "in_normal", 3, AE_FLOAT, stride, _houseModel.GetNormalOffset()),
		ArrayElement(_houseBuffer, "in_vertex", 3, AE_FLOAT, stride, _houseModel.GetVertexOffset()),
	};

	_houseTexture.Create(renderer, "assets/house_diffuse.jpg");
	_houseSnowTexture.Create(renderer, "assets/house_diffuse_snow.jpg");

	renderer.GetStandardUniforms(_shaderProgram, _standardUniforms);
	_uniformTextureBlend = _shaderProgram.GetUniform("textureBlend");
	Uniform diffuseMap = _shaderProgram.GetUniform("diffuseMap");
	Uniform diffuseMap2 = _shaderProgram.GetUniform("diffuseMap2");

	_shaderProgram.Use();
	_shaderProgram.SetUniform(diffuseMap, 0);
	_shaderProgram.SetUniform(diffuseMap2, 1);

	Uniform matSpecular = _shaderProgram.GetUniform("matSpecular");
	_shaderProgram.SetUniform(matSpecular, Vector3(0.2f));

	_vertBinding.Create(renderer, _shaderProgram, vertexLayout, 3, _houseIndices, AE_UINT);
}

void House::Dispose()
{
	_vertBinding.Dispose();
	_houseBuffer.Dispose();
	_houseIndices.Dispose();

	_shaderProgram.Dispose();
	_vertShader.Dispose();
	_fragShader.Dispose();

	_houseTexture.Dispose();
	_houseSnowTexture.Dispose();
}

void House::Draw(const Renderer& renderer)
{
	_shaderProgram.Use();

	Matrix4 identity;
	Matrix4::Identity(identity);

	Draw(renderer, identity);
}

void House::DrawReflection(const Renderer& renderer)
{
	Matrix4 mirror;
	Matrix4::Scale(mirror, Vector3(1, -1, 1));

	Draw(renderer, mirror);
}

void House::Draw(const Renderer& renderer, const Matrix4& modelMatrix)
{
	_shaderProgram.Use();

	Matrix4 houseRotation;
	Matrix4::RotationAxis(houseRotation, Vector3(0, 1, 0), -0.4f);
	Matrix4 houseTranslation;
	Matrix4::Translation(houseTranslation, Vector3(-1.3f, 0, -3.4f));

	_shaderProgram.SetUniform(_standardUniforms.Model, modelMatrix * houseTranslation * houseRotation);
	_shaderProgram.SetUniform(_uniformTextureBlend, GetTextureBlend());
	_houseTexture.Bind(0);
	_houseSnowTexture.Bind(1);
	
	renderer.UpdateStandardUniforms(_shaderProgram, _standardUniforms);
	renderer.Draw(_vertBinding, PT_TRIANGLES, 0, _houseModel.GetNumIndices());
}

void House::Update(float delta)
{
	_elapsed += delta;

	if (_state == SNOW_MELTING && _elapsed > SNOW_TRANSITION_OUT_TIME)
	{
		_state = SNOW_MELTED;
	}

	if (_state == SNOW_ACCUMULATE && _elapsed > SNOW_TRANSITION_IN_TIME)
	{
		_state = SNOW_VISIBLE;
	}
}

void House::SnowBegin()
{
	_elapsed = 0.0f;
	_state = SNOW_ACCUMULATE;
}

void House::SnowEnd()
{
	_elapsed = 0.0f;
	_state = SNOW_MELTING;
}

float House::GetTextureBlend() const
{
	if (_state == SNOW_VISIBLE)
		return 1.0f;
	else if (_state == SNOW_MELTED)
		return 0.0f;
	else if (_state == SNOW_MELTING)
		return 1.0f - _elapsed / SNOW_TRANSITION_OUT_TIME;
	else
		return _elapsed / SNOW_TRANSITION_IN_TIME;
}