#include "GXBase.h"
#include "MyWindow.h"
using namespace gxbase;

class MyApp :public App
{
	
public:
	
	MyApp();

private:

	MyWindow w;
	
};

MyApp::MyApp()
{
}

static MyApp tut1;
