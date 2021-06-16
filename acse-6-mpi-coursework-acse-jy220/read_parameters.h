#pragma once
#include <ctype.h>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class read_parameters
{
    public:
    ifstream *infile;
    read_parameters();
    ~read_parameters();
    string getValue(const string & name);
    void set_splashes();
    void print_parameters();

    //wave equation parameters
    double length;
    double width;
    int imax;
    int jmax;
    double c;
    double dt;
    string boundary_type;
    bool display_option;
    bool output;
    
    //output parameters
    double t_end;
    double t_out;

    // Intial Condition parameters
    double* rs;
    double* xs;
    double* ys;
    int num_splash;
};