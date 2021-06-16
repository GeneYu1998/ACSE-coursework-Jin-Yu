/*
  Team: inheritorplus
  Github handle: acse-jy220; acse-xc520; acse-sw3620
*/
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

    void SaveImage(string out_address);

    void CreateImage(int height, int width);
};
