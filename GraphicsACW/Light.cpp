#include "Light.h"
#include "Util.h"
#include "ShaderProgram.h"
#include <sstream>

const std::string Light::strBefore("lights");
const std::string Light::strPosition(".position");
const std::string Light::strSpotDirection(".spotDirection");
const std::string Light::strSpot(".spot");
const std::string Light::strAttenuation(".attenuation");
const std::string Light::strSpecular(".specular");
const std::string Light::strDiffuse(".diffuse");

Light::Light() : 
	_position(0),
	_specular(0),
	_diffuse(0),
	_spotDirection(0),
	_spot(0),
	_attenuation(1, 0, 0)
{

}

void Light::Off(Light& light)
{
	light._position = Vector4(0);
}

void Light::Directional(Light& light, const Vector3& direction)
{
	light._position = Vector4(direction, 0);
}

void Light::Point(Light& light, const Vector3& position)
{
	light._position = Vector4(position, 1);
}

void Light::Spot(Light& light, const Vector3& position, const Vector3& direction, float innerangle, float outerangle, float falloffPower)
{
	light._position = Vector4(position, 1);
	light._spotDirection = direction;
	light._spot.x(cos(Util::DegreesToRadians(innerangle)));
	light._spot.y(cos(Util::DegreesToRadians(outerangle)));
	light._spot.z(falloffPower);
}

void Light::UpdateShaderUniforms(const ShaderProgram& shaderProgram, unsigned int lightID) const
{
	std::ostringstream idStream;
	idStream << '[' << lightID << ']';

	std::string uniform;

	uniform = strBefore + idStream.str() + strPosition;
	shaderProgram.SetUniform(uniform.c_str(), _position);

	uniform = strBefore + idStream.str() + strSpotDirection;
	shaderProgram.SetUniform(uniform.c_str(), _spotDirection);

	uniform = strBefore + idStream.str() + strSpot;
	shaderProgram.SetUniform(uniform.c_str(), _spot);

	uniform = strBefore + idStream.str() + strAttenuation;
	shaderProgram.SetUniform(uniform.c_str(), _attenuation);

	uniform = strBefore + idStream.str() + strSpecular;
	shaderProgram.SetUniform(uniform.c_str(), _specular);

	uniform = strBefore + idStream.str() + strDiffuse;
	shaderProgram.SetUniform(uniform.c_str(), _diffuse);
}
