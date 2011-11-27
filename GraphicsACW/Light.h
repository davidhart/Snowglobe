#pragma once

#include "Vector3.h"
#include "Vector4.h"

class ShaderProgram;

class Light
{

public:

	Light();

	void SetDiffuseColor(const Vector3& diffuse);
	void SetSpecularColor(const Vector3& specular);
	void SetSpecularPower(float power);

	void SetConstantAttenuation(float constant);
	void SetLinearAttenuation(float linear);
	void SetQuadraticAttenuation(float quadratic);
	
	static void Off(Light& light);
	static void Directional(Light& light, Vector3& direction);
	static void Point(Light& light, Vector3& position);
	static void Spot(Light& light, Vector3& position, Vector3& direction, float innerangle, float outerangle, float falloffPower);

	void UpdateShaderUniforms(const ShaderProgram& shaderProgram, unsigned int lightID) const;

private:

	Vector4 _position;		// { pos.xyz, 1} for point / spotlight 
							// or { direction.xyz, 0 } for directional light
	Vector4 _specular;		// {specular.rgb, specular power}
	Vector3 _diffuse;
	Vector3 _spotDirection;
	Vector3 _spot;			// { cos(innerangle), cos(outerangle), power }
							// or { 0, 0, 0 } if point / directional light
	Vector3 _attenuation;	// {constant attenuation, linear attenuation, quadratic attenuation}

	static const std::string strBefore;
	static const std::string strPosition;
	static const std::string strSpotDirection;
	static const std::string strSpot;
	static const std::string strAttenuation;
	static const std::string strSpecular;
	static const std::string strDiffuse;
};

inline void Light::SetDiffuseColor(const Vector3& diffuse)
{
	_diffuse = diffuse;
}

inline void Light::SetSpecularColor(const Vector3& specular)
{
	_specular.x(specular.x());
	_specular.y(specular.y());
	_specular.z(specular.z());
}

inline void Light::SetSpecularPower(float power)
{
	_specular.w(power);
}

inline void Light::SetConstantAttenuation(float constant)
{
	_attenuation.x(constant);
}

inline void Light::SetLinearAttenuation(float linear)
{
	_attenuation.y(linear);
}

inline void Light::SetQuadraticAttenuation(float quadratic)
{
	_attenuation.z(quadratic);
}