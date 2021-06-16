#include <fstream>
#include "PNG.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
//#define STBI_ONLY_PNG // reduce overhead
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"
#include "bitmap_image.hpp"


PNG::PNG()
{
	//this->type_ = "png";
}


PNG::~PNG()
{
}

//string PNG::GetType()
//{
//	return this->type_;
//}


/*
void PNG::LoadImage(string in_address)
{
	// check if file is of appropriate extension
	if (!CheckType(in_address, this->type_))
	{
		cerr << "Error - Unsurpported format " << in_address << endl;
		return;
	}

	// Check in_address is valid
	ifstream  test(in_address);
	if (!test)
	{
		cerr << " \n\n Error - Could not open file " << in_address << " \n\n" << std::endl;
		return;
	}
	test.close();

	in_address_ = in_address;

	int width, height, channels;

	// This is the load command, all data is stored in the a list of unsigned chars pointed to by data. starting top left, row major
	// going r g b r g b ...etc for each pixel
	// assignes the value to height width and channels
	unsigned char *data = stbi_load(this->in_address_.c_str(), &width, &height, &channels, 0);

	this->height_ = height;
	this->width_ = width;

	// initialise the pixel array
	pixels_.reserve(this->height_ * this->width_);

	for (unsigned int y = 0; y < this->height_; y++)
	{
		for (unsigned int x = 0; x < this->width_; x++)
		{
			int index = 3 * x + y * 3 * this->width_; // formular get RGB values appertaining to certain x and y
			int red = (int) data[index];
			int green = (int) data[index + 1];
			int blue = (int) data[index + 2];

			Pixel P(x, y, int(red), int(green), int(blue));
			pixels_.push_back(P);
		}
	}

this->loaded_ = true;
}
*/


void PNG::SaveImage(string out_address)
{
	this->saved_ = false; // in case save image in interface is called more than once this needs reseting.
//
//	// check if file is of appropriate extension
//	if (!CheckType(out_address, this->type_))
//	{
//		cerr << "Error - Unsurpported format " << out_address << endl;
//		return;
//	}
//
//	if (!this->loaded_)
//	{
//		cerr << "Image not loaded. Call LoadImage()." << endl;
//		return;
//	}
//
//	// check out_address is valid
//	ofstream  test(out_address);
//	if (!test)
//	{
//		cerr << " \n\n Error - Could not open file " << out_address << " for writing! \n\n" << std::endl;
//		return;
//	}
//	test.close();


	this->out_address_ = out_address;


	auto *save_data = new unsigned char[3 * this->width_ * this->height_]; // slab of memory for saving

	//make sure pixels are row major ordered
	//this->SortPixelsRowMajor();

	for (unsigned int i = 0; i < pixels_.size(); i++)
	{
		// index relates to 3 rgb for every pixel
		//int index = 3 * i;
		int index = 3*i;
		Pixel P = pixels_[i];

		// set values into the unsigned char list from the pixel

		save_data[index] = (unsigned char) P.GetRed();
		save_data[index + 1] = (unsigned char) P.GetGreen();
		save_data[index + 2] = (unsigned char) P.GetBlue();
	}

	// this command should save to file based on these arguments but doesnt
	stbi_write_png(this->out_address_.c_str(), this->width_, this->height_, 3, save_data, 3 * this->width_ );
	delete[] save_data;
	this->saved_ = true;
}

void PNG::CreateImage(int height, int width) {
    this->height_ = height ;
    this->width_ = width;
    pixels_.reserve(this->height_ * this->width_);
    //cout << "size:" << pixels_.size();

    //auto *save_data = new unsigned char[3 * this->width_ * this->height_]; // slab of memory for saving
    //this->SortPixelsRowMajor();

    for (unsigned int i = 0; i < this->height_ * this->width_; i++)
    {
        // index relates to 3 rgb for every pixel
        //int index = 3 * i;
        int index = 3*i;
        Pixel P = pixels_[i];
        P.SetRed(255);
        P.SetBlue(255);
        P.SetGreen(255);
        pixels_.push_back(P);

        // set values into the unsigned char list from the pixel
//        save_data[index] = (unsigned char) P.GetRed();
//        save_data[index + 1] = (unsigned char) P.GetGreen();
//        save_data[index + 2] = (unsigned char) P.GetBlue();
    }

}
