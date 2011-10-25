#include "Util.h"

#include <fstream>
#include <cassert>

void Util::ReadTextFileToString(const std::string& filename, std::string& text)
{
	std::ifstream file(filename);
	assert(file.is_open());

	text.clear();

	std::string line;
	while (std::getline(file, line))
	{
		text.append(line);
		text.append(1, '\n');
	}
}