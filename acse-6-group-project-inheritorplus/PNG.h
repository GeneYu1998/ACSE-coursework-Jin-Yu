#pragma once
#include "Image.h"
#include <iostream>

using namespace std;

class PNG :
	public Image
{
public:
	PNG();
	~PNG();

//	string GetType();
//	void LoadImage(string in_address);
	void SaveImage(string out_address);

    void CreateImage(int height, int width);
};
