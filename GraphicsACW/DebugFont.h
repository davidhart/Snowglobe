// David Hart - 2012
//
// class DebugFont
//   Very simple font drawing code that makes use of deprecated display list fonts.
//   This class is not particularly fast or flexible.
//

#pragma once

#include <GXBase.h>
#include "Renderer.h"

class DebugFont
{
public:
	DebugFont();
	~DebugFont();

	void Create(const Renderer& renderer, const gxbase::GLWindowEx& window);
	void Dispose();

	void Print(const std::string& string, int x, int y);

private:

	GLint _fontList;
	const gxbase::GLWindowEx* _window;
};