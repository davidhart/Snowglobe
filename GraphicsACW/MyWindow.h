#pragma once

#include "GXBase.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBinding.h"
#include "Shader.h"
#include "ShaderProgram.h"

class MyWindow : public gxbase::GLWindowEx
{

public:

	MyWindow();
	void OnCreate();
	void OnDisplay();
	void OnIdle();
	void OnDestroy();
	void OnKeyboard(int key, bool down);

private:
	Renderer _renderer;
	VertexBuffer _testBuffer;
	VertexBinding _testBinding;
	VertexShader _testVertShader;
	FragmentShader _testFragShader;
	ShaderProgram _testShader;
};

