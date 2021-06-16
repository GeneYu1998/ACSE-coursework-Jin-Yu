#include <iostream>
#include "Image.h"
//#include "bitmap_image.hpp"

Image::Image() {
    //type_ = "Not specified";
}


Image::~Image() {
}

/*
bool Image::CheckType(string address, string type) {
    std::cout << address << std::endl;
    string t = address.substr(address.size() - 3, 3);
    if (t == type) {
        return true;
    } else {
        return false;
    }
}


void Image::Update(int height, int width, vector<vector<bool>> grid) {
    for (unsigned int y = 0; y < height_; y++) {
        for (unsigned int x = 0; x < width_; x++) {
            rgb_t colour;
            Pixel P(x, y, int(colour.red), int(colour.green), int(colour.blue));
            pixels_[y + x * width].SetRed(0);
            pixels_[y + x * width].SetGreen(0);
            pixels_[y + x * width].SetBlue(0);
        }
    }
}

void Image::SortPixelsRowMajor() {
    // Sorts pixel vector of the image by Row Major form
//    if (this->loaded_)
//    {
    vector<Pixel> copy_pixels;
    copy_pixels.reserve(this->pixels_.size());
    for (unsigned int i = 0; i < this->pixels_.size(); i++) {
        copy_pixels.push_back(this->pixels_[i]);
    }

    for (Pixel P : copy_pixels) {
        int k = P.x + P.y * width_;
        this->pixels_[k] = P;
    }
    //}
}*/
