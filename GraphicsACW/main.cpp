#include "GXBase.h"
#include "MyWindow.h"

class MyApp :public gxbase::App
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
