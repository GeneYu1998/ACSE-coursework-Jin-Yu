#include "Pixel.h"

Pixel::Pixel(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b) :
	x(x), y(y), r_(r), g_(g), b_(b)
{
}


Pixel::~Pixel()
{
}


int Pixel::GetRed()
{
	return this->r_;
}


int Pixel::GetGreen()
{
	return this->g_;
}


int Pixel::GetBlue()
{
	return this->b_;
}


void Pixel::SetBlue(int b)
{
	// here we cap values higher than 255 to 255. The unsigned int argument makes sure that
	// if a negative value has passed, it will become zero;
	while (b > 255 || b < 0)
	{
		if (b < 0) { b = 0; }
		else if (b > 255)  { b -= 255; }
		
	}
	this->b_ = b;

}


void Pixel::SetRed(int r)
{

	while (r > 255 || r < 0)
	{
		if (r < 0) { r = 0; }
		else if (r > 255) { r -= 255; }

	}

	this->r_ = r;
}


void Pixel::SetGreen(int g)
{
	while (g > 255 || g < 0)
	{
		if (g < 0) { g = 0; }
		else if (g > 255) { g -= 255; }

	}

	this->g_ = g;
}
