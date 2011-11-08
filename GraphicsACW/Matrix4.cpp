#include "Matrix4.h"

Matrix4::Matrix4()
{

}

Matrix4::Matrix4(const Vector4& c0, const Vector4& c1, const Vector4& c2, const Vector4& c3)
{
	_m[0] = c0;
	_m[1] = c1;
	_m[2] = c2;
	_m[3] = c3;
}