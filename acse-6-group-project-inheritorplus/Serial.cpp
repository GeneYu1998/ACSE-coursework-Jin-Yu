#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include "Serial.h"




Serial::Serial() {}

Serial::Serial(int size, int max_steps) {
    this->imax = size;
    this->jmax = size;
    this->maxiter = max_steps;
}

Serial::~Serial() {
}

int Serial::num_neighbours(int ii, int jj)
{
	int ix, jx;
	int cnt = 0;
	for (int i = -1; i <= 1; i++)
		for (int j = -1; j <= 1; j++)
			if (i != 0 || j != 0)
			{
				ix = (i + ii + imax) % imax;
				jx = (j + jj + jmax) % jmax;
				if (grid[ix][jx]) cnt++;
			}
	return cnt;
}

void Serial::grid_to_file(int it)
{
	stringstream fname;
	fstream f1;
    fname << "./output/output_" << it << "_size(" << imax << "*" << jmax <<")"<< ".txt";
    f1.open(fname.str().c_str(), ios_base::out);
	for (int i = 0; i < imax; i++)
	{   
		for (int j = 0; j < jmax; j++)
			f1 << grid[i][j] << "\t";
		f1 << endl;
	}
	f1.close();
}

void Serial::do_iteration()
{
	for (int i = 0; i < imax; i++)
		for (int j = 0; j < jmax; j++)
		{
			new_grid[i][j] = grid[i][j];
			int num_n = num_neighbours(i, j);
			if (grid[i][j])
			{
				if (num_n != 2 && num_n != 3)
					new_grid[i][j] = false;
			}
			else if (num_n == 3) new_grid[i][j] = true;
		}
	grid.swap(new_grid);
}

void Serial::randomInit()
{
    for (int i = 0; i < imax; i++)
        for (int j = 0; j < jmax; j++) grid[i][j] = (rand() % 2);
}

void Serial::gridToPNG(int it) {
    if (it == 0 || it == (maxiter - 1) || it == maxiter / 2) {
        PNG *Img = new PNG();
        Img->CreateImage(imax, jmax);
        stringstream fname;
        for (int i = 0; i < imax; i++) {
            for (int j = 0; j < jmax; j++) {
                if (grid[i][j] == 1) {
                    Img->pixels_[i * jmax + j].SetRed(0);
                    Img->pixels_[i * jmax + j].SetGreen(0);
                    Img->pixels_[i * jmax + j].SetBlue(0);
                }
            }
        }
        fname << "./data/" << "Inheritorplus_" << "GenNum(" << it << ")_size(" << imax << "*" << jmax << ")"
              << ".png";
        grid_to_file(it);
        Img->SaveImage(fname.str().c_str());
        //delete[]Img;
    }
}




