#pragma once

#include "Util.h"
#include <iostream>
#include <cmath>
#include <cassert>

class Vector3f
{

public:

	Vector3f();
	explicit Vector3f(float v);
	Vector3f(float x, float y, float z);

	inline Vector3f& operator+=(const Vector3f& rhs);
	inline Vector3f& operator-=(const Vector3f& rhs);
	inline Vector3f& operator*=(const Vector3f& rhs);
	inline Vector3f& operator*=(float rhs);
	inline Vector3f& operator/=(const Vector3f& rhs);
	inline Vector3f& operator/=(float rhs);
	inline Vector3f operator-() const;
	
	inline float length() const;
	inline float dot(const Vector3f& rhs) const;
	inline Vector3f Cross(const Vector3f& rhs) const;

	inline float x() const;
	inline float y() const;
	inline float z() const;

	inline void x(float x);
	inline void y(float y);
	inline void z(float z);

	inline float element(unsigned int i) const;
	inline void element(unsigned int i, float value);
	inline float operator[](unsigned int i) const;

private:

	float _v[3];

};

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

inline float Vector3f::length() const
{
	return sqrt(x() * x() + y() * y() + z() * z());
}

inline float Vector3f::dot(const Vector3f& rhs) const
{
	return x() * rhs.x() + y() * rhs.y() + z() * rhs.z();
}

inline Vector3f Vector3f::Cross(const Vector3f& rhs) const
{
	return Vector3f(y() * rhs.z() - z() * rhs.y(), z() * rhs.x() - x() * rhs.z(), x() * rhs.y() - y() * rhs.x());
}

inline float Vector3f::element(unsigned int i) const
{
	assert(i < 3);
	return _v[i];
}

inline void Vector3f::element(unsigned int i, float value)
{
	assert(i < 3);
	_v[i] = value;
}

inline float Vector3f::operator[](unsigned int i) const
{
	return element(i);
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

inline std::ostream& operator<<(std::ostream& lhs, const Vector3f& rhs)
{
	lhs << "v3(" << rhs.x() << ", " << rhs.y() << ", " << rhs.z() << ")";
	return lhs;
}

inline std::istream& operator>>(std::istream& lhs, Vector3f& rhs)
{
	char c;
	float x,y,z;
	lhs >> c >> c >> c >> x >> c >> y >> c >> z >> c;
	rhs = Vector3f(x,y,z);

	return lhs;
}