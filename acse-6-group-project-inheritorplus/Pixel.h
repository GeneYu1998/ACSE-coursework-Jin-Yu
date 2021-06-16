#pragma once
//#include "main.h"
#include <vector>

class Pixel
{
public:
	Pixel(unsigned int x, unsigned int y, unsigned int r, unsigned int g, unsigned int b);
	~Pixel();

	unsigned int x;
	unsigned int y;

	int GetRed();
	int GetGreen();
	int GetBlue();

	std::vector<Pixel> neighbours;
	
	void SetRed(int r);
	void SetGreen(int g);
	void SetBlue(int b);

private:
	unsigned int r_;
	unsigned int g_;
	unsigned int b_;
};

