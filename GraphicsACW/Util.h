#pragma once

#include <string>
#include "Maths.h"

namespace Util
{
	const float EPSILON = 0.00001f;

	void ReadTextFileToString(const std::string& filename, std::string& shaderSource);

	bool FloatEquality(float a, float b, float precision = EPSILON);

	template <typename T> T Max(T a, T b)
	{
		return a > b ? a : b;
	}

	template <typename T> T Min(T a, T b)
	{
		return a < b ? a : b;
	}

	inline float DegreesToRadians(float degrees)
	{
		return degrees * PI / 180.0f;
	}

	inline float RadiansToDegrees(float radians)
	{
		return radians * 180.0f / PI;
	}
};