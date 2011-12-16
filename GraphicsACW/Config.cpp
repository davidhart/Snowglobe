#include "Config.h"
#include "LSystem.h"
#include "Util.h"
#include "Application.h"
#include <fstream>
#include <string>

Config::Config()
{

}

bool Config::Read(const char* filename, Application& application)
{
	std::ifstream file(filename);

	if (!file.good())
		return false;

	while (file.good())
	{
		std::string token;
		file >> token;

		if (token == "tree")
		{
			if (!ReadTree(file, application))
				return false;
		}

	}

	return true;
}

bool Config::ReadTree(std::istream& stream, Application& application)
{
	if (!Util::ReadToken(stream, "{"))
		return false;

	if (!Util::ReadToken(stream, "seed"))
		return false;

	std::string treeSeed;
	stream >> treeSeed;

	if (!Util::ReadToken(stream, "iterations"))
		return false;

	unsigned int iterations;
	stream >> iterations;

	if (!Util::ReadToken(stream, "leaves"))
		return false;

	unsigned int numLeaves;
	stream >> numLeaves;

	if (!Util::ReadToken(stream, "rules"))
		return false;

	unsigned int count;
	stream >> count;

	if (!Util::ReadToken(stream, "{"))
		return false;

	LSystem lsystem;

	for (unsigned int i = 0; i < count; ++i)
	{
		std::string predecessor;
		std::string successor;
		float weighting;

		stream >> predecessor >> successor >> weighting;

		lsystem.AddRule(predecessor, successor, weighting);
	}

	if (!Util::ReadToken(stream, "}"))
		return false;

	if (!Util::ReadToken(stream, "}"))
		return false;
	
	std::string result;
	lsystem.EvaluateRules(treeSeed, result, iterations);

	application.SetTreePattern(result);
	application.SetNumTreeLeaves(numLeaves);
	
	return true;
}