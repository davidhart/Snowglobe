#include "GXBase.h"
#include "MyWindow.h"
using namespace gxbase;

#include "LSystem.h"
#include <iostream>

class MyApp :public App
{
	
public:
	
	MyApp();

private:

	MyWindow w;
	
};

MyApp::MyApp()
{
	LSystem test;
	test.AddRule("L", "B[L+L+L]");

	std::string axiom = "L";
	std::string result;

	std::cout << axiom << std::endl;

	for (unsigned int i = 0; i < 3; ++i)
	{
		test.EvaluateRules(axiom, result);

		std::cout << result << std::endl;

		axiom.swap(result);
	}
}

static MyApp tut1;
