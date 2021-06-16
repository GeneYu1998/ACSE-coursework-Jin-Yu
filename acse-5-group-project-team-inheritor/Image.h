/*
  Team: inheritorplus
  Github handle: acse-jy220; acse-xc520; acse-sw3620
*/

#pragma once
#include <string>
#include "Pixel.h"


using namespace std;

class Image {
public:
    Image();

    ~Image();


    bool saved_ = false; // will be set to true when image has been correctly saved};

    virtual void CreateImage(int height, int width) = 0;
    virtual void SaveImage(string out_address) = 0;


    int height_ = -1;       // initialising height and width as -1, want code to break if these are not properly set
    int width_ = -1;

    string out_address_;
    vector<Pixel> pixels_;

};