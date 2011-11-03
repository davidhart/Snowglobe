#pragma once

#include "Util.h"
#include "Vector3.h"
#include <cmath>
#include <cassert>

class Vector4f
{

public:

	inline Vector4f();
	inline explicit Vector4f(float v);
	inline Vector4f(const Vector3f& v, float w);
	inline Vector4f(float x, float y, float z, float w);

	inline Vector4f& operator+=(const Vector4f& rhs);
	inline Vector4f& operator-=(const Vector4f& rhs);
	inline Vector4f& operator*=(const Vector4f& rhs);
	inline Vector4f& operator*=(float rhs);
	inline Vector4f& operator/=(const Vector4f& rhs);
	inline Vector4f& operator/=(float rhs);
	inline Vector4f operator-() const;
	
	inline float Length() const;
	inline float Dot(const Vector4f& rhs) const;
	inline Vector4f Cross(const Vector4f& rhs) const;

	inline float x() const;
	inline float y() const;
	inline float z() const;
	inline float w() const;

	inline void x(float x);
	inline void y(float y);
	inline void z(float z);
	inline void w(float w);

	float operator[](unsigned int i) const;

private:

	float _v[4];

};

inline Vector4f::Vector4f()
{
	_v[0] = 0;
	_v[1] = 0;
	_v[2] = 0;
	_v[3] = 0;
}

inline Vector4f::Vector4f(float v)
{
	_v[0] = v;
	_v[1] = v;
	_v[2] = v;
	_v[3] = v;
}

inline Vector4f::Vector4f(const Vector3f& v, float w)
{
	_v[0] = v.x();
	_v[1] = v.y();
	_v[2] = v.z();
	_v[3] = w;
}

inline Vector4f::Vector4f(float x, float y, float z, float w)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
	_v[3] = w;
}

inline Vector4f& Vector4f::operator+=(const Vector4f& rhs)
{
	x(x() + rhs.x());
	y(y() + rhs.y());
	z(z() + rhs.z());
	w(w() + rhs.w());
	return *this;
}

inline Vector4f& Vector4f::operator-=(const Vector4f& rhs)
{
	x(x() - rhs.x());
	y(y() - rhs.y());
	z(z() - rhs.z());
	w(w() - rhs.w());
	return *this;
}

inline Vector4f& Vector4f::operator*=(const Vector4f& rhs)
{
	x(x() * rhs.x());
	y(y() * rhs.y());
	z(z() * rhs.z());
	w(w() * rhs.w());
	return *this;
}

inline Vector4f& Vector4f::operator/=(const Vector4f& rhs)
{
	x(x() / rhs.x());
	y(y() / rhs.y());
	z(z() / rhs.z());
	w(w() / rhs.w());
	return *this;
}

inline Vector4f& Vector4f::operator/=(float rhs)
{
	x(x() / rhs);
	y(y() / rhs);
	z(z() / rhs);
	w(w() / rhs);
	return *this;
}

inline Vector4f Vector4f::operator-() const
{
	return Vector4f(-x(), -y(), -z(), - w());
}

inline float Vector4f::Length() const
{
	return sqrt(x() * x() + y() * y() + z() * z() + w() + w());
}

inline float Vector4f::Dot(const Vector4f& rhs) const
{
	return x() * rhs.x() + y() * rhs.y() + z() * rhs.z() + w() + rhs.w();
}

inline Vector4f Vector4f::Cross(const Vector4f& rhs) const
{
	assert(false); // not implemented yet
	return Vector4f(y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x());
}

inline float Vector4f::operator[](unsigned int i) const
{
	assert(i < 4);
	return _v[i];
}

inline float Vector4f::x() const
{
	return _v[0];
}

inline float Vector4f::y() const
{
	return _v[1];
}

inline float Vector4f::z() const
{
	return _v[2];
}

inline float Vector4f::w() const
{
	return _v[3];
}

inline void Vector4f::x(float x)
{
	_v[0] = x;
}

inline void Vector4f::y(float y)
{
	_v[1] = y;
}

inline void Vector4f::z(float z)
{
	_v[2] = z;
}

inline void Vector4f::w(float w)
{
	_v[3] = w;
}

inline Vector4f operator+(const Vector4f& lhs, const Vector4f& rhs)
{
	return Vector4f(lhs) += rhs;
}

inline Vector4f operator-(const Vector4f& lhs, const Vector4f& rhs)
{
	return Vector4f(lhs) -= rhs;
}

inline Vector4f operator*(const Vector4f& lhs, const Vector4f& rhs)
{
	return Vector4f(lhs) *= rhs;
}

inline Vector4f operator*(float lhs, const Vector4f& rhs)
{
	return Vector4f(rhs) *= lhs;
}

inline Vector4f operator*(const Vector4f& lhs, float rhs)
{
	return Vector4f(lhs) *= rhs;
}

inline Vector4f operator/(const Vector4f& lhs, const Vector4f& rhs)
{
	return Vector4f(lhs) /= rhs;
}

inline Vector4f operator/(float lhs, const Vector4f& rhs)
{
	return Vector4f(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z(), lhs / rhs.w());
}

inline Vector4f operator/(const Vector4f& lhs, float rhs)
{
	return Vector4f(lhs) /= rhs;
}

inline bool operator==(const Vector4f& lhs, const Vector4f& rhs)
{
	return Util::FloatEquality(lhs.x(), rhs.y()) &&
		Util::FloatEquality(lhs.y(), rhs.y()) &&
		Util::FloatEquality(lhs.z(), rhs.z() &&
		Util::FloatEquality(lhs.w(), rhs.w());
}

inline bool operator!=(const Vector4f& lhs, const Vector4f& rhs)
{
	return !(lhs == rhs);
}
