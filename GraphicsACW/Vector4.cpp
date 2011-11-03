#include "Vector4.h"

Vector4f::Vector4f()
{
	_v[0] = 0;
	_v[1] = 0;
	_v[2] = 0;
	_v[3] = 0;
}

Vector4f::Vector4f(float v)
{
	_v[0] = v;
	_v[1] = v;
	_v[2] = v;
	_v[3] = v;
}

Vector4f::Vector4f(const Vector3f& v, float w)
{
	_v[0] = v.x();
	_v[1] = v.y();
	_v[2] = v.z();
	_v[3] = w;
}

Vector4f::Vector4f(float x, float y, float z, float w)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
	_v[3] = w;
}