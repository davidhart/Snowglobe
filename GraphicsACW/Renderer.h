#pragma once

#include "GXBase.h"
#include "Uncopyable.h"
#include "Matrix4.h"
#include "ShaderProgram.h"
#include "Light.h"

class glex;
class VertexBinding;
class ShaderProgram;
class Texture;

enum Primitive
{
	PT_TRIANGLES = GL_TRIANGLES,
	PT_POINTS = GL_POINTS,
	PT_LINES = GL_LINES,
	PT_LINESTRIP = GL_LINE_STRIP,
	PT_LINELOOP = GL_LINE_LOOP,
};

class Renderer : public Uncopyable
{

public:

	struct StandardUniformBlock
	{
		Uniform Model;
		Uniform View;
		Uniform Projection;
		Uniform ClipPlane;

		Uniform Ambient;
		Light::LightUniformBlock Lights[Light::MAX_LIGHTS];
	};

	Renderer();
	~Renderer();
	void Create(glex* glex);
	void Dispose();

	void Draw(VertexBinding& binding, Primitive primitive, unsigned int offset, unsigned int indices) const;
	void DrawInstances(VertexBinding& binding, Primitive primitive, unsigned int offset, unsigned int indices, unsigned int instances) const;

	void ProjectionMatrix(const Matrix4& projection);
	void ViewMatrix(const Matrix4& view);
	void ClipPlane(const Vector4& plane);
	
	void SetLight(unsigned int id, const Light& light);
	void GetLight(unsigned int id, Light& light) const;

	void SetAmbient(const Vector3& ambient);

	void UpdateStandardUniforms(const ShaderProgram& shader, const StandardUniformBlock& uniforms) const;
	void GetStandardUniforms(const ShaderProgram& shader, StandardUniformBlock& uniforms) const;

	glex* GetEx() const;

private:

	Light _lights[Light::MAX_LIGHTS];
	Vector3 _ambient;
	Matrix4 _view;
	Matrix4 _projection;
	Vector4 _clipPlane;
	glex* _glex;

};
