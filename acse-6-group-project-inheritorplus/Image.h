#pragma once
#include <string>
#include "Pixel.h"

using namespace std;

class Image {
public:
    Image();

    ~Image();

    // Loading and Saving
    //bool loaded_ = false; // will be set to true when image has been correctly loaded
    bool saved_ = false; // will be set to true when image has been correctly saved};
//    bool CheckType(string address, string type);

//    virtual string GetType() = 0;

    //virtual void LoadImage(string in_address) = 0;
    virtual void CreateImage(int height, int width) = 0;
    virtual void SaveImage(string out_address) = 0;

//    void Update(int height, int width, vector<vector<bool>> grid);

    int height_ = -1;       // initialising height and width as -1, want code to break if these are not properly set
    int width_ = -1;
    string type_;
    //string in_address_;
    string out_address_;
    vector<Pixel> pixels_;  // FOR IMPROVEMENT would have made life easier if vector was stored as
    // pointers to objects rather than objects themselves: vector<Pixel *>
    //void SortPixelsRowMajor();
};