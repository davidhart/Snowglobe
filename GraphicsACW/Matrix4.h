#pragma once

#include "Vector4.h"

class Matrix4
{

public:

	Matrix4();
	Matrix4(const Vector4f& c0, const Vector4f& c1, const Vector4f& c2, const Vector4f& c3);

	inline Matrix4& operator*= (const Matrix4& rhs);

	inline void Cell(unsigned int column, unsigned int row, float value);
	inline float Cell(unsigned int column, unsigned int row) const;

	inline void Column(unsigned int column, const Vector4f& value);
	inline const Vector4f& Column(unsigned int column) const;

	inline void Row(unsigned int row, const Vector4f& value);
	inline const Vector4f& Row(unsigned int row) const;

	inline const Vector4f& operator[](unsigned int column) const;

private:

	Vector4f _m[4];

};

inline Matrix4 operator* (const Matrix4& rhs);