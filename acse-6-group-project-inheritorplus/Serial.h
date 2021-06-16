#pragma once
#include <vector>
#include <iostream>
#include "PNG.h"

using namespace std;

class Serial{
public:
    Serial();

    Serial(int size, int max_steps);

    virtual ~Serial();

    int num_neighbours(int ii, int jj);

    void grid_to_file(int it);

    void do_iteration();

    void randomInit();

    void gridToPNG(int it);

    vector<vector<bool>> grid;
    vector<vector<bool>> new_grid;
    int maxiter;

private:
    int imax;
    int jmax;



};
