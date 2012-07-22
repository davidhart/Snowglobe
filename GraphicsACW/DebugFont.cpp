#include "DebugFont.h"

// Deprecated opengl follows

DebugFont::DebugFont() :
	_fontList(0),	
	_window(NULL)
{
}

DebugFont::~DebugFont()
{
}

void DebugFont::Create(const Renderer& renderer, const gxbase::GLWindowEx& window)
{
	_window = &window;

	HFONT   font;                                                                           
	HFONT   oldfont;                                                                        
	_fontList = glGenLists(256);                                                        
	font = CreateFont(14, 0, 0, 0, FALSE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, FF_DONTCARE|DEFAULT_PITCH, "Lucida Console");                                      
	HDC hDC = GetWindowDC(window.GetSafeHwnd());
	oldfont = (HFONT)SelectObject(hDC, font);          
	wglUseFontBitmaps(hDC, 32, 96, _fontList);                         
	SelectObject(hDC, oldfont);                                            
	DeleteObject(font);
}

void DebugFont::Dispose()
{
	glDeleteLists(_fontList, 256);
}

void DebugFont::Print(const std::string& string, int x, int y)
{
	// Calculate clipping space position
	float xPos = x / (float)_window->Width() * 2 - 1;
	float yPos = y / (float)_window->Height() * -2 + 1;

	glRasterPos2f(xPos, yPos);

	glPushAttrib(GL_LIST_BIT);                                              
	glListBase(_fontList - 32);                                                               
	glCallLists(string.size()+1, GL_UNSIGNED_BYTE, string.c_str());
	glPopAttrib(); 
}