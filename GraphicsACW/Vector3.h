#pragma once

#include "Util.h"
#include <cmath>
#include <cassert>

class Vector3f
{

public:

	inline Vector3f();
	inline explicit Vector3f(float v);
	inline Vector3f(float x, float y, float z);

	inline Vector3f& operator+=(const Vector3f& rhs);
	inline Vector3f& operator-=(const Vector3f& rhs);
	inline Vector3f& operator*=(const Vector3f& rhs);
	inline Vector3f& operator*=(float rhs);
	inline Vector3f& operator/=(const Vector3f& rhs);
	inline Vector3f& operator/=(float rhs);
	inline Vector3f operator-() const;
	
	inline float Length() const;
	inline float Dot(const Vector3f& rhs) const;
	inline Vector3f Cross(const Vector3f& rhs) const;

	inline float x() const;
	inline float y() const;
	inline float z() const;

	inline void x(float x);
	inline void y(float y);
	inline void z(float z);

	float operator[](unsigned int i) const;

private:

	float _v[3];

};

inline Vector3f::Vector3f()
{
	_v[0] = 0;
	_v[1] = 0;
	_v[2] = 0;
}

inline Vector3f::Vector3f(float v)
{
	_v[0] = v;
	_v[1] = v;
	_v[2] = v;
}

inline Vector3f::Vector3f(float x, float y, float z)
{
	_v[0] = x;
	_v[1] = y;
	_v[2] = z;
}

inline Vector3f& Vector3f::operator+=(const Vector3f& rhs)
{
	x(x() + rhs.x());
	y(y() + rhs.y());
	z(z() + rhs.z());
	return *this;
}

inline Vector3f& Vector3f::operator-=(const Vector3f& rhs)
{
	x(x() - rhs.x());
	y(y() - rhs.y());
	z(z() - rhs.z());
	return *this;
}

inline Vector3f& Vector3f::operator*=(const Vector3f& rhs)
{
	x(x() * rhs.x());
	y(y() * rhs.y());
	z(z() * rhs.z());
	return *this;
}

inline Vector3f& Vector3f::operator/=(const Vector3f& rhs)
{
	x(x() / rhs.x());
	y(y() / rhs.y());
	z(z() / rhs.z());
	return *this;
}

inline Vector3f& Vector3f::operator/=(float rhs)
{
	x(x() / rhs);
	y(y() / rhs);
	z(z() / rhs);
	return *this;
}

inline Vector3f Vector3f::operator-() const
{
	return Vector3f(-x(), -y(), -z());
}

inline float Vector3f::Length() const
{
	return sqrt(x() * x() + y() * y() + z() * z());
}

inline float Vector3f::Dot(const Vector3f& rhs) const
{
	return x() * rhs.x() + y() * rhs.y() + z() * rhs.z();
}

inline Vector3f Vector3f::Cross(const Vector3f& rhs) const
{
	return Vector3f(y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x());
}

inline float Vector3f::operator[](unsigned int i) const
{
	assert(i < 3);
	return _v[i];
}

inline float Vector3f::x() const
{
	return _v[0];
}

inline float Vector3f::y() const
{
	return _v[1];
}

inline float Vector3f::z() const
{
	return _v[2];
}

inline void Vector3f::x(float x)
{
	_v[0] = x;
}

inline void Vector3f::y(float y)
{
	_v[1] = y;
}

inline void Vector3f::z(float z)
{
	_v[2] = z;
}

inline Vector3f operator+(const Vector3f& lhs, const Vector3f& rhs)
{
	return Vector3f(lhs) += rhs;
}

inline Vector3f operator-(const Vector3f& lhs, const Vector3f& rhs)
{
	return Vector3f(lhs) -= rhs;
}

inline Vector3f operator*(const Vector3f& lhs, const Vector3f& rhs)
{
	return Vector3f(lhs) *= rhs;
}

inline Vector3f operator*(float lhs, const Vector3f& rhs)
{
	return Vector3f(rhs) *= lhs;
}

inline Vector3f operator*(const Vector3f& lhs, float rhs)
{
	return Vector3f(lhs) *= rhs;
}

inline Vector3f operator/(const Vector3f& lhs, const Vector3f& rhs)
{
	return Vector3f(lhs) /= rhs;
}

inline Vector3f operator/(float lhs, const Vector3f& rhs)
{
	return Vector3f(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z());
}

inline Vector3f operator/(const Vector3f& lhs, float rhs)
{
	return Vector3f(lhs) /= rhs;
}

inline bool operator==(const Vector3f& lhs, const Vector3f& rhs)
{
	return Util::FloatEquality(lhs.x(), rhs.y()) &&
		Util::FloatEquality(lhs.y(), rhs.y()) &&
		Util::FloatEquality(lhs.z(), rhs.z());
}

inline bool operator!=(const Vector3f& lhs, const Vector3f& rhs)
{
	return !(lhs == rhs);
}
