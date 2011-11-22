#pragma once

#include <string>

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
};