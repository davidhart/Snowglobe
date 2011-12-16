// David Hart - 2011
//
// class Config
//   The roles of the Config class are parsing of the config file and
//   sending notifications of config parameters to the application class

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