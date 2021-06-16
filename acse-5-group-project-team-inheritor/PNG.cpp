/*
  Team: inheritorplus
  Github handle: acse-jy220; acse-xc520; acse-sw3620
*/
#include <fstream>
#include "PNG.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#define STBI_ONLY_PNG // reduce overhead
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"
#include "bitmap_image.hpp"


PNG::PNG(){}


PNG::~PNG(){}


void PNG::SaveImage(string out_address)
{
    this->saved_ = false; // in case save image in interface is called more than once this needs reseting.

    this->out_address_ = out_address;


    auto *save_data = new unsigned char[3 * this->width_ * this->height_]; // slab of memory for saving


    for (unsigned int i = 0; i < pixels_.size(); i++)
    {
        // index relates to 3 rgb for every pixel
        int index = 3*i;
        Pixel P = pixels_[i];
        save_data[index] = (unsigned char) P.GetRed();
        save_data[index + 1] = (unsigned char) P.GetGreen();
        save_data[index + 2] = (unsigned char) P.GetBlue();
    }

    stbi_write_png(this->out_address_.c_str(), this->width_, this->height_, 3, save_data, 3 * this->width_ );
    delete[] save_data;
    this->saved_ = true;
}

void PNG::CreateImage(int height, int width) {
    this->height_ = height ;
    this->width_ = width;
    pixels_.reserve(this->height_ * this->width_);

    for (unsigned int i = 0; i < this->height_ * this->width_; i++)
    {
        Pixel P = pixels_[i];
        P.SetRed(255);
        P.SetBlue(255);
        P.SetGreen(255);
        pixels_.push_back(P);
    }
}
