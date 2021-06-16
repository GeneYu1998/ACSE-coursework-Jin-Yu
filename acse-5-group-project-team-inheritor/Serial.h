/*
  Team: inheritorplus
  Github handle: acse-jy220; acse-xc520; acse-sw3620
*/
#pragma once
#include <vector>
#include <iostream>
#include "PNG.h"

using namespace std;

class Serial{
public:

    Serial(int size);

    virtual ~Serial();

    int num_neighbours(int ii, int jj);

    void grid_to_file(int it);

    void do_iteration();

    void randomInit();

    void gridToPNG(int it);

    vector<vector<bool>> grid;

    vector<vector<bool>> new_grid;

private:
    int imax;
    int jmax;

};
