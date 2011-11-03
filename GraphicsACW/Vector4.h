#pragma once

#include "Util.h"
#include "Vector3.h"
#include <cmath>
#include <cassert>

class Vector4f
{

public:

	Vector4f();
	explicit Vector4f(float v);
	Vector4f(const Vector3f& v, float w);
	Vector4f(float x, float y, float z, float w);

	inline Vector4f& operator+=(const Vector4f& rhs);
	inline Vector4f& operator-=(const Vector4f& rhs);
	inline Vector4f& operator*=(const Vector4f& rhs);
	inline Vector4f& operator*=(float rhs);
	inline Vector4f& operator/=(const Vector4f& rhs);
	inline Vector4f& operator/=(float rhs);
	inline Vector4f operator-() const;
	
	inline float length() const;
	inline float dot(const Vector4f& rhs) const;

	inline float x() const;
	inline float y() const;
	inline float z() const;
	inline float w() const;

	inline void x(float x);
	inline void y(float y);
	inline void z(float z);
	inline void w(float w);

	inline float element(unsigned int i) const;
	inline void element(unsigned int i, float value);
	inline float operator[](unsigned int i) const;

private:

	float _v[4];

};

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

inline float Vector4f::length() const
{
	return sqrt(x() * x() + y() * y() + z() * z() + w() + w());
}

inline float Vector4f::dot(const Vector4f& rhs) const
{
	return x() * rhs.x() + y() * rhs.y() + z() * rhs.z() + w() + rhs.w();
}

inline float Vector4f::element(unsigned int i) const
{
	assert(i < 4);
	return _v[i];
}

inline void Vector4f::element(unsigned int i, float value)
{
	assert(i < 4);
	_v[i] = value;
}

inline float Vector4f::operator[](unsigned int i) const
{
	return element(i);
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
		Util::FloatEquality(lhs.z(), rhs.z()) &&
		Util::FloatEquality(lhs.w(), rhs.w());
}

inline bool operator!=(const Vector4f& lhs, const Vector4f& rhs)
{
	return !(lhs == rhs);
}

inline std::ostream& operator<<(std::ostream& lhs, const Vector4f& rhs)
{
	lhs << "v4(" << rhs.x() << ", " << rhs.y() << ", " << rhs.z() << ", " << rhs.w() << ")";
	return lhs;
}

inline std::istream& operator>>(std::istream& lhs, Vector4f& rhs)
{
	char c;
	float x,y,z, w;
	lhs >> c >> c >> c >> x >> c >> y >> c >> z >> c >> w >> c;
	rhs = Vector4f(x,y,z,w);

	return lhs;
}