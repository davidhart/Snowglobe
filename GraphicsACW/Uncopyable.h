#pragma once

class Uncopyable
{

protected:

	Uncopyable();

private: 

	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);

};
