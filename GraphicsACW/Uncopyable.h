#pragma once

class Uncopyable
{

public:

	Uncopyable();

private:

	Uncopyable(const Uncopyable&);
	Uncopyable& operator=(const Uncopyable&);

};
