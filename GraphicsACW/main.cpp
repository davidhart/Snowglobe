#include "GXBase.h"
#include "MyWindow.h"
using namespace gxbase;

#include <iostream>
#include "Matrix4.h"

class MyApp :public App
{
	
public:
	
	MyApp();

private:

	MyWindow w;
	
};

MyApp::MyApp()
{
	Matrix4 m;
	Matrix4 m2;
	Matrix4::Identity(m);
	Matrix4::Translation(m2, Vector3(10, 10, 10));

	Vector4 v (1,2,3,1);

	std::cout << m * m2 << std::endl;
	std::cout << m2 * v << std::endl;
}

static MyApp tut1;
