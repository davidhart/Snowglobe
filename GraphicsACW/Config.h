#pragma once

#include <string>
class Application;

class Config
{

public:

	Config();
	bool Read(const char* filename, Application& application);

};