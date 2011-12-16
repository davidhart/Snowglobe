// David Hart - 2011

#include "Light.h"
#include "Util.h"
#include "ShaderProgram.h"
#include <sstream>

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

void Light::UpdateShaderUniforms(const ShaderProgram& shaderProgram, const LightUniformBlock& uniforms) const
{
	shaderProgram.SetUniform(uniforms.Position, _position);
	shaderProgram.SetUniform(uniforms.SpotDirection, _spotDirection);
	shaderProgram.SetUniform(uniforms.Spot, _spot);
	shaderProgram.SetUniform(uniforms.Attenuation, _attenuation);
	shaderProgram.SetUniform(uniforms.Specular, _specular);
	shaderProgram.SetUniform(uniforms.Diffuse, _diffuse);
}

void Light::GetShaderUniforms(const ShaderProgram& shaderProgram, unsigned int lightID, LightUniformBlock& uniforms)
{
	std::ostringstream prefix;
	prefix << "lights[" << lightID << "].";

	uniforms.Position =		 shaderProgram.GetUniform((prefix.str() + std::string("position")).c_str());
	uniforms.SpotDirection = shaderProgram.GetUniform((prefix.str() + std::string("spotDirection")).c_str());
	uniforms.Spot =			 shaderProgram.GetUniform((prefix.str() + std::string("spot")).c_str());
	uniforms.Attenuation =	 shaderProgram.GetUniform((prefix.str() + std::string("attenuation")).c_str());
	uniforms.Specular =		 shaderProgram.GetUniform((prefix.str() + std::string("specular")).c_str());
	uniforms.Diffuse =		 shaderProgram.GetUniform((prefix.str() + std::string("diffuse")).c_str());
}

void Light::ReflectInYAxis()
{
	_position.y(-_position.y());
	_spotDirection.y(-_spotDirection.y());
}