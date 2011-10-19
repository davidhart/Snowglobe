#pragma once

#include "GXBase.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"

class MyWindow : public gxbase::GLWindowEx
{

public:

	MyWindow();
	void OnCreate();
	void OnDisplay();
	void OnIdle();

private:

	VertexBuffer _testBuffer;
	VertexBinding _testBinding;

};

