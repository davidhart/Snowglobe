#pragma once

#include <string>

namespace Util
{
	const float EPSILON = 0.00001f;

	void ReadTextFileToString(const std::string& filename, std::string& shaderSource);

	bool FloatEquality(float a, float b, float precision = EPSILON);
};