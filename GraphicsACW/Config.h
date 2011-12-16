#pragma once

#include <iosfwd>

class Application;

class Config
{

public:

	Config();
	bool Read(const char* filename, Application& application);

private:

	bool ReadTree(std::istream& stream, Application& application);

};