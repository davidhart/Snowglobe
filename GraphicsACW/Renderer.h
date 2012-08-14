// David Hart - 2011
// 
// class Renderer
//   Renderer is a thin wrapper on the OpenGL state. Renderer
//   provides access to an OpenGL 3.0 implementation of the  following
//   deprecated fixed function pipline features:
//     -Lighting
//     -Model, View, Projection matrices
//     -Clip Plane

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

enum ePrimitive
{
	PT_TRIANGLES = GL_TRIANGLES,
	PT_POINTS = GL_POINTS,
	PT_LINES = GL_LINES,
	PT_LINESTRIP = GL_LINE_STRIP,
	PT_LINELOOP = GL_LINE_LOOP,
};

enum eCullFace
{
	C_FRONT = GL_FRONT,
	C_FRONT_AND_BACK = GL_FRONT_AND_BACK,
	C_BACK = GL_BACK,
};

enum eBlendMode
{
	BLEND_ADDITIVE,
	BLEND_ALPHA,
};

enum eStencilOp
{
	STENCIL_KEEP = GL_KEEP,
	STENCIL_REPLACE = GL_REPLACE,
	STENCIL_ZERO = GL_ZERO,
	STENCIL_INCR = GL_INCR,
};

enum eStencilFunc
{
	STENCIL_EQUAL = GL_EQUAL,
	STENCIL_LESS = GL_LESS,
	STENCIL_GREATER = GL_GREATER,
	STENCIL_NEVER = GL_NEVER,
	STENCIL_ALWAYS = GL_ALWAYS,
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

		Uniform shadowMatrix;

		Light::LightUniformBlock Lights[Light::MAX_LIGHTS];
	};

	Renderer();
	~Renderer();
	void Create(glex* glex);
	void Dispose();

	void Draw(VertexBinding& binding, ePrimitive primitive, unsigned int offset, unsigned int indices) const;
	void DrawInstances(VertexBinding& binding, ePrimitive primitive, unsigned int offset, unsigned int indices, unsigned int instances) const;

	void ProjectionMatrix(const Matrix4& projection);
	void ViewMatrix(const Matrix4& view);
	void ClipPlane(const Vector4& plane);
	
	void SetLight(unsigned int id, const Light& light);
	void GetLight(unsigned int id, Light& light) const;

	void SetAmbient(const Vector3& ambient);

	void UpdateStandardUniforms(const ShaderProgram& shader, const StandardUniformBlock& uniforms) const;
	void GetStandardUniforms(const ShaderProgram& shader, StandardUniformBlock& uniforms) const;

	void EnableCullFace(bool enable) const;
	void CullFace(eCullFace face) const;

	void EnableBlend(bool enable) const;
	void BlendMode(eBlendMode mode) const;

	void EnableDepthTest(bool enable) const;
	void EnableDepthWrite(bool enable) const;

	void EnableColorWrite(bool enable) const;

	void EnableStencilTest(bool enable) const;
	void StencilOp(eStencilOp stencilFail, eStencilOp stencilPassDepthFail, eStencilOp stencilDepthPass) const;
	void StencilTest(eStencilFunc func, int ref = 0) const;

	void SetShadowMatrix(const Matrix4& shadowMatrix);

	void SetClearColor(const Vector4& color) const;
	void Clear() const;

	glex* GetEx() const;

private:

	Light _lights[Light::MAX_LIGHTS];
	Vector3 _ambient;
	Matrix4 _view;
	Matrix4 _projection;
	Matrix4 _shadowMatrix;
	Vector4 _clipPlane;
	glex* _glex;

};
