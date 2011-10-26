#include "Util.h"

#include <fstream>
#include <cassert>

void Util::ReadTextFileToString(const std::string& filename, std::string& text)
{
	std::ifstream file(filename);
	assert(file.is_open());

	text.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}